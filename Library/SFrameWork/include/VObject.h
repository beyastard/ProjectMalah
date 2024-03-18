/*
 * FILE: VObject.h
 *
 * DESCRIPTION: virtual entity object
 *
 * CREATED BY: Hedi, 2002/9/19
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#ifndef _VOBJECT_H_
#define _VOBJECT_H_

class VObject
{
private:

protected:

public:
	VObject();
	~VObject();

	virtual bool Tick(float vDeltaTime) = 0;
	virtual bool Render(A3DViewport * pCurrentViewport) = 0;
};

#endif//_VOBJECT_H_