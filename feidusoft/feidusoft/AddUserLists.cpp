// AddUserLists.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "feidusoft.h"
#include "AddUserLists.h"
#include "afxdialogex.h"


// AddUserLists �Ի���

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


// AddUserLists ��Ϣ�������


void AddUserLists::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
