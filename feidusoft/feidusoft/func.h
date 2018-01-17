#pragma once


CString IntToCString(
	int val
	);//int转换CString

int CStringToInt(
	CString val
	);//CString转换int

LPSTR CStringToLpstr(
	CString val
	);//CString转换LPSTR(char *)

BOOL compare(
	lvXY currentXY
	,lvXY lastXY
	);//比较两个坐标组，如果超出限定则返回TRUE，未超出则返回FALSE

lvXY getXY(
	CString sXY
	,char d = ','
	);//获取字符串的坐标并返回


int CStringToArray(
	CString source
	,TCHAR d
	,CStringArray& road
	);//将CString字符串按指定字符分割

CString Lng2Str(
	long val
	);//将长整形转为字符串型

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

