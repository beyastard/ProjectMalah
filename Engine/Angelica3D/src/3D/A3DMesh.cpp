#include "A3DPlatform.h"

#include "A3DEngine.h"
#include "A3DMaterial.h"
#include "A3DMesh.h"
#include "A3DTexture.h"
#include "A3DErrLog.h"
#include "A3DOBBFuncs.h"
#include "A3DBox.h"
#include "A3DMeshCollector.h"
#include "A3DViewport.h"
#include "A3DEngine.h"
#include "A3DTextureMan.h"
#include "A3DConfig.h"

A3DMesh::A3DMesh()
{
	m_pA3DDevice	= NULL;
	m_pA3DStream	= NULL;

	m_bHWIMesh		= false;

	m_ppVertsBuffer = NULL;
	m_pIndices		= NULL;
	m_pTexture		= NULL;

	m_pDetailTexture= NULL;
	m_pMaterialHost	= NULL;

	m_pAutoAABBs	= NULL;
	m_pAutoOBBs		= NULL;
	m_pA3DBox		= NULL;

	m_nFrame		= 0;
	m_nVertCount	= 0;
	m_nIndexCount	= 0;
	
	m_pMapTable		= NULL;
	m_pRDIndices	= NULL;
	m_iLODLimit		= 0;
	m_bHasLOD		= false;
	m_vLODMaxDis	= 1.0f;

	m_MeshCollectorStoreHandle = NULL;
	m_bStreamFilled = false;

	m_szTextureMap[0] = '\0';
	m_szDetailTextureMap[0] = '\0';
}

A3DMesh::~A3DMesh()
{
}

bool A3DMesh::Init(A3DDevice * pDevice, int nFrameCount, int nVertCount, int nIndexCount, bool bHasLOD, bool bWire)
{
	int i;

	if( g_pA3DConfig->GetRunEnv() == A3DRUNENV_PURESERVER )
	{
		m_bHWIMesh = true;
		return true;
	}

	m_pA3DDevice = pDevice;
	m_nFrameCount = nFrameCount;
	m_nVertCount = nVertCount;
	m_nIndexCount = nIndexCount;
	m_nShowVertCount = m_nVertCount;
	m_nShowIndexCount = m_nIndexCount;
	m_bHasLOD = bHasLOD;
	m_bWire = bWire;

	if( m_bWire )
	{
		if( m_nIndexCount % 2 != 0 )
			return false;
	}
	else
	{
		if( m_nIndexCount % 3 != 0 )
			return false;
	}

	//Allocate memory for internal storage;
	m_pIndices = (WORD *) malloc(sizeof(WORD) * m_nIndexCount);
	if( NULL == m_pIndices ) return false;

	if( m_bHasLOD )
	{
		m_pMapTable = (WORD *) malloc(sizeof(WORD) * m_nVertCount);
		if( NULL == m_pMapTable ) return false;
		m_pRDIndices = (WORD *) malloc(sizeof(WORD) * m_nIndexCount);
		if( NULL == m_pRDIndices ) return false;

		//At least we should keep one triangle;
		m_iLODLimit = 3;
	}

	m_ppVertsBuffer = (A3DVERTEX **) malloc(sizeof(A3DVERTEX *) * m_nFrameCount);
	if( NULL == m_ppVertsBuffer ) return false;

	for(i=0; i<m_nFrameCount; i++)
	{
		m_ppVertsBuffer[i] = (A3DVERTEX *) malloc(sizeof(A3DVERTEX) * m_nVertCount);
		if( NULL == m_ppVertsBuffer[i] ) return false;
	}

	m_pA3DStream = new A3DStream();
	if( NULL == m_pA3DStream )
	{
		g_pA3DErrLog->ErrLog("A3DMesh::Init() Not enough memory!");
		return false;
	}

	if( !m_pA3DStream->Init(m_pA3DDevice, A3DVT_VERTEX, nVertCount, nIndexCount, 0, true, true) )
	{
		g_pA3DErrLog->ErrLog("A3DMesh::Init() m_pA3DStream Init Fail!");
		return false;
	}

	if( !m_Material.Init(m_pA3DDevice) )
		return false;


	m_pA3DBox = new A3DBox();
	if( NULL == m_pA3DBox )
		return false;

	m_pA3DBox->Init(m_pA3DDevice, A3DCOLORRGBA(255, 255, 255, 255));

	m_nFrame = 0;
	return true;
}

bool A3DMesh::SetMaterial(A3DCOLORVALUE clAmbient, A3DCOLORVALUE clDiffuse, A3DCOLORVALUE clEmissive, A3DCOLORVALUE clSpecular, FLOAT fShininess, bool b2Sided)
{
	if( m_bHWIMesh )	return true;
	return m_Material.SetValues(clAmbient, clDiffuse, clEmissive, clSpecular, fShininess, b2Sided);
}

bool A3DMesh::SetDetailTexture(char * szDetailTextureName, A3DMATRIX4 matDetailTM)
{
	if( !m_pA3DDevice || m_bHWIMesh )
		return true;

	if( m_pDetailTexture )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pDetailTexture);
		m_pDetailTexture = NULL;
	}
	
	if( !m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szDetailTextureName, &m_pDetailTexture, A3DTF_DETAILMAP) )
		return false;

	m_matDetailTM = matDetailTM;
	return true;
}

bool A3DMesh::SetTexture(char * szTextureName)
{		
	if( !m_pA3DDevice || m_bHWIMesh )
		return true;

	if( m_pTexture )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pTexture);
		m_pTexture = NULL;
	}
	
	if( !m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szTextureName, &m_pTexture) )
		return false;

	return true;
}

