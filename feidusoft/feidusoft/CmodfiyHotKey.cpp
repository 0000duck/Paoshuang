// CmodfiyHotKey.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "feidusoft.h"
#include "CmodfiyHotKey.h"
#include "afxdialogex.h"


// CmodfiyHotKey �Ի���

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


// CmodfiyHotKey ��Ϣ�������



void CmodfiyHotKey::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(_T("�ȼ��Ѹ���"));
}
