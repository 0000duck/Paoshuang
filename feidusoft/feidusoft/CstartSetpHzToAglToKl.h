#pragma once


// CstartSetpHzToAglToKl 对话框

class CstartSetpHzToAglToKl : public CDialogEx
{
	DECLARE_DYNAMIC(CstartSetpHzToAglToKl)

public:
	CstartSetpHzToAglToKl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CstartSetpHzToAglToKl();

// 对话框数据
	enum { IDD = IDD_DIALOG_HZ_AGL_KL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int startSetpId;//启动进度的ID

	afx_msg void OnBnClickedBtnSetp1();
	afx_msg void OnBnClickedBtnSetp2();
	afx_msg void OnBnClickedBtnSetp3();
	afx_msg void OnBnClickedBtnSetp4();
	afx_msg void OnBnClickedBtnSetp5();
	afx_msg void OnBnClickedBtnSetp6();
	afx_msg void OnBnClickedBtnSetp7();
	afx_msg void OnBnClickedBtnSetp8();
	afx_msg void OnBnClickedBtnSetp9();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
};