bool A3DMesh::Release()
{
	if( m_bHWIMesh )	return true;

	int		i;

	if( m_pA3DBox )
	{
		m_pA3DBox->Release();
		delete m_pA3DBox;
		m_pA3DBox = NULL;
	}
	if( m_pAutoAABBs )
	{
		free(m_pAutoAABBs);
		m_pAutoAABBs = NULL;
	}
	if( m_pAutoOBBs )
	{
		free(m_pAutoOBBs);
		m_pAutoOBBs = NULL;
	}
	if( m_pA3DStream )
	{
		m_pA3DStream->Release();
		delete m_pA3DStream;
		m_pA3DStream = NULL;
	}
	if( m_pMapTable )
	{
		free(m_pMapTable);
		m_pMapTable = NULL;
	}
	if( m_pRDIndices )
	{
		free(m_pRDIndices);
		m_pRDIndices = NULL;
	}
	if( m_pIndices )
	{
		free(m_pIndices);
		m_pIndices = NULL;
	}
	if( m_ppVertsBuffer )
	{
		for(i=0; i<m_nFrameCount; i++)
		{
			free(m_ppVertsBuffer[i]);
			m_ppVertsBuffer[i] = NULL;
		}
		free(m_ppVertsBuffer);
		m_ppVertsBuffer = NULL;
	}

	if( m_pTexture && m_pA3DDevice )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pTexture);
		m_pTexture = NULL;
	}
	
	m_Material.Release();

	m_nVertCount = m_nIndexCount = 0;
	m_nShowVertCount = m_nShowIndexCount = 0;
	return true;
}

bool A3DMesh::Render(A3DViewport * pCurrentViewport)
{
	//If this object is created outside D3D;
	if( !m_pA3DDevice || m_bHWIMesh )
		return true;

	if( !UpdateLOD(pCurrentViewport, m_pA3DDevice->GetWorldMatrix()) )
		return false;

	m_pA3DStream->Appear();

	if( m_pTexture )
		m_pTexture->Appear(0);
	else
		m_pA3DDevice->ClearTexture(0);
	
	if( m_pDetailTexture && g_pA3DConfig->GetFlagDetailModel() )
	{
		m_pDetailTexture->Appear(1);
		m_pA3DDevice->SetTextureColorOP(1, A3DTOP_ADDSIGNED);
		m_pA3DDevice->SetTextureCoordIndex(1, 0);
		m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
		m_pA3DDevice->SetTextureTransformFlags(1, A3DTTFF_COUNT2);
		m_pA3DDevice->SetTextureMatrix(1, m_matDetailTM);
	}

	if( m_pMaterialHost )
		m_pMaterialHost->Appear();
	else
		m_Material.Appear();

	if( m_bWire )
	{
		if( !m_pA3DDevice->DrawIndexedPrimitive(A3DPT_LINELIST, 0, GetShowVertCount(), 0, GetShowIndexCount() / 2) )
		{
			g_pA3DErrLog->ErrLog("A3DMesh::Render() Draw Indexed Primitive Fail!");
			return false;
		}
	}
	else
	{
		if( !m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, GetShowVertCount(), 0, GetShowIndexCount() / 3) )
		{
			g_pA3DErrLog->ErrLog("A3DMesh::Render() Draw Indexed Primitive Fail!");
			return false;
		}
	}
	
	if( m_pTexture )
		m_pTexture->Disappear(0);

	if( m_pDetailTexture && g_pA3DConfig->GetFlagDetailModel() )
	{
		m_pDetailTexture->Disappear(1);
		m_pA3DDevice->SetTextureColorOP(1, A3DTOP_DISABLE);
	}

	if( m_pA3DBox && 0 )
	{
		if( m_pAutoOBBs )
		{
			m_pA3DBox->UpdateBox(m_pAutoOBBs[m_nFrame], A3DCOLORRGBA(255, 255, 255, 255));
			m_pA3DBox->Render(pCurrentViewport);
		}
		if( m_pAutoAABBs )
		{
			m_pA3DBox->UpdateBox(m_pAutoAABBs[m_nFrame], A3DCOLORRGBA(255, 255, 0, 255));
			m_pA3DBox->Render(pCurrentViewport);
		}
	}

	return true;
}

bool A3DMesh::RenderDirect(A3DViewport * pCurrentViewport)
{
	//If this object is created outside D3D;
	if( !m_pA3DDevice || m_bHWIMesh )
		return true;

	m_pA3DStream->Appear();

	if( m_bWire )
	{
		if( !m_pA3DDevice->DrawIndexedPrimitive(A3DPT_LINELIST, 0, GetShowVertCount(), 0, GetShowIndexCount() / 2) )
		{
			g_pA3DErrLog->ErrLog("A3DMesh::Render() Draw Indexed Primitive Fail!");
			return false;
		}
	}
	else
	{
		if( !m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, GetShowVertCount(), 0, GetShowIndexCount() / 3) )
		{
			g_pA3DErrLog->ErrLog("A3DMesh::Render() Draw Indexed Primitive Fail!");
			return false;
		}
	}
	
	return true;
}

bool A3DMesh::UpdateVertexBuffer()
{
	//See if is used outside the d3d;
	if( !m_pA3DDevice || m_bHWIMesh )
		return true;

	if( m_bStreamFilled == false || m_nFrameCount > 1 )
	{
		if( !m_pA3DStream->SetVerts((LPBYTE)(m_ppVertsBuffer[m_nFrame]), m_nVertCount) )
		{
			g_pA3DErrLog->ErrLog("A3DMesh::UpdateVertexBuffer() SetVerts into A3DStream Fail");
			return false;
		}
		m_bStreamFilled = true;
	}
	
	return true;
}

bool A3DMesh::SetVerts(int nFrame, A3DVERTEX * pVerts, int nVertCount)
{
	if( m_bHWIMesh )	return true;

	if( nVertCount != m_nVertCount || nFrame >= m_nFrameCount )
		return false;

	//Store it in our own data structure;
	memcpy(m_ppVertsBuffer[nFrame], pVerts, sizeof(A3DVERTEX) * m_nVertCount);

	if( nFrame == 0 ) //For first frame automated be the current render frame;
		return UpdateVertexBuffer();

	return true;
}

bool A3DMesh::SetIndices(WORD * pIndices, int nIndexCount)
{
	if( m_bHWIMesh )	return true;

	if( nIndexCount != m_nIndexCount )
		return false;

	//Store it in our own data structure;
	memcpy(m_pIndices, pIndices, sizeof(WORD) * m_nIndexCount);

	UpdateIndexBuffer(m_pIndices, m_nIndexCount);
	return true;
}

bool A3DMesh::UpdateIndexBuffer(WORD * pIndices, int nShowIndexCount)
{
	//If outside d3d;
	if( !m_pA3DDevice || m_bHWIMesh )	return true;

	if( !m_pA3DStream->SetIndices(pIndices, nShowIndexCount) )
	{
		g_pA3DErrLog->ErrLog("A3DMesh::UpdateIndexBuffer() SetIndices into A3DStream Fail");
		return false;
	}

	m_nShowIndexCount = nShowIndexCount;
	return true;
}

bool A3DMesh::UpdateToFrame(int nFrame)
{
	if( m_bHWIMesh )	return true;

	//Here we should use some scripts to control the mesh's behavior;
	if( m_nFrameCount > 1 )
	{
		m_nFrame = nFrame % m_nFrameCount;
		UpdateVertexBuffer();
	}
	return true;
}

bool A3DMesh::Save(AFile * pFileToSave)
{
	if( m_bHWIMesh )	return true;

	if( pFileToSave->IsBinary() )
	{
		int			i, size;
		DWORD		dwWriteLength;
		char		szLineBuffer[2048];
		
		sprintf(szLineBuffer, "MESH: %s", GetName());
		pFileToSave->Write(szLineBuffer, strlen(szLineBuffer) + 1, &dwWriteLength);

		//Vertex Count;
		pFileToSave->Write(&m_nVertCount, sizeof(int), &dwWriteLength);
		//Index Count;
		pFileToSave->Write(&m_nIndexCount, sizeof(int), &dwWriteLength);
		//Frame Count:
		pFileToSave->Write(&m_nFrameCount, sizeof(int), &dwWriteLength);
		//Has LOD or not;
		pFileToSave->Write(&m_bHasLOD, sizeof(bool), &dwWriteLength);
		//Triangle List or not;
		pFileToSave->Write(&m_bWire, sizeof(bool), &dwWriteLength);
		//User defined properties;
		size = sizeof(m_property);
		pFileToSave->Write(&size, sizeof(int), &dwWriteLength);
		pFileToSave->Write(&m_property, size, &dwWriteLength);

		//Indices;
		pFileToSave->Write(m_pIndices, sizeof(WORD) * m_nIndexCount, &dwWriteLength);

		if( m_bHasLOD )
		{
			//LOD's Map Table;
			pFileToSave->Write(&m_vLODMinDis, sizeof(FLOAT), &dwWriteLength);
			pFileToSave->Write(&m_vLODMaxDis, sizeof(FLOAT), &dwWriteLength);
			pFileToSave->Write(&m_iLODLimit, sizeof(int), &dwWriteLength);
			pFileToSave->Write(m_pMapTable, sizeof(WORD) * m_nVertCount, &dwWriteLength);
		}
		//Frame Vertex;
		for(i=0; i<m_nFrameCount; i++)
			pFileToSave->Write(m_ppVertsBuffer[i], sizeof(A3DVERTEX) * m_nVertCount, &dwWriteLength);

		if( m_pDetailTexture )
		{
			sprintf(szLineBuffer, "DETAILTEXTURE: %s", m_pDetailTexture->GetMapFile());
			pFileToSave->Write(szLineBuffer, strlen(szLineBuffer) + 1, &dwWriteLength);

			pFileToSave->Write(&m_matDetailTM, sizeof(A3DMATRIX4), &dwWriteLength);
		}

		//Texture;
		if( m_pTexture )
			sprintf(szLineBuffer, "TEXTURE: %s", m_pTexture->GetMapFile());
		else
			sprintf(szLineBuffer, "TEXTURE: %s", "");
		pFileToSave->Write(szLineBuffer, strlen(szLineBuffer) + 1, &dwWriteLength);

		//Material
		if( !m_Material.Save(pFileToSave) )
			return false;
	}
	else
	{
		char szLineBuffer[2048];
		sprintf(szLineBuffer, "MESH: %s\n{", GetName());
		pFileToSave->WriteLine(szLineBuffer);

		sprintf(szLineBuffer, "VERTCOUNT: %d", m_nVertCount);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "INDEXCOUNT: %d", m_nIndexCount);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "FRAMECOUNT: %d", m_nFrameCount);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "HASLOD: %d", m_bHasLOD);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "TRIANGLELIST: %d", !m_bWire);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "2SIDED: %d", m_property.b2Sided);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "CANSHOOTTHROUGH: %d", m_property.bCanShootThrough);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "NOMARK: %d", m_property.bNoMark);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "CANPICKTHROUGH: %d", m_property.bCanPickThrough);
		pFileToSave->WriteLine(szLineBuffer);
		sprintf(szLineBuffer, "MATERIALTYPE: %d", m_property.nMaterialType);
		pFileToSave->WriteLine(szLineBuffer);

		pFileToSave->WriteLine("INDEX: \n{");
		if( m_bWire )
		{
			for(int n=0; n<m_nIndexCount / 2; n++)
			{
				sprintf(szLineBuffer, "%d, %d", m_pIndices[n * 2], m_pIndices[n * 2 + 1]) ;
				pFileToSave->WriteLine(szLineBuffer);
			}
		}
		else
		{
			for(int n=0; n<m_nIndexCount / 3; n++)
			{
				sprintf(szLineBuffer, "%d, %d, %d", m_pIndices[n * 3], m_pIndices[n * 3 + 1], m_pIndices[n * 3 + 2]) ;
				pFileToSave->WriteLine(szLineBuffer);
			}
		}
		//<== INDEX: {
		pFileToSave->WriteLine("}");

		//LOD vert map table
		if( m_bHasLOD )
		{
			sprintf(szLineBuffer, "LODMINDIS: %f", m_vLODMinDis);
			pFileToSave->WriteLine(szLineBuffer);

			sprintf(szLineBuffer, "LODMAXDIS: %f", m_vLODMaxDis);
			pFileToSave->WriteLine(szLineBuffer);

			sprintf(szLineBuffer, "LODLIMIT: %d", m_iLODLimit);
			pFileToSave->WriteLine(szLineBuffer);

			pFileToSave->WriteLine("VERTMAP: \n{");
			for(int n=0; n<m_nVertCount; n++)
			{
				sprintf(szLineBuffer, "%d", m_pMapTable[n]) ;
				pFileToSave->WriteLine(szLineBuffer);
			}
			//<== VERTMAP: {
			pFileToSave->WriteLine("}");
		}

		pFileToSave->WriteLine("VERTEX: \n{");
		for(int i=0; i<m_nFrameCount; i++)
		{
			sprintf(szLineBuffer, "FRAME%d\n{", i);
			pFileToSave->WriteLine(szLineBuffer);

			for(int v=0; v<m_nVertCount; v++)
			{
				sprintf(szLineBuffer, "(%f, %f, %f, %f, %f, %f, %f, %f)",
					m_ppVertsBuffer[i][v].x, m_ppVertsBuffer[i][v].y, m_ppVertsBuffer[i][v].z,
					m_ppVertsBuffer[i][v].nx, m_ppVertsBuffer[i][v].ny, m_ppVertsBuffer[i][v].nz,
					m_ppVertsBuffer[i][v].tu, m_ppVertsBuffer[i][v].tv);
				pFileToSave->WriteLine(szLineBuffer);
			}
			//<==FRAME%d{
			pFileToSave->WriteLine("}");
		}
		//<==VERTEX: {
		pFileToSave->WriteLine("}");

		if( m_pDetailTexture )
		{
			sprintf(szLineBuffer, "DETAILTEXTURE: %s", m_pDetailTexture->GetMapFile());
			pFileToSave->WriteLine(szLineBuffer);

			sprintf(szLineBuffer, "(%f, %f, %f, %f)", m_matDetailTM._11, m_matDetailTM._12, m_matDetailTM._13, m_matDetailTM._14);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f, %f)", m_matDetailTM._21, m_matDetailTM._22, m_matDetailTM._23, m_matDetailTM._24);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f, %f)", m_matDetailTM._31, m_matDetailTM._32, m_matDetailTM._33, m_matDetailTM._34);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f, %f)", m_matDetailTM._41, m_matDetailTM._42, m_matDetailTM._43, m_matDetailTM._44);
			pFileToSave->WriteLine(szLineBuffer);
		}

		if( m_pTexture )
			sprintf(szLineBuffer, "TEXTURE: %s", m_pTexture->GetMapFile());
		else
			sprintf(szLineBuffer, "TEXTURE: %s", "");
		pFileToSave->WriteLine(szLineBuffer);

		if( !m_Material.Save(pFileToSave) )
			return false;

		//<== MESH: %s {
		pFileToSave->WriteLine("}");
	}
	return true;
}

