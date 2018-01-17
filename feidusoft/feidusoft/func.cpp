#include "stdafx.h"




CString IntToCString(int val){
	CString temp;
	temp.Format(_T("%d"),val);
	return temp;
}

int CStringToInt(CString val){
	if(val.IsEmpty()){
		val = _T("0");
	}
	return _ttoi(val);
}

LPSTR CStringToLpstr(CString val){
	wchar_t *str =val.GetBuffer(val.GetLength());  
	val.ReleaseBuffer();
	size_t len = wcslen(str) + 1;
	size_t converted = 0;
	char *CStr;
	CStr=(char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, CStr, len, str, _TRUNCATE);
	return CStr;
}

//比较两个坐标组是否超出限定
//超出则返回TRUE 未超出则返回FALSE
int compare(lvXY currentXY,lvXY lastXY){
	if(abs((currentXY.x-lastXY.x)) == 0 && abs((currentXY.y-lastXY.y)) == 0){
		//相较于上次，没有移动
		return STOP_MOVE;
	}
	if(abs((currentXY.x-lastXY.x))>5 || abs((currentXY.y-lastXY.y))>5){
		//超出了限定范围
		return BEYOND;
	}
	return BEYOND_NOT;
}

//将CString字符串按指定字符分割
int CStringToArray(CString source,TCHAR d,CStringArray& road){
	int s0=source.ReverseFind(d);
	road.Add(source.Mid(s0+1));

	while(s0>0)
	{
		source=source.Mid(0,s0);
		s0=source.ReverseFind(d);
		road.Add(source.Mid(s0+1));
	}
	return 0;
}

//解析当前坐标
lvXY getXY(CString sXY,char d){
	lvXY xy;
	CString sX = _T(""),sY = _T(""),temp;

	int count;
	CStringArray road;
	CStringToArray(sXY,d,road);
	count = road.GetCount();//获取数据数量

	//count>2则只取前两个元素，0为Y坐标；1为X坐标
	for (int i=0;i<count;i++)
	{
		if(!sX.IsEmpty() && !sY.IsEmpty()){
			continue;
		}
		temp = road.GetAt(i);
		if (!temp.IsEmpty())
		{
			if(sY.IsEmpty()){
				sY = temp;
			}else{
				sX = temp;
			}		
		}
	}
	xy.sx = sX;
	xy.sy = sY;
	xy.x = _ttoi(sX);
	xy.y = _ttoi(sY);
	return xy;
}


//整数型转换为字符串
CString Lng2Str(long val){
	LPTSTR str = new TCHAR[10];
	wsprintf(str,L"%d",val);
	return str;
}
//获取当前应该程目录
CString getFilePos(CString filename){
	CString m_FilePath;
	GetModuleFileName(NULL,m_FilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	m_FilePath.ReleaseBuffer();
	int m_iPosIndex;
	m_iPosIndex = m_FilePath.ReverseFind('\\');
	m_FilePath = m_FilePath.Left(m_iPosIndex);
	CString file;
	file.Format(_T("%s\\%s"),m_FilePath,filename);
	return file;
}

DWORD XSleepThread(LPVOID pWaitTime)
{
	XSleep_Structure *sleep = (XSleep_Structure *)pWaitTime;

	Sleep(sleep->duration);
	SetEvent(sleep->eventHandle);

	return 0;
}

void XSleep(int nWaitInMSecs)
{
	XSleep_Structure sleep;
	sleep.duration		= nWaitInMSecs;
	sleep.eventHandle	= CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD threadId;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)XSleepThread, &sleep, 0, &threadId);
	MSG msg;
	while(::WaitForSingleObject(sleep.eventHandle, 0) == WAIT_TIMEOUT)
	{
		//get and dispatch messages
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	CloseHandle(sleep.eventHandle);
}

int HttpPost(LPSTR url,CString param)
{
	IXMLHTTPRequestPtr xmlrequest; //定义变量
	LPSTR str = "0";
	CoInitialize(NULL);//COM初始化
	xmlrequest.CreateInstance("Msxml2.ServerXMLHTTP");//建立对像

	xmlrequest->open(_bstr_t("POST"),_bstr_t(url),_variant_t("true")); //打开链接
	xmlrequest->setRequestHeader(_bstr_t("Content-Type"),_bstr_t("application/x-www-form-urlencoded"));//设置编码
	xmlrequest->send(_variant_t(param)); //发送数据
	int i=1;
	while (i<=100)
	{
		if(xmlrequest->readyState==4)
			break;
		i=i+1;
		XSleep(100);
	}
	if(i>=100)
	{
		//strcpy(str,str4);
		//return str;
		str="0";
	}
	if (xmlrequest->status==200)
	{
		str=_com_util::ConvertBSTRToString(xmlrequest->responseText.copy());
		TRACE(str);
	}else{
		//strcpy(str,str4);
		//str="-1";
		CoUninitialize();//COM结束
		return xmlrequest->status;
	}
	CoUninitialize();//COM结束
	int ret=atoi(str);
	return ret;
}



