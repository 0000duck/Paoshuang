#pragma once


// CmodfiyHotKey �Ի���

class CmodfiyHotKey : public CDialogEx
{
	DECLARE_DYNAMIC(CmodfiyHotKey)

public:
	CmodfiyHotKey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CmodfiyHotKey();

// �Ի�������
	enum { IDD = IDD_MODFIYHOTKEY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
};
