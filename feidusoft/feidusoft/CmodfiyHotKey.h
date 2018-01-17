#pragma once


// CmodfiyHotKey 对话框

class CmodfiyHotKey : public CDialogEx
{
	DECLARE_DYNAMIC(CmodfiyHotKey)

public:
	CmodfiyHotKey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CmodfiyHotKey();

// 对话框数据
	enum { IDD = IDD_MODFIYHOTKEY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
};
