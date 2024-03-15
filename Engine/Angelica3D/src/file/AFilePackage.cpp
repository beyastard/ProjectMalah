#include "AFilePackage.h"
#include "AFI.h"
#include "AFPI.h"
#include "StringConvert.h"
#include "zlib.h"

#include <io.h>

AFilePackage* g_pAFilePackage = nullptr;

AFilePackage::AFilePackage()
    : m_bHasChanged(false)
    , m_bReadOnly(false)
    , m_header()
    , m_mode()
    , m_nNumFiles(0)
    , m_pFileEntries(nullptr)
    , m_fpPackageFile(nullptr)
    , m_pBuffer(nullptr)
    , m_dwBufferLen(0)
    , m_bHasSorted(false)
{}

AFilePackage::~AFilePackage() = default;

bool AFilePackage::Open(std::wstring& szPckPath, AFPCK_OPENMODE mode)
{
	if (g_bCompressEnable)
	{
		if (!PrepareBuffer(1024 * 1024))
		{
			return false;
		}
	}

	switch (mode)
	{
	case AFPCK_OPENEXIST:
		m_bReadOnly = false;
		m_fpPackageFile = _wfopen(szPckPath.c_str(), L"r+b");
		if (m_fpPackageFile == nullptr)
		{
			m_fpPackageFile = _wfopen(szPckPath.c_str(), L"rb");
			if (m_fpPackageFile == nullptr)
			{
				AFERRLOG((L"AFilePackage::Open(), Can not open file [%s]", szPckPath));
				return false;
			}

			m_bReadOnly = true;
		}

		// Now analyze the file entries of the package
		uint32_t dwVersion;

		fseek(m_fpPackageFile, 0 - sizeof(uint32_t), SEEK_END);
		fread(&dwVersion, sizeof(uint32_t), 1, m_fpPackageFile);
		if (dwVersion == 0x00010003)
		{
			// First version
			// Now read file number
			fseek(m_fpPackageFile, 0 - (sizeof(int32_t) + sizeof(uint32_t)), SEEK_END);
			fread(&m_nNumFiles, sizeof(int32_t), 1, m_fpPackageFile);
			fseek(m_fpPackageFile, 0 - (sizeof(AFPCK_FILEHEADER) + sizeof(uint32_t) + sizeof(int32_t)), SEEK_END);
			fread(&m_header, sizeof(AFPCK_FILEHEADER), 1, m_fpPackageFile);

			m_pFileEntries = (AFPCK_FILEENTRY*)malloc(sizeof(AFPCK_FILEENTRY) * m_nNumFiles);
			if (m_pFileEntries == nullptr)
			{
				AFERRLOG((L"AFilePackage::Open(), Not enough memory!"));
				return false;
			}

			// Seek to entry list
			fseek(m_fpPackageFile, m_header.dwEntryOffset, SEEK_SET);
			for (int32_t i = 0; i < m_nNumFiles; i++)
			{
				int32_t nNameLength;
				fread(&nNameLength, sizeof(int32_t), 1, m_fpPackageFile);
				fread(m_pFileEntries[i].szFileName, nNameLength, 1, m_fpPackageFile);
				fread(&m_pFileEntries[i].dwOffset, sizeof(uint32_t), 1, m_fpPackageFile);
				fread(&m_pFileEntries[i].dwLength, sizeof(uint32_t), 1, m_fpPackageFile);
				fread(&m_pFileEntries[i].dwCompressedLength, sizeof(uint32_t), 1, m_fpPackageFile);
			}

			ResortEntries();
		}
		else
		{
			AFERRLOG((L"AFilePackage::Open(), Incorrect version!"));
			return false;
		}
		break;

	case AFPCK_CREATENEW:
		m_bReadOnly = false;
		m_fpPackageFile = _wfopen(szPckPath.c_str(), L"wb");
		if (m_fpPackageFile == nullptr)
		{
			AFERRLOG((L"AFilePackage::Open(), Can not create file [%s]", szPckPath));
			return false;
		}

		// Init header
		ZeroMemory(&m_header, sizeof(AFPCK_FILEHEADER));
		m_header.dwEntryOffset = 0;
		m_header.dwVersion = AFPCK_VERSION;
		strncpy(m_header.szDescription, "Angelica File Package, Beijing E-Pie Entertainment Corporation 2002~2008. All Rights Reserved. ", 256);

		m_nNumFiles = 0;
		m_pFileEntries = nullptr;
		break;

	default:
		AFERRLOG((L"AFilePackage::Open(), Unknown open mode [%d]!", mode));
		return false;
	}

	m_mode = mode;
	m_bHasChanged = false;

	return true;
}

