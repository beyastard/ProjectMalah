/*
 * FILE: AUString.h
 *
 * DESCRIPTION:�ַ�������
 *
 * CREATED BY: ����ӯ, 2002/8/20
 *
 * HISTORY:
 *
 * Copyright (c) 2001~2008 Epie-Game, All Rights Reserved.
 */


#ifndef __AUSTRING__H__
#define __AUSTRING__H__

class AUString
{
public:

	AUString();
	AUString(const char * string);
	AUString(const AUString & right);
	AUString(char ch,int repeat);
	~AUString();

	char * GetBufferSetLength(int length);
	void Free();
	//�����ַ�
	bool SetAt(int index,char ch);
	bool GetAt(int index,char & ch);
	
	//�Ƴ��ִ��е�ָ���ַ�������ɾ���������ĸ���
	int FilterChar(char ch,bool remove = true);

	//�Ƴ�������ַ�
	bool RemoveAt(int index);

	//��Ϊ��д
	void MakeUpper();

	//��ΪСд
	void MakeLower();
	bool InsertAt(int index,char ch);
	void Left(AUString & str,int count);
	void Right(AUString & str,int count);
	void Mid(AUString & str,int start,int end);
	
protected:
	bool FailLink(int iLen, char *pFailLink);

public:
	//���������
	char operator [] (int index);
	const char operator [](int index) const;
	const AUString & operator = (const AUString & right);
	const char * operator = (const char * string);
	AUString operator + (AUString right);

	operator char *() 
	{
		return GetBuffer();
	}

public:

	int FindString(AUString substr, int start = 0);
	void Empty();
	char * GetBuffer() const;
	bool IsEmpty() const;
	int GetLength() const;
	int FindChar(char ch,int start = 0);
	int ReverseFind(char ch);

private:

	char * m_buff;
};

bool operator == (AUString v1, AUString v2);
bool operator != (AUString v1, AUString v2);

#endif//endif __AUSTRING__H__