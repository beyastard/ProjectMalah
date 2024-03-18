/*
 * FILE: AMVideoClip.cpp
 *
 * DESCRIPTION: The class standing for one video clip in Angelica Media Engine
 *
 * CREATED BY: Hedi, 2002/5/22
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2003 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _AMVIDEOCLIP_H_
#define _AMVIDEOCLIP_H_

#include "AMPlatform.h"
#include "AMTypes.h"
#include "AMVideoEngine.h"

#include <A3DDevice.h>
#include <A2DSprite.h>

#define AMVID_ALIGN_LEFT				0x1
#define AMVID_ALIGN_RIGHT				(1 << 1)

#define AMVID_ALIGN_TOP					(1 << 2)
#define AMVID_ALIGN_BOTTOM				(1 << 3)

#define AMVID_ALIGN_CENTERX				(1 << 4)
#define AMVID_ALIGN_CENTERY				(1 << 5)

// We will check fit flag first, if found one, the align flag will be ignored;
#define AMVID_FIT_KEEPCONSTRAINT		(1 << 6)
#define AMVID_FIT_X						(1 << 7)
#define AMVID_FIT_Y						(1 << 8)

enum AMVIDEOCLIP_STATE
{
	AMVIDEOCLIP_STATE_STOPPED = 0,
	AMVIDEOCLIP_STATE_PAUSED,
	AMVIDEOCLIP_STATE_PLAYING
};

class AMSurfaceRenderer;

class AMVideoClip
{
private:
	// class objects;
	AMVideoEngine *			m_pAMVideoEngine;
	AMSurfaceRenderer *		m_pSurfaceRenderer;
	CRITICAL_SECTION		m_SurfaceAccess;

	A3DDevice *				m_pA3DDevice;
	A2DSprite *				m_pScreenSprite;

	char					m_szClipName[MAX_PATH];

	// Direct show objects;
	IGraphBuilder *			m_pGraphBuilder;
	IMediaControl *			m_pMediaControl;
	IMediaEvent *			m_pMediaEvent;
	IMediaPosition *		m_pMediaPosition;
							  
	// Hanlde of the window that notify message will be passed;
	HWND					m_hNotifyWnd;

	// The state of the video clip;
	AMVIDEOCLIP_STATE		m_state;

	// video clip's properties;
	int						m_nWidth;
	int						m_nHeight;
	A3DFORMAT				m_VideoFormat;
	bool					m_bHasSound;

	// now for the scale factor
	// A flags combination to say how the put the video onto screen
	DWORD					m_dwAutoScaleFlags;

	// individual parameters for scale control;
	// each time you call SetAutoScaleFlags(), all below params will be recalculated;
	FLOAT					m_vScaleX;
	FLOAT					m_vScaleY;
	A3DPOINT2				m_ptVidPos;
	A3DCOLOR				m_color;
	
	bool					m_bLoop;
protected:

public:
	AMVideoClip();
	~AMVideoClip();

	bool Init(A3DDevice * pA3DDevice, AMVideoEngine * pVideoEngine, char * szVideoFile);
	bool Release();

	// Auto calculate the scale factors according to the dwFlags;
	bool SetAutoScaleFlags(DWORD dwFlags);

	bool SetBlendMode(A3DBLEND srcBlend, A3DBLEND destBlend);
	bool SetColor(A3DCOLOR color);
	bool SetScaleFactors(FLOAT vScaleX, FLOAT vScaleY);
	bool SetVidPos(A3DPOINT2 ptVidPos);

	bool Start(bool bLoop=false);
	bool Stop();
	bool Tick();

	bool DrawToBack();

	inline A2DSprite * GetScreenSprite() { return m_pScreenSprite; }
	inline char * GetClipName() { return m_szClipName; }
	inline int GetVideoWidth() { return m_nWidth; }
	inline int GetVideoHeight() { return m_nHeight; }
	inline A3DFORMAT GetVideoFormat() { return m_VideoFormat; }
	inline bool GetHasSound() { return m_bHasSound; }

	inline bool IsStopped() { return m_state == AMVIDEOCLIP_STATE_STOPPED; }
	inline bool IsPlaying() { return m_state == AMVIDEOCLIP_STATE_PLAYING; }
};

typedef class AMVideoClip * PAMVideoClip;

#endif