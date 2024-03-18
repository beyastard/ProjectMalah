#include "A3DFrame.h"
#include "A3DFuncs.h"
#include "A3DCamera.h"
#include "A3DEngine.h"
#include "A3DErrLog.h"
#include "A3DMeshSorter.h"
#include "A3DMeshCollector.h"
#include "A3DOBBFuncs.h"
#include "A3DViewport.h"
#include "A3DConfig.h"
#include "A3DCollision.h"

A3DFrame::A3DFrame()
{
	m_pA3DDevice = NULL;
	m_pParent = NULL;
	m_pRelativeTM = NULL;

	m_pAutoAABBs = NULL;
	m_pAutoOBBs = NULL;

	m_nBoundingBoxNum = 0;
	m_pA3DBox = NULL;
	m_pBoundingBox = NULL;
	m_nBoundingBoxNumRecursive = 0;
	
	m_nFrameRecursive = 0;
	m_nFrame = 0;
	m_vecPos = A3DVECTOR3(0.0f);

	m_nVertCount = 0;
	m_nIndexCount = 0;

	m_bDuplicated = false;
}

A3DFrame::~A3DFrame()
{
}

bool A3DFrame::Init(A3DDevice * pDevice, int nFrameCount)
{
	if( !m_ChildList.Init() )
		return false;
	if( !m_MeshList.Init() )
		return false;
	m_pA3DDevice = pDevice;

	m_nFrameCount = nFrameCount;
	m_nFrameCountRecursive = m_nFrameCount;

	m_pRelativeTM = (A3DMATRIX4 *) malloc(sizeof(A3DMATRIX4) * m_nFrameCount);
	if( NULL == m_pRelativeTM )
		return false;
	ZeroMemory(m_pRelativeTM, sizeof(A3DMATRIX4) * m_nFrameCount);
	m_nFrameRecursive = 0;
	m_nFrame = 0;
	m_nVertCount = 0;
	m_nIndexCount = 0;

	m_matAbsoluteTM = IdentityMatrix();

	m_nBoundingBoxNum = 0;
	m_nBoundingBoxNumRecursive = 0;

	m_pA3DBox = new A3DBox();
	if( NULL == m_pA3DBox ) return false;

	if( !m_pA3DBox->Init(m_pA3DDevice, A3DCOLORRGBA(0, 255, 0, 255)) ) 
		return false;
	return true;
}

bool A3DFrame::Release()
{
	m_pParent = NULL;

	if( !m_bDuplicated )
	{
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
		if( m_pRelativeTM )
		{
			free(m_pRelativeTM);
			m_pRelativeTM = NULL;
		}
		if( m_pBoundingBox )
		{
			free(m_pBoundingBox);
			m_pBoundingBox = NULL;
		}
		ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
		while( pThisMeshElement != 	m_MeshList.GetTail() )
		{
			A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
			pMesh->Release(); //We should not release mesh here, we should use a global meshman;
			delete pMesh;
			pThisMeshElement = pThisMeshElement->pNext;
		}
	}

	if( m_pA3DBox )
	{
		m_pA3DBox->Release();
		delete m_pA3DBox;
		m_pA3DBox = NULL;
	}

	//Then, release my childs;
	ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
	while( pThisChildElement != m_ChildList.GetTail() )
	{
		A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
		pFrame->Release();
		delete pFrame;
		pThisChildElement = pThisChildElement->pNext;
	}

	//Now release the list;
	m_ChildList.Release();
	m_MeshList.Release();
	return true;
}

bool A3DFrame::AddChild(A3DFrame * pChild)
{
	bool bval;
	bval = m_ChildList.Append((LPVOID) pChild);
	if( !bval ) return false;

	m_nVertCount += pChild->GetVertCount();
	m_nIndexCount += pChild->GetIndexCount();

	m_nBoundingBoxNumRecursive += pChild->GetFrameOBBNumRecursive();
	return true;
}

bool A3DFrame::AddMesh(A3DMesh * pMesh)
{
	bool		bval;
	bval = m_MeshList.Append((LPVOID) pMesh);
	if( !bval ) return false;

	m_nVertCount += pMesh->GetVertCount();
	m_nIndexCount += pMesh->GetIndexCount();
	return true;
}

A3DFrame * A3DFrame::GetFirstChildFrame()
{
	if( m_ChildList.GetSize() == 0 )
		return NULL;

	m_pLastFrameElement = m_ChildList.GetHead()->pNext;
	return (A3DFrame *) m_pLastFrameElement->pData;
}

A3DFrame * A3DFrame::GetNextChildFrame()
{
	if( m_pLastFrameElement )
	{
		m_pLastFrameElement = m_pLastFrameElement->pNext;
		if( m_ChildList.GetTail() == m_pLastFrameElement )
			return NULL;
		else
			return (A3DFrame *) m_pLastFrameElement->pData;
	}
	//Not in GetFirstChild call;
	return NULL;
}
	
