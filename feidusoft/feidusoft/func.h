#pragma once


CString IntToCString(
	int val
	);//intת��CString

int CStringToInt(
	CString val
	);//CStringת��int

LPSTR CStringToLpstr(
	CString val
	);//CStringת��LPSTR(char *)

BOOL compare(
	lvXY currentXY
	,lvXY lastXY
	);//�Ƚ����������飬��������޶��򷵻�TRUE��δ�����򷵻�FALSE

lvXY getXY(
	CString sXY
	,char d = ','
	);//��ȡ�ַ��������겢����


int CStringToArray(
	CString source
	,TCHAR d
	,CStringArray& road
	);//��CString�ַ�����ָ���ַ��ָ�

CString Lng2Str(
	long val
	);//��������תΪ�ַ�����

CString getFilePos(
	CString filename
	);

void XSleep(
	int nWaitInMSecs
	);

int HttpPost(
	LPSTR url
	,CString param
	);

