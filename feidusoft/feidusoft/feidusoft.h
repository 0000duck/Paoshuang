
// feidusoft.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfeidusoftApp:
// �йش����ʵ�֣������ feidusoft.cpp
//

class CfeidusoftApp : public CWinApp
{
public:
	CfeidusoftApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfeidusoftApp theApp;