#pragma once
#include "afxwin.h"


// CaddUser �Ի���

class CaddUser : public CDialogEx
{
	DECLARE_DYNAMIC(CaddUser)

public:
	CaddUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CaddUser();


// �Ի�������
	enum { IDD = IDD_ADDUSER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