bool AFilePackage::Close()
{
	switch (m_mode)
	{
	case AFPCK_OPENEXIST:
		if (m_bHasChanged)
		{
			DWORD dwFileSize = m_header.dwEntryOffset;

			// Rewrite file entries and file header here;
			fseek(m_fpPackageFile, m_header.dwEntryOffset, SEEK_SET);
			for (int32_t i = 0; i < m_nNumFiles; i++)
			{
				int32_t nNameLength = strlen(m_pFileEntries[i].szFileName) + 1; // Plus '\0'
				fwrite(&nNameLength, sizeof(int32_t), 1, m_fpPackageFile);
				dwFileSize += sizeof(int32_t);
				fwrite(m_pFileEntries[i].szFileName, nNameLength, 1, m_fpPackageFile);
				dwFileSize += nNameLength;
				fwrite(&m_pFileEntries[i].dwOffset, sizeof(uint32_t), 1, m_fpPackageFile);
				dwFileSize += sizeof(uint32_t);
				fwrite(&m_pFileEntries[i].dwLength, sizeof(uint32_t), 1, m_fpPackageFile);
				dwFileSize += sizeof(uint32_t);
				fwrite(&m_pFileEntries[i].dwCompressedLength, sizeof(uint32_t), 1, m_fpPackageFile);
				dwFileSize += sizeof(uint32_t);
			}

			// Write file header here;
			fwrite(&m_header, sizeof(AFPCK_FILEHEADER), 1, m_fpPackageFile);
			dwFileSize += sizeof(AFPCK_FILEHEADER);
			fwrite(&m_nNumFiles, sizeof(int32_t), 1, m_fpPackageFile);
			dwFileSize += sizeof(int32_t);
			fwrite(&m_header.dwVersion, sizeof(uint32_t), 1, m_fpPackageFile);
			dwFileSize += sizeof(uint32_t);

			int32_t fileHandle = _fileno(m_fpPackageFile);
			int32_t discard = _chsize(fileHandle, dwFileSize);

			m_bHasChanged = false;
		}
		break;
	case AFPCK_CREATENEW:
		// Write file entries and file header here;
		fseek(m_fpPackageFile, m_header.dwEntryOffset, SEEK_SET);
		for (int32_t i = 0; i < m_nNumFiles; i++)
		{
			int32_t nNameLength = strlen(m_pFileEntries[i].szFileName) + 1; // Plus '\0'
			fwrite(&nNameLength, sizeof(int32_t), 1, m_fpPackageFile);
			fwrite(m_pFileEntries[i].szFileName, nNameLength, 1, m_fpPackageFile);
			fwrite(&m_pFileEntries[i].dwOffset, sizeof(uint32_t), 1, m_fpPackageFile);
			fwrite(&m_pFileEntries[i].dwLength, sizeof(uint32_t), 1, m_fpPackageFile);
			fwrite(&m_pFileEntries[i].dwCompressedLength, sizeof(uint32_t), 1, m_fpPackageFile);
		}

		// Write file header here;
		fwrite(&m_header, sizeof(AFPCK_FILEHEADER), 1, m_fpPackageFile);
		fwrite(&m_nNumFiles, sizeof(int32_t), 1, m_fpPackageFile);
		fwrite(&m_header.dwVersion, sizeof(uint32_t), 1, m_fpPackageFile);
		break;
	}

	if (m_fpPackageFile)
	{
		fclose(m_fpPackageFile);
		m_fpPackageFile = nullptr;
	}

	if (m_pFileEntries)
	{
		free(m_pFileEntries);
		m_pFileEntries = nullptr;
	}

	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = nullptr;
		m_dwBufferLen = 0;
	}

	m_nNumFiles = 0;

	return true;
}

