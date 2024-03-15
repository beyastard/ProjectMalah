#include "AFileImage.h"
#include "AFilePackage.h"
#include "AFI.h"
#include "AFPI.h"

#include <filesystem>

AFileImage::AFileImage()
    : AFile()
    , m_pFileImage(nullptr)
    , m_nCurPtr(0)
    , m_nFileLength(0)
{}

AFileImage::~AFileImage()
{
    AFileImage::Close();
}

bool AFileImage::Open(const std::wstring& szFullPath, uint32_t dwFlags)
{
    if (m_bHasOpened)
        Close();

    bool bTempMem = (dwFlags & AFILE_TEMPMEMORY);

    if (!Init(szFullPath, bTempMem))
        return false;

    if (!(dwFlags & AFILE_OPENEXIST))
    {
        AFERRLOG((L"AFileImage::Open - Currently, we only support the read flag to operate a file image"));
        return false;
    }

    uint32_t dwFOURCC = 0;
    int32_t nRead;
    if (!fimg_read(reinterpret_cast<LPBYTE>(&dwFOURCC), 4, &nRead))
        return false;

    if (dwFOURCC == 0x42584f4d)      // 'MOXB'
        m_flags = AFILE_BINARY;
    else if (dwFOURCC == 0x54584f4d) // 'MOXT'
        m_flags = AFILE_TEXT;
    else
    {
        m_flags = AFILE_TEXT;
        fimg_seek(0, AFILE_SEEK_SET);
    }

    m_flags |= (dwFlags & ~AFILE_BINARY & ~AFILE_TEXT);
    m_bHasOpened = true;

    return true;
}

bool AFileImage::Open(const std::wstring& szFolderName, const std::wstring& szFileName, uint32_t dwFlags)
{
    std::wstring szFullPath;
    af_GetFullPath(szFullPath, szFolderName, szFileName);
    return Open(szFullPath, dwFlags);
}

bool AFileImage::Close()
{
    Release();

    m_nCurPtr = 0;
    m_strFilename.clear();

    return true;
}

bool AFileImage::Read(LPVOID pBuffer, uint32_t dwBufferLength, uint32_t* pReadLength)
{
    int32_t nReadSize;
    if (!fimg_read((LPBYTE)pBuffer, dwBufferLength, &nReadSize))
        return false;

    *pReadLength = nReadSize;

    return true;
}

bool AFileImage::Write(LPVOID pBuffer, uint32_t dwBufferLength, uint32_t* pWriteLength)
{
    return false;
}

bool AFileImage::ReadLine(char* szLineBuffer, uint32_t dwBufferLength, size_t* pdwReadLength)
{
    int32_t nReadSize;

    if (!fimg_read_line(szLineBuffer, dwBufferLength, &nReadSize))
        return false;

    //chop the \n\r
    if (szLineBuffer[0] && (szLineBuffer[std::strlen(szLineBuffer) - 1] == '\n' || szLineBuffer[std::strlen(szLineBuffer) - 1] == '\r'))
        szLineBuffer[std::strlen(szLineBuffer) - 1] = '\0';

    if (szLineBuffer[0] && (szLineBuffer[std::strlen(szLineBuffer) - 1] == '\n' || szLineBuffer[std::strlen(szLineBuffer) - 1] == '\r'))
        szLineBuffer[std::strlen(szLineBuffer) - 1] = '\0';

    *pdwReadLength = std::strlen(szLineBuffer) + 1;

    return true;
}

bool AFileImage::WriteLine(const char* szLineBuffer)
{
    return false;
}

bool AFileImage::ReadString(char* szLineBuffer, uint32_t dwBufferLength, uint32_t* pdwReadLength)
{
    char c;
    uint32_t dwReadLength;
    uint32_t nStrLen = 0;

    Read(&c, 1, &dwReadLength);
    while (c)
    {
        szLineBuffer[nStrLen] = c;
        nStrLen++;

        if (nStrLen >= dwBufferLength)
            return false;

        Read(&c, 1, &dwReadLength);
    }

    szLineBuffer[nStrLen] = '\0';
    *pdwReadLength = nStrLen + 1;

    return true;
}

bool AFileImage::WriteString(const std::string& s)
{
    return false;
}

bool AFileImage::ReadString(std::string& s)
{
    int32_t iRead;

    // Read length of string
    int32_t iLen;
    if (!fimg_read((BYTE*)&iLen, sizeof(int), &iRead))
        return false;

    // Read string data
    if (iLen)
    {
        char* szBuf = new char[iLen + 1];
        if (!szBuf)
            return false;

        if (!fimg_read((BYTE*)szBuf, iLen, &iRead))
        {
            delete[] szBuf;
            szBuf = nullptr;
            return false;
        }

        szBuf[iLen] = '\0';
        s = szBuf;

        delete[] szBuf;
        szBuf = nullptr;
    }
    else
        s = "";

    return true;
}

uint32_t AFileImage::Tell() const
{
    return(uint32_t)m_nCurPtr;
}

bool AFileImage::Seek(int32_t iOffset, AFILE_SEEK origin)
{
    return fimg_seek(iOffset, (int32_t)origin);
}

