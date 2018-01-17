
// startDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define CONFIG_PATH _T("c:\\windows\\paoshang")

// CstartDlg 对话框
class CstartDlg : public CDialogEx
{
// 构造
public:
	CstartDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_START_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_MachineCode;
	// //注册机器码时的身份识别信息
	CEdit m_regBeizhu;
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnStart();
private:
	// //获取应该程序目录
	CString getFilePos(CString filename);
};