bool A3DFrame::FindChildByName(char * szChildName, A3DFrame ** ppFrame)
{
	//Depth First Search;
	ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
	while( pThisChildElement != m_ChildList.GetTail() )
	{
		A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
		if( 0 == _stricmp(szChildName, pFrame->GetName()) )
		{
			*ppFrame = pFrame;
			return true;
		}
		else
		{
			if( pFrame->FindChildByName(szChildName, ppFrame) )
				return true;
		}

		pThisChildElement = pThisChildElement->pNext;
	}

	*ppFrame = NULL;
	return false;
}

A3DMesh * A3DFrame::GetFirstMesh()
{
	if( m_MeshList.GetSize() == 0 )
		return NULL;

	m_pLastMeshElement = m_MeshList.GetHead()->pNext;
	return (A3DMesh *) m_pLastMeshElement->pData;
}

A3DMesh * A3DFrame::GetNextMesh()
{
	if( m_pLastMeshElement )
	{
		m_pLastMeshElement = m_pLastMeshElement->pNext;
		if( m_MeshList.GetTail() == m_pLastMeshElement )
			return NULL;
		else
			return (A3DMesh *) m_pLastMeshElement->pData;
	}
	//Not in any GetFirstMesh Call;
	return NULL;
}

A3DMATRIX4& A3DFrame::GetAbsoluteTM()
{
	return m_matAbsoluteTM;
}

A3DMATRIX4& A3DFrame::GetRelativeTM(int nFrame)
{
	if( nFrame >= m_nFrameCount )
		nFrame = 0;

	return m_pRelativeTM[nFrame];
}

A3DMATRIX4& A3DFrame::GetRelativeTM()
{
	return m_pRelativeTM[m_nFrame];
}

bool A3DFrame::UpdateToFrame(int nFrame, A3DMATRIX4 * pMatParent)
{
	//Now determine the absolute Transform matrix and update the current frame number;
	m_nFrameRecursive = nFrame;
	m_nFrame = nFrame % m_nFrameCount;

	if( pMatParent )
	{
		m_matAbsoluteTM = GetRelativeTM() * (*pMatParent);
	}
	else if( m_pParent )
		m_matAbsoluteTM = GetRelativeTM() * m_pParent->GetAbsoluteTM();
	else
		m_matAbsoluteTM = GetRelativeTM();

	//First update my meshes;
	ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
	while( pThisMeshElement != 	m_MeshList.GetTail() )
	{
		A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
		if( !pMesh->UpdateToFrame(nFrame) )
			return false;
		pThisMeshElement = pThisMeshElement->pNext;
	}

	//Then, update my childs;
	ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
	while( pThisChildElement != m_ChildList.GetTail() )
	{
		A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
		if( !pFrame->UpdateToFrame(nFrame) )
			return false;
		pThisChildElement = pThisChildElement->pNext;
	}
	return true;
}

int A3DFrame::CalculateFrameCountRecursive()
{
	m_nFrameCountRecursive = m_nFrameCount;

	 //First update my meshes;
	ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
	while( pThisMeshElement != 	m_MeshList.GetTail() )
	{
		A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
		if( m_nFrameCountRecursive < pMesh->GetFrameCount() )
		{
			m_nFrameCountRecursive = pMesh->GetFrameCount();
		}
		pThisMeshElement = pThisMeshElement->pNext;
	}

	//Then, update my childs;
	ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
	while( pThisChildElement != m_ChildList.GetTail() )
	{
		A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
		if( m_nFrameCountRecursive < pFrame->CalculateFrameCountRecursive() )
		{
			m_nFrameCountRecursive = pFrame->GetFrameCountRecursive();
		}
		pThisChildElement = pThisChildElement->pNext;
	}

	return m_nFrameCountRecursive;
}

