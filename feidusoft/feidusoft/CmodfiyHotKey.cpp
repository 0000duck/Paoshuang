// CmodfiyHotKey.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "CmodfiyHotKey.h"
#include "afxdialogex.h"


// CmodfiyHotKey 对话框

IMPLEMENT_DYNAMIC(CmodfiyHotKey, CDialogEx)

CmodfiyHotKey::CmodfiyHotKey(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmodfiyHotKey::IDD, pParent)
{

}

CmodfiyHotKey::~CmodfiyHotKey()
{
}

void CmodfiyHotKey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CmodfiyHotKey, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CmodfiyHotKey::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CmodfiyHotKey 消息处理程序



void CmodfiyHotKey::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("热键已更新"));
}
