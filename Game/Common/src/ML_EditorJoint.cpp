/*
 * FILE: ML_EditorJoint.h
 *
 * DESCRIPTION: Joint to Malah editor
 *
 * CREATED BY: Duyuxin, 2002/11/19
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#include "ML_ABase.h"
#include "ML_EditorJoint.h"
#include "ML_ID.h"
#include "ML_ABasePatch.h"

///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement namespace EditorJoint
//
///////////////////////////////////////////////////////////////////////////

namespace EditorJoint
{
	/*	Create a data object according to specified ID

		Return object's address for success, otherwise return NULL.
	*/
	AData* CreateDataObject(int iID)
	{
		if (iID == ID_SKYOBJECT)	//	Is sky object
		{
			CMLSky* pSkyData = new CMLSky;
			if (!pSkyData)
			{
				assert(0);
				return NULL;
			}

			pSkyData->SetId(iID);
			pSkyData->Init();

			return pSkyData;
		}
		else if (iID == ID_ENVOBJECT)	//	Is environment object
		{
			CMLEnvironment* pEnvData = new CMLEnvironment;
			if (!pEnvData)
			{
				assert(0);
				return NULL;
			}

			pEnvData->SetId(iID);
			pEnvData->Init();

			return pEnvData;
		}

		return NULL;
	}
}