bool A3DFrame::Render(A3DViewport * pCurrentViewport, bool bNeedSort, bool bNeedCollect)
{
	if( IsEmptyFrame() )
	{
		if( 0 && m_pA3DDevice->GetA3DEngine()->GetBoxRenderFlag() )
		{
			A3DOBB obb;
			ZeroMemory(&obb, sizeof(A3DOBB));
			obb.XAxis.x = 1.0f;
			obb.YAxis.y = 1.0f;
			obb.ZAxis.z = 1.0f;
			obb.Extents = A3DVECTOR3(0.5f, 0.5f, 0.5f);
			CompleteOBB(&obb);
			m_pA3DBox->UpdateBox(obb, A3DCOLORRGBA(255, 0, 255, 255));
			m_pA3DDevice->SetWorldMatrix(m_matAbsoluteTM);
			m_pA3DBox->Render(pCurrentViewport);
		}
		return true;
	}

	//If this object is created outside D3D;
	if( m_pA3DDevice == NULL || g_pA3DConfig->GetRunEnv() == A3DRUNENV_PURESERVER )
		return true;

	//We just use the precalculated world transform matrix;
	m_pA3DDevice->SetWorldMatrix(m_matAbsoluteTM);

	//First render my meshes;
	if( m_MeshList.GetSize() )
	{
		ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
		while( pThisMeshElement != 	m_MeshList.GetTail() )
		{
			A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;

			if( bNeedCollect )
			{
				if( !m_pA3DDevice->GetA3DEngine()->GetMeshCollector()->PrepareMeshToRender(pMesh, pMesh->GetFrame(), m_matAbsoluteTM, pMesh->GetMeshCollectorStoreHandle()) )
					return false;
			}
			else if( pMesh->IsAlphaMesh() && bNeedSort )
			{
				if( !m_pA3DDevice->GetA3DEngine()->GetMeshSorter()->InsertMesh(m_matAbsoluteTM, m_pA3DDevice->GetA3DEngine()->GetCurIBLLightParam(), pMesh->GetFrame(), pMesh) )
					return false;
			}
			else
			{
				if( !pMesh->Render(pCurrentViewport) )
					return false;
			}
			pThisMeshElement = pThisMeshElement->pNext;
		}
	}

	//Last, we render the bounding boxes;
	if( 0 )//m_nBoundingBoxNum == 0 )
	{
		if( m_pAutoOBBs )
		{
			m_pA3DBox->UpdateBox(m_pAutoOBBs[m_nFrameRecursive], A3DCOLORRGBA(0, 128, 255, 128));
			m_pA3DBox->Render(pCurrentViewport);
		}
		if( m_pAutoAABBs )
		{
			m_pA3DBox->UpdateBox(m_pAutoAABBs[m_nFrameRecursive], A3DCOLORRGBA(255, 128, 0, 128));
			m_pA3DBox->Render(pCurrentViewport);
		}
	}
	else
	{
		for(int i=0; i<m_nBoundingBoxNum; i++)
		{
			m_pA3DBox->UpdateBox(GetFrameOBB(i).a3dOBB, A3DCOLORRGBA(0, 255, 0, 255));
			m_pA3DBox->Render(pCurrentViewport);
		}
	}
		
	//Then, render my childs;
	if( m_ChildList.GetSize() )
	{
		ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
		while( pThisChildElement != m_ChildList.GetTail() )
		{
			A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
			if( !pFrame->Render(pCurrentViewport, bNeedSort, bNeedCollect) )
				return false;
			pThisChildElement = pThisChildElement->pNext;
		}
	}
	return true;
}

bool A3DFrame::SetTM(A3DMATRIX4 TM, int nFrame)
{
	if( nFrame >= m_nFrameCount )
		return false;

	m_pRelativeTM[nFrame] = TM;
	return true;
}

bool A3DFrame::SetPos(A3DFrame * pRefFrame, A3DVECTOR3 vecPos, int nFrame)
{
	m_vecPos = vecPos;

	return true;
}

bool A3DFrame::SetOrientation(A3DFrame * pRefFrame, A3DVECTOR3 vecDir, A3DVECTOR3 vecUp, int nFrame)
{
	return true;
}

bool A3DFrame::AddRotation(A3DFrame * pRefFrame, A3DVECTOR3 vecAxis, FLOAT fDegree, int nFrame)
{
	return true;
}

bool A3DFrame::AddTranslation(A3DFrame * pRefFrame, A3DVECTOR3 vecOffset, int nFrame)
{
	return true;
}

bool A3DFrame::Scale(FLOAT fScale, int nFrame)
{
	return true;
}

A3DVECTOR3 A3DFrame::GetPos()
{
	return m_vecPos;
}

A3DVECTOR3 A3DFrame::Transform(A3DVECTOR3 vec)
{
	A3DVECTOR3 ret;
	A3DVECTOR4 ret4;

	//Before calling this you must call UpdateToFrame explicitly or the TM will not be 
	//corrent;

	ret = vec * m_matAbsoluteTM;
	return ret;
}

A3DVECTOR3 A3DFrame::InverseTransform(A3DVECTOR3 vec)
{
	A3DVECTOR3 ret = 0.0f;

	return ret;
}

