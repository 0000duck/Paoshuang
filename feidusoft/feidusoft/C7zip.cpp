// C7zip.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "C7zip.h"


// C7zip

IMPLEMENT_DYNAMIC(C7zip, CWnd)

C7zip::C7zip()
{

}

C7zip::~C7zip()
{
}


BEGIN_MESSAGE_MAP(C7zip, CWnd)
END_MESSAGE_MAP()



CString C7zip::GetUnZipError(int ret)
{
	// These are the result codes:
	switch(ret)
	{
	case ZR_RECENT: return _T("but can be passed to FormatResMessage");
	case ZR_GENMASK: return _T("gen mask");
	case ZR_NODUPH: return _T("couldn't duplicate the handle");
	case ZR_NOFILE: return _T("couldn't create/open the file");
	case ZR_NOALLOC: return _T("failed to allocate some resource");
	case ZR_WRITE: return _T("a general error writing to the file");
	case ZR_NOTFOUND: return _T("couldn't find that file in the res");
	case ZR_MORE: return _T("there's still more data to be resfile");
	case ZR_CORRUPT: return _T("the resfile is corrupt or not a resfile");
	case ZR_READ: return _T("a general error reading the file");
	case ZR_PASSWORD: return _T("we didn't get the right password to res the file");
	case ZR_CALLERMASK: return _T("call er mask");
	case ZR_ARGS: return _T("general mistake with the arguments");
	case ZR_NOTMMAP: return _T("tried to ResGetMemory, but that only works on mmap resfiles, which yours wasn't");
	case ZR_MEMSIZE: return _T("the memory size is too small");
	case ZR_FAILED: return _T("the thing was already failed when you called this function");
	case ZR_ENDED: return _T("the res creation has already been closed");
	case ZR_MISSIZE: return _T("the indicated input file size turned out mistaken");
	case ZR_PARTIALUNZ: return _T("the file had already been partially res");
	case ZR_ZMODE: return _T("tried to mix creating/opening a res");
	case ZR_BUGMASK: return _T("bug mask");
	case ZR_NOTINITED: return _T("initialisation didn't work");
	case ZR_SEEK: return _T("trying to seek in an unseekable file");
	case ZR_NOCHANGE: return _T("changed its mind on storage, but not allowed");
	case ZR_FLATE: return _T("an internal error in the de/inflation code");
	default : return _T("unknown error");
	}
}


// //解压zip文件至指定位置
int C7zip::Unzip(CString strFilePath, CString strDirPath, LPCSTR password)
{
	int ret = 0;
	int zip_ret = 0;
	CString strMsg;

	ZIPENTRY ze = {0};
	HZIP hz = NULL;
	int numitems = 0;
	int i = 0;

	strFilePath.TrimLeft();
	strFilePath.TrimRight();
	if(strFilePath.IsEmpty())
	{
		return -1;
	}
	
	strDirPath.TrimLeft();
	strDirPath.TrimRight();
	if(strDirPath.IsEmpty())
	{
		return -2;
	}

	hz = OpenZip(strFilePath.GetBuffer(0),password);
	if(hz == NULL)
	{
		MessageBox(_T("打开资源文件失败！"), _T("提示"), MB_OK);
		goto end;
	}
	ret = SetUnzipBaseDir(hz, strDirPath);
	if(ret != ZR_OK)
	{
		strMsg = _T("SetResBaseDir error: ");
		strMsg += GetUnZipError(ret);
		//MessageBox(strMsg, _T("提示"), MB_OK);
		goto end;
	}
	ret = GetZipItem(hz,-1,&ze);
	if(ret != ZR_OK)
	{
		strMsg = _T("GetResItem error: ");
		strMsg += GetUnZipError(ret);
		//MessageBox(strMsg, _T("提示"), MB_OK);
		goto end;
	}
	numitems = ze.index;
	for (i = 0; i < numitems; i++)
	{
		ret = GetZipItem(hz,i,&ze);
		if(ret != ZR_OK)
		{
			strMsg = _T("GetResItem error: ");
			strMsg += GetUnZipError(ret);
			//MessageBox(strMsg, _T("提示"), MB_OK);
			goto end;
		}
		TRACE(ze.name);
		TRACE("\n");
		ret = UnzipItem(hz,i,ze.name);
		if(ret != ZR_OK)
		{
			if(ret == ZR_FLATE && ze.unc_size == 0)
			{
				strMsg.Format(_T("ResItem %s %d error: "), ze.name, ze.unc_size);
				strMsg += GetUnZipError(ret);
				strMsg += _T("\r\n");
				TRACE(strMsg);
			}
			else
			{
				strMsg.Format(_T("ResItem %s error: "), ze.name);
				strMsg += GetUnZipError(ret);
				//MessageBox(strMsg, _T("提示"), MB_OK);
				goto end;
			}
		}
	}
	if(hz != NULL)
	{
		CloseZip(hz);
		hz = NULL;
	}
	return 1;
end:
	if(hz != NULL)
	{
		CloseZip(hz);
		hz = NULL;
	}
	return 0;
}
