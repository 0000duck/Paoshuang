#pragma once

#include "unzip.h"
#include "zip.h"

// C7zip

class C7zip : public CWnd
{
	DECLARE_DYNAMIC(C7zip)

public:
	C7zip();
	virtual ~C7zip();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CString GetUnZipError(int ret);
	// //解压zip文件至指定位置
	int Unzip(CString zipFile, CString saveDir, LPCSTR password);
};


