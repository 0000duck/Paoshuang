// CaddUser.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "CaddUser.h"
#include "afxdialogex.h"
#include "feidusoftDlg.h"


// CaddUser 对话框

IMPLEMENT_DYNAMIC(CaddUser, CDialogEx)

CaddUser::CaddUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CaddUser::IDD, pParent)
{
	//isModfiy = false;
}

CaddUser::~CaddUser()
{
}

void CaddUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_user);
	DDX_Control(pDX, IDC_EDIT2, m_password);
	DDX_Control(pDX, IDC_EDIT3, m_sum);
	DDX_Control(pDX, IDC_EDIT4, m_role);
	setTextContent();
	SetWindowText(title);
}


BEGIN_MESSAGE_MAP(CaddUser, CDialogEx)
	ON_BN_CLICKED(IDOK, &CaddUser::OnBnClickedOk)
END_MESSAGE_MAP()


// CaddUser 消息处理程序
void CaddUser::setTextContent(){
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	CString user = parent->m_WaitUserlit.GetItemText(nItem,0);
	CString password = parent->m_WaitUserlit.GetItemText(nItem,1);
	CString sum = parent->m_WaitUserlit.GetItemText(nItem,2);
	CString role = parent->m_WaitUserlit.GetItemText(nItem,3);
	m_user.SetWindowText(user);
	m_password.SetWindowText(password);
	if (sum.IsEmpty())
	{
		sum  = _T("0");
	}
	m_sum.SetWindowText(sum);
	m_role.SetWindowText(role);
}


void CaddUser::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	CString user,password,sum,role;
	this->m_user.GetWindowText(user);//获取当前子窗口编辑框中的值
	this->m_password.GetWindowText(password);
	this->m_sum.GetWindowText(sum);
	this->m_role.GetWindowText(role);
	if(user.IsEmpty() || password.IsEmpty() || sum.IsEmpty()){
		MessageBox(_T("帐号、密码或票数不能为空"),_T("信息提示"));
		return;
	}
	if (isModfiy)
	{
		parent->modfiyUser(nItem,user,password,sum,role);//把当前编辑框中的值回传给父窗口
	}else{
		parent->addUser(user,password,sum,role);//把当前编辑框中的值回传给父窗口
	}
	
	CDialogEx::OnOK();
}