bool A3DFrame::Save(AFile * pFileToSave)
{
	if( pFileToSave->IsBinary() )
	{
		DWORD dwWriteLength;
		DWORD dwData;
		char  szLineBuffer[2048];
		
		sprintf(szLineBuffer, "FRAME: %s", GetName());
		pFileToSave->Write(szLineBuffer, strlen(szLineBuffer) + 1, &dwWriteLength);

		//Save Frame matrix;
		pFileToSave->Write(&m_nFrameCount, sizeof(int), &dwWriteLength);
		pFileToSave->Write(m_pRelativeTM, sizeof(A3DMATRIX4) * m_nFrameCount, &dwWriteLength);

		//Save Bounding Box;
		pFileToSave->Write(&m_nBoundingBoxNum, sizeof(int), &dwWriteLength);
		pFileToSave->Write(m_pBoundingBox, sizeof(A3DFRAMEOBB) * m_nBoundingBoxNum, &dwWriteLength);

		//Now save my meshes;
		dwData = m_MeshList.GetSize();
		pFileToSave->Write(&dwData, sizeof(DWORD), &dwWriteLength);
		
		ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
		while( pThisMeshElement != 	m_MeshList.GetTail() )
		{
			A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
			if( !pMesh->Save(pFileToSave) )
				return false;
			pThisMeshElement = pThisMeshElement->pNext;
		}
		
		dwData = m_ChildList.GetSize();
		pFileToSave->Write(&dwData, sizeof(DWORD), &dwWriteLength);
		
		ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
		while( pThisChildElement != m_ChildList.GetTail() )
		{
			A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
			if( !pFrame->Save(pFileToSave) )
				return false;
			pThisChildElement = pThisChildElement->pNext;
		}
	}
	else
	{
		char szLineBuffer[AFILE_LINEMAXLEN];
		sprintf(szLineBuffer, "FRAME: %s\n{", GetName());
		pFileToSave->WriteLine(szLineBuffer);

		sprintf(szLineBuffer, "FRAMECOUNT: %d\n{", m_nFrameCount);
		pFileToSave->WriteLine(szLineBuffer);
		for(int i=0; i<m_nFrameCount; i++)
		{
			sprintf(szLineBuffer, "\t(%d):", i);
			pFileToSave->WriteLine(szLineBuffer);
			for(int j=0; j<4; j++)
			{
				sprintf(szLineBuffer, "\t\t[%f, %f, %f, %f]", m_pRelativeTM[i].m[j][0],
					m_pRelativeTM[i].m[j][1], m_pRelativeTM[i].m[j][2], m_pRelativeTM[i].m[j][3]);
				pFileToSave->WriteLine(szLineBuffer);
			}
		}
		//<== FRAMECOUNT: %d {
		pFileToSave->WriteLine("}");

		//Save Bounding Boxes;
		sprintf(szLineBuffer, "BOUNDINGBOXNUM: %d\n{", m_nBoundingBoxNum);
		pFileToSave->WriteLine(szLineBuffer);
		for(int i=0; i<m_nBoundingBoxNum; i++)
		{
			sprintf(szLineBuffer, "%s", m_pBoundingBox[i].szName);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f)", m_pBoundingBox[i].a3dOBB.Center.x, m_pBoundingBox[i].a3dOBB.Center.y, m_pBoundingBox[i].a3dOBB.Center.z);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f)", m_pBoundingBox[i].a3dOBB.XAxis.x, m_pBoundingBox[i].a3dOBB.XAxis.y, m_pBoundingBox[i].a3dOBB.XAxis.z);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f)", m_pBoundingBox[i].a3dOBB.YAxis.x, m_pBoundingBox[i].a3dOBB.YAxis.y, m_pBoundingBox[i].a3dOBB.YAxis.z);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f)", m_pBoundingBox[i].a3dOBB.ZAxis.x, m_pBoundingBox[i].a3dOBB.ZAxis.y, m_pBoundingBox[i].a3dOBB.ZAxis.z);
			pFileToSave->WriteLine(szLineBuffer);
			sprintf(szLineBuffer, "(%f, %f, %f)", m_pBoundingBox[i].a3dOBB.Extents.x, m_pBoundingBox[i].a3dOBB.Extents.y, m_pBoundingBox[i].a3dOBB.Extents.z);
			pFileToSave->WriteLine(szLineBuffer);
		}
		//<== BOUNDINGBOXNUM: %d {
		pFileToSave->WriteLine("}");

		//Now save my meshes;
		sprintf(szLineBuffer, "MESHCOUNT: %d\n{", m_MeshList.GetSize());
		pFileToSave->WriteLine(szLineBuffer);
		
		ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
		while( pThisMeshElement != 	m_MeshList.GetTail() )
		{
			A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
			if( !pMesh->Save(pFileToSave) )
				return false;
			pThisMeshElement = pThisMeshElement->pNext;
		}
		//<== MESHCOUNT: %d	{
		pFileToSave->WriteLine("}");

		sprintf(szLineBuffer, "CHILDCOUNT: %d\n{", m_ChildList.GetSize());
		pFileToSave->WriteLine(szLineBuffer);
		
		ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
		while( pThisChildElement != m_ChildList.GetTail() )
		{
			A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
			if( !pFrame->Save(pFileToSave) )
				return false;
			pThisChildElement = pThisChildElement->pNext;
		}
		// <== CHILDCOUNT: %d { 
		pFileToSave->WriteLine("}");

		// <== FRAME: %s { 
		pFileToSave->WriteLine("}");
	}
	return true;
}

bool A3DFrame::Load(A3DDevice * pDevice, AFile * pFileToLoad)
{
	if( pFileToLoad->IsBinary() )
	{
		DWORD dwData;
		DWORD dwReadLength;
		char  szLineBuffer[2048];
		char  szResult[2048];
		int	  i, nBoxNum;
		DWORD n;

		pFileToLoad->ReadString(szLineBuffer, 2048, &dwReadLength);
		pFileToLoad->GetStringAfter(szLineBuffer, "FRAME: ", szResult);
		if( strlen(szResult) >= MAX_NAME )//Format error;
			return false;
		SetName(szResult);

		pFileToLoad->Read(&m_nFrameCount, sizeof(int), &dwReadLength);

		//if( m_nFrameCount > MAX_FRAME )
		//	return false;

		if( !Init(pDevice, m_nFrameCount) )
			return false;

		pFileToLoad->Read(m_pRelativeTM, sizeof(A3DMATRIX4) * m_nFrameCount, &dwReadLength);
		
		//Load Bounding Box;
		pFileToLoad->Read(&nBoxNum, sizeof(int), &dwReadLength);
		
		for(i=0; i<nBoxNum; i++)
		{
			A3DFRAMEOBB obb;
			pFileToLoad->Read(&obb, sizeof(A3DFRAMEOBB), &dwReadLength);
			CompleteOBB(&obb.a3dOBB);
			if( !AddBoundingBox(obb.szName, obb.a3dOBB, obb.property) )
			{
				return false;
			}
		}

		//Now load my meshes;
		pFileToLoad->Read(&dwData, sizeof(DWORD), &dwReadLength);
		for(n=0; n<dwData; n++)
		{
			A3DMesh * pMesh = new A3DMesh();
			if( NULL == pMesh )
				return false;
			
			if( !pMesh->Load(pDevice, pFileToLoad) )
				return false;

			if( g_pA3DConfig->GetRunEnv() == A3DRUNENV_PURESERVER )
			{
				// we need not the mesh any more;
				pMesh->Release();
				delete pMesh;
			}
			else
				AddMesh(pMesh);
		}
		
		//now load my childs;
		pFileToLoad->Read(&dwData, sizeof(DWORD), &dwReadLength);
		for(n=0; n<dwData; n++)
		{
			A3DFrame * pFrame = new A3DFrame();
			if( NULL == pFrame )
				return false;

			if( !pFrame->Load(pDevice, pFileToLoad) )
				return false;

			AddChild(pFrame);
			pFrame->SetParent(this);
		}
	}
	else
	{
		char	szLineBuffer[AFILE_LINEMAXLEN];
		char	szResult[AFILE_LINEMAXLEN];
		DWORD	dwReadLen;
		int		nMeshCount, nChildCount, nBoxNum;
		int		i;

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		pFileToLoad->GetStringAfter(szLineBuffer, "FRAME: ", szResult);
		SetName(szResult);

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "{") )
			return false;

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		pFileToLoad->GetStringAfter(szLineBuffer, "FRAMECOUNT: ", szResult);
		m_nFrameCount = atoi(szResult);
		if( m_nFrameCount <= 0 )
			return false;

		if( !Init(pDevice, m_nFrameCount) )
			return false;

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "{") != 0 )
			return false;

		for(i=0; i<m_nFrameCount; i++)
		{
			int n;
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			sscanf(szLineBuffer, "\t(%d):", &n);
			if( n != i )
				return false;
			for(int j=0; j<4; j++)
			{
				pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
				sscanf(szLineBuffer, "\t\t[%f, %f, %f, %f]", &m_pRelativeTM[i].m[j][0],
					&m_pRelativeTM[i].m[j][1], &m_pRelativeTM[i].m[j][2], &m_pRelativeTM[i].m[j][3]);
			}
		}

		//<== FRAMECOUNT: %d {
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "}") )
			return false;

		//Load Bounding Boxes;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		pFileToLoad->GetStringAfter(szLineBuffer, "BOUNDINGBOXNUM: ", szResult);
		nBoxNum = atoi(szResult);
		if( nBoxNum < 0 )
			return false;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "{") != 0 )
			return false;
		for(i=0; i<nBoxNum; i++)
		{
			char				szName[32];
			A3DOBB				box;
			A3DFRAMEOBB_PROP	property; // Not implemented yet;

			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			strncpy(szName, szLineBuffer, 32);
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			sscanf(szLineBuffer, "(%f, %f, %f)", &box.Center.x, &box.Center.y, &box.Center.z);
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			sscanf(szLineBuffer, "(%f, %f, %f)", &box.XAxis.x, &box.XAxis.y, &box.XAxis.z);
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			sscanf(szLineBuffer, "(%f, %f, %f)", &box.YAxis.x, &box.YAxis.y, &box.YAxis.z);				
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			sscanf(szLineBuffer, "(%f, %f, %f)", &box.ZAxis.x, &box.ZAxis.y, &box.ZAxis.z);				
			pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
			sscanf(szLineBuffer, "(%f, %f, %f)", &box.Extents.x, &box.Extents.y, &box.Extents.z);				
			CompleteOBB(&box);
			if( !AddBoundingBox(szName, box, property) )
				return false;
		}
		//<== BOUNDINGBOXNUM: %d {
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "}") != 0 )
			return false;

		//Now load my meshes;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		pFileToLoad->GetStringAfter(szLineBuffer, "MESHCOUNT: ", szResult);
		nMeshCount = atoi(szResult);

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "{") )
			return false;

		for(i=0; i<nMeshCount; i++)
		{
			A3DMesh * pMesh = new A3DMesh();
			
			if( !pMesh->Load(pDevice, pFileToLoad) )
				return false;

			if( g_pA3DConfig->GetRunEnv() == A3DRUNENV_PURESERVER )
			{
				pMesh->Release();
				delete pMesh;
			}
			else
				AddMesh(pMesh);
		}
		
		//<== MESHCOUNT: %d	{
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "}") )
			return false;

		//now load my childs;
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		pFileToLoad->GetStringAfter(szLineBuffer, "CHILDCOUNT: ", szResult);
		nChildCount = atoi(szResult);

		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "{") )
			return false;

		for(i=0; i<nChildCount; i++)
		{
			A3DFrame * pFrame = new A3DFrame();

			if( !pFrame->Load(pDevice, pFileToLoad) )
				return false;

			AddChild(pFrame);
			pFrame->SetParent(this);
		}
		
		// <== CHILDCOUNT: {
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "}") )
			return false;
		
		// <== FRAME: %s { 
		pFileToLoad->ReadLine(szLineBuffer, AFILE_LINEMAXLEN, &dwReadLen);
		if( strcmp(szLineBuffer, "}") )
			return false;

	}

	UpdateToFrame(0);
	CalculateFrameCountRecursive();

	return true;
}
	
