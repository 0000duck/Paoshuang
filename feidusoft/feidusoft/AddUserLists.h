#pragma once

#include "feidusoftDlg.h"

// AddUserLists 对话框

class AddUserLists : public CDialogEx
{
	DECLARE_DYNAMIC(AddUserLists)

public:
	AddUserLists(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddUserLists();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