bool AFilePackage::AppendFile(std::wstring& szFileName, uint8_t* pFileBuffer, uint32_t dwFileLength)
{
	// We should use a function to check whether szFileName has been added into the package;
	if (m_bReadOnly)
	{
		AFERRLOG((L"AFilePackage::AppendFile(), Read only package, can not append!"));
		return false;
	}

	// Realloc the file entries;
	m_nNumFiles++;
	m_pFileEntries = (AFPCK_FILEENTRY*)realloc(m_pFileEntries, sizeof(AFPCK_FILEENTRY) * m_nNumFiles);
	if (m_pFileEntries == nullptr)
	{
		AFERRLOG((L"AFilePackage::AppendFile(), Not enough memory!"));
		return false;
	}

	uLongf dwCompressedLength = dwFileLength;
	// First we should compress the file if needed
	if (g_bCompressEnable)
	{
		if (!PrepareBuffer(dwFileLength))
			return false;

		compress2(m_pBuffer, &dwCompressedLength, pFileBuffer, dwFileLength, 1);
	}

	// store this file
	strncpy(m_pFileEntries[m_nNumFiles - 1].szFileName, wstring_to_string(szFileName).c_str(), MAX_PATH);
	m_pFileEntries[m_nNumFiles - 1].dwOffset = m_header.dwEntryOffset;
	m_pFileEntries[m_nNumFiles - 1].dwLength = dwFileLength;
	m_pFileEntries[m_nNumFiles - 1].dwCompressedLength = dwCompressedLength;

	fseek(m_fpPackageFile, m_header.dwEntryOffset, SEEK_SET);

	// If can't compress, we will use the origin file buffer directly
	if (dwCompressedLength < dwFileLength)
	{
		// We write the compressed buffer into the disk
		fwrite(m_pBuffer, dwCompressedLength, 1, m_fpPackageFile);
		m_header.dwEntryOffset += dwCompressedLength;
	}
	else
	{
		fwrite(pFileBuffer, dwFileLength, 1, m_fpPackageFile);
		m_header.dwEntryOffset += dwFileLength;
	}

	m_bHasChanged = true;
	m_bHasSorted = false;
	return true;
}

bool AFilePackage::RemoveFile(std::wstring& szFileName)
{
	if (m_bReadOnly)
	{
		AFERRLOG((L"AFilePackage::RemoveFile(), Read only package, can not remove file!"));
		return false;
	}

	AFPCK_FILEENTRY entry;
	int32_t nIndex;
	if (!GetFileEntry(szFileName, &entry, &nIndex))
	{
		AFERRLOG((L"AFilePackage::RemoveFile(), Can not find file %s", szFileName));
		return false;
	}

	// We only remove the file's entry, but leave the file's body there
	for (int32_t i = nIndex; i < m_nNumFiles - 1; i++)
		memcpy(&m_pFileEntries[i], &m_pFileEntries[i + 1], sizeof(AFPCK_FILEENTRY));
	
	m_nNumFiles--;
	m_pFileEntries = (AFPCK_FILEENTRY*)realloc(m_pFileEntries, sizeof(AFPCK_FILEENTRY) * m_nNumFiles);
	if (m_pFileEntries == nullptr)
	{
		AFERRLOG((L"AFilePackage::AppendFile(), Not enough memory!"));
		return false;
	}

	m_bHasChanged = true;
	m_bHasSorted = false;

	return true;
}