bool A3DFrame::AddBoundingBox(char * szName, const A3DOBB& a3dOBB, const A3DFRAMEOBB_PROP& property)
{
	m_pBoundingBox = (A3DFRAMEOBB *) realloc(m_pBoundingBox, sizeof(A3DFRAMEOBB) * (m_nBoundingBoxNum + 1));
	if( NULL == m_pBoundingBox )
	{
		g_pA3DErrLog->ErrLog("A3DFrame::AddBoundingBox Not enough memory!");
		return false;
	}
	
	strncpy(m_pBoundingBox[m_nBoundingBoxNum].szName, szName, 32);
	m_pBoundingBox[m_nBoundingBoxNum].a3dOBB = a3dOBB;
	m_pBoundingBox[m_nBoundingBoxNum].property = property;

	m_nBoundingBoxNum ++;
	m_nBoundingBoxNumRecursive ++;
	return true;
}

bool A3DFrame::BuildAutoOBB()
{
	// See if it is an empty frame;
	if( IsEmptyFrame() )
		return true;

	if( m_pAutoOBBs )
	{
		free(m_pAutoOBBs);
		m_pAutoOBBs = NULL;
	}

	m_pAutoOBBs = (A3DOBB *) malloc(sizeof(A3DOBB) * m_nFrameCountRecursive);
	if( NULL == m_pAutoOBBs )
	{
		g_pA3DErrLog->ErrLog("A3DFrame::BuildAutoOBB Not enough memory!");
		return false;
	}
	ZeroMemory(m_pAutoOBBs, sizeof(A3DOBB) * m_nFrameCountRecursive);

	int			i;
	for(i=0; i<m_nFrameCountRecursive; i++)
	{
		UpdateToFrame(i);
		m_pAutoOBBs[i] = GetChildOBB();
	}
	return true;
}

bool A3DFrame::BuildAutoAABB()
{
	// See if it is an empty frame;
	if( IsEmptyFrame() )
		return true;

	if( m_pAutoAABBs )
	{
		free(m_pAutoAABBs);
		m_pAutoAABBs = NULL;
	}

	m_pAutoAABBs = (A3DAABB *) malloc(sizeof(A3DAABB) * m_nFrameCountRecursive);
	if( NULL == m_pAutoAABBs )
	{
		g_pA3DErrLog->ErrLog("A3DFrame::BuildAutoAABB Not enough memory!");
		return false;
	}
	ZeroMemory(m_pAutoAABBs, sizeof(A3DAABB) * m_nFrameCountRecursive);

	int			i;
	for(i=0; i<m_nFrameCountRecursive; i++)
	{
		UpdateToFrame(i);
		m_pAutoAABBs[i] = GetChildAABB();
	}
	return true;
}

