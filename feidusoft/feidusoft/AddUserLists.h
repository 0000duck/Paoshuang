#pragma once

#include "feidusoftDlg.h"

// AddUserLists �Ի���

class AddUserLists : public CDialogEx
{
	DECLARE_DYNAMIC(AddUserLists)

public:
	AddUserLists(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddUserLists();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
