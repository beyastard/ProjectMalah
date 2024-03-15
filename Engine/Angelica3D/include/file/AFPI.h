#ifndef __AFPI_H__
#define __AFPI_H__

#include "ALog.h"

extern ALog* g_pAFErrLog;
extern std::wstring g_strBaseDir;

// You must use a () to include the fmt string;
// For example AFERRLOG(("Error Occurs at %d", nval))
#define AFERRLOG(fmt) { if (g_pAFErrLog) g_pAFErrLog->Log fmt; }

#endif