bool A3DMesh::Load(A3DDevice * pDevice, AFile * pFileToLoad)
{
	char szMoxRelativePath[MAX_PATH];
	char szTexturePath[MAX_PATH];
	AFileMod_GetFilePath(pFileToLoad->GetRelativeName(), szMoxRelativePath, MAX_PATH);
	sprintf(szTexturePath, "%s\\Textures", szMoxRelativePath);

	if( pFileToLoad->IsBinary() )
	{
		int			i, size;
		DWORD		dwReadLength;
		char		szLineBuffer[2048];
		char		szResult[2048];
		
		pFileToLoad->ReadString(szLineBuffer, 2048, &dwReadLength);
		pFileToLoad->GetStringAfter(szLineBuffer, "MESH: ", szResult);
		if( strlen(szResult) >= MAX_NAME )
			return false;
		SetName(szResult);

		//Vertex Count;
		pFileToLoad->Read(&m_nVertCount, sizeof(int), &dwReadLength);
		//Index Count;
		pFileToLoad->Read(&m_nIndexCount, sizeof(int), &dwReadLength);
		//Frame Count:
		pFileToLoad->Read(&m_nFrameCount, sizeof(int), &dwReadLength);
		//Has LOD or not;
		pFileToLoad->Read(&m_bHasLOD, sizeof(bool), &dwReadLength);
		//Triangle List or not;
		pFileToLoad->Read(&m_bWire, sizeof(bool), &dwReadLength);

		if( !Init(pDevice, m_nFrameCount, m_nVertCount, m_nIndexCount, m_bHasLOD, m_bWire) )
			return false;

		if( m_bHWIMesh )
		{
			//We have to seek to the next section in the .mox file;
			//User defined property;
			pFileToLoad->Read(&size, sizeof(int), &dwReadLength);
			pFileToLoad->Read(&m_property, size, &dwReadLength);

			//Indices;
			pFileToLoad->Seek(sizeof(WORD) * m_nIndexCount, AFILE_SEEK_CUR);

			if( m_bHasLOD )
			{
				pFileToLoad->Seek(sizeof(FLOAT), AFILE_SEEK_CUR);
				pFileToLoad->Seek(sizeof(FLOAT), AFILE_SEEK_CUR);
				pFileToLoad->Seek(sizeof(int), AFILE_SEEK_CUR);
				pFileToLoad->Seek(sizeof(WORD) * m_nVertCount, AFILE_SEEK_CUR);
			}

			//Frame Vertex;
			for(i=0; i<m_nFrameCount; i++)
				pFileToLoad->Seek(sizeof(A3DVERTEX) * m_nVertCount, AFILE_SEEK_CUR);

			//Texture;
			pFileToLoad->ReadString(szLineBuffer, 2048, &dwReadLength);
			if( strstr(szLineBuffer, "DETAILTEXTURE: ") == szLineBuffer )
			{
				// Yes, I have the detail map;
				pFileToLoad->Seek(sizeof(A3DMATRIX4), AFILE_SEEK_CUR);
				
				// Now prepare for the next texture loading;
				pFileToLoad->ReadString(szLineBuffer, 2048, &dwReadLength);
			}
		}
		else
		{
			//User defined property;
			pFileToLoad->Read(&size, sizeof(int), &dwReadLength);
			pFileToLoad->Read(&m_property, size, &dwReadLength);

			//Indices;
			pFileToLoad->Read(m_pIndices, sizeof(WORD) * m_nIndexCount, &dwReadLength);

			if( m_bHasLOD )
			{
				pFileToLoad->Read(&m_vLODMinDis, sizeof(FLOAT), &dwReadLength);
				pFileToLoad->Read(&m_vLODMaxDis, sizeof(FLOAT), &dwReadLength);
				pFileToLoad->Read(&m_iLODLimit, sizeof(int), &dwReadLength);
				pFileToLoad->Read(m_pMapTable, sizeof(WORD) * m_nVertCount, &dwReadLength);
			}

			//Frame Vertex;
			for(i=0; i<m_nFrameCount; i++)
				pFileToLoad->Read(m_ppVertsBuffer[i], sizeof(A3DVERTEX) * m_nVertCount, &dwReadLength);

			//You must call UpdateVertexBuffer to fill the IDirect3DVertexBuffer;
			UpdateVertexBuffer();
			UpdateIndexBuffer(m_pIndices, m_nIndexCount);

			//Texture;
			pFileToLoad->ReadString(szLineBuffer, 2048, &dwReadLength);
			if( strstr(szLineBuffer, "DETAILTEXTURE: ") == szLineBuffer )
			{
				// Yes, I have the detail map;
				pFileToLoad->GetStringAfter(szLineBuffer, "DETAILTEXTURE: ", szResult);
				if( strlen(szResult) >= MAX_PATH )
					return false;

				sprintf(m_szDetailTextureMap, "%s\\%s", szTexturePath, szResult);

				if( m_pA3DDevice )
				{
					if( !m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(m_szDetailTextureMap, &m_pDetailTexture, A3DTF_DETAILMAP) )
						return false;
				}

				pFileToLoad->Read(&m_matDetailTM, sizeof(A3DMATRIX4), &dwReadLength);
				
				// Now prepare for the next texture loading;
				pFileToLoad->ReadString(szLineBuffer, 2048, &dwReadLength);
			}
			
			pFileToLoad->GetStringAfter(szLineBuffer, "TEXTURE: ", szResult);
			if( strlen(szResult) >= MAX_PATH )
				return false;

			sprintf(m_szTextureMap, "%s\\%s", szTexturePath, szResult);

			if (pDevice)
			{
				if( !pDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(m_szTextureMap, &m_pTexture) )
					return false;
			}
		}

		//Material
		if( !m_Material.Load(pDevice, pFileToLoad) )
			return false;
	}
	else
	{
		char szLineBuffer[AFILE_LINEMAXLEN];
		char szResult[AFILE_LINEMAXLEN];
		int n0, n1, n2;
		DWORD dwReadLen;
		A3DMESH_PROP property;
		int nval;

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		pFileToLoad->GetStringAfter(szLineBuffer, "MESH: ", szResult);
		SetName(szResult);

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "{") )
			return false;

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "VERTCOUNT: %d", &m_nVertCount);
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "INDEXCOUNT: %d", &m_nIndexCount);
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "FRAMECOUNT: %d", &m_nFrameCount);
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "HASLOD: %d", &nval);
		m_bHasLOD = nval ? true : false;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "TRIANGLELIST: %d", &nval);
		m_bWire = !(nval ? true : false);
		
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "2SIDED: %d", &nval);
		property.b2Sided = nval ? true : false;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "CANSHOOTTHROUGH: %d", &nval);
		property.bCanShootThrough = nval ? true : false;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "NOMARK: %d", &nval);
		property.bNoMark = nval ? true : false;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "CANPICKTHROUGH: %d", &nval);
		property.bCanPickThrough = nval ? true : false;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		sscanf(szLineBuffer, "MATERIALTYPE: %d", &nval);
		property.nMaterialType = (MATERIAL_TYPE) nval;

		if( !Init(pDevice, m_nFrameCount, m_nVertCount, m_nIndexCount, m_bHasLOD, m_bWire) )
			return false;

		if( m_bHWIMesh )
		{
			//Index Buffer;
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( m_bWire )
			{
				for(int n=0; n<m_nIndexCount / 2; n++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				}
			}
			else
			{
				for(int n=0; n<m_nIndexCount / 3; n++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				}
			}

			//<== INDEX: {
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			//Vertex Map Table;
			if( m_bHasLOD )
			{
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				for(int n=0; n<m_nVertCount; n++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				}
				//<== VERTMAP: {
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			}

			//Vertex Buffer;
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			for(int i=0; i<m_nFrameCount; i++)
			{
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				for(int v=0; v<m_nVertCount; v++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				}
				//<==FRAME%d{
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			}
			//<==VERTEX: {
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strstr(szLineBuffer, "DETAILTEXTURE: ") == szLineBuffer )
			{
				// Yes, I have the detail map;
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				
				// Now prepare for the next texture loading;
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			}
		}
		else
		{
			SetProperty(&property);
			
			//Index Buffer;
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strcmp(szLineBuffer, "INDEX: ") )
				return false;

			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strcmp(szLineBuffer, "{") )
				return false;

			if( m_bWire )
			{
				for(int n=0; n<m_nIndexCount / 2; n++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
					sscanf(szLineBuffer, "%d, %d", &n0, &n1);
					m_pIndices[n * 2] = n0;
					m_pIndices[n * 2 + 1] = n1;
				}
			}
			else
			{
				for(int n=0; n<m_nIndexCount / 3; n++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
					sscanf(szLineBuffer, "%d, %d, %d", &n0, &n1, &n2);
					m_pIndices[n * 3] = n0;
					m_pIndices[n * 3 + 1] = n1;
					m_pIndices[n * 3 + 2] = n2;
				}
			}

			//<== INDEX: {
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strcmp(szLineBuffer, "}") )
				return false;

			//Vertex Map Table;
			if( m_bHasLOD )
			{
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "LODMINDIS: %d", &m_vLODMinDis);

				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "LODMAXDIS: %d", &m_vLODMaxDis);

				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "LODLIMIT: %d", &m_iLODLimit);

				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				if( strcmp(szLineBuffer, "VERTMAP: ") )
					return false;
				
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				if( strcmp(szLineBuffer, "{") )
					return false;

				for(int n=0; n<m_nVertCount; n++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
					sscanf(szLineBuffer, "%d", &n0);
					m_pMapTable[n] = n0;
				}
				//<== VERTMAP: {
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				if( strcmp(szLineBuffer, "}") )
					return false;
			}

			//Vertex Buffer;
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strcmp(szLineBuffer, "VERTEX: ") )
				return false;

			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strcmp(szLineBuffer, "{") )
				return false;

			for(int i=0; i<m_nFrameCount; i++)
			{
				int n;

				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				pFileToLoad->GetStringAfter(szLineBuffer, "FRAME", szResult);
				n = atoi(szResult);
				if( n != i ) 
					return false;
				
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				if( strcmp(szLineBuffer, "{") )
					return false;
				
				for(int v=0; v<m_nVertCount; v++)
				{
					pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);

					sscanf(szLineBuffer, "(%f, %f, %f, %f, %f, %f, %f, %f)",
						&m_ppVertsBuffer[i][v].x, &m_ppVertsBuffer[i][v].y, &m_ppVertsBuffer[i][v].z,
						&m_ppVertsBuffer[i][v].nx, &m_ppVertsBuffer[i][v].ny, &m_ppVertsBuffer[i][v].nz,
						&m_ppVertsBuffer[i][v].tu, &m_ppVertsBuffer[i][v].tv);
				}
				//<==FRAME%d{
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				if( strcmp(szLineBuffer, "}") )
					return false;
			}
			//<==VERTEX: {
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			if( strcmp(szLineBuffer, "}") )
				return false;

			//You must call UpdateVertexBuffer to fill the IDirect3DVertexBuffer;
			UpdateVertexBuffer();
			UpdateIndexBuffer(m_pIndices, m_nIndexCount);

			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);

			if( strstr(szLineBuffer, "DETAILTEXTURE: ") == szLineBuffer )
			{
				// Yes, I have the detail map;
				pFileToLoad->GetStringAfter(szLineBuffer, "DETAILTEXTURE: ", szResult);

				sprintf(m_szDetailTextureMap, "%s\\%s", szTexturePath, szResult);
				if( !m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(m_szDetailTextureMap, &m_pDetailTexture, A3DTF_DETAILMAP) )
					return false;

				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "(%f, %f, %f, %f)", &m_matDetailTM._11, &m_matDetailTM._12, &m_matDetailTM._13, &m_matDetailTM._14);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "(%f, %f, %f, %f)", &m_matDetailTM._21, &m_matDetailTM._22, &m_matDetailTM._23, &m_matDetailTM._24);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "(%f, %f, %f, %f)", &m_matDetailTM._31, &m_matDetailTM._32, &m_matDetailTM._33, &m_matDetailTM._34);
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "(%f, %f, %f, %f)", &m_matDetailTM._41, &m_matDetailTM._42, &m_matDetailTM._43, &m_matDetailTM._44);

				// Now prepare for the next texture loading;
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			}

			pFileToLoad->GetStringAfter(szLineBuffer, "TEXTURE: ", szResult);

			sprintf(m_szTextureMap, "%s\\%s", szTexturePath, szResult);
			if( !m_pA3DDevice->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(m_szTextureMap, &m_pTexture) )
				return false;
		}
		if( !m_Material.Load(pDevice, pFileToLoad) )
			return false;

		//<== MESH: %s {
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "}") )
			return false;
	}

	if( m_pA3DDevice && m_pA3DDevice->GetA3DEngine()->IsOptimizedEngine() )
	{
		m_MeshCollectorStoreHandle = m_pA3DDevice->GetA3DEngine()->GetMeshCollector()->RegisterTexture(m_pTexture, &m_Material);
		if( !m_MeshCollectorStoreHandle )
			return false;
	}

	BuildAutoAABB();
	BuildAutoOBB();
	return true;
}

