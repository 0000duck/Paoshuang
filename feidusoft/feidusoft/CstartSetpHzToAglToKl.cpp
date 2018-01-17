// CstartSetpHzToAglToKl.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "CstartSetpHzToAglToKl.h"
#include "afxdialogex.h"
#include "feidusoftDlg.h"


// CstartSetpHzToAglToKl 对话框

IMPLEMENT_DYNAMIC(CstartSetpHzToAglToKl, CDialogEx)

CstartSetpHzToAglToKl::CstartSetpHzToAglToKl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CstartSetpHzToAglToKl::IDD, pParent)
{

}

CstartSetpHzToAglToKl::~CstartSetpHzToAglToKl()
{
}

void CstartSetpHzToAglToKl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CstartSetpHzToAglToKl, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETP1, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp1)
	ON_BN_CLICKED(IDC_BTN_SETP2, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp2)
	ON_BN_CLICKED(IDC_BTN_SETP3, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp3)
	ON_BN_CLICKED(IDC_BTN_SETP4, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp4)
	ON_BN_CLICKED(IDC_BTN_SETP5, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp5)
	ON_BN_CLICKED(IDC_BTN_SETP6, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp6)
	ON_BN_CLICKED(IDC_BTN_SETP7, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp7)
	ON_BN_CLICKED(IDC_BTN_SETP8, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp8)
	ON_BN_CLICKED(IDC_BTN_SETP9, &CstartSetpHzToAglToKl::OnBnClickedBtnSetp9)
	ON_BN_CLICKED(IDC_BUTTON2, &CstartSetpHzToAglToKl::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &CstartSetpHzToAglToKl::OnBnClickedButton7)
END_MESSAGE_MAP()


// CstartSetpHzToAglToKl 消息处理程序


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp1()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,1);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp2()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,2);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp3()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,3);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp4()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,4);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp5()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,5);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp6()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,6);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp7()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,7);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp8()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,8);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedBtnSetp9()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,9);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,10);
	CDialogEx::OnCancel();
}


void CstartSetpHzToAglToKl::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	parent->SetStartSetp(startSetpId,11);
	CDialogEx::OnCancel();
}
