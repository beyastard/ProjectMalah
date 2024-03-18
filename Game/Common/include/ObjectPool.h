/*
 * FILE: ObjectPool.h
 *
 * DESCRIPTION: Template used to manager large number of objects which is 
 *				allocated and freed	frequently
 *
 * CREATED BY: Duyuxin, 2002/12/27
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#ifndef	_OBJECTPOOL_H_
#define _OBJECTPOOL_H_

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Class CObjectPool
//
///////////////////////////////////////////////////////////////////////////

template <class T> class CObjectPool
{
public:		//	Types

	struct OBJECTNODE
	{
		T*			pObject;	//	Object pointer
		bool		bUsed;		//	Used flag
		OBJECTNODE*	pNext;		//	Next node
	};
	
	struct OBJECTPOOL
	{
		T*			aObjects;	//	Object pool
		OBJECTNODE*	aNodes;		//	Nodes list
		OBJECTPOOL*	pNext;		//	Point to next pool
	};

public:		//	Constructor and Destructor

	CObjectPool();
	virtual ~CObjectPool();

public:		//	Attributes

public:		//	Operations

	bool		Init(int iPoolSize=32);		//	Initialize object
	bool		Release();					//	Release object

	T*			AllocObject(bool* pbNew=NULL);	//	New a object
	void		FreeObject(T* pObject);			//	Free a object

	int			CheckObjectLeak();	//	Check whether all allocated objects have been freed

protected:	//	Attributes

	OBJECTNODE*	m_pFreeHead;		//	Head of free object list
	OBJECTNODE*	m_pUsedHead;		//	Head of used node list
	OBJECTPOOL*	m_pPoolHead;		//	Head of pool list
	int			m_iPoolSize;		//	Size of pool in objects

protected:	//	Operations

	void		Constructor();		//	Constructor
	OBJECTNODE*	AllocNewPool();		//	Allocate a new object pool
};

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////

template <class T>
CObjectPool<T>::CObjectPool()
{
	Constructor();
}

template <class T>
CObjectPool<T>::~CObjectPool()
{
}

template <class T>
void CObjectPool<T>::Constructor()
{
	m_pFreeHead	= NULL;
	m_pPoolHead	= NULL;
	m_pUsedHead = NULL;
	m_iPoolSize	= 32;
}

/*	Initialize object

	iPoolSize: size of pool in objects
*/
template <class T>
bool CObjectPool<T>::Init(int iPoolSize/* 32 */)
{
	if (iPoolSize < 1)
		return false;

	m_iPoolSize = iPoolSize;

	return true;
}

//	Release object
template <class T>
bool CObjectPool<T>::Release()
{
	//	Release all resources
	while (m_pPoolHead)
	{
		OBJECTPOOL* pPool = m_pPoolHead;
		m_pPoolHead = m_pPoolHead->pNext;

		delete [] pPool->aObjects;
		delete [] pPool->aNodes;
		delete pPool;		
	}

	Constructor();

	return true;
}

/*	Check whether all allocated objects have been freed

	Return values:
	
	  -1: it's impossible to check object leak.
	  >=0: number of leaked objects.
*/
template <class T>
int	CObjectPool<T>::CheckObjectLeak()
{
	int iNumLeak = 0;

	OBJECTNODE* pNode = m_pUsedHead;
	while (pNode)
	{
		iNumLeak++;
		pNode = pNode->pNext;
	}

	return iNumLeak;
}

//	Allocate a new object node
template <class T>
typename CObjectPool<T>::OBJECTNODE*	CObjectPool<T>::AllocNewPool()
{
	OBJECTPOOL* pPool = new OBJECTPOOL;
	if (!pPool)
		return NULL;

	//	Allocate object pool
	pPool->aObjects = new T[m_iPoolSize];
	if (!pPool->aObjects)
	{
		delete pPool;
		return NULL;
	}

	//	Allocate node list
	pPool->aNodes = new OBJECTNODE[m_iPoolSize];
	if (!pPool->aNodes)
	{
		delete pPool->aObjects;
		delete pPool;
		return NULL;
	}

	//	Initialize nodes
	int i;
	for (i=0; i < m_iPoolSize-1; i++)
	{
		pPool->aNodes[i].pObject	= &pPool->aObjects[i];
		pPool->aNodes[i].bUsed		= false;
		pPool->aNodes[i].pNext		= &pPool->aNodes[i+1];
	}

	//	Handle the last node
	pPool->aNodes[i].pObject	= &pPool->aObjects[i];
	pPool->aNodes[i].bUsed		= false;
	pPool->aNodes[i].pNext		= NULL;

	//	Add this pool to pool list
	pPool->pNext = m_pPoolHead;
	m_pPoolHead  = pPool;

	return pPool->aNodes;
}

/*	New a object of type T

	Return NULL if failed

	pbNew (out): if *pbNew is true, it indicates the allocated object is a
				 completly new one. if *pbNew is false, the allocated object is
				 gotten from freed pool.
				 This parameter can be NULL.
*/
template <class T>
T* CObjectPool<T>::AllocObject(bool* pbNew/* NULL */)
{
	//	Have free object ?
	if (!m_pFreeHead)
	{
		if (!(m_pFreeHead = AllocNewPool()))
			return NULL;	//	Not enough memory
	}

	OBJECTNODE* pNode = m_pFreeHead;
	m_pFreeHead = m_pFreeHead->pNext;

	if (pbNew)
		*pbNew = !pNode->bUsed;

	pNode->bUsed = true;

	//	Throw this node to used node list
	pNode->pNext = m_pUsedHead;
	m_pUsedHead  = pNode;

	return pNode->pObject;
}

/*	Free a object of type T. This function doesn't delete object really, 
	just put it into freed buffer. pObject should be a object which is allocated
	by CObjectPool::AllocObject()
*/
template <class T>
void CObjectPool<T>::FreeObject(T* pObject)
{
	if (!pObject)
		return;

	if (!m_pUsedHead)
	{
		//	Some object must has been freed twice
		assert(0);
		return;
	}

	//	Get a node
	OBJECTNODE* pNode = m_pUsedHead;
	m_pUsedHead = m_pUsedHead->pNext;
	
	pNode->pObject = pObject;

	//	Add to free list
	pNode->pNext = m_pFreeHead;
	m_pFreeHead  = pNode;
}


#endif	//	_OBJECTPOOL_H_

