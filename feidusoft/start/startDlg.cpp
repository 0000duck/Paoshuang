
// startDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "start.h"
#include "startDlg.h"
#include "afxdialogex.h"
#include "D:\github\QianNuQiaoYan\QianNuYanQiao\QianNuYanQiao\QianNuYanQiao\tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstartDlg �Ի���

tools m_dm;



CstartDlg::CstartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CstartDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CstartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_MachineCode);
	DDX_Control(pDX, IDC_EDIT2, m_regBeizhu);
}

BEGIN_MESSAGE_MAP(CstartDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CstartDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_START, &CstartDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CstartDlg ��Ϣ�������

BOOL CstartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_MachineCode.SetWindowText(m_dm.GetMachineCode());
	m_dm.SetPath(CONFIG_PATH);
	m_regBeizhu.SetWindowText(m_dm.ReadIni(_T("INFO"),_T("beizhu"),_T("Mcode.ini")));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CstartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CstartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CstartDlg::OnBnClickedBtnClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CstartDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString beizhu,Mcode;
	m_regBeizhu.GetWindowText(beizhu);
	m_MachineCode.GetWindowText(Mcode);
	if(beizhu.IsEmpty()){
		MessageBox(_T("���ʶ����Ϣ������д�����������ı������޷�����ʹ�ú��һػ�����"),_T("ע����Ϣ��ʾ��"));
	}
	m_dm.WriteIni(_T("INFO"),_T("code"),Mcode,_T("Mcode.ini"));
	m_dm.WriteIni(_T("INFO"),_T("beizhu"),beizhu,_T("Mcode.ini"));
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;
	BOOL fRet=CreateProcess(getFilePos(_T("feidusoft.exe")),NULL,NULL,FALSE,NULL,NULL,NULL,NULL,&si,&pi);
	//m_dm.RunApp(_T("C:\\feidu\\Release_exe\\feidusoft.exe"),0);
	DWORD ErrorCode =  GetLastError();
	if(GetLastError() != 0){
		HLOCAL LocalAddress=NULL;    
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,  
			NULL,ErrorCode,0,(PTSTR)&LocalAddress,0,NULL);    
		MessageBox((LPCTSTR)LocalAddress,_T("������Ϣ��ʾ��")); 
		return;
	}
	CDialogEx::OnOK();
}



// //��ȡӦ�ó���Ŀ¼
CString CstartDlg::getFilePos(CString filename)
{
	CString m_FilePath;
	GetModuleFileName(NULL,m_FilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	m_FilePath.ReleaseBuffer();
	int m_iPosIndex;
	m_iPosIndex = m_FilePath.ReverseFind('\\');
	m_FilePath = m_FilePath.Left(m_iPosIndex);
	CString file;
	file.Format(_T("%s\\%s"),m_FilePath,filename);
	return file;
}