bool AFilePackage::ReplaceAFile(std::wstring& szFileName, uint8_t* pFileBuffer, uint32_t dwFileLength)
{
	if (m_bReadOnly)
	{
		AFERRLOG((L"AFilePackage::ReplaceFile(), Read only package, can not replace!"));
		return false;
	}

	AFPCK_FILEENTRY	entry;
	int32_t nIndex;

	if (!GetFileEntry(szFileName, &entry, &nIndex))
	{
		AFERRLOG((L"AFilePackage::ReplaceFile(), Can not find file %s", szFileName));
		return false;
	}

	// we only add a new file copy at the end of the file part, and modify the 
	// file entry point to that file body;
	uLongf dwCompressedLength = dwFileLength;

	// First we should compress the file if needed
	if (g_bCompressEnable)
	{
		if (!PrepareBuffer(dwFileLength))
			return false;

		compress2(m_pBuffer, &dwCompressedLength, pFileBuffer, dwFileLength, 1);
	}

	// modify this file entry to point to the new file body		
	m_pFileEntries[nIndex].dwOffset = m_header.dwEntryOffset;
	m_pFileEntries[nIndex].dwLength = dwFileLength;
	m_pFileEntries[nIndex].dwCompressedLength = dwCompressedLength;

	fseek(m_fpPackageFile, m_header.dwEntryOffset, SEEK_SET);

	// If can't compress, we will use the origin file buffer directly
	if (dwCompressedLength < dwFileLength)
	{
		// We write the compressed buffer into the disk
		fwrite(m_pBuffer, dwCompressedLength, 1, m_fpPackageFile);
		m_header.dwEntryOffset += dwCompressedLength;
	}
	else
	{
		fwrite(pFileBuffer, dwFileLength, 1, m_fpPackageFile);
		m_header.dwEntryOffset += dwFileLength;
	}

	m_bHasChanged = true;
	m_bHasSorted = false;

	return true;
}

bool AFilePackage::ResortEntries()
{
	int32_t nBestIndex;
	char* szBestName;
	AFPCK_FILEENTRY temp;

	for (int i = 0; i < m_nNumFiles - 1; i++)
	{
		nBestIndex = i;
		szBestName = m_pFileEntries[i].szFileName;
		for (int32_t j = i + 1; j < m_nNumFiles; j++)
		{
			if (_stricmp(m_pFileEntries[j].szFileName, szBestName) < 0)
			{
				nBestIndex = j;
				szBestName = m_pFileEntries[j].szFileName;
			}
		}

		if (nBestIndex != i)
		{
			temp = m_pFileEntries[i];
			m_pFileEntries[i] = m_pFileEntries[nBestIndex];
			m_pFileEntries[nBestIndex] = temp;
		}
	}

	m_bHasSorted = true;

	return true;
}

bool AFilePackage::ReadAFile(std::wstring& szFileName, uint8_t* pFileBuffer, uint32_t dwOffset, LPDWORD pdwBufferLen)
{
	AFPCK_FILEENTRY fileEntry;

	if (!GetFileEntry(szFileName, &fileEntry))
	{
		AFERRLOG((L"AFilePackage::ReadFile(), Can not find file entry [%s]!", szFileName));
		return false;
	}

	return ReadAFile(fileEntry, pFileBuffer, dwOffset, pdwBufferLen);
}

bool AFilePackage::ReadAFile(AFPCK_FILEENTRY& fileEntry, uint8_t* pFileBuffer, uint32_t dwOffset, LPDWORD pdwBufferLen)
{
	if (fileEntry.dwLength < dwOffset)
	{
		AFERRLOG((L"AFilePackage::ReadFile(), Offset [%d] beyond the end of the file!", dwOffset));
		return false;
	}

	if (*pdwBufferLen < fileEntry.dwLength - dwOffset)
	{
		AFERRLOG((L"AFilePackage::ReadFile(), Buffer is too small!"));
		return false;
	}

	fseek(m_fpPackageFile, fileEntry.dwOffset + dwOffset, SEEK_SET);

	// We can automaticly determine whether compression has been used
	if (fileEntry.dwLength > fileEntry.dwCompressedLength)
	{
		DWORD dwFileLength = fileEntry.dwLength;
		if (!PrepareBuffer(fileEntry.dwCompressedLength))
			return false;

		if (dwOffset > 0)
		{
			AFERRLOG((L"AFilePackage::ReadFile(), use a offset for read is not allowed when using a compressed package!"));
			return false;
		}

		fread(m_pBuffer, fileEntry.dwCompressedLength, 1, m_fpPackageFile);
		uncompress(pFileBuffer, &dwFileLength, m_pBuffer, fileEntry.dwCompressedLength);
	}
	else
		fread(pFileBuffer, fileEntry.dwLength - dwOffset, 1, m_fpPackageFile);

	return true;
}

