
// feidusoftDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <comutil.h>
#include <stdio.h>
#include <conio.h>
#include <atlconv.h>


// CfeidusoftDlg 对话框
class CfeidusoftDlg : public CDialogEx
{
// 构造
public:
	CfeidusoftDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FEIDUSOFT_DIALOG };
	enum{

	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	api m_api;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	CListCtrl m_userlist;
private:
	bool isHide;
	long cwMin;
	long ch;
	long cw;
	CStatusBar m_statusbar;
	
	bool AddListItem(int id);// 新增一行数据，最多上限五行，ID只能从0-4
	
	CString GetRoleId(int id);// 获取当前线程控制窗口的角色ID
	
	bool SetRoleId(int id);// 设置指定线程索引的角色ID
	
	int GetRoleState(int id);// 获取角色的状态
	
	int SetRoleState(int id);// 设置角色状态，并返回状态值
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnUserlist();
	CListCtrl m_WaitUserlit;
	
	void addUser(CString user, CString pwd,CString sum,CString role);//添加列表帐号信息
	
	void modfiyUser(int nItem,CString user, CString pwd,CString sum,CString role);//修改列表帐号信息
	
	void setConfig(CString section,CString key,CString value,CString file);//配置文件写入
	
	void getConfig(CString section,CString key,CString file,CString& value,CString defVal=_T("0"));//配置文件读取
	
	void setWaitUserlist();//设置待上号区域的帐号列表
	
	void getWaitUserlist();//获取待上号区域的帐号列表
	afx_msg void OnBnClickedBtnModfiyUser();
	CButton m_hideWaitUserlist;
	afx_msg LRESULT SetUserListMsgAsOneRow( WPARAM wp,LPARAM lp );//更新进行中列表的一行信息
	
	afx_msg LRESULT SetUserListMsgAsCell( WPARAM wp,LPARAM lp );//更新或设置进行表中指定单元格的数据
	
	afx_msg LRESULT GetUserListMsgAsCell( WPARAM wp,LPARAM lp );//获取进行表中指定单元格的数据
	
	afx_msg LRESULT SetWaitUserListMsgAsOneRow( WPARAM wp,LPARAM lp );//更新等待列表的一行信息
	
	afx_msg LRESULT SetWaitUserListMsgAsCell( WPARAM wp,LPARAM lp );//更新或设置等待表中指定单元格的数据
	
	afx_msg LRESULT GetWaitUserListMsgAsCell( WPARAM wp,LPARAM lp );//获取等待表中指定单元格的数据
	afx_msg void OnBnClickedBtnSetHotkey();
	CHotKeyCtrl m_setHotKey;
	void UpdateHotKey(CString HotkeyName,UINT modifier, UINT vk);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnAllStop();
	// 跑商类型下拉选择框
	CComboBox m_psType;
	// 跑商线路选择
	CComboBox m_psLine;
	afx_msg void OnBnClickedBtnSaveSetting();
	void SaveSetting(void);
	// 更新配置信息
	void UpdateConfigData(int id);
	afx_msg void OnBnClickedBtnHideShow();
	// 设置启动进度
	int SetStartSetp(int id,int setp);
	afx_msg void OnBnClickedBtnTest();
	// 设置一条待替换帐号
	void setWaitUserListByOne(CString username, CString password,CString sum,CString role=_T(""),CString state = _T(""));
	// 获取待替换帐号列表
	void GetWaitUserLists(CString section_filename,CString list_filename);
	afx_msg void OnBnClickedBtnMoveUp();
	afx_msg void OnBnClickedBtnMoveDown();
	afx_msg void OnBnClickedBtnGamePath();
	CEdit m_GamePathValue;
	// //答题选择下拉框
	CComboBox m_ans;
	// 设置单条跑商列表帐号数据
	void SetWaitUserAsOne(CString list_filename, CString username, CString password, int sum,CString role=_T(""),CString state = _T(""));
	// 按帐号要变更列表数据GetWaitUserListNewUsername
	afx_msg LRESULT SetWaitUserAsCellByUsername( WPARAM wp,LPARAM lp );
	afx_msg LRESULT GetWaitUserListNewUsername(WPARAM wp,LPARAM lp);
	//判断当前角色ID对应的帐号信息，如果在待替列表中找不到择不让启动
	afx_msg LRESULT IsCurrentRoleAsUsername(WPARAM wp,LPARAM lp);
	afx_msg void OnBnClickedBtnWaitUserDel();
	// 重新排序待替换帐号列表
	void ReSortWaitUserList(void);
protected:
	// 解压资源包到指定位置
	bool unResPakage(CString filename, CString undir, LPCSTR password);
public:
	// 机器码
	CEdit m_MachineCode;
	afx_msg void OnBnClickedBtnCopyCode();
	// 跑商量换号
	CComboBox m_paoshangSum;
	CButton m_autohuanhao;
	CButton m_buyYao;
};
