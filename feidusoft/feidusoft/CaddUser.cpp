// CaddUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "feidusoft.h"
#include "CaddUser.h"
#include "afxdialogex.h"
#include "feidusoftDlg.h"


// CaddUser �Ի���

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


// CaddUser ��Ϣ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CfeidusoftDlg * parent = (CfeidusoftDlg*)GetParent();
	CString user,password,sum,role;
	this->m_user.GetWindowText(user);//��ȡ��ǰ�Ӵ��ڱ༭���е�ֵ
	this->m_password.GetWindowText(password);
	this->m_sum.GetWindowText(sum);
	this->m_role.GetWindowText(role);
	if(user.IsEmpty() || password.IsEmpty() || sum.IsEmpty()){
		MessageBox(_T("�ʺš������Ʊ������Ϊ��"),_T("��Ϣ��ʾ"));
		return;
	}
	if (isModfiy)
	{
		parent->modfiyUser(nItem,user,password,sum,role);//�ѵ�ǰ�༭���е�ֵ�ش���������
	}else{
		parent->addUser(user,password,sum,role);//�ѵ�ǰ�༭���е�ֵ�ش���������
	}
	
	CDialogEx::OnOK();
}