bool AFilePackage::GetFileEntry(std::wstring& szFileName, AFPCK_FILEENTRY* pFileEntry, int32_t* pnIndex)
{
	char szFindName[MAX_PATH];
	int32_t nLength;
	int32_t i;

	strncpy(szFindName, wstring_to_string(szFileName).c_str(), MAX_PATH);

	// First we should unite the path seperator to '\'
	nLength = strlen(szFindName);
	for (i = 0; i < nLength; i++)
	{
		if (szFindName[i] == '/')
			szFindName[i] = '\\';
	}

	// Get rid of the preceding .\ string
	if (nLength > 2 && szFindName[0] == '.' && szFindName[1] == '\\')
	{
		for (i = 0; i < nLength - 2; i++)
			szFindName[i] = szFindName[i + 2];
		szFindName[i] = '\0';
	}

	// Get rid of extra space at the tail of the string
	nLength = strlen(szFindName);
	for (i = nLength - 1; i >= 0; i--)
	{
		if (szFindName[i] != ' ')
			break;
		else
			szFindName[i] = '\0';
	}

	// We should also get rid of the preceding space
	// TrimString()...

	ZeroMemory(pFileEntry, sizeof(AFPCK_FILEENTRY));

	if (!m_bHasSorted)
	{
		// Use a linear search now
		// For better performance we should use a binary search, that you need call ResortEntry
		for (i = 0; i < m_nNumFiles; i++)
		{
			if (_stricmp(szFindName, m_pFileEntries[i].szFileName) == 0)
			{
				*pFileEntry = m_pFileEntries[i];
				if (pnIndex)
					*pnIndex = i;
				return true;
			}
		}
	}
	else
	{
		// Use binary search
		if (m_nNumFiles == 0)
			return false;

		int32_t nLeft = 0;
		int32_t nRight = m_nNumFiles - 1;
		int32_t nMiddle;

		while (nLeft <= nRight)
		{
			nMiddle = (nLeft + nRight) / 2;

			int32_t nCompare = _stricmp(m_pFileEntries[nMiddle].szFileName, szFindName);
			if (nCompare == 0)
			{
				*pFileEntry = m_pFileEntries[nMiddle];
				if (pnIndex)
					*pnIndex = nMiddle;
				return true;
			}
			else if (nCompare < 0)
				nLeft = nMiddle + 1;
			else if (nCompare > 0)
				nRight = nMiddle - 1;
		}
	}

	return false;
}

bool AFilePackage::GetFileEntryByIndex(int32_t nIndex, AFPCK_FILEENTRY* pFileEntry)
{
	if (nIndex >= m_nNumFiles)
		return false;

	*pFileEntry = m_pFileEntries[nIndex];

	return true;
}

bool AFilePackage::PrepareBuffer(uint32_t dwBufferLen)
{
    if (dwBufferLen <= m_dwBufferLen)
        return true;

    m_dwBufferLen = dwBufferLen;
    m_pBuffer = (uint8_t*)realloc(m_pBuffer, m_dwBufferLen);
    if (m_pBuffer == nullptr)
    {
        AFERRLOG((L"AFilePackage::PrepareBuffer(), Not enough memory!"));
        return false;
    }

    return true;
}

bool OpenFilePackage(std::wstring& szPackFile)
{
	if (g_pAFilePackage)
		CloseFilePackage();

	g_pAFilePackage = new AFilePackage();

	if (g_pAFilePackage == nullptr)
	{
		AFERRLOG((L"OpenFilePackage(), Not enough memory!"));
		return false;
	}

	if (!g_pAFilePackage->Open(szPackFile, AFPCK_OPENEXIST))
	{
		AFERRLOG((L"OpenFilePackage(), Can not open package [%s]", szPackFile));
		return false;
	}
	return true;
}

bool CloseFilePackage()
{
	if (g_pAFilePackage)
	{
		g_pAFilePackage->Close();
		delete g_pAFilePackage;
		g_pAFilePackage = nullptr;
	}

	return true;
}