A3DOBB A3DFrame::GetChildOBB()
{
	A3DOBB a3dOBB;
	ZeroMemory(&a3dOBB, sizeof(A3DOBB));

	// First use my child mesh's obbs;
	ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
	while( pThisMeshElement != 	m_MeshList.GetTail() )
	{
		A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
		A3DOBB	obb = pMesh->GetMeshAutoOBB(pMesh->GetFrame());

		if( a3dOBB.Extents.x == 0.0f && a3dOBB.Extents.y == 0.0f && a3dOBB.Extents.z == 0.0f )
			a3dOBB = obb;
		else
			a3dOBB = GetOBB(a3dOBB, obb);

		pThisMeshElement = pThisMeshElement->pNext;
	}
	
	// Then use my child frame's obbs;
	ALISTELEMENT * pThisChildFrameElement = m_ChildList.GetFirst();
	while( pThisChildFrameElement != m_ChildList.GetTail() )
	{
		A3DVECTOR3 vecOrg;
		A3DFrame * pFrame = (A3DFrame *) pThisChildFrameElement->pData;

		if( !pFrame->IsEmptyFrame() )
		{
			A3DOBB		obb = pFrame->GetChildOBB();
			A3DMATRIX4	tm = pFrame->GetRelativeTM();

			//Now update the obb according the frame's obb and this tm;
			obb.Center = obb.Center * tm;
			vecOrg = tm.GetRow(3);
			obb.XAxis = obb.XAxis * tm - vecOrg;
			obb.YAxis = obb.YAxis * tm - vecOrg;
			obb.ZAxis = obb.ZAxis * tm - vecOrg;
			CompleteOBB(&obb);

			if( a3dOBB.Extents.x == 0.0f && a3dOBB.Extents.y == 0.0f && a3dOBB.Extents.z == 0.0f )
				a3dOBB = obb;
			else
				a3dOBB = GetOBB(a3dOBB, obb);
		}

		pThisChildFrameElement = pThisChildFrameElement->pNext;
	}

	return a3dOBB;
}

A3DAABB A3DFrame::GetChildAABB()
{
	A3DAABB a3dAABB;

	ClearAABB(a3dAABB.Mins, a3dAABB.Maxs);
	// First use my child mesh's obbs;
	ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
	while( pThisMeshElement != m_MeshList.GetTail() )
	{
		A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
		A3DAABB	aabb = pMesh->GetMeshAutoAABB(pMesh->GetFrame());

		ExpandAABB(a3dAABB.Mins, a3dAABB.Maxs, aabb);
		pThisMeshElement = pThisMeshElement->pNext;
	}
	
	// Then use my child frame's aabbs;
	ALISTELEMENT * pThisChildFrameElement = m_ChildList.GetFirst();
	while( pThisChildFrameElement != m_ChildList.GetTail() )
	{
		A3DVECTOR3 vecOrg;
		A3DFrame * pFrame = (A3DFrame *) pThisChildFrameElement->pData;

		if( !pFrame->IsEmptyFrame() )
		{
			A3DAABB		aabb = pFrame->GetChildAABB();
			A3DOBB		obb;
			A3DMATRIX4	tm = pFrame->GetRelativeTM();

			//Now compose an obb according to the frame's obb and its tm;
			obb.Center = aabb.Center * tm;
			obb.XAxis = tm.GetRow(0);
			obb.YAxis = tm.GetRow(1);
			obb.ZAxis = tm.GetRow(2);
			obb.Extents = aabb.Extents;
			CompleteOBB(&obb);

			ExpandAABB(a3dAABB.Mins, a3dAABB.Maxs, obb);
		}

		pThisChildFrameElement = pThisChildFrameElement->pNext;
	}

	CompleteAABB(&a3dAABB);
	return a3dAABB;
}