bool AFileImage::ResetFilePointer()
{
    fimg_seek(0, AFILE_SEEK_SET);
    return true;
}

bool AFileImage::Init(const std::wstring& szFullName, bool bTempMem)
{
    m_strFilename = szFullName;
    af_GetRelativePath(szFullName, m_strRelativePath);

    // If g_pAFilePackage is set, this means that we
    // are using a file package, instead of seperate files
    if (!g_pAFilePackage)
    {
        FILE* pFile;

        pFile = _wfopen(szFullName.c_str(), L"rb");
        if (pFile == nullptr)
        {
            AFERRLOG((L"AFileImage::Init Can't open file [%s] to create image in memory", szFullName));
            return false;
        }

        fseek(pFile, 0, SEEK_END);
        m_nFileLength = ftell(pFile);
        if (0 == m_nFileLength)
        {
            AFERRLOG((L"AFileImage::Init The file [%s] is zero length!"));
            return false;
        }

        fseek(pFile, 0, SEEK_SET);
        m_pFileImage = (LPBYTE)malloc(m_nFileLength * sizeof(BYTE));
        if (m_pFileImage == nullptr)
        {
            AFERRLOG((L"AFileImage::Init Not enough memory!"));
            return false;
        }

        fread(m_pFileImage, m_nFileLength, 1, pFile);

        fclose(pFile);
    }
    else
    {
        // Init from a package
        AFPCK_FILEENTRY fileEntry;
        if (!g_pAFilePackage->GetFileEntry(m_strRelativePath, &fileEntry))
        {
            AFERRLOG((L"AFileImage::Can not locate file [%s] in the package!", m_strRelativePath));
            return false;
        }

        m_nFileLength = fileEntry.dwLength;
        m_pFileImage = (uint8_t*)malloc(m_nFileLength * sizeof(uint8_t));

        if (m_pFileImage == nullptr)
        {
            AFERRLOG((L"AFileImage::Init(), Not enough memory!"));
            return false;
        }

        if (!g_pAFilePackage->ReadAFile(fileEntry, m_pFileImage, 0, (LPDWORD)&m_nFileLength))
        {
            AFERRLOG((L"AFileImage::Init(), Error Reading file [%s] from package!", m_strRelativePath));
            return false;
        }
    }

    return true;
}

bool AFileImage::Init(std::wstring& szFullPath)
{
    return Init(szFullPath, true);
}

bool AFileImage::Release()
{
    if (m_pFileImage)
    {
        free(m_pFileImage);
        m_pFileImage = nullptr;
    }

    return true;
}

bool AFileImage::fimg_read(LPBYTE pBuffer, int32_t nSize, int32_t* pReadSize)
{
    *pReadSize = 0;
    int32_t nSizeToRead = nSize;

    if (m_nCurPtr + nSizeToRead > m_nFileLength)
        nSizeToRead = m_nFileLength - m_nCurPtr;

    if (nSizeToRead <= 0)
    {
        *pReadSize = 0;
        return nSize == 0 ? true : false;
    }

    std::memcpy(pBuffer, m_pFileImage + m_nCurPtr, nSizeToRead);
    m_nCurPtr += nSizeToRead;
    *pReadSize = nSizeToRead;

    return true;
}

bool AFileImage::fimg_read_line(char* szLineBuffer, int32_t nMaxLength, int32_t* pReadSize)
{
    size_t nSizeRead = 0;

    ZeroMemory(szLineBuffer, nMaxLength);
    while (m_nCurPtr < m_nFileLength)
    {
        uint8_t byteThis = m_pFileImage[m_nCurPtr];

        if (byteThis != 0x0d && byteThis != 0x0a)
        {
            // Not \n or \r, so copy it into the buffer;
            szLineBuffer[nSizeRead++] = m_pFileImage[m_nCurPtr++];
        }
        else
        {
            // We also need to copy \n into the buffer;
            szLineBuffer[nSizeRead++] = m_pFileImage[m_nCurPtr++];
            szLineBuffer[nSizeRead] = '\0';
            if (byteThis == 0x0d)
            {
                // We need to check if next byte is \r, if so, just remove it;
                if (m_nCurPtr < m_nFileLength)
                {
                    if (m_pFileImage[m_nCurPtr] == 0x0a)
                    {
                        m_nCurPtr++;
                        nSizeRead++;
                    }
                }
            }

            break;
        }
    }

    *pReadSize = nSizeRead;

    if (nSizeRead <= 0)
        return false;

    return true;
}

bool AFileImage::fimg_seek(int32_t nOffset, int32_t startPos)
{
    switch (startPos)
    {
    case AFILE_SEEK_SET:
        m_nCurPtr = nOffset;
        break;
    case AFILE_SEEK_CUR:
        m_nCurPtr += nOffset;
        break;
    case AFILE_SEEK_END:
        m_nCurPtr = m_nFileLength + nOffset;
        break;
    default:
        return false;
    }
    if (m_nCurPtr < 0)
        m_nCurPtr = 0;
    else if (m_nCurPtr > m_nFileLength) // To be compatible with fseek, we have
        m_nCurPtr = m_nFileLength;      //   to let the file pointer beyond the last character

    return true;
}
