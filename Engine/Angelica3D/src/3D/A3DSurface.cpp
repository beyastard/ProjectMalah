#include "A3DSurface.h"
#include "A3DErrLog.h"
#include "A3DConfig.h"

A3DSurface::A3DSurface()
{
	m_pA3DDevice		= NULL;
	m_pDXSurface		= NULL;
	m_szSurfaceMap[0]	= '\0';

	m_bHWISurface		= false;
}

A3DSurface::~A3DSurface()
{
}

bool A3DSurface::Init(A3DDevice * pA3DDevice, IDirect3DSurface8 * pDXSurface, char * szMapFile)
{
	m_pA3DDevice = pA3DDevice;
	m_pDXSurface = pDXSurface;
	strncpy(m_szSurfaceMap, szMapFile, MAX_PATH);

	if( g_pA3DConfig->GetRunEnv() == A3DRUNENV_PURESERVER )
	{
		m_bHWISurface = true;
		return true;
	}

	D3DSURFACE_DESC		desc; 
	pDXSurface->GetDesc(&desc);

	m_nWidth = desc.Width;
	m_nHeight = desc.Height;   
	m_format = (A3DFORMAT) desc.Format;
	return true;
}

bool A3DSurface::Release()
{
	if( m_pDXSurface )
	{
		m_pDXSurface->Release();
		m_pDXSurface = NULL;
	}
	return true;
}

bool A3DSurface::DrawToBack(int x, int y)
{
	if( m_bHWISurface )		return true;

	bool bval;

	bval = m_pA3DDevice->CopyToBack(x, y, m_nWidth, m_nHeight, m_pDXSurface);
	if( !bval )
		return false;

	return true;
}

bool A3DSurface::Create(A3DDevice* pDevice, int iWidth, int iHeight, A3DFORMAT Format)
{
	if( m_pDXSurface )
	{
		g_pA3DErrLog->ErrLog("A3DSurface::Create(), This surface has been created, can not recreated!");
		return false;
	}

	IDirect3DSurface8 * pDXSurface;
	if (FAILED(pDevice->GetD3DDevice()->CreateImageSurface(iWidth, iHeight, 
		(D3DFORMAT)Format, &pDXSurface)))
		return false;

	if (!Init(pDevice, pDXSurface, "CREATED_SURFACE"))
		return false;

	return true;
}

bool A3DSurface::LockRect(RECT* pRect, void** ppData, int* iPitch, DWORD dwFlags)
{
	D3DLOCKED_RECT LockedRect;

	if (!m_pDXSurface)
		return false;

	if (FAILED(m_pDXSurface->LockRect(&LockedRect, pRect, 0)))
		return false;

	*ppData = LockedRect.pBits;
	*iPitch = LockedRect.Pitch;

	return true;

}

bool A3DSurface::UnlockRect()
{
	if (!m_pDXSurface)
		return false;

	if (FAILED(m_pDXSurface->UnlockRect()))
		return false;

	return true;
}
