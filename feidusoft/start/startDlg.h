
// startDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#define CONFIG_PATH _T("c:\\windows\\paoshang")

// CstartDlg �Ի���
class CstartDlg : public CDialogEx
{
// ����
public:
	CstartDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_START_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_MachineCode;
	// //ע�������ʱ�����ʶ����Ϣ
	CEdit m_regBeizhu;
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnStart();
private:
	// //��ȡӦ�ó���Ŀ¼
	CString getFilePos(CString filename);
};
