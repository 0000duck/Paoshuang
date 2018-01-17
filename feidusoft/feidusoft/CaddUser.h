#pragma once
#include "afxwin.h"


// CaddUser 对话框

class CaddUser : public CDialogEx
{
	DECLARE_DYNAMIC(CaddUser)

public:
	CaddUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CaddUser();


// 对话框数据
	enum { IDD = IDD_ADDUSER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_user;
	afx_msg void OnBnClickedOk();
	CEdit m_password;
	bool isModfiy;
	int nItem;
	CString title;
	//CString role;
	void setTextContent();
	CEdit m_sum;
	CEdit m_role;
};
