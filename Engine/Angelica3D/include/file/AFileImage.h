#ifndef __AFILE_IMAGE_H__
#define __AFILE_IMAGE_H__

#include "ABaseDef.h"
#include "AFile.h"

class AFileImage : public AFile
{
public:
	AFileImage();
	~AFileImage();

	bool Open(const std::wstring& szFullPath, uint32_t dwFlags);
	bool Open(const std::wstring& szFolderName, const std::wstring& szFileName, uint32_t dwFlags);
	bool Close();

	bool Read(LPVOID pBuffer, uint32_t dwBufferLength, uint32_t* pReadLength);
	bool Write(LPVOID pBuffer, uint32_t dwBufferLength, uint32_t* pWriteLength);

	bool ReadLine(char* szLineBuffer, uint32_t dwBufferLength, size_t* pdwReadLength);
	bool WriteLine(const char* szLineBuffer);

	bool ReadString(char* szLineBuffer, uint32_t dwBufferLength, uint32_t* pdwReadLength);
	bool WriteString(const std::string& s);
	bool ReadString(std::string& s);

	uint32_t Tell() const;
	bool Seek(int32_t iOffset, AFILE_SEEK origin);
	bool ResetFilePointer();

	LPBYTE GetFileBuffer() { return m_pFileImage; }
	uint32_t GetFileLength() const { return (uint32_t)m_nFileLength; }

protected:
	bool Init(const std::wstring& szFullName, bool bTempMem);
	bool Init(std::wstring& szFullPath);
	bool Release();

private:
	LPBYTE m_pFileImage;	// Memory pointer of the file image in memory;
	int32_t m_nCurPtr;		// In index into the file image buffer;
	int32_t m_nFileLength;	// File length;

	bool fimg_read(LPBYTE pBuffer, int32_t nSize, int32_t* pReadSize); // read some size of data into a buffer;
	bool fimg_read_line(char* szLineBuffer, int32_t nMaxLength, int32_t* pReadSize); // read a line into a buffer;
	bool fimg_seek(int32_t nOffset, int32_t startPos); // offset current pointer
};

typedef AFileImage* PAFileImage;

#endif
