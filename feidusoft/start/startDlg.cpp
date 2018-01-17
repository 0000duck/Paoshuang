
// startDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "start.h"
#include "startDlg.h"
#include "afxdialogex.h"
#include "D:\github\QianNuQiaoYan\QianNuYanQiao\QianNuYanQiao\QianNuYanQiao\tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstartDlg 对话框

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


// CstartDlg 消息处理程序

BOOL CstartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_MachineCode.SetWindowText(m_dm.GetMachineCode());
	m_dm.SetPath(CONFIG_PATH);
	m_regBeizhu.SetWindowText(m_dm.ReadIni(_T("INFO"),_T("beizhu"),_T("Mcode.ini")));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CstartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CstartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CstartDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CstartDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString beizhu,Mcode;
	m_regBeizhu.GetWindowText(beizhu);
	m_MachineCode.GetWindowText(Mcode);
	if(beizhu.IsEmpty()){
		MessageBox(_T("身份识别信息必须填写，否则机器码改变后软件无法继续使用和找回机器码"),_T("注册信息提示："));
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
		MessageBox((LPCTSTR)LocalAddress,_T("启动信息提示：")); 
		return;
	}
	CDialogEx::OnOK();
}



// //获取应该程序目录
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