bool A3DFrame::Duplicate(A3DDevice * pA3DDevice, A3DFrame * pOriginFrame)
{
	m_bDuplicated = true;

	// First duplicate myself;
	m_pA3DDevice = pA3DDevice;
	m_ChildList.Init();
	m_MeshList.Init();
	
	// Use direct;
	m_pRelativeTM = pOriginFrame->GetRelativeTMPointer();
	m_nVertCount = pOriginFrame->GetVertCount();
	m_nIndexCount = pOriginFrame->GetIndexCount();
	m_nFrameCountRecursive = pOriginFrame->GetFrameCountRecursive();
	m_nFrameCount = pOriginFrame->GetFrameCount();
	
	m_nFrameRecursive = 0;
	m_nFrame = 0;
	
	m_matAbsoluteTM = IdentityMatrix();

	m_pA3DBox = new A3DBox();
	if( NULL == m_pA3DBox ) return false;

	if( !m_pA3DBox->Init(m_pA3DDevice, A3DCOLORRGBA(0, 255, 0, 255)) ) 
		return false;

	// Use direct;
	m_pAutoOBBs = pOriginFrame->GetFrameAutoOBBPointer();
	m_pAutoAABBs = pOriginFrame->GetFrameAutoAABBPointer();

	m_nBoundingBoxNum = pOriginFrame->GetFrameOBBNum();
	// Use direct;
	m_pBoundingBox = pOriginFrame->GetFrameOBBPointer();

	// Now duplicate the meshes;
	A3DMesh * pMesh = pOriginFrame->GetFirstMesh();
	while( pMesh )
	{
		// Use direct;
		m_MeshList.Append((LPVOID) pMesh);
		pMesh = pOriginFrame->GetNextMesh();
	}

	A3DFrame * pFrame = pOriginFrame->GetFirstChildFrame();
	while( pFrame )
	{
		A3DFrame * pNewChildFrame = new A3DFrame();
		if( NULL == pNewChildFrame )
		{
			g_pA3DErrLog->ErrLog("A3DFrame::Duplicate(), Not enough memory!");
			return false;
		}
		if( !pNewChildFrame->Duplicate(m_pA3DDevice, pFrame) )
		{
			g_pA3DErrLog->ErrLog("A3DFrame::Duplicate(), Call child frame's duplicate fail!");
			return false;
		}

		pNewChildFrame->SetParent(this);
		m_ChildList.Append((LPVOID) pNewChildFrame);

		pFrame = pOriginFrame->GetNextChildFrame();
	}

	// Then duplicate my child frame;
	m_vecPos = A3DVECTOR3(0.0f);
	SetName(pOriginFrame->GetName());
	return true;
}

// Notice: vecStart and vecDelta must in world coordinates;
bool A3DFrame::RayTraceMesh(A3DVECTOR3& vecStart, A3DVECTOR3& vecDelta, A3DVECTOR3 * pvecPos, A3DVECTOR3 * pvecNormal, float * pvFraction)
{
	A3DMATRIX4 matInvAbsTM = InverseTM(m_matAbsoluteTM);
	A3DVECTOR3 vecLocalStart, vecLocalEnd, vecLocalDelta;
	vecLocalStart = vecStart * matInvAbsTM;
	vecLocalEnd = (vecStart + vecDelta) * matInvAbsTM;
	vecLocalDelta = vecLocalEnd - vecLocalStart;

	float		vMinFraction = 99999.9f;
	float		vFraction;
	A3DVECTOR3  vecHitPos;
	A3DVECTOR3	vecHitNormal;
	bool		bIntersect = false;

	// First if it is not intersect my bounding box, it will not intersect me
	if( m_pAutoAABBs )
	{
		A3DAABB aabb = m_pAutoAABBs[m_nFrame];
		if( !CLS_RayToAABB3(vecLocalStart, vecLocalDelta, aabb.Mins, aabb.Maxs, vecHitPos, &vFraction, vecHitNormal) )
			return false;
	}

	A3DMesh * pMesh = GetFirstMesh();
	while( pMesh )
	{
		if( pMesh->RayTrace(vecLocalStart, vecLocalDelta, &vecHitPos, &vecHitNormal, &vFraction) )
		{
			if( vFraction < vMinFraction )
			{
				*pvFraction = vMinFraction = vFraction;
				*pvecPos = vecHitPos;
				*pvecNormal = vecHitNormal;
				bIntersect = true;
			}
		}

		pMesh = GetNextMesh();
	}

	A3DFrame * pFrame = GetFirstChildFrame();
	while( pFrame )
	{
		if( pFrame->RayTraceMesh(vecStart, vecDelta, &vecHitPos, &vecHitNormal, &vFraction) )
		{
			if( vFraction < vMinFraction )
			{
				*pvFraction = vMinFraction = vFraction;
				*pvecPos = vecHitPos;
				*pvecNormal = vecHitNormal;
				bIntersect = true;
			}
		}

		pFrame = GetNextChildFrame();
	}

	if( bIntersect )
	{
		*pvecPos = *pvecPos * m_matAbsoluteTM;
		*pvecNormal = *pvecNormal * m_matAbsoluteTM - m_matAbsoluteTM.GetRow(3);
		return true;
	}

	return false;
}

bool A3DFrame::SetExtraMaterial(A3DMaterial * pMaterial)
{
	ALISTELEMENT * pThisMeshElement = m_MeshList.GetHead()->pNext;
	while( pThisMeshElement != 	m_MeshList.GetTail() )
	{
		A3DMesh * pMesh = (A3DMesh *) pThisMeshElement->pData;
		pMesh->SetHostMaterial(pMaterial);

		pThisMeshElement = pThisMeshElement->pNext;
	}

	//Then, update my childs;
	ALISTELEMENT * pThisChildElement = m_ChildList.GetHead()->pNext;
	while( pThisChildElement != m_ChildList.GetTail() )
	{
		A3DFrame * pFrame = (A3DFrame *) pThisChildElement->pData;
		if( !pFrame->SetExtraMaterial(pMaterial) )
			return false;

		pThisChildElement = pThisChildElement->pNext;
	}
	return true;
}