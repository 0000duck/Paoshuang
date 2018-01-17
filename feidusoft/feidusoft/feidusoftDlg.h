
// feidusoftDlg.h : ͷ�ļ�
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


// CfeidusoftDlg �Ի���
class CfeidusoftDlg : public CDialogEx
{
// ����
public:
	CfeidusoftDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FEIDUSOFT_DIALOG };
	enum{

	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	
	bool AddListItem(int id);// ����һ�����ݣ�����������У�IDֻ�ܴ�0-4
	
	CString GetRoleId(int id);// ��ȡ��ǰ�߳̿��ƴ��ڵĽ�ɫID
	
	bool SetRoleId(int id);// ����ָ���߳������Ľ�ɫID
	
	int GetRoleState(int id);// ��ȡ��ɫ��״̬
	
	int SetRoleState(int id);// ���ý�ɫ״̬��������״ֵ̬
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnUserlist();
	CListCtrl m_WaitUserlit;
	
	void addUser(CString user, CString pwd,CString sum,CString role);//����б��ʺ���Ϣ
	
	void modfiyUser(int nItem,CString user, CString pwd,CString sum,CString role);//�޸��б��ʺ���Ϣ
	
	void setConfig(CString section,CString key,CString value,CString file);//�����ļ�д��
	
	void getConfig(CString section,CString key,CString file,CString& value,CString defVal=_T("0"));//�����ļ���ȡ
	
	void setWaitUserlist();//���ô��Ϻ�������ʺ��б�
	
	void getWaitUserlist();//��ȡ���Ϻ�������ʺ��б�
	afx_msg void OnBnClickedBtnModfiyUser();
	CButton m_hideWaitUserlist;
	afx_msg LRESULT SetUserListMsgAsOneRow( WPARAM wp,LPARAM lp );//���½������б��һ����Ϣ
	
	afx_msg LRESULT SetUserListMsgAsCell( WPARAM wp,LPARAM lp );//���»����ý��б���ָ����Ԫ�������
	
	afx_msg LRESULT GetUserListMsgAsCell( WPARAM wp,LPARAM lp );//��ȡ���б���ָ����Ԫ�������
	
	afx_msg LRESULT SetWaitUserListMsgAsOneRow( WPARAM wp,LPARAM lp );//���µȴ��б��һ����Ϣ
	
	afx_msg LRESULT SetWaitUserListMsgAsCell( WPARAM wp,LPARAM lp );//���»����õȴ�����ָ����Ԫ�������
	
	afx_msg LRESULT GetWaitUserListMsgAsCell( WPARAM wp,LPARAM lp );//��ȡ�ȴ�����ָ����Ԫ�������
	afx_msg void OnBnClickedBtnSetHotkey();
	CHotKeyCtrl m_setHotKey;
	void UpdateHotKey(CString HotkeyName,UINT modifier, UINT vk);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnAllStop();
	// ������������ѡ���
	CComboBox m_psType;
	// ������·ѡ��
	CComboBox m_psLine;
	afx_msg void OnBnClickedBtnSaveSetting();
	void SaveSetting(void);
	// ����������Ϣ
	void UpdateConfigData(int id);
	afx_msg void OnBnClickedBtnHideShow();
	// ������������
	int SetStartSetp(int id,int setp);
	afx_msg void OnBnClickedBtnTest();
	// ����һ�����滻�ʺ�
	void setWaitUserListByOne(CString username, CString password,CString sum,CString role=_T(""),CString state = _T(""));
	// ��ȡ���滻�ʺ��б�
	void GetWaitUserLists(CString section_filename,CString list_filename);
	afx_msg void OnBnClickedBtnMoveUp();
	afx_msg void OnBnClickedBtnMoveDown();
	afx_msg void OnBnClickedBtnGamePath();
	CEdit m_GamePathValue;
	// //����ѡ��������
	CComboBox m_ans;
	// ���õ��������б��ʺ�����
	void SetWaitUserAsOne(CString list_filename, CString username, CString password, int sum,CString role=_T(""),CString state = _T(""));
	// ���ʺ�Ҫ����б�����GetWaitUserListNewUsername
	afx_msg LRESULT SetWaitUserAsCellByUsername( WPARAM wp,LPARAM lp );
	afx_msg LRESULT GetWaitUserListNewUsername(WPARAM wp,LPARAM lp);
	//�жϵ�ǰ��ɫID��Ӧ���ʺ���Ϣ������ڴ����б����Ҳ�����������
	afx_msg LRESULT IsCurrentRoleAsUsername(WPARAM wp,LPARAM lp);
	afx_msg void OnBnClickedBtnWaitUserDel();
	// ����������滻�ʺ��б�
	void ReSortWaitUserList(void);
protected:
	// ��ѹ��Դ����ָ��λ��
	bool unResPakage(CString filename, CString undir, LPCSTR password);
public:
	// ������
	CEdit m_MachineCode;
	afx_msg void OnBnClickedBtnCopyCode();
	// ����������
	CComboBox m_paoshangSum;
	CButton m_autohuanhao;
	CButton m_buyYao;
};
