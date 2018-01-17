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

//�Ƚ������������Ƿ񳬳��޶�
//�����򷵻�TRUE δ�����򷵻�FALSE
int compare(lvXY currentXY,lvXY lastXY){
	if(abs((currentXY.x-lastXY.x)) == 0 && abs((currentXY.y-lastXY.y)) == 0){
		//������ϴΣ�û���ƶ�
		return STOP_MOVE;
	}
	if(abs((currentXY.x-lastXY.x))>5 || abs((currentXY.y-lastXY.y))>5){
		//�������޶���Χ
		return BEYOND;
	}
	return BEYOND_NOT;
}

//��CString�ַ�����ָ���ַ��ָ�
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

//������ǰ����
lvXY getXY(CString sXY,char d){
	lvXY xy;
	CString sX = _T(""),sY = _T(""),temp;

	int count;
	CStringArray road;
	CStringToArray(sXY,d,road);
	count = road.GetCount();//��ȡ��������

	//count>2��ֻȡǰ����Ԫ�أ�0ΪY���ꣻ1ΪX����
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


//������ת��Ϊ�ַ���
CString Lng2Str(long val){
	LPTSTR str = new TCHAR[10];
	wsprintf(str,L"%d",val);
	return str;
}
//��ȡ��ǰӦ�ó�Ŀ¼
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
	IXMLHTTPRequestPtr xmlrequest; //�������
	LPSTR str = "0";
	CoInitialize(NULL);//COM��ʼ��
	xmlrequest.CreateInstance("Msxml2.ServerXMLHTTP");//��������

	xmlrequest->open(_bstr_t("POST"),_bstr_t(url),_variant_t("true")); //������
	xmlrequest->setRequestHeader(_bstr_t("Content-Type"),_bstr_t("application/x-www-form-urlencoded"));//���ñ���
	xmlrequest->send(_variant_t(param)); //��������
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
		CoUninitialize();//COM����
		return xmlrequest->status;
	}
	CoUninitialize();//COM����
	int ret=atoi(str);
	return ret;
}



