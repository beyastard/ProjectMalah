#ifndef __AFILE_PACKAGE_H__
#define __AFILE_PACKAGE_H__

#include "ABaseDef.h"

//#define AFPCK_VERSION 0x00010001
//#define AFPCK_VERSION 0x00010002 // Add compression
#define AFPCK_VERSION 0x00010003 // The final release version on June 2002

typedef struct _AFPCK_FILEENTRY
{
	char szFileName[MAX_PATH];   // The file name of this entry; this may contain a path
	uint32_t dwOffset;           // The offset from the beginning of the package file
	uint32_t dwLength;           // The length of this file
	uint32_t dwCompressedLength; // The compressed data length

	_AFPCK_FILEENTRY()
		: dwOffset(0)
		, dwLength(0)
		, dwCompressedLength(0)
	{
		std::strcpy(szFileName, "");
	}

} AFPCK_FILEENTRY, * PAFPCK_FILEENTRY;

typedef struct _AFPCK_FILEHEADER
{
	uint32_t dwVersion;     // Composed by two word version, major part and minor part
	uint32_t dwEntryOffset; // The entry list offset from the beginning
	char szDescription[256];

	_AFPCK_FILEHEADER()
		: dwVersion(0)
		, dwEntryOffset(0)
	{
		std::strcpy(szDescription, "");
	}

} AFPCK_FILEHEADER, * PAFPCK_FILEHEADER;

enum AFPCK_OPENMODE
{
	AFPCK_OPENEXIST = 0,
	AFPCK_CREATENEW = 1
};

class AFilePackage
{
public:
	AFilePackage();
	~AFilePackage();

	bool Open(std::wstring& szPckPath, AFPCK_OPENMODE mode);
	bool Close();

	//	Append a file into the package
	//	parameter:
	//		IN: szFileName		file name
	//		IN: pFileBuffer		the buffer containing file content
	//		IN: dwFileLength	the length of the buffer
	bool AppendFile(std::wstring& szFileName, uint8_t* pFileBuffer, uint32_t dwFileLength);

	//	Remove a file from the package, we will only remove the file entry from the package
	//	the file's data will remain in the package
	//	parameter:
	//		IN: szFileName		file name
	bool RemoveFile(std::wstring& szFileName);

	//	Replace a file in the package, we will only replace the file entry in the package
	//	the old file's data will remain in the package
	//	parameter:
	//		IN: szFileName		file name
	bool ReplaceAFile(std::wstring& szFileName, uint8_t* pFileBuffer, uint32_t dwFileLength);

	//	Read the file's content from the package
	//	parameter:
	//		IN: szFileName			file name
	//		IN: pFileBuffer			buffer to contain the file's content
	//		IN: dwOffset			offset of the file to read start
	//		IN/OUT: pdwBufferLen	in: the max buffer size of pFileBuffer, out: the actually filled length of pFileBuffer

	// Sort the file entry list
	bool ResortEntries();

	bool ReadAFile(std::wstring& szFileName, uint8_t* pFileBuffer, uint32_t dwOffset, LPDWORD pdwBufferLen);
	bool ReadAFile(AFPCK_FILEENTRY& fileEntry, uint8_t* pFileBuffer, uint32_t dwOffset, LPDWORD pdwBufferLen);

	// Find a file entry
	// return true if found, false if not found
	bool GetFileEntry(std::wstring& szFileName, AFPCK_FILEENTRY* pFileEntry, int32_t* pnIndex = nullptr);
	bool GetFileEntryByIndex(int32_t nIndex, AFPCK_FILEENTRY* pFileEntry);

	int32_t GetFileNumber() { return m_nNumFiles; }
	AFPCK_FILEHEADER GetFileHeader() { return m_header; }

private:
	bool m_bHasChanged;
	bool m_bReadOnly;

	AFPCK_FILEHEADER m_header;
	AFPCK_OPENMODE m_mode;

	int32_t m_nNumFiles;
	AFPCK_FILEENTRY* m_pFileEntries;

	FILE* m_fpPackageFile;

	uint8_t* m_pBuffer;		// A buffer for compression and uncompression
	uint32_t m_dwBufferLen;	// The length of the buffer

	bool m_bHasSorted;	// Flag indicating whether the file entry has been sorted

	// Prepare a compression usage buffer
	bool PrepareBuffer(uint32_t dwBufferLen);
};

typedef class AFilePackage* PAFilePackage;

bool OpenFilePackage(std::wstring& szPackFile);
bool CloseFilePackage();

extern AFilePackage* g_pAFilePackage;

#endif