/*	Set index map tab.

	pMapTable: array stores index map.
	iSize: size of pMapTab in items.
*/
bool A3DMesh::SetMapTable(WORD* pMapTable, int iSize)
{
	if( m_bHWIMesh )	return true;

	if( iSize != m_nVertCount )
		return false;

	if( !m_bHasLOD )
	{
		m_pMapTable = (WORD *) malloc(sizeof(WORD) * m_nVertCount);
		if( NULL == m_pMapTable ) return false;
		m_pRDIndices = (WORD *) malloc(sizeof(WORD) * m_nIndexCount);
		if( NULL == m_pRDIndices ) return false;
		m_bHasLOD = true;
	}

	memcpy(m_pMapTable, pMapTable, m_nVertCount * sizeof (WORD));

	return true;
}

/*	Prepare index used to render mesh, results will be stored in m_pRDIndices.
	This operation only can be used by LOD meshes.

	Return number of indices stored in m_pRDIndices.

	iVertLimit: number of vertices will be rendered.
*/
int A3DMesh::PrepareIndex(WORD * pIndex, int iVertLimit, int * pRealVertCount)
{
	if( m_bHWIMesh )	return true;

	if (iVertLimit < m_iLODLimit)
		iVertLimit = m_iLODLimit;

	int i, iNumIndex = 0;
	WORD a, b, c;

	for (i=0; i < m_nIndexCount; )
	{
		a = MapIndex(m_pIndices[i++], iVertLimit);
		b = MapIndex(m_pIndices[i++], iVertLimit);
		c = MapIndex(m_pIndices[i++], iVertLimit);

		if (a == b || a == c || b == c)
			break;

		iNumIndex += 3;

		*pIndex++ = a;
		*pIndex++ = b;
		*pIndex++ = c;
	}

	*pRealVertCount = iVertLimit;
	return iNumIndex;
}

//Recalculate the current render index buffer and update it into the A3DStream;
bool A3DMesh::UpdateLOD(A3DViewport * pCurrentViewport, A3DMATRIX4 matWorld)
{
	if( m_bHWIMesh ) return true;
	if( !m_bHasLOD ) return true;
	
	m_pA3DDevice->GetA3DEngine()->BeginPerformanceRecord(A3DENGINE_PERFORMANCE_MESHUPDATELOD);

	int			nShowVertCount = 0;

	FLOAT		vLODPercent = 1.0f;
	A3DCamera * pCamera = pCurrentViewport->GetCamera();
	
	A3DVECTOR3 vecCamPos = pCamera->GetPos();
	A3DVECTOR3 vecCenter = GetMeshAutoAABB(m_nFrame).Center * matWorld;
	FLOAT	   vDis = Magnitude(vecCamPos - vecCenter) * (pCamera->GetFOV() / DEG2RAD(70.0f));
	if( vDis < m_vLODMinDis )
		vLODPercent = 1.0f;
	else if( vDis > m_vLODMaxDis )
		vLODPercent = 0.0f;
	else
	{
		vLODPercent = 1.0f - (vDis - m_vLODMinDis) / (m_vLODMaxDis - m_vLODMinDis);
		vLODPercent = vLODPercent;
	}

	nShowVertCount = int((m_nVertCount - m_iLODLimit) * vLODPercent + m_iLODLimit);
	
	int nNewIndexCount = PrepareIndex(m_pRDIndices, nShowVertCount, &nShowVertCount);
	if( !UpdateIndexBuffer(m_pRDIndices, nNewIndexCount) )
		return false;

	m_nShowVertCount = nShowVertCount;

	m_pA3DDevice->GetA3DEngine()->EndPerformanceRecord(A3DENGINE_PERFORMANCE_MESHUPDATELOD);
	return true;
}

//This function calculates a mesh's oriented bounding box for each frame;
bool A3DMesh::BuildAutoOBB()
{
	if( m_bHWIMesh )	return true;

	if( m_pAutoOBBs )
	{
		free(m_pAutoOBBs);
		m_pAutoOBBs = NULL;
	}
	
	m_pAutoOBBs = (A3DOBB *) malloc(sizeof(A3DOBB) * m_nFrameCount);
	if( NULL == m_pAutoOBBs )
		return false;

	for(int i=0; i<m_nFrameCount; i++)
	{
		m_pAutoOBBs[i] = GetOBB(m_ppVertsBuffer[i], m_nVertCount);
	}
	return true;
}

//This function calculates a mesh's axis aligned bounding box for each frame;
bool A3DMesh::BuildAutoAABB()
{
	if( m_bHWIMesh )	return true;

	if( m_pAutoAABBs )
	{
		free(m_pAutoAABBs);
		m_pAutoAABBs = NULL;
	}
	
	m_pAutoAABBs = (A3DAABB *) malloc(sizeof(A3DAABB) * m_nFrameCount);
	if( NULL == m_pAutoAABBs )
		return false;

	for(int i=0; i<m_nFrameCount; i++)
	{
		m_pAutoAABBs[i] = GetAABB(m_ppVertsBuffer[i], m_nVertCount);
	}
	return true;
}

// Transform all vertex into a buffer;
bool A3DMesh::RenderToBuffer(A3DViewport * pCurrentViewport, int nStartVert, A3DVERTEX * pVertexBuffer, WORD * pIndices, A3DMATRIX4 absoluteTM,  int nCurrentFrame, int * pNewVerts, int * pNewIndices)
{
	if( m_bHWIMesh )	return true;

	if( m_pMaterialHost )
	{
		// We can not use optimized rendering buffer here; so render directly
		*pNewVerts = 0;
		*pNewIndices = 0;
		return Render(pCurrentViewport);
	}

	int			i;

	m_pA3DDevice->GetA3DEngine()->BeginPerformanceRecord(A3DENGINE_PERFORMANCE_MESHRENDERTOBUFFER);

	A3DVERTEX * pMyVertBuffer = m_ppVertsBuffer[nCurrentFrame];

	if( m_bHasLOD )
	{
		if( !UpdateLOD(pCurrentViewport, absoluteTM) )
			return false;
		
		for(i=0; i<m_nShowVertCount; i++)
		{
			pVertexBuffer[i] = pMyVertBuffer[i];
			
			A3DVECTOR3 vecPos(pMyVertBuffer[i].x, pMyVertBuffer[i].y, pMyVertBuffer[i].z);
			A3DVECTOR3 vecNormal(pMyVertBuffer[i].nx, pMyVertBuffer[i].ny, pMyVertBuffer[i].nz);
			vecPos = vecPos * absoluteTM;
			
			vecNormal.x = vecNormal.x * absoluteTM._11 + vecNormal.y * absoluteTM._21 + vecNormal.z * absoluteTM._31;
			vecNormal.y = vecNormal.x * absoluteTM._12 + vecNormal.y * absoluteTM._22 + vecNormal.z * absoluteTM._32;
			vecNormal.z = vecNormal.x * absoluteTM._13 + vecNormal.y * absoluteTM._23 + vecNormal.z * absoluteTM._33;

			pVertexBuffer[i].x = vecPos.x;
			pVertexBuffer[i].y = vecPos.y;
			pVertexBuffer[i].z = vecPos.z;
			pVertexBuffer[i].nx = vecNormal.x;
			pVertexBuffer[i].ny = vecNormal.y;
			pVertexBuffer[i].nz = vecNormal.z;
		}

		for(i=0; i<m_nShowIndexCount; i++)
		{
			pIndices[i] = m_pRDIndices[i] + nStartVert;
		}

		*pNewVerts = m_nShowVertCount;
		*pNewIndices = m_nShowIndexCount;
	}
	else
	{
		for(i=0; i<m_nVertCount; i++)
		{
			A3DVECTOR3 vecPos(pMyVertBuffer[i].x, pMyVertBuffer[i].y, pMyVertBuffer[i].z);
			A3DVECTOR3 vecNormal(pMyVertBuffer[i].nx, pMyVertBuffer[i].ny, pMyVertBuffer[i].nz);
			vecPos = vecPos * absoluteTM;
			vecNormal.x = vecNormal.x * absoluteTM._11 + vecNormal.y * absoluteTM._12 + vecNormal.z * absoluteTM._13;
			vecNormal.y = vecNormal.x * absoluteTM._21 + vecNormal.y * absoluteTM._22 + vecNormal.z * absoluteTM._23;
			vecNormal.z = vecNormal.x * absoluteTM._31 + vecNormal.y * absoluteTM._32 + vecNormal.z * absoluteTM._33;
			pVertexBuffer[i] = A3DVERTEX(vecPos, vecNormal, pMyVertBuffer[i].tu, pMyVertBuffer[i].tv);
		}

		for(i=0; i<m_nIndexCount; i++)
		{
			pIndices[i] = m_pIndices[i] + nStartVert;
		}

		*pNewVerts = m_nVertCount;
		*pNewIndices = m_nIndexCount;
	}

	m_pA3DDevice->GetA3DEngine()->EndPerformanceRecord(A3DENGINE_PERFORMANCE_MESHRENDERTOBUFFER);
	return true;
}

bool A3DMesh::GetNbrTriangle(int nTriIndex, int * pnNbrTriIndex)
{
	if( m_bHWIMesh )	return true;
	
	return true;
}

bool RayIntersectTriangle(A3DVECTOR3& vecStart, A3DVECTOR3& vecDelta, A3DVECTOR3 * pTriVerts, float * pvFraction, A3DVECTOR3 * pVecPos)
{
	FLOAT t, u, v;
	A3DVECTOR3 v0, v1, v2;

#define ERR_T	1e-6f

	v0 = pTriVerts[0];
	v1 = pTriVerts[1];
	v2 = pTriVerts[2];
	
	// Find vectors for two edges sharing vert0
	A3DVECTOR3 edge1 = v1 - v0;
	A3DVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	A3DVECTOR3 pvec;
	pvec = CrossProduct(vecDelta, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = DotProduct(edge1, pvec);
	if( det < ERR_T )
		return false;

	// Calculate distance from vert0 to ray origin
	A3DVECTOR3 tvec = vecStart - v0;

	// Calculate U parameter and test bounds
	u = DotProduct(tvec, pvec);
	if( u < -ERR_T || u > det + ERR_T )
		return false;

	// Prepare to test V parameter
	A3DVECTOR3 qvec;
	qvec = CrossProduct(tvec, edge1);

	// Calculate V parameter and test bounds
	v = DotProduct(vecDelta, qvec);
	if( v < -ERR_T || u + v > det + ERR_T)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	t = DotProduct(edge2, qvec);
	FLOAT fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	//If the line is short for touch that triangle;
	if( t < 0.0f || t > 1.0f )
		return false;

	* pvFraction = t;// * 0.99f;
	* pVecPos = vecStart + vecDelta * (* pvFraction);
	return true;
}

bool A3DMesh::RayTrace(A3DVECTOR3& vecStart, A3DVECTOR3& vecDelta, A3DVECTOR3 * pvecPos, A3DVECTOR3 * pvecNormal, float * pvFraction)
{
	if( m_nFrame < 0 ) // Not visible yet;
		return false;

	A3DVERTEX * pVerts = m_ppVertsBuffer[m_nFrame];
	WORD * pIndices = m_pIndices;

	bool			bIntersect = false;
	float			vFraction;
	A3DVECTOR3		vecHitPos;
	float			vMinFraction = 99999.9f;
	
	A3DVECTOR3		vecPos[3];
	int				nIndex;

	for(int i=0; i<m_nIndexCount; i+=3)
	{
		nIndex = pIndices[i];
		vecPos[0] = A3DVECTOR3(pVerts[nIndex].x, pVerts[nIndex].y, pVerts[nIndex].z);
		nIndex = pIndices[i + 1];
		vecPos[1] = A3DVECTOR3(pVerts[nIndex].x, pVerts[nIndex].y, pVerts[nIndex].z);
		nIndex = pIndices[i + 2];
		vecPos[2] = A3DVECTOR3(pVerts[nIndex].x, pVerts[nIndex].y, pVerts[nIndex].z);

		if( RayIntersectTriangle(vecStart, vecDelta, vecPos, &vFraction, &vecHitPos) )
		{
			if( vFraction < vMinFraction )
			{
				*pvFraction = vMinFraction = vFraction;
				*pvecPos = vecHitPos;
				bIntersect = true;
			}
		}
	}

	if( bIntersect )
	{
		// Should calcuate normal here;
		return true;
	}

	return false;
}
