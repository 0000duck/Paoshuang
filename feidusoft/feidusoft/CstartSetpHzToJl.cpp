// CstartSetpHzToJl.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "CstartSetpHzToJl.h"
#include "afxdialogex.h"
#include "feidusoftDlg.h"


// CstartSetpHzToJl 对话框

IMPLEMENT_DYNAMIC(CstartSetpHzToJl, CDialogEx)

CstartSetpHzToJl::CstartSetpHzToJl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CstartSetpHzToJl::IDD, pParent)
{

}

CstartSetpHzToJl::~CstartSetpHzToJl()
{
}

void CstartSetpHzToJl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CstartSetpHzToJl, CDialogEx)

	ON_BN_CLICKED(IDC_BTN_SETP1, &CstartSetpHzToJl::OnBnClickedBtnSetp1)
	ON_BN_CLICKED(IDC_BTN_SETP2, &CstartSetpHzToJl::OnBnClickedBtnSetp2)
	ON_BN_CLICKED(IDC_BTN_SETP3, &CstartSetpHzToJl::OnBnClickedBtnSetp3)
	ON_BN_CLICKED(IDC_BTN_SETP4, &CstartSetpHzToJl::OnBnClickedBtnSetp4)
	ON_BN_CLICKED(IDC_BTN_SETP5, &CstartSetpHzToJl::OnBnClickedBtnSetp5)
	ON_BN_CLICKED(IDC_BTN_SETP6, &CstartSetpHzToJl::OnBnClickedBtnSetp6)
	ON_BN_CLICKED(IDC_BTN_SETP7, &CstartSetpHzToJl::OnBnClickedBtnSetp7)
	ON_BN_CLICKED(IDC_BTN_SETP8, &CstartSetpHzToJl::OnBnClickedBtnSetp8)
	ON_BN_CLICKED(IDC_BTN_SETP9, &CstartSetpHzToJl::OnBnClickedBtnSetp9)
END_MESSAGE_MAP()


// CstartSetpHzToJl 消息处理程序





void CstartSetpHzToJl::OnBnClickedBtnSetp1()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,1);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp2()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,2);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp3()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,3);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp4()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,4);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp5()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,5);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp6()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,6);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp7()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,7);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp8()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,8);
	CDialogEx::OnCancel();
}


void CstartSetpHzToJl::OnBnClickedBtnSetp9()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,9);
	CDialogEx::OnCancel();
}
