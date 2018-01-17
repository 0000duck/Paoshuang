// AddUserLists.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "AddUserLists.h"
#include "afxdialogex.h"


// AddUserLists 对话框

IMPLEMENT_DYNAMIC(AddUserLists, CDialogEx)

AddUserLists::AddUserLists(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddUserLists::IDD, pParent)
{

}

AddUserLists::~AddUserLists()
{
}

void AddUserLists::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddUserLists, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddUserLists::OnBnClickedOk)
END_MESSAGE_MAP()


// AddUserLists 消息处理程序


void AddUserLists::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

}
