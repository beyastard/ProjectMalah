// Copyright (C) Beijing E-Pie Entertainment & Technology Co., Ltd.
// All rights reserved.
// File: DataQueue.h
// Creator: Wei Hua (κ��)
// ���ݶ���

#ifndef	__DATAQUEUE_H__
#define	__DATAQUEUE_H__

#define	DATAQUEUE_ERROR_OVERFLOW				-1	// ������
#define	DATAQUEUE_ERROR_EMPTY					-2	// ���п�
#define	DATAQUEUE_ERROR_DATATOOBIG				-3	// ����̫��������ڽ������ݵĻ�����̫С

#define	DATAQUEUE_DFT_MAXDATASIZE				255

class	DataQueue
{
protected:
	char	*m_pBuffer;								// �ܵ��ڴ���ʼ
	int		m_nTotalBytes;							// �������ܹ����Դ�ŵ��ֽ���
	int		m_nLogicBytes;							// �߼����ܹ����Դ�ŵ��ֽ���(��������п��е����)
	int		m_nHead;								// ��Ч���ݵ���ʼλ��
	int		m_nTail;								// ��Ч���ݵĽ���λ��
	int		m_nNum;									// ����������
	int		m_nMaxDataSize;							// �����������ݳ���
protected:
	bool	TestIncTail(int nDelta);				// �������ӵ�β���Ƿ�������Χ��
													// ����trueʱ��m_nDataTail��Ϊ�µĿ����ӵ�β��λ��(�����Ѿ����Ʊ��0��)
													// Ȼ����������β������nDelta������һ�����ᳬ��
													// ����false��ʾ�������
public:
	inline void	SetMaxDataSize(int nMax)			{ m_nMaxDataSize = nMax; }
public:
	DataQueue();
	~DataQueue();
	int		Init(int nTotalBytes);
	int		Release();
	int		Push(void *pData, int nSize);			// ѹ��
	int		PopQueue(void *pData, int *pnSize);
	int		PeekQueue(void **ppData, int *pnSize);
	int		DelQueueHead();
	int		ClearAll();
	int		GetNum();								// ���Ŀǰ�����е���������
};

#endif