
// feidusoftDlg.cpp : ʵ���ļ�
//


#include "stdafx.h"


#include "feidusoft.h"
#include "feidusoftDlg.h"
#include "AddUserLists.h"
#include "CmodfiyHotKey.h"
#include "CaddUser.h"
#include "CstartSetpHzToJl.h"
#include "CstartSetpHzToAglToKl.h"
#include "afxdialogex.h"

#include "func.h"
#include "YunSu_Interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//ȫ�ֶ���
api m_tapi;//����ȫ��api
tools dm[opens+1];//ȫ�ִ�Į����
BOOL isStartThread[opens+1];//�߾��̱߳��
BOOL isStopThread[opens+1];
long hwnd[opens+1];//���ھ��
BOOL isMaze[opens+1];
tools m_dm;
HANDLE Handle[opens+1];//�߳̾��
CString pathConfig;
CString configName;
lvUserList msgUserlist[opens+1];
lvWaitUserList msgWaitUserlist[opens+1];
BOOL isReward[opens+1];
int isAutoHuanhao[opens+1];
int isBuyYao[opens+1];
lvConfig ConfigData[opens+1];//����������Ϣ
int StartSetp[opens+1];//�������ȱ���
lvAccount AccountInfo[opens+1];//���������ʺ���Ϣ
bool oneRun = false;//����falseʱ����������ȥ��true������������ȥ
CString game_path;
bool isUserEnd = false;//�������ʺſ��Ը���ʱ����false,û���ʺŸ���ʱ����true
CString bmpDir;
bool is_windows[opens+1];
CString yaoNames;
long gt1[opens+1],gt2[opens+1];


HANDLE m_hMutex = CreateMutex(NULL, FALSE, NULL);//������
CString GetRoleId(int id);//��ȡ��ɫID
long GetWindowHwnd(int id);//��ȡ��Ϸ���ھ��
long BindGameWindow(int id);//����Ϸ����
BOOL Delay(int id,long minMs,long maxMs);//��ʱ����װSleep��Maze,FALSE���Թ���,TRUEû���Թ�
void AutoPaoshuang(int id);//��ʼ�������̹���
CString CurrentMap(int id);//ȡ��ǰ��ͼ
STATE MoveGhNpc(int id);//����ɫ�ƶ������NPC
STATE HuiBang(int id);//���ذ��
BOOL paoshangTakeTask(int id);//��ȡ��������
STATE paoshangMove(int id);//�ƶ����￪ʼ����
STATE paoshangGMSP(int id);//�������
STATE paoshangCSSP(int id);//���ۻ���
STATE AutoPathfindingToShangRen(int id,CString keyStr,CString npcName);//�Զ�Ѱ·������/����NPC
STATE GoNextMap(int id,LPTSTR npcName,CString keyStr1,LPTSTR npcMsg,LPTSTR nextMap);//ǰȥ��һ����ͼ��NPC��ʽ��
STATE GoNextMapBySpot(int id,LPTSTR npcName,CString keyStr,LPTSTR stopPos,lvXY ClickPos,LPTSTR nextMap);//ǰȥ��һ����ͼ�����͵㷽ʽ��
STATE OpenSHSD(int id);//���̻��̵�
STATE paoshangGiveTask(int id);//������ɣ��ͻ�����
BOOL TakeTask(int id);//������
STATE TakeReward(int id);//���ͽ�
BOOL Maze(int id);//�Թ���
lvXY MazeMove(int id);//�Թ����ƶ��ж�
BOOL MazePosition(int id,lvXY inXY);//false���򲻶ԣ�true������ȷ
lvXY MazePos(int id);//��ȡ��ǰ��ɫ�����Թ�λ�ã��ַ������꣩
int isLastPos(int currentPos,int OkPos);
void Verifiy(int id);//��֤����
void ExitMsg(int id,int s);//�����Ϸ�����д���
void msgExit(int id);//�رյ�������Ϣ����
void KeyPressChar(int id,CString str);//�����ı�
void PublicAnswer(int id,LPSTR filename,int x,int y);
void Haoi_Answer(int id,LPSTR filename,int x,int y);
void YunSu_Answer(int id,LPSTR filename,int x,int y);

void movePosClick(int id,long x,long y);
int timeBuyYao(int id);

BOOL MinExit(int id);

BOOL AutoExit(int id,CString username);
BOOL AutoLogin(int id,CString password);
void CancelShangRen(int id);//ȡ����λ

void Log(int id,CString str);//��־�ļ�д��

void setRunState(int id,CString state);//��������״̬

void SetWaitUserAsOne(CString list_filename,CString username,CString password,int sum,CString role=_T(""),CString state = _T(""));//���µ������̴����б�����

void UpdateWaitUserDataBySum(int id,lvAccount la);//�������̼���
//���ߣ����ݵ�����
void InLineHzToJl(int id);
void InLineHzToJl(int id){
	//0�ж��Ƿ��ں��ݣ�������ڣ�Ҳ���ڰ����ƶ�������
	//1���������
	//2��������
	//3��ʼѰ�Һ���NPC���̻��̵�
	//3-1������Ʒ
	//4ǰȥ��һ����ͼ�����꣩
	//5Ѱ�ҽ���NPC���̻��̵�
	//5-1������Ʒ
	//5-2������Ʒ
	//6ǰȥ��һ����ͼ�����ݣ�
	//7Ѱ�Һ���NPC���̻��̵�
	//7-1������Ʒ
	//7-2������Ʒ
	//8������4����ֱ����Ʊ
	//����ÿһ������װ�ã����з���ֵ��ָ������ֵ���ʾ�����Թ��������ˣ�Ȼ����ת���ϴβ�����������
	//MinExit(id);
	//return;
	//AutoExit(id,_T("1326862663@qq.com"));
	//AutoLogin(id,_T("*963.*963."));
	//return;
	switch(StartSetp[id])
	{
	case 1:
		goto setp1;
		break;
	case 2:
		goto setp2;
		break;
	case 3:
		goto setp4;
		break;
	case 4:
		goto setp5;
		break;
	case 5:
		goto setp8_1;
		break;
	case 6:
		goto setp9;
		break;
	case 7:
		goto setp10;
		break;
	case 8:
		goto setp13_1;
		break;
	case 9:
		goto give;
		break;
	default:
		MessageBox(0,_T("�޷�ʶ��Ľ��ȣ�����ֹͣ"),_T("������ʾ��"),MB_OK);
		return;
	}
	STATE result;
setp1:
	MoveGhNpc(id);//�ƶ����ذ�
setp1_1:
	paoshangGiveTask(id);//������|������
setp2:
	result = paoshangMove(id);//�ƶ����ﵽ����NPC������
	if (result == MAZE)goto setp2;
setp3:
	result = OpenSHSD(id);//���̻��̵�
	if (result == MAZE || result == FALSE)goto setp2;
setp3_1:
	result = paoshangGMSP(id);
	if (result == MAZE)goto setp2;
setp4:
	result = GoNextMap(id,_T("�ܺ���"),_T("zhf"),_T("ǰȥ����"),_T("����"));//ȥ���¸���ͼ
	if (result == MAZE)goto setp4;
setp5:
	result = AutoPathfindingToShangRen(id,_T("jlsr"),_T("��������"));//Ѱ�����˻���NPC
	if (result == MAZE)goto setp5;
setp6:
	result = OpenSHSD(id);//���̻��̵�
	if (result == FALSE)goto setp5;
	else if(result == MAZE)goto setp5;
setp7:
	result = paoshangCSSP(id);
	if(result == MAZE)goto setp5;
setp8:
	result = paoshangGMSP(id);
	if (result == FALSE)goto give;
	else if (result == MAZE)
	{
setp8_1:
		result = AutoPathfindingToShangRen(id,_T("jlsr"),_T("��������"));//Ѱ�����˻���NPC
		if (result == MAZE)goto setp8_1;
		result = OpenSHSD(id);//���̻��̵�
		if (result == FALSE)goto setp8_1;
		else if(result == MAZE)goto setp8_1;
		goto setp8;
	}
setp9:
	result = GoNextMap(id,_T("��ȫ��"),_T("cqf"),_T("ǰȥ����"),_T("����"));
	if(result == MAZE)goto setp9;
setp10:
	result = AutoPathfindingToShangRen(id,_T("hzsr"),_T("��������"));//Ѱ�����˻���NPC
	if(result == MAZE)goto setp10;
setp11:
	result = OpenSHSD(id);//���̻��̵�
	if (result == FALSE)goto setp10;
	else if(result == MAZE)goto setp10;
setp12:
	result = paoshangCSSP(id);//
	if(result == MAZE)goto setp10;
setp13:
	result = paoshangGMSP(id);
	if (result == FALSE)goto give;
	else if (result == MAZE)
	{
setp13_1:
		result = AutoPathfindingToShangRen(id,_T("hzsr"),_T("��������"));//Ѱ�����˻���NPC
		if(result == MAZE)goto setp13_1;
		result = OpenSHSD(id);//���̻��̵�
		if (result == FALSE)goto setp13_1;
		else if(result == MAZE)goto setp13_1;
		goto setp13;
	}
	goto setp4;
give://��Ʊ���
	result = MoveGhNpc(id);
	if(result == MAZE)goto give;
	result = paoshangGiveTask(id);
	if(result == MAZE)goto give;
	else if(result == QUIT)return;
	goto setp2;
	
}

//���ߣ�����-������-����
void OutLineHzToAglToKl(int id);
void OutLineHzToAglToKl(int id){
	lvXY hz2tzhaClickPos;//���ݵ�̨�ݺ����ĵ������
	hz2tzhaClickPos.x = 520;
	hz2tzhaClickPos.y = 266;
	//goto setp8_1;
	switch(StartSetp[id])
	{
	case 1:
		goto setp1;
		break;
	case 2:
		goto setp2;
		break;
	case 3:
		goto setp4;
		break;
	case 4:
		goto setp5;
		break;
	case 5:
		goto setp5_1;
		break;
	case 6:
		goto setp6;
		break;
	case 7:
		goto setp6_1;
		break;
	case 8:
		goto setp7;
		break;
	case 9:
		goto setp7_1;
		break;
	case 10:
		goto setp8;
		break;
	case 11:
		goto give;
		break;
	default:
		MessageBox(0,_T("�޷�ʶ��Ľ��ȣ�����ֹͣ"),_T("������ʾ��"),MB_OK);
		return;
	}
	STATE result;
setp1:
	MoveGhNpc(id);//�ƶ����ذ�
setp1_1:
	paoshangGiveTask(id);//������|������
setp2:
	result = paoshangMove(id);//�ƶ����ﵽ����NPC������
	if (result == MAZE)goto setp2;
setp3:
	result = OpenSHSD(id);//���̻��̵�
	if (result == MAZE || result == FALSE)goto setp2;
setp3_1:
	result = paoshangGMSP(id);
	if (result == MAZE)goto setp2;
setp4:
	result = GoNextMapBySpot(id,_T("���͵�̨�ݺ���"),_T("csd"),HZTOTZHA,hz2tzhaClickPos,_T("̨�ݺ���"));
	if (result == MAZE)goto setp4;
setp5:
	result = GoNextMap(id,_T("����ɽ"),_T("xws"),_T("ǰ��������ɽ"),_T("������ɽ"));//ȥ���¸���ͼ
	if (result == MAZE)goto setp5;
setp5_1:
	result = AutoPathfindingToShangRen(id,_T("xssr"),_T("��ɽ����"));//Ѱ�����˻���NPC
	if (result == MAZE)goto setp5_1;
setp5_2:
	result = OpenSHSD(id);//���̻��̵�
	if (result == FALSE)goto setp5_1;
	else if(result == MAZE)goto setp5_1;
setp5_3:
	result = paoshangCSSP(id);
	if(result == MAZE)goto setp5_1;
setp5_4:
	result = paoshangGMSP(id);
	if (result == FALSE)goto give;
	else if (result == MAZE)
	{
setp5_5:
		result = AutoPathfindingToShangRen(id,_T("xssr"),_T("��ɽ����"));//Ѱ�����˻���NPC
		if (result == MAZE)goto setp5_5;
		result = OpenSHSD(id);//���̻��̵�
		if (result == FALSE)goto setp5_5;
		else if(result == MAZE)goto setp5_5;
		goto setp5_4;
	}
setp6:
	result = GoNextMap(id,_T("����ȫ"),_T("clq"),_T("�ϴ�ǰȥ������"),_T("������"));
	if (result == MAZE)goto setp6;
setp6_1:
	result = AutoPathfindingToShangRen(id,_T("aglsr"),_T("����������"));//Ѱ�����˻���NPC
	if (result == MAZE)goto setp6_1;
setp6_2:
	result = OpenSHSD(id);//���̻��̵�
	if (result == FALSE)goto setp6_1;
	else if(result == MAZE)goto setp6_1;
setp6_3:
	result = paoshangCSSP(id);
	if(result == MAZE)goto setp6_1;
setp6_4:
	result = paoshangGMSP(id);
	if (result == FALSE)goto give;
	else if (result == MAZE)
	{
setp6_5:
		result = AutoPathfindingToShangRen(id,_T("aglsr"),_T("����������"));//Ѱ�����˻���NPC
		if (result == MAZE)goto setp6_5;
		result = OpenSHSD(id);//���̻��̵�
		if (result == FALSE)goto setp6_5;
		else if(result == MAZE)goto setp6_5;
		goto setp6_4;
	}
setp7:
	result = GoNextMap(id,_T("����"),_T("lj"),_T("���������̶�"),_T("���ػ�Į"));
	if (result == MAZE)goto setp7;
setp7_1:
	result = AutoPathfindingToShangRen(id,_T("klsr"),_T("��������"));//Ѱ�����˻���NPC
	if (result == MAZE)goto setp7_1;
setp7_2:
	result = OpenSHSD(id);//���̻��̵�
	if (result == FALSE)goto setp7_1;
	else if(result == MAZE)goto setp7_1;
setp7_3:
	result = paoshangCSSP(id);
	if(result == MAZE)goto setp7_1;
setp7_4:
	result = paoshangGMSP(id);
	if (result == FALSE)goto give;
	else if (result == MAZE)
	{
setp7_5:
		result = AutoPathfindingToShangRen(id,_T("klsr"),_T("��������"));//Ѱ�����˻���NPC
		if (result == MAZE)goto setp7_5;
		result = OpenSHSD(id);//���̻��̵�
		if (result == FALSE)goto setp7_5;
		else if(result == MAZE)goto setp7_5;
		goto setp7_4;
	}
setp8:
	result = GoNextMap(id,_T("�˹�"),_T("dk"),_T("���밢�����̶�"),_T("������"));
	if (result == MAZE)goto setp8;
	goto setp6_1;
give://��Ʊ���
	result = MoveGhNpc(id);
	if(result == MAZE)goto give;
	result = paoshangGiveTask(id);
	if(result == MAZE)goto give;
	else if(result == QUIT)return;
	goto setp2;
}

void PublicAnswer(int id,LPSTR filename,int x,int y){
	switch(ConfigData[id].DM_line)
	{
	case 1:
		Haoi_Answer(id,filename,x,y);
		break;
	case 2:
		YunSu_Answer(id,filename,x,y);
		break;
	default:
		YunSu_Answer(id,filename,x,y);
		break;
	}
	
}


BOOL Delay(int id,long minMs,long maxMs){
	
	if(id>=0 && id<=opens){
		dm[id].Delays(minMs,maxMs);
        return Maze(id);
	}else{
		m_dm.Delays(minMs,maxMs);
	}
	
	return TRUE;
}

void MoveTo(int id,int x,int y,bool isrun = true){
	bool win = false;
	if(is_windows[id] == true && isrun == true){
		while(oneRun){
			dm[id].Delay(50);
			if(!win)win = true;
		}
		if(dm[id].GetWindowState(hwnd[id],1)==0){
			dm[id].SetWindowState(hwnd[id],1);
		}
		//if(win)dm[id].SetWindowState(hwnd[id],1);
	}
	dm[id].MoveTo(x,y);
}

void LeftClick(int id,bool isrun = true){
	bool win = false;
	if(is_windows[id] == true && isrun == true){
		while(oneRun){
			dm[id].Delay(50);
			if(!win)win = true;
		}
		if(dm[id].GetWindowState(hwnd[id],1)==0){
			dm[id].SetWindowState(hwnd[id],1);
		}
	}
	dm[id].LeftClick();
}

void msgExit(int id){
	VARIANT x,y;
	int index_dict = dm[id].GetNowDict();
	dm[id].UseDict(DICT_DEFAULT);
	dm[id].FindStr(250,240,630,400,_T("�ر�"),_T("ffffff-101010"),1.0,&x,&y);
	if(x.intVal>=0 && y.intVal>=0){
		MoveTo(id,x.intVal+10,y.intVal+5);
		dm[id].Delay(66);
		LeftClick(id);
	}
	dm[id].UseDict(index_dict);
}

//�����̺߳��� id �߳�����ID
void paoshangThread(int id){//���̹���
	BOOL result;
	CString msg;
	gt1[id] = dm[id].GetTime();
	//WaitForSingleObject(m_hMutex, INFINITE);
	while(oneRun){
		dm[id].Delay(50);
	}
	oneRun = true;
	hwnd[id] = GetWindowHwnd(id);
	if(hwnd[id] == 0){
		CloseHandle(Handle[id]);
		isStartThread[id] = FALSE;
		return;
	}
	if (hwnd[id]==0)
	{
		MessageBox(0,_T("δ��ȡ����Ϸ����,��������ʧ�ܣ�"),_T("��Ϣ��ʾ��"),MB_OK);
		CloseHandle(Handle[id]);
		oneRun = false;
		//ReleaseMutex(m_hMutex);
		isStartThread[id] = FALSE;
		isStopThread[id] = FALSE;
		return;
	}
	AccountInfo[id].ret = true;
	msgUserlist[id].hwnd = Lng2Str(hwnd[id]);
	msgUserlist[id].nItem = id;
	msgUserlist[id].roleId = GetRoleId(id);
	AccountInfo[id].roleId = msgUserlist[id].roleId;
	msgUserlist[id].state = _T("������");
	msgUserlist[id].currentMap = _T("����");
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_USERLIST_SETONEROW_MSG,(WPARAM)&msgUserlist[id],0);
	//�����￪ʼ�ɽ�ɫID�ж��ʺ���Ϣ
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_IS_CURRENT_ROLE_AS_USERNAME,(WPARAM)&AccountInfo[id],id);
	if(AccountInfo[id].ret == FALSE){
		
		CloseHandle(Handle[id]);
		oneRun = false;
		//ReleaseMutex(m_hMutex);
		isStartThread[id] = FALSE;
		MessageBox(0,AccountInfo[id].msg,_T("��Ϣ��ʾ��"),MB_OK);
		return;
	}
	oneRun = false;
	//ReleaseMutex(m_hMutex);
	switch(ConfigData[id].paoshangType)
	{
	case 1:
		break;
	case 2:
		break;
	default:
		CloseHandle(Handle[id]);
		//ReleaseMutex(m_hMutex);
		isStartThread[id] = FALSE;
		msg.Format(_T("�޷�ʶ����������ͣ���ɫID��%s ��ֹͣ����"),msgUserlist[id].roleId);
		MessageBox(0,msg,_T("������ʾ��"),MB_OK);
		goto exitPaoShang;
	}
	int test_bind = BindGameWindow(id);
	TRACE("�����룺%d\n",dm[id].GetLastError());
	TRACE("���԰�״̬%d\n",test_bind);
	isMaze[id] = FALSE;
	//Delay(-1,2000,2500);
	if(AccountInfo[id].ret == TABUSER){
		//�л��ʺ�
		//AccountInfo[id].sum = 0;
		while(oneRun){
			dm[id].Delay(50);
		}
		oneRun = true;
		::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_WAIT_USERLIST_GET_NEW_USERNAME,(WPARAM)&AccountInfo[id],id);
		oneRun = false;
		if(AutoExit(id,AccountInfo[id].username) == QUIT){
			setRunState(id,_T("������"));
			return;
		}
		if(AutoLogin(id,AccountInfo[id].password) == QUIT){
			setRunState(id,_T("������"));
			return;
		}
	}
	//dm[id].UseDict(DICT_DEFAULT);
	setRunState(id,_T("������"));
	TRACE("��ǰ��·��%d\n",ConfigData[id].PaoshangLine);
	switch(ConfigData[id].PaoshangLine)
	{
	case 1:
		InLineHzToJl(id);
		break;
	case 2:
		OutLineHzToAglToKl(id);
		break;
	default:
		setRunState(id,_T("����ʧ��"));
		msg.Format(_T("�޷�ʶ���������·,��ɫID��%s��ֹͣ����"),msgUserlist[id].roleId);
		MessageBox(0,msg,_T("������ʾ��"),MB_OK);
		goto exitPaoShang;
		break;
	}

exitPaoShang:
	dm[id].UnBindWindow();
	CloseHandle(Handle[id]);
	isStartThread[id] = FALSE;
}

void setRunState(int id,CString state){
	msgUserlist[id].nItem = id;
	msgUserlist[id].nSubItem = 4;
	msgUserlist[id].resultTitle = state;
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_USERLIST_SETCELL_MSG,(WPARAM)&msgUserlist[id],0);
}

/************************************************************************/
/* С����Ϸ���л���ɫ��                                                                     */
/************************************************************************/
BOOL MinExit(int id){
	VARIANT intX,intY,x,y,x2,y2;
	long t1,t2;
	int index_dict = dm[id].GetNowDict();
	dm[id].UseDict(DICT_DEFAULT);
	setRunState(id,_T("С����"));
	//�޸��ļ�Ŀ¼program\setting.ini  [Login]->[Save]->1 (������ס�ʺ�)
	//�޸��ļ�Ŀ¼program\setting.ini  [Login]->[LoginUser]->value (Ĭ��ѡ��)
reExit:
	t1 = dm[id].GetTime();
	MoveTo(id,784,510);
	dm[id].Delays(100,200);
	LeftClick(id);
ExitOk:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("�л���ɫ"),_T("fff7be-101010"),1.0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
		//�л���ɫ
		MoveTo(id,intX.intVal+10,intY.intVal+5);
		dm[id].Delays(100,150);
		LeftClick(id);
	}else{
		t2 = dm[id].GetTime();
		if ((t2-t1)/1000>5)
		{
			dm[id].Delays(100,200);
			goto reExit;
		}
		goto ExitOk;
	}
	//�˳�����������Ƿ����˳�
isExit:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("��ȷ��Ҫ����ѡ���ɫ��?"),_T("ffffff-101010"),1.0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
		MoveTo(id,intX.intVal+100,intY.intVal+70);
		dm[id].Delays(100,150);
		LeftClick(id);
	}else{
		goto isExit;
	}
reJryx:
	bmpDir.Format(_T("%s%s"),pathConfig,_T("jryx.bmp"));
	dm[id].FindPic(0,0,800,600,bmpDir,_T("101010"),0.95,0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		//ȷ�Ͻ�ɫ������ɹ�
		TRACE("���ؽ�ɫ\n");
		dm[id].Delays(3000,4000);
		dm[id].FindStr(0,0,800,600,_T("���������ɫ��"),_T("ffffff-101010"),1.0,&x2,&y2);
		if (x2.intVal>=0 && x2.intVal>=0)
		{
			TRACE("���¼��ؽ�ɫ\n");
			goto reJryx;
		}
		TRACE("���ؽ�ɫ�ɹ�\n");
		MoveTo(id,x.intVal+10,y.intVal+5);
		dm[id].Delays(200,300);
		LeftClick(id);
	}else{
		TRACE("�����ؽ�ɫ\n");
		dm[id].Delays(80,120);
		goto reJryx;
	}
isLoad:
	dm[id].FindStr(0,450,185,570,_T("���������"),_T("a8e0ff-101010"),1.0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		TRACE("���뵽��Ϸ\n");
		ExitMsg(id,2);
		dm[id].Delays(500,800);
		MoveTo(id,x.intVal+102,y.intVal-22);
		dm[id].Delays(80,130);
		LeftClick(id);
	}else{
		TRACE("��û�н��뵽��Ϸ\n");
		dm[id].Delays(300,500);
		goto isLoad;
	}
	while(oneRun){
		dm[id].Delay(50);
	}
	oneRun = true;
	//WaitForSingleObject(m_hMutex, INFINITE);
	m_dm.SetWindowState(hwnd[id],1);
	dm[id].Delays(200,300);
	m_dm.KeyDown(17);
	dm[id].Delays(200,300);
	dm[id].KeyPress(121);
	dm[id].Delays(200,300);
	dm[id].KeyPress(121);
	dm[id].Delays(200,300);
	dm[id].KeyPress(122);
	dm[id].Delays(200,300);
	dm[id].KeyPress(122);
	dm[id].Delays(200,300);
	m_dm.KeyUp(17);
	//ReleaseMutex(m_hMutex);
	oneRun = false;
	setRunState(id,_T("������"));
	dm[id].UseDict(index_dict);
	return TRUE;
}
/************************************************************************/
/* �Զ��˳���Ϸ���µ�¼                                                                     */
/************************************************************************/

BOOL AutoExit(int id,CString username){
	if (isAutoHuanhao[id] == 0)
	{
		return QUIT;
	}
	VARIANT intX,intY,x,y;
	long t1,t2;
	int index_dict = dm[id].GetNowDict();
	dm[id].UseDict(DICT_DEFAULT);
	setRunState(id,_T("�л��ʺ�"));
	//�޸��ļ�Ŀ¼program\setting.ini  [Login]->[Save]->1 (������ס�ʺ�)
	//�޸��ļ�Ŀ¼program\setting.ini  [Login]->[LoginUser]->value (Ĭ��ѡ��)
	//WaitForSingleObject(m_hMutex, INFINITE);
	while(oneRun){
		dm[id].Delay(50);
	}
	oneRun = true;
reExit:
	t1 = dm[id].GetTime();
	MoveTo(id,784,510,false);
	dm[id].Delays(100,200);
	LeftClick(id,false);
ExitOk:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("���µ�¼"),_T("fff7be-101010"),1.0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
		//�˳���Ϸʱ���ʺ�д�������ļ�Ĭ���ʺ�
		CString game_setting_path = game_path;
		game_setting_path.Append(_T("\\program\\setting.ini"));
		dm[id].WriteIni(_T("Login"),_T("LoginUser"),username,game_setting_path);
		MoveTo(id,intX.intVal+10,intY.intVal+5,false);
		dm[id].Delays(100,150);
		LeftClick(id,false);
	}else{
		t2 = dm[id].GetTime();
		if ((t2-t1)/1000>5)
		{
			dm[id].Delays(100,200);
			goto reExit;
		}
		goto ExitOk;
	}
	//�˳�����������Ƿ����˳�
isExit:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("��ȷ��Ҫ���µ�¼��?"),_T("ffffff-101010"),1.0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
		MoveTo(id,intX.intVal+100,intY.intVal+70,false);
		dm[id].Delays(100,150);
		LeftClick(id,false);
	}else{
		goto isExit;
	}
exitOK:
	dm[id].Delays(100,150);
	bmpDir.Format(_T("%s%s"),pathConfig,_T("loginUI.bmp"));
	dm[id].FindPic(230,290,323,335,bmpDir,_T("101010"),0.95,0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
	}else{
	    goto exitOK;
    }
	//ReleaseMutex(m_hMutex);
	oneRun = false;
	dm[id].UseDict(index_dict);
    return TRUE;
}

/************************************************************************/
/*�Զ���¼(�Զ��Ϻ�)
/************************************************************************/

BOOL AutoLogin(int id,CString password){
	if (isAutoHuanhao[id] == 0)
	{
		return QUIT;
	}
	VARIANT x,y,x2,y2;
	int index_dict = dm[id].GetNowDict();
	dm[id].UseDict(DICT_DEFAULT);
	bmpDir.Format(_T("%s%s"),pathConfig,_T("loginUI.bmp"));
	dm[id].FindPic(230,290,323,335,bmpDir,_T("101010"),0.95,0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		MoveTo(id,x.intVal+237,y.intVal+61);
		dm[id].Delays(50,100);
		LeftClick(id);
		KeyPressChar(id,password);//��������
		dm[id].Delays(50,100);
		MoveTo(id,x.intVal+106,y.intVal+102);
		dm[id].Delays(50,100);
		LeftClick(id);//���ȷ��
		//�����ֵ���֤��
	}
re_v:
	dm[id].Delays(100,230);
	dm[id].FindStr(0,0,800,600,_T("������ͼƬ�е��ַ�"),_T("ffffff-101010"),1.0,&x,&y);
	if (x.intVal>=0 && x.intVal>=0)
	{
		CString MeDir = dm[id].GetDir(4);
		MeDir.AppendFormat(_T("\\pic_v\\v_%d.jpg"),id);
		LPSTR filename = CStringToLpstr(MeDir);
		dm[id].CaptureJpg(x.intVal,y.intVal,x.intVal+267,y.intVal+98,MeDir,80);
		dm[id].Delays(200,300);
		PublicAnswer(id,filename,x.intVal+66,y.intVal+115);
		dm[id].DeleteFile(MeDir);
		//�ȴ�1������Ƿ��������
		dm[id].Delays(1000,1300);
		dm[id].FindStr(0,0,800,600,_T("��֤���������"),_T("ffffff-101010"),1.0,&x2,&y2);
		if (x2.intVal>=0 && x2.intVal>=0)
		{
			MoveTo(id,x2.intVal+160,y2.intVal+74);
			dm[id].Delays(200,300);
			LeftClick(id);
			dm[id].Delays(100,230);
			goto re_v;
		}
	}else{
		goto re_v;
	}
	//�����֤��
reJryx:
	TRACE("׼��������Ϸ\n");
	bmpDir.Format(_T("%s%s"),pathConfig,_T("jryx.bmp"));
	dm[id].FindPic(0,0,800,600,bmpDir,_T("101010"),0.95,0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		//ȷ�Ͻ�ɫ������ɹ�
		TRACE("���ؽ�ɫ\n");
		dm[id].Delays(3000,4000);
		dm[id].FindStr(0,0,800,600,_T("���������ɫ��"),_T("ffffff-101010"),1.0,&x2,&y2);
		if (x2.intVal>=0 && x2.intVal>=0)
		{
			TRACE("���¼��ؽ�ɫ\n");
			goto reJryx;
		}
		TRACE("���ؽ�ɫ�ɹ�\n");
		MoveTo(id,x.intVal+10,y.intVal+5);
		dm[id].Delays(200,300);
		LeftClick(id);
	}else{
		TRACE("�����ؽ�ɫ\n");
		dm[id].Delays(80,120);
		goto reJryx;
	}
isLoad:
	dm[id].FindStr(0,450,185,570,_T("���������"),_T("a8e0ff-101010"),1.0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		TRACE("������Ϸ��\n");
		ExitMsg(id,2);
		dm[id].Delays(500,800);
		MoveTo(id,x.intVal+102,y.intVal-22);
		dm[id].Delays(80,130);
		LeftClick(id);
	}else{
		TRACE("��û�н�����Ϸ\n");
		dm[id].Delays(300,500);
		goto isLoad;
	}
	AccountInfo[id].roleId = GetRoleId(id);
	//��������
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_WAIT_USERLIST_SETCELL_BY_USERNAME,(WPARAM)&AccountInfo[id],id);
	msgUserlist[id].nItem = id;
	msgUserlist[id].nSubItem = 2;
	msgUserlist[id].resultTitle = AccountInfo[id].roleId;
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_USERLIST_SETCELL_MSG,(WPARAM)&msgUserlist[id],0);
	//WaitForSingleObject(m_hMutex, INFINITE);
	while(oneRun){
		dm[id].Delay(50);
	}
	oneRun = true;
	m_dm.SetWindowState(hwnd[id],1);
	dm[id].Delays(200,300);
	m_dm.KeyDown(17);
	dm[id].Delays(200,300);
	dm[id].KeyPress(121);
	dm[id].Delays(200,300);
	dm[id].KeyPress(121);
	dm[id].Delays(200,300);
	dm[id].KeyPress(122);
	dm[id].Delays(200,300);
	dm[id].KeyPress(122);
	dm[id].Delays(200,300);
	m_dm.KeyUp(17);
	//ReleaseMutex(m_hMutex);
	oneRun = false;
	setRunState(id,_T("������"));
	dm[id].UseDict(index_dict);
	return TRUE;
}

/************************************************************************/
/*��ȡָ���̵߳�ǰ��ͼ����
/************************************************************************/
CString CurrentMap(int id){
	return _T('');
}

//1,or end����ɫ�ƶ������NPC
STATE MoveGhNpc(int id){
	VARIANT intX,intY;
	dm[id].UseDict(DEFAULT_DICT);
	if(Delay(id,60,120)==FALSE)return MAZE;
	//���ݻذ���̿�ʼ
	int dm_ret = dm[id].FindStr(645,0,800,34,_T("����|����|������|����"),_T("66ff00-101010|ff2d2d-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0 && dm_ret == 0){
		//���ں���
		if(HuiBang(id)==MAZE)return MAZE;
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 1)
	{
		//���ڽ���,�Ȼغ���
		if(AutoPathfindingToShangRen(id,_T("cqf"),_T("��ȫ��"))==MAZE)return MAZE;
tabMapBtn:
        dm[id].UseDict(DEFAULT_DICT);
		if(Delay(id,100,180)==FALSE)return MAZE;
		dm[id].UseDict(DICT_NPCMSG);
		dm[id].FindStr(220,80,560,530,_T("ǰȥ����"),_T("ffffff-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal+10,intY.intVal+5);
			if(Delay(id,60,120)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,120)==FALSE)return MAZE;
		}else{
			TRACE("����NPC�Ի���\n");

			goto tabMapBtn;
		}
		dm[id].UseDict(DICT_DEFAULT);
tabMap:
		if(Delay(id,60,120)==FALSE)return MAZE;;
		dm[id].FindStr(645,0,800,34,_T("����"),_T("66ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			ExitMsg(id,4);
			Verifiy(id);
			//goto nextMap;
		}else{
			goto tabMap;
		}
		if(HuiBang(id)==MAZE)return MAZE;
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 2){
		//���ڰ�����
		if(GoNextMap(id,_T("��³��"),_T("blw"),_T("�ϴ�ǰȥ����"),_T("����"))==MAZE)return MAZE;
		if(HuiBang(id)==MAZE)return MAZE;
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 3){
		//��������
		 if(GoNextMap(id,_T("�˹�"),_T("dk"),_T("���밢�����̶�"),_T("������"))==MAZE)return MAZE;
		 if(GoNextMap(id,_T("��³��"),_T("blw"),_T("�ϴ�ǰȥ����"),_T("����"))==MAZE)return MAZE;
		 if(HuiBang(id)==MAZE)return MAZE;
	}
	return TRUE;
}


STATE HuiBang(int id){
	TRACE("\n�ذ����\n");
	VARIANT intX,intY;
bhjbxx:
	if(Delay(id,60,120)==FALSE)return MAZE;
	MoveTo(id,785,481);
	if(Delay(id,60,120)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,600,800)==FALSE)return MAZE;
	dm[id].FindStr(0,0,400,285,_T("������Ϣ"),_T("c9d2fd-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		if(Delay(id,60,120)==FALSE)return MAZE;
		MoveTo(id,intX.intVal+35,intY.intVal+10);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,600,800)==FALSE)return MAZE;
		dm[id].FindStr(430,460,800,600,_T("�ذ�"),_T("fff7be-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			if(Delay(id,60,120)==FALSE)return MAZE;
			MoveTo(id,intX.intVal+25,intY.intVal+10);
			if(Delay(id,60,120)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,120)==FALSE)return MAZE;
			dm[id].KeyPress(27);
			if(Delay(id,60,120)==FALSE)return MAZE;
		}
		int i =0;
bhNpc://��ѯ��ᴫ��ʹ
		dm[id].FindStr(645,0,800,34,_T("276,136"),_T("ff2d2d-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			if(Delay(id,60,120)==FALSE)return MAZE;
			ExitMsg(id,3);
			if(Delay(id,400,600)==FALSE)return MAZE;;
			//��������ƶ�һ�����Ұ�ᴫ��ʹ
			AutoPathfindingToShangRen(id,_T("fyz"),_T("������"));
qjk:
				dm[id].FindStr(220,80,560,530,_T("ȥ���"),_T("ffffff-101010"),1.0,&intX,&intY);
				if(intX.intVal>=0 && intY.intVal>=0){
					//�ҵ���ᴫ��NPC
					MoveTo(id,intX.intVal,intY.intVal+8);
					if(Delay(id,60,120)==FALSE)return MAZE;
					LeftClick(id);
					if(Delay(id,60,120)==FALSE)return MAZE;
					dm[id].KeyPress(27);
					if(Delay(id,60,120)==FALSE)return MAZE;
				}else{
					if(Delay(id,60,120)==FALSE)return MAZE;
					goto qjk;
				}
		}else{
			if(Delay(id,60,120)==FALSE)return MAZE;
			goto bhNpc;
		}
	}else{
		goto bhjbxx;
	}
	return TRUE;
}

//2-1,������NPC����ȡ����
BOOL paoshangTakeTask(int id){
	VARIANT intX,intY;
	int i = 0;
	MoveTo(id,780,130);
	Delay(-1,70,110);
	LeftClick(id);
	Delay(-1,400,600);;
	//��NPC���ҿ��Ի���300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("����"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		Delay(-1,70,110);
		LeftClick(id);
		Delay(-1,70,110);
		KeyPressChar(id,_T("ley"));
		i = 0;
souso:
		Delay(id,400,600);
		dm[id].FindStr(220,80,560,530,_T("�����"),_T("00ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal,intY.intVal+8);
			Delay(-1,70,110);
			LeftClick(id);
			Delay(-1,70,110);
		}else{
			i++;
			//û���ҵ�����ָ��NPC��ֹͣ����
			if(i>=10){
				//MessageBox(0,_T("Ѱ������NPCʧ�ܣ����ֹͣ�˽�ɫ����"),_T("��Ϣ��ʾ��"),MB_OK);
				return FALSE;
			}else{
				goto souso;
			}
		}
	}
sr:
	Delay(-1,60,80);
	//�����Ϊ����
	dm[id].FindStr(190,215,600,540,_T("�����Ϊ����"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+60,intY.intVal+10);
		Delay(-1,70,110);
		LeftClick(id);
	    Delay(-1,70,110);
	}else{
		goto sr;
	}
jrw:
	Delay(-1,60,80);
	//����������
	dm[id].FindStr(190,215,600,540,_T("�̻�����֮����"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		Delay(-1,70,110);
		LeftClick(id);
		Delay(-1,1100,1600);
	}else{
		goto jrw;
	}
	dm[id].FindStr(0,0,800,600,_T("����"),_T("fff7be-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+10,intY.intVal+5);
		Delay(-1,70,110);
		LeftClick(id);
		Delay(-1,70,110);
		Verifiy(id);
	}
	return TRUE;
}


//3,��ȡ������ƶ������������̻�NPC��
STATE paoshangMove(int id){
	VARIANT intX,intY;
	long t1,t2;
reMove:
	ExitMsg(id,5);
	//���ȫ�����ں�����񴰿�
	MoveTo(id,785,307);
	if(Delay(id,60,120)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,60,120)==FALSE)return MAZE;
	t1 = dm[id].GetTime();
	dm[id].UseDict(DICT_NPCMSG);
rwhz:
	if(Delay(id,60,80)==FALSE)return MAZE;
	dm[id].FindStr(0,0,800,600,_T("����"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+90,intY.intVal+5);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		//10��ESC��λȡ��ȫ������
		t2 = dm[id].GetTime();
		if ((t2-t1)/1000>20)
		{
			ExitMsg(id,5);
		    //���ȫ�����ں�����񴰿�
		    MoveTo(id,785,307);
		    if(Delay(id,60,120)==FALSE)return MAZE;
		    LeftClick(id);
		    if(Delay(id,60,120)==FALSE)return MAZE;
		}
		goto rwhz;
	}
	dm[id].UseDict(DICT_DEFAULT);

	return TRUE;
}

STATE paoshangGMSP(int id){
	VARIANT intX,intY;
	int make = 2;
gmsp:
	int x,y;//�������
	if(Delay(id,120,140)==FALSE)return MAZE;
	dm[id].FindStr(0,0,800,600,_T("���̻���"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		x = intX.intVal;
		y = intY.intVal;
		for (int i=0;i<3;i++)
		{
			if(i == 0){
				make = 1;
			}else if(i == 1){
				make = 0;
			}else{
				make = 2;
			}
			//����make+1�����е���Ʒ
			MoveTo(id,(x-95)+19+38*make,y+30);
			if(Delay(id,60,110)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,110)==FALSE)return MAZE;
			MoveTo(id,x+78,y+265);
			if(Delay(id,60,110)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,110)==FALSE)return MAZE;
			MoveTo(id,x+154,y+265);
			if(Delay(id,60,110)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,110)==FALSE)return MAZE;
			//�ж��ж�NPC��û�г�������ǰ
			bmpDir.Format(_T("%s%s"),pathConfig,_T("psmp.bmp"));
			dm[id].FindPic(200,402,332,465,bmpDir,_T("101010"),0.95,0,&intX,&intY);
			if(intX.intVal>=0 && intY.intVal>=0){
				TRACE("��Ʊ�����Խ�������\n");
				return FALSE;
			}
			//�ж��ж�NPC��û�г�������ǰ
			bmpDir.Format(_T("%s%s"),pathConfig,_T("spbj.bmp"));
			dm[id].FindPic(20,402,332,465,bmpDir,_T("101010"),0.95,0,&intX,&intY);
			if(intX.intVal>=0 && intY.intVal>=0){
				TRACE("��Ʒ����ˣ�ˢ��һ��\n");
				if(Delay(id,60,120)==FALSE)return MAZE;
				MoveTo(id,x-70,y+265);
				if(Delay(id,60,120)==FALSE)return MAZE;
				LeftClick(id);
				if(Delay(id,60,120)==FALSE)return MAZE;
				i--;
			}
		}
		ExitMsg(id,4);
	}else{
		goto gmsp;
	}
	return TRUE;
}

//���ۻ���
STATE paoshangCSSP(int id){
	VARIANT intX,intY;
cssp:
	int x,y;//�������
	if(Delay(id,120,140)==FALSE)return MAZE;
	dm[id].FindStr(0,0,800,600,_T("��������"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		x = intX.intVal;
		y = intY.intVal;
		for (int i=0;i<3;i++)
		{
			//����make+1�����е���Ʒ
			MoveTo(id,(x-109)+19,y+30);
			if(Delay(id,60,110)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,110)==FALSE)return MAZE;
			MoveTo(id,x+43,y+265);
			if(Delay(id,60,110)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,110)==FALSE)return MAZE;
			MoveTo(id,x+121,y+265);
			if(Delay(id,60,110)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,110)==FALSE)return MAZE;
			//�ж��ж�NPC��û�г�������ǰ
			bmpDir.Format(_T("%s%s"),pathConfig,_T("spbj.bmp"));
			dm[id].FindPic(20,402,332,465,bmpDir,_T("101010"),0.95,0,&intX,&intY);
			if(intX.intVal>=0 && intY.intVal>=0){
				TRACE("��Ʒ����ˣ�ˢ��һ��\n");
				if(Delay(id,60,110)==FALSE)return MAZE;
				MoveTo(id,x-370,y+265);
				if(Delay(id,60,110)==FALSE)return MAZE;
				LeftClick(id);
				if(Delay(id,60,110)==FALSE)return MAZE;
				i--;
			}
		}
	}else{
		goto cssp;
	}
	return TRUE;
}

//ǰ����һ����ͼ�����͵㷽ʽ��
STATE GoNextMapBySpot(int id,LPTSTR npcName,CString keyStr,LPTSTR stopPos,lvXY ClickPos,LPTSTR nextMap){
	VARIANT intX,intY;
	long t1,t2;
	t1 = dm[id].GetTime();
tabMapBtn_0:
	if(Delay(id,60,120)==FALSE)return MAZE;
	if(AutoPathfindingToShangRen(id,keyStr,npcName)==MAZE)return MAZE;
	if(Delay(id,80,120)==FALSE)return MAZE;
	ExitMsg(id,1);
tabMapBtn:
	dm[id].UseDict(DICT_DEFAULT);
	if(Delay(id,100,200)==FALSE)return MAZE;
	dm[id].UseDict(DICT_COORDINATE);
	dm[id].FindStr(645,0,800,34,stopPos,_T("66ff00-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,ClickPos.x,ClickPos.y);
		if(Delay(id,60,120)==FALSE)return MAZE;
		dm[id].LeftDoubleClick();
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		t2=dm[id].GetTime();
		if((t2-t1)/1000>45){
			t1 = dm[id].GetTime();
			goto tabMapBtn_0;
		}
		TRACE("����NPC�Ի���");
		goto tabMapBtn;
	}
	dm[id].UseDict(0);
tabMap:
	if(Delay(id,300,400)==FALSE)return MAZE;
	dm[id].FindStr(645,0,800,34,nextMap,_T("66ff00-101010|ff2d2d-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		ExitMsg(id,6);
		Verifiy(id);
		//goto nextMap;
	}else{
		goto tabMap;
	}
	return TRUE;
}

//ǰ����һ����ͼ��NPC��ʽ��
STATE GoNextMap(int id,LPTSTR npcName,CString keyStr1,LPTSTR npcMsg,LPTSTR nextMap){
	VARIANT intX,intY;
	long t1,t2;
	t1 = dm[id].GetTime();
tabMapBtn_0:
	if(Delay(id,60,120)==FALSE)return MAZE;
    if(AutoPathfindingToShangRen(id,keyStr1,npcName)==MAZE)return MAZE;
tabMapBtn:
	dm[id].UseDict(DICT_DEFAULT);
	if(Delay(id,300,400)==FALSE)return MAZE;
	dm[id].UseDict(DICT_NPCMSG);
	dm[id].FindStr(220,80,560,530,npcMsg,_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+10,intY.intVal+5);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		t2=dm[id].GetTime();
		if((t2-t1)/1000>45){
			t1 = dm[id].GetTime();
			goto tabMapBtn_0;
		}
		TRACE("����NPC�Ի���");
		goto tabMapBtn;
	}
	dm[id].UseDict(0);
tabMap:
	if(Delay(id,300,400)==FALSE)return MAZE;
	dm[id].FindStr(645,0,800,34,nextMap,_T("66ff00-101010|ff2d2d-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		ExitMsg(id,6);
		Verifiy(id);
		//goto nextMap;
	}else{
		goto tabMap;
	}
	return TRUE;
}


//�Զ�Ѱ·������/����
STATE AutoPathfindingToShangRen(int id,CString keyStr,CString npcName){
	VARIANT intX,intY;
	long t1,t2;
	dm[id].UseDict(DICT_DEFAULT);
	ExitMsg(id,4);
	t1 = dm[id].GetTime();
	MoveTo(id,780,130);
	if(Delay(id,40,80)==FALSE)return MAZE;
	LeftClick(id);
reOpen:
	if(Delay(id,150,200)==FALSE)return MAZE;
	//��NPC���ҿ��Ի���300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("����"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,20,30)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,20,30)==FALSE)return MAZE;
		KeyPressChar(id,keyStr);
		if(Delay(id,400,600)==FALSE)return MAZE;;
		//���ҽ��ѡ��
		dm[id].FindStr(220,80,560,530,npcName,_T("00ff00-101010|ff9900-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			//�ҵ���ᴫ��NPC
			MoveTo(id,intX.intVal,intY.intVal+8);
			if(Delay(id,50,80)==FALSE)return MAZE;
			dm[id].LeftDoubleClick();
			if(Delay(id,50,80)==FALSE)return MAZE;
		}else{
			t2 = dm[id].GetTime();
			if ((t2-t1)/1000>5)
			{
				ExitMsg(id,2);
				MoveTo(id,780,130);
				if(Delay(id,60,120)==FALSE)return MAZE;
				LeftClick(id);
				if(Delay(id,60,120)==FALSE)return MAZE;
				t1 = dm[id].GetTime();
			}
			goto reOpen;
		}
		Verifiy(id);//ɨ����֤��
	}else{
		t2 = dm[id].GetTime();
		if ((t2-t1)/1000>5)
		{
			ExitMsg(id,2);
			MoveTo(id,780,130);
			if(Delay(id,60,120)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,120)==FALSE)return MAZE;
			t1 = dm[id].GetTime();
		}
		goto reOpen;
	}
	//ExitMsg(id,1);
	return TRUE;
}

//������̻���ﴰ��
STATE OpenSHSD(int id){
	VARIANT intX,intY;
	long start_time,end_time,ms;
	start_time = dm[id].GetTime();
dkshsd://���̻��̵�
	msgExit(id);
	if(Delay(id,60,120)==FALSE)return MAZE;
	dm[id].FindStr(190,215,600,540,_T("���̻��̵�"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+30,intY.intVal+5);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		end_time = dm[id].GetTime();
		ms = (end_time-start_time)/1000;
		if (ms>=45)
		{
			start_time = dm[id].GetTime();
			return FALSE;
		}
		goto dkshsd;
	}
	Verifiy(id);//ɨ����֤��
	if(Delay(id,60,120)==FALSE)return MAZE;
	return TRUE;
}

//������ɣ��ͻ�����
STATE paoshangGiveTask(int id){
	VARIANT intX,intY;
	int i = 0;
	int dm_ret;
reopen:
	TRACE("�ӻ�����\n");
	MoveTo(id,780,130);
	dm[id].Delay(100);
	dm[id].GetCursorPos(&intX,&intY);
	TRACE("���꣺%d,%d\n",intX.intVal,intY.intVal);
	dm[id].CaptureJpg(intX.intVal-15,intY.intVal-15,intX.intVal+15,intY.intVal+15,_T("C:\\pos.jpg"),80);
	if(Delay(id,80,150)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,400,500)==FALSE)return MAZE;
	//��NPC���ҿ��Ի���300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("����"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	dm[id].CaptureJpg(140,400,640,540,_T("C:\\tttt.jpg"),80);
	TRACE("���꣺%d��%d\n",intX.intVal,intY.intVal);
	if(intX.intVal>=0 && intY.intVal>=0){
		TRACE("�ҵ�\n");
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
		KeyPressChar(id,_T("ley"));
		i = 0;
souso:
		if(Delay(id,400,500)==FALSE)return MAZE;
		dm[id].FindStr(220,80,560,530,_T("�����"),_T("00ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal,intY.intVal+8);
			if(Delay(id,60,120)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,120)==FALSE)return MAZE;
		}else{
			i++;
			//û���ҵ�����ָ��NPC��ֹͣ����
			if(i>=10){
				MessageBox(0,_T("Ѱ�������ʧ�ܣ����ֹͣ�˽�ɫ����"),_T("��Ϣ��ʾ��"),MB_OK);
				return QUIT;
			}else{
				goto souso;
			}
		}
	}else{
		goto reopen;
	}
sr:
	msgExit(id);
	if(Delay(id,60,120)==FALSE)return MAZE;
	//�����Ϊ����
	dm[id].FindStr(190,215,600,540,_T("�����Ϊ����"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+60,intY.intVal+10);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		goto sr;
	}
	dm_ret = TakeTask(id);
	if (dm_ret == FALSE)
	{
		goto reopen;
	}else if(dm_ret == MAZE){
		return MAZE;
	}else if(dm_ret == QUIT){
		return QUIT;
	}
	return TRUE;
}

//��ȡ��������
STATE TakeTask(int id){
	VARIANT intX,intY;
	int dm_ret,i=0;
	if(Delay(id,200,400)==FALSE)return MAZE;
jrw:
	msgExit(id);
	if(Delay(id,60,80)==FALSE)return MAZE;
	//����������
	dm[id].FindStr(190,215,600,540,_T("�̻�����֮����"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,60,80)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,800,1300)==FALSE)return MAZE;
	}else{
		goto jrw;
	}
jjrw:
	if(Delay(id,60,80)==FALSE)return MAZE;
	dm_ret = dm[id].FindStr(0,0,800,600,_T("����|���"),_T("fff7be-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0 && dm_ret == 0){
		if(isReward[id] == TRUE){
			ExitMsg(id,5);
			dm_ret = TakeReward(id);
			if(dm_ret==MAZE)return MAZE;
			else if(dm_ret==QUIT)return QUIT;
		}else{
			MoveTo(id,intX.intVal+10,intY.intVal+5);
			if(Delay(id,60,80)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,80)==FALSE)return MAZE;
			Verifiy(id);
		}
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 1)
	{
		//���ý����Թ�״̬
		isMaze[id] = FALSE;
		MoveTo(id,intX.intVal+10,intY.intVal+5);
		if(Delay(id,60,80)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,80)==FALSE)return MAZE;
		ExitMsg(id,4);
		timeBuyYao(id);
		AccountInfo[id].sum++;
		CString djp;
		djp.Format(_T("�ս����ǵڣ�%dƱ"),AccountInfo[id].sum);
		Log(id,djp);
		::SendMessage(
			AfxGetApp()->m_pMainWnd->GetSafeHwnd()
			,WM_WAIT_USERLIST_SETCELL_BY_USERNAME
			,(WPARAM)&AccountInfo[id]
		    ,id);
		//�����������һ���ź󻻺ŵĹ���
		if (AccountInfo[id].sum>=ConfigData[id].sum)
		{
			CancelShangRen(id);
			Log(id,_T("��ʮƱ"));
			//AccountInfo[id].sum = 0;
			//WaitForSingleObject(m_hMutex, INFINITE);
			while(oneRun){
				dm[id].Delay(50);
			}
			oneRun = true;
			::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_WAIT_USERLIST_GET_NEW_USERNAME,(WPARAM)&AccountInfo[id],id);
			oneRun = false;
			//ReleaseMutex(m_hMutex);
			
			if(AutoExit(id,AccountInfo[id].username) == QUIT){
				setRunState(id,_T("������"));
				return QUIT;
			}
			if(isUserEnd==false){
				if(AutoLogin(id,AccountInfo[id].password) == QUIT){
					setRunState(id,_T("������"));
					return QUIT;
				}
			}else{
				setRunState(id,_T("�޺ſɻ�"));
				return QUIT;
			}
			return MAZE;
		}
		return FALSE;
	}else{
		if(Delay(id,60,80)==FALSE)return MAZE;
		goto jjrw;
	}
	
	return TRUE;
}

void CancelShangRen(int id){
	VARIANT intX,intY;
	long dm_ret,t1,t2;
	t1 = dm[id].GetTime();
reOpenPath:
	msgExit(id);
	AutoPathfindingToShangRen(id,_T("ley"),_T("�����"));
	//ȡ����λ  ����ʱ������������  �������ְλ��ȡ����(ȷ����100,70)������180��220|600,380��
re:
	msgExit(id);
	dm_ret = dm[id].FindStr(190,215,600,540,_T("����ʱ������������"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+30,intY.intVal+5);
		dm[id].Delays(50,80);
		LeftClick(id);
		dm[id].Delays(50,80);
	}else{
		t2 = dm[id].GetTime();
		if((t2-t1)/1000>15){
			ExitMsg(id,3);
			t1 = dm[id].GetTime();
			goto reOpenPath;
		}
		goto re;
	}
	dm[id].Delays(500,700);
	dm_ret = dm[id].FindStr(180,220,600,380,_T("�������ְλ��ȡ����"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+100,intY.intVal+70);
		dm[id].Delays(50,80);
		LeftClick(id);
		dm[id].Delays(50,80);
	}
	ExitMsg(id,4);
}

//��ȡ�ͽ�����
STATE TakeReward(int id){
	VARIANT intX,intY,x,y;
	int dm_ret,i=0;
	if(Delay(id,60,80)==FALSE)return MAZE;;
	MoveTo(id,780,130);
	if(Delay(id,60,80)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,400,600)==FALSE)return MAZE;;
	//��NPC���ҿ��Ի���300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("����"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,60,80)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,80)==FALSE)return MAZE;
		KeyPressChar(id,_T("cmy"));
		i = 0;
souso:
		if(Delay(id,300,400)==FALSE)return MAZE;
		dm[id].FindStr(220,80,560,530,_T("������"),_T("00ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal,intY.intVal+8);
			if(Delay(id,60,80)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,80)==FALSE)return MAZE;
			i=0;
		}else{
			i++;
			//û���ҵ�����ָ��NPC��ֹͣ����
			if(i>=10){
				MessageBox(0,_T("Ѱ���ͽ�NPCʧ�ܣ����ֹͣ�˽�ɫ����"),_T("��Ϣ��ʾ��"),MB_OK);
				return FALSE;
			}else{
				goto souso;
			}
		}
	}
npcmsg:
	msgExit(id);
	//�򿪽��ͽ����
	dm[id].FindStr(190,215,600,540,_T("�����ҵ�������"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+60,intY.intVal+10);
		if(Delay(id,60,80)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,80)==FALSE)return MAZE;
	}else{
		goto npcmsg;
	}
	dm[id].UseDict(DICT_DEFAULT);
	if(Delay(id,600,1300)==FALSE)return MAZE;
fbr://�ҷ�����
	dm[id].FindStr(210,60,650,800,_T("������"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		dm[id].UseDict(DICT_REWARD);
		dm[id].FindStr(intX.intVal+120,intY.intVal+30,intX.intVal+208,intY.intVal+48,_T("/"),_T("f6f6ce-101010"),1.0,&x,&y);
		if(x.intVal>=0 && y.intVal>=0){//���ͽ�����
			//dm[id].UseDict(DICT_DEFAULT);
			/*dm[id].Delays(100,130);
			MoveTo(id,intX.intVal,intY.intVal+5);
			dm[id].Delays(100,130);
			LeftClick(id);*/
			for (int j=0;j<10;j++)
			{
				
				int y_pos = j*18;
				//CString file;
				//file.Format(_T("%s_%d.jpg"),_T("test"),j);
				dm[id].CaptureJpg(intX.intVal+120,intY.intVal+30+y_pos,intX.intVal+208,intY.intVal+48+y_pos,_T("C:\\test.jpg"),80);

				CString str = dm[id].Ocr(intX.intVal+120,intY.intVal+30+y_pos,intX.intVal+208,intY.intVal+48+y_pos,_T("f6f6ce-101010"),1.0);//��ȡ�����ַ������п��ܻ���ֶ������","
				Log(id,str);
				lvXY rewa= getXY(str,'/');
				if(rewa.x<rewa.y){
					Log(id,_T("��ȡ�ͽ�"));
					dm[id].UseDict(DICT_DEFAULT);
					dm[id].Delays(200,300);
					MoveTo(id,intX.intVal+120,intY.intVal+30+y_pos+8);
					dm[id].Delays(200,300);
					LeftClick(id);
					goto rewa_take;
				}
			}
			return QUIT;
		}else{
			dm[id].Delays(500,600);
			dm[id].UseDict(DICT_DEFAULT);
			i++;
			if(i>=10){
				//֤��û���ͽ�����
				return QUIT;
			}
			goto fbr;
		}
	}else{
		goto fbr;
	}
rewa_take:
	if(Delay(id,200,300)==FALSE)return MAZE;
	dm[id].UseDict(DICT_DEFAULT);
	dm[id].FindStr(0,0,800,600,_T("��ȡ����"),_T("f6f6ce-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		dm[id].Delays(200,300);
		MoveTo(id,intX.intVal+35,intY.intVal+7);
		dm[id].Delays(200,300);
		LeftClick(id);
		dm[id].Delays(200,300);
	}
	Verifiy(id);
	return TRUE;
}

void MazeMove(int id,int pos,bool isFree);//��ָ�������ƶ���ɫ
void MazeMove(int id,int pos,bool isFree = false){
	long up=38,down=40,left=37,right=39;
	switch(pos)
	{
	case UP:
		if (isFree == false)
		{
			dm[id].KeyDown(up);
			dm[id].KeyDown(left);
		}else{
			dm[id].KeyUp(up);
			dm[id].KeyUp(left);
		}
		break;
	case LEFT:
		if (isFree == false)
		{
			dm[id].KeyDown(left);
			dm[id].KeyDown(down);
		}else{
			dm[id].KeyUp(left);
			dm[id].KeyUp(down);
		}
		break;
	case DOWN:
		if (isFree == false)
		{
			dm[id].KeyDown(down);
			dm[id].KeyDown(right);
		}else{
			dm[id].KeyUp(down);
			dm[id].KeyUp(right);
		}
		break;
	case RIGHT:
		if (isFree == false)
		{
			dm[id].KeyDown(right);
			dm[id].KeyDown(up);
		}else{
			dm[id].KeyUp(right);
			dm[id].KeyUp(up);
		}
		break;
	default:
		break;
	}
}


//���Թ���NEW��
BOOL Maze(int id){
	if(isMaze[id] == TRUE)return TRUE;
	VARIANT x,y,x1,y2;
	long t1,t2;
	//�ж��Ƿ����Թ���
	dm[id].FindStr(645,0,800,34,_T("�����Թ�"),_T("ff2d2d-101010"),1.0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		setRunState(id,_T("�����Թ�"));
		Verifiy(id);
		ExitMsg(id,4);
		t1 = dm[id].GetTime();
	}else{
		return TRUE;
	}
	
	if (is_windows[id]==true)
	{
		while(oneRun){
			dm[id].Delay(50);
		}
		oneRun = true;
	}

	int Position = UP;//��ǰҪ�ߵķ���
	int isOK = -1;//���һ����ȷ����
	int LastPos = -1;//�ϴ��ߵķ���
	lvXY currentXY;
	BOOL MoveResult;
Loop:
	t2 = dm[id].GetTime();
	if((t2-t1)/1000>=240){
		msgExit(id);
		setRunState(id,_T("��ʱС��"));
		MinExit(id);
		if (is_windows[id]==true)oneRun=false;
		return FALSE;
	}
	setRunState(id,_T("�ҳ���"));
	currentXY = MazePos(id);
	switch(Position)
	{
	case UP:
		MazeMove(id,UP);//��ʼ�ƶ�
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = UP;
			Position = isLastPos(UP,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(UP,isOK);
			if (Position == 100)
			{
				//�����˳���Ϸ�����Ϻ���
				MinExit(id);
				return FALSE;
			}
		}else if (MoveResult == MAZE)
		{
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,UP,true);//ֹͣ�ƶ�
		break;
	case DOWN:
		MazeMove(id,DOWN);//��ʼ�ƶ�
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = DOWN;
			Position = isLastPos(DOWN,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(DOWN,isOK);
			if (Position == 100)
			{
				//�����˳���Ϸ�����Ϻ���
				MinExit(id);
				return FALSE;
			}
		}else if(MoveResult == MAZE){
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,DOWN,true);//ֹͣ�ƶ�
		break;
	case LEFT:
		MazeMove(id,LEFT);//��ʼ�ƶ�
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = LEFT;
			Position = isLastPos(LEFT,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(LEFT,isOK);
			if (Position == 100)
			{
				//�����˳���Ϸ�����Ϻ���
				MinExit(id);
				return FALSE;
			}
		}else if(MoveResult == MAZE){
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,LEFT,true);//ֹͣ�ƶ�
		break;
	case RIGHT:
		MazeMove(id,RIGHT);//��ʼ�ƶ�
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = RIGHT;
			Position = isLastPos(RIGHT,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(RIGHT,isOK);
			if (Position == 100)
			{
				MinExit(id);
				return FALSE;
				//�����˳���Ϸ�����Ϻ���
			}
		}else if(MoveResult == MAZE){
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,RIGHT,true);//ֹͣ�ƶ�
		break;
	}
	goto Loop;
}


BOOL MazePosition(int id,lvXY inXY){
	VARIANT intX,intY,x,y;
	lvXY currentXY,lastXY,moveXY;
	bool lock = false;
	int i = 0;
	TRACE("��λ���꣺%d,%d\n",inXY.x,inXY.y);
getpos:
	msgExit(id);
	bmpDir.Format(_T("%s%s"),pathConfig,_T("mgnpc.bmp"));
	dm[id].FindPic(242,84,570,400,bmpDir,_T("101010"),1.0,0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MazeMove(id,UP,true);//ֹͣ�ƶ�
		MazeMove(id,DOWN,true);//ֹͣ�ƶ�
		dm[id].UseDict(DEFAULT_DICT);
		TRACE("�ҵ��Թ��뿪NPC");
		MoveTo(id,intX.intVal+30,intY.intVal+30,false);
		dm[id].Delays(300,500);
		LeftClick(id,false);
		dm[id].Delays(1500,2000);
		msgExit(id);
		dm[id].FindStr(0,0,800,600,_T("�뿪�Թ�"),_T("ffffff-101010"),1.0,&x,&y);
		if (x.intVal>=0 && y.intVal>=0)
		{
			setRunState(id,_T("�뿪�Թ�"));
			MoveTo(id,x.intVal+10,y.intVal+5,false);
			dm[id].Delays(50,80);
			LeftClick(id,false);
waitOutMaze:
			dm[id].Delays(50,80);
			dm[id].FindStr(645,0,800,34,_T("����|����|����|������|̨�ݺ���|��ɽ"),_T("66ff00-101010|ff2d2d-101010"),1.0,&intX,&intY);
			if (intX.intVal>=0 && intY.intVal>=0)
			{
				setRunState(id,_T("������"));
				isMaze[id] = TRUE;
				return MAZE;
			}else{
				goto waitOutMaze;
			}
		}else{
			ExitMsg(id,2);
			goto getpos;
		}
	}
	
	if(lock == false){//����ǰ���궨λ
		lastXY = inXY;
		lock = true;
		currentXY = inXY;
	}
	moveXY = currentXY;
	TRACE("�ƶ�ǰ���꣺%d,%d\n",moveXY.x,moveXY.y);
	currentXY = MazePos(id);//��ȡ��ǰ����
	TRACE("��ǰ���꣺%d,%d\n",currentXY.x,currentXY.y);
	if(compare(currentXY,moveXY) == STOP_MOVE){//û���ƶ���
		//��ǰ������ϴζ�λ�����жϣ��Ƿ���ȷ·��
		if(abs((currentXY.x-lastXY.x))>5 || abs((currentXY.y-lastXY.y))>5){
			TRACE("û���ƶ��ˣ������곬��5\n");
			//��ȷ·��
			i++;
			if(i>12){
				TRACE("������ȷ\n");
				return TRUE;
			}
		}else{
			//����·��
			TRACE("û���ƶ��ˣ�������û����5\n");
			i++;
			if(i>12){
				TRACE("�������\n");
				return FALSE;
			}
		}
	}else{
		i=0;
	}
	msgExit(id);
	dm[id].Delay(490);
	goto getpos;
}


//ʹ�õ�ǰ��������һ����ȷ�����ж��´�Ӧ�����ĸ�����
int isLastPos(int currentPos,int OkPos)
{
	int pos;//�����´��ߵķ���
	TRACE("�жϷ���ǰ��������%d\n",OkPos);
	switch(OkPos)
	{
	case UP:
		if(currentPos == UP){
			pos = LEFT;
		}else if (currentPos == LEFT)
		{
			pos = RIGHT;
		}else if(currentPos == RIGHT){
			pos = DOWN;
		}
		break;
	case DOWN:
		if(currentPos == DOWN){
			pos = RIGHT;
		}else if (currentPos == RIGHT)
		{
			pos = LEFT;
		}else if(currentPos == LEFT){
			pos = UP;
		}
		break;
	case LEFT:
		if(currentPos == LEFT){
			pos = DOWN;
		}else if (currentPos == DOWN)
		{
			pos = UP;
		}else if(currentPos == UP){
			pos = RIGHT;
		}
		break;
	case RIGHT:
		if(currentPos == RIGHT){
			pos = UP;
		}else if (currentPos == UP)
		{
			pos = DOWN;
		}else if(currentPos == DOWN){
			pos = LEFT;
		}
		break;
	default:
		if(currentPos == UP){
			pos = LEFT;
		}else if (currentPos == LEFT)
		{
			pos = DOWN;
		}else if(currentPos == DOWN){
			pos = RIGHT;
		}else{
			pos = 100;
			TRACE("�Ҳ������򣬿����˳���Ϸ�ˣ�");
		}
		break;
	}
	TRACE("�жϷ����׼���ƶ��ķ���%d\n",pos);
	return pos;
}

//��֤���鹦��
//�˹��ܳ��ֳ������£���
void Verifiy(int id){
	VARIANT intX,intY;
	long start_time,end_time;
	start_time = dm[id].GetTime();
	//��ʽһ�������룺1b93f82a����ʽ����6561614a
	int topX=320,topY=250,BottomX=590,bottomY=430;//ɨ����֤�������
	//�����֤���Ƿ���ֵļ���ʱ����1���̵�ʱ��2�ƶ�ʱ���ݽ��ȼ���������ʱ��
	int setpX=35,setpY=15,secondX,secondY,x,y;
	CString mouse_tz;
	long double tempX,tempY;
	tempX = (BottomX-topX)/setpX;
	tempY = (bottomY-topY)/setpY;
	secondX = ceil(tempX);
	secondY = ceil(tempY);
	dm[id].EnableRealMouse(0,5,35);
	//��ʼɨ��
	
	for (int i=0;i<=secondY;i++)
	{
		y = i*setpY+topY;
		for(int j=0;j<=secondX;j++){
			x = j*setpX+topX;
			MoveTo(id,x,y);
			dm[id].Delay(5);
			mouse_tz = dm[id].GetCursorShape();
			if(mouse_tz == _T("1b93f82a")){
				setRunState(id,_T("��֤��"));
				Log(id,_T("�ҵ���֤��"));
				TRACE("�ҵ���֤�룡\n");
				CString filename = dm[id].GetDir(4);
				filename.AppendFormat(_T("\\pic_v\\v_%d.jpg"),id);
				LPSTR lpFilename = CStringToLpstr(filename);
				TRACE(lpFilename);
				//��ʼִ�д��⶯��
				dm[id].GetCursorPos(&intX,&intY);
				//��ͼ����
				int dm_ret = dm[id].CaptureJpg(intX.intVal-147,intY.intVal-126,intX.intVal+100,intY.intVal,filename,80);
				TRACE("%d",dm_ret);
				if(dm_ret == 1){
					Log(id,_T("��ͼ�ɹ�"));
				}else{
					Log(id,_T("��ͼʧ��"));
				}
				dm[id].Delay(666);
				Log(id,_T("׼��������֤����"));
				PublicAnswer(id,lpFilename,intX.intVal+6,intY.intVal+3);
				Log(id,_T("��ɴ������"));
				dm[id].DeleteFile(filename);
				goto result;
			}
		}
	}
result:
	dm[id].EnableRealMouse(2,60,120);//��궯��ģ����ʵ����,���ƶ��켣,�Լ������ʱ���. 
	end_time = dm[id].GetTime();
	setRunState(id,_T("������"));
	TRACE("ɨ����֤����ʱ��%d ms\n",end_time-start_time);
}

void Haoi_Answer(int id,LPSTR filename,int x,int y){
	
	char Reply[512];//����𰸱���
	int i;
	char TID[512];//������Ŀ��ű���
	CString log;
rePost:
	SendFile("jqrfhnwfg|DB47D229725C456B","3004",filename,55,0,"",TID);
	if (IsRight(TID)){
		TRACE("��ȡ����1\n");
		Log(id,_T("�ð�������ɹ����ȴ���"));
		//Go on GetAnswer ����
	}else{
		
		log.Format(_T("����ʧ�ܣ�һ������·��ͣ�ʧ�ܷ���ֵ:%s"),TID);
		Log(id,log);
		dm[id].Delay(1000);
		goto rePost;
	}
	CString strTid(TID);
	TRACE("��ȡ��:%S\n",strTid);

	for(i=1;i<65;i++){	

		Sleep(1000);//�ӳ�1�����ϣ������ӳ�Ŷ
		TRACE("��ȡ����3\n");
		GetAnswer(TID,Reply);//������ˮ��� ��ȡ��
		CString strAns(Reply);
		log = _T("");
		log.Format(_T("��ȡ�𰸣��ȴ��𰸣�ֵ��%s"),strAns);
		Log(id,log);
		TRACE("��ȡ��:%S\n",strAns);
		if (!strAns.IsEmpty())
		{
			goto isok;
		}
		
	}
	isok:
	if (IsRight(Reply)){
		TRACE("�д���");
		if(dm[id].GetWindowState(hwnd[id],1)==0){
			dm[id].SetWindowState(hwnd[id],1);
		}
		dm[id].Delays(100,150);
		MoveTo(id,x,y);
		dm[id].Delays(100,150);
		LeftClick(id);
		dm[id].Delays(100,150);
		CString answer(Reply);
		log = _T("");
		log.Format(_T("�õ��𰸣��𰸣�%s"),answer);
		Log(id,log);
		KeyPressChar(id,answer);
		TRACE("�𰸣�%S\n",answer);
		dm[id].Delays(100,180);
		dm[id].KeyPress(13);
	}else{
		CString answer(Reply);
		log = _T("");
		log.Format(_T("ȡ�𰸴��󣬷��أ�%s"),answer);
		Log(id,log);;
		TRACE("������ȷ��");
		goto rePost;
	}

}

void YunSu_Answer(int id,LPSTR filename,int x,int y){
	CYunSu_Interface Yunsu;
	Yunsu.YunSu_SoftInit(_T("66429"),_T("c052c8191a9449a3b5f9cdafc24a0375"),_T("3040"));
	Yunsu.YunSu_UserInit(_T("yiqiren"),_T("yiqirenadmin1234"));
	string result = "";
	CString strfile(filename);
	CString log;
	BOOL ret = Yunsu.YunSu_UploadPicByLocal(strfile,result);
	CString ans = CString(result.c_str());
	//MessageBox(0,ans,_T("��"),MB_OK);
	//return;
	
	int start = ans.Find(_T("<Result>"));
	int end = ans.Find(_T("</Result>"));
	
	if (ret == TRUE)
	{
		log = _T("");
		log.Format(_T("���٣����ݴ��󣬷��أ�%s"),ans);
		Log(id,log);
		return;
	}
	log = _T("");
	log.Format(_T("���٣�ȡ�����ݣ����أ�%s"),ans);
	Log(id,log);

	TRACE("�д���");
	if(dm[id].GetWindowState(hwnd[id],1)==0){
		dm[id].SetWindowState(hwnd[id],1);
	}
	dm[id].Delays(100,150);
	MoveTo(id,x,y);
	dm[id].Delays(100,150);
	LeftClick(id);
	dm[id].Delays(100,150);
	KeyPressChar(id,ans.Mid(start+8,end-9));
	TRACE("�𰸣�%S\n",ans.Mid(start+8,end-9));
	dm[id].Delays(100,180);
	dm[id].KeyPress(13);
}

lvXY MazePos(int id){
	//�л��ֿ�
	//Sleep(3000);
	//VARIANT intX,intY;
	long dict_index = dm[id].GetNowDict();
	dm[id].UseDict(DICT_COORDINATE);
	CString str = dm[id].Ocr(650,0,785,26,_T("ff2d2d-101010"),1.0);//��ȡ�����ַ������п��ܻ���ֶ������","
	//dm[id].CaptureJpg(650,0,785,26,_T("c:\\pos.jpg"),80);
	//TRACE("��ǰʹ���ֿ�%d\n",dm[id].GetNowDict());
	dm[id].UseDict(dict_index);
	//CString str = _T(",,,483,686,,");
	return getXY(str);
}



void ExitMsg(int id,int s = 10){
	for (int i= 0;i<s;i++)
	{
		Delay(-1,50,120);
		dm[id].KeyPress(27);
	}
}

/************************************************************************/
/*��ȡָ���̵߳ĵ�ǰ��ɫID
/************************************************************************/
CString GetRoleId(int id){
	CStringArray road;
	CString str = dm[id].GetWindowTitle(hwnd[id]);
	CStringToArray(str,'[',road);
	CString val = road.GetAt(0);
	int len = val.GetLength();
	return val.Mid(0,len-1);
}

long GetWindowHwnd(int id){
	long h = dm[id].FindWindowEx(0,_T(""),_T("��ٻŮ�Ļ�Online"));
	for (int i=0;i<=opens;i++)
	{
		if (hwnd[i] == h && i!=id && hwnd[i]!=0)
		{
			CloseHandle(Handle[id]);
			isStartThread[id] = FALSE;
			MessageBox(0,_T("������Ϣ�Ѵ��ڣ�"),_T("��Ϣ��ʾ��"),MB_OK);
			return 0;
		}
	}
	return h;
}

//��ָ����Ϸ����
long BindGameWindow(int id){
	int dm_ret;
	dm[id].SetWindowState(hwnd[id],1);
	dm[id].Delay(150);//ClientToScreen
	is_windows[id] = false;
	dm_ret = dm[id].BindWindowEx(
		hwnd[id]
	    ,_T("dx2")
		,_T("dx.mouse.position.lock.api|dx.mouse.position.lock.message|dx.mouse.focus.input.api|dx.mouse.focus.input.message|dx.mouse.input.lock.api|dx.mouse.state.api|dx.mouse.state.message|dx.mouse.cursor")
		,_T("dx.keypad.state.api|dx.keypad.api|dx.keypad.input.lock.api|dx.keypad.raw.input")
		,_T("dx.public.active.api|dx.public.active.api|dx.public.active.message"),101);
	if(dm_ret == 0){
		dm_ret = dm[id].BindWindow(hwnd[id],_T("dx2"),_T("normal"),_T("normal"),101);
		is_windows[id] = true;
	}
	//int dm_ret = dm[id].BindWindowEx(hwnd[id],_T("dx3"),_T("windows3"),_T("windows"),_T(""),101);
	dm[id].Delay(2500);
	return dm_ret;
}


void KeyPressChar(int id,CString str){
	int len = str.GetLength();
	int keycode;
	for (int i=0;i<len;i++)
	{
		dm[id].Delays(80,130);
		if(str.Mid(i,1) == '*'){
			dm[id].KeyPress(106);
			continue;
		}
		dm[id].KeyPressChar(str.Mid(i,1));
		//MessageBox(0,str.Mid(i,1),_T("ff"),MB_OK);
	}
}


//���´��滻�б���ʵ����
void SetWaitUserAsOne(CString list_filename,CString username,CString password,int sum){
	CTime time;
	time = CTime::GetCurrentTime();//��������
	CString section = time.Format("%Y-%m-%d");
	CString sum_s;
	sum_s.Format(_T("%d"),sum);
	m_dm.WriteIni(section,username,password+"|"+sum_s,list_filename);
}

//���´��滻�б��������
void UpdateWaitUserDataBySum(int id,lvAccount la){
	CString result;
	msgWaitUserlist[id].nItem = la.nItem;
	msgWaitUserlist[id].nSubItem = 2;
	result.Format(_T("%d"),la.sum);
	msgWaitUserlist[id].resultTitle = result;
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_WAIT_USERLIST_SETCELL_MSG,(WPARAM)&msgWaitUserlist[id],0);
}


void Log(int id,CString str){
	CString log;
	CTime time;
	time = CTime::GetCurrentTime();//��������
	CString section = time.Format("%Y-%m-%d %H:%M:%S");
	log.Format(_T("%s   %s\r\n"),section,str);
	CString filename;
	filename.Format(_T("c:\\feidu_log_%d.txt"),id);
	m_dm.WriteFile(filename,log);
}

//�ж��Ƿ���Ա༭�����б��ʺ���Ϣ
BOOL isEditWaitUserList(){
	for (int i =0;i<=opens;i++)
	{
		if(isStartThread[i] == TRUE){
			MessageBox(0,_T("��������ʱ���ܶԴ��滻�ʺ��б���б༭����ز�����"),_T("��ʾ��Ϣ��"),MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}
//��ʼ��ȫ���ʺ�����
void initAccount(){
	for (int i=0;i<=opens;i++)
	{
		AccountInfo[i].nItem = 0;
		AccountInfo[i].username = _T("");
		AccountInfo[i].password = _T("");
		AccountInfo[i].roleId = _T("");
		/*if(i==0){
			AccountInfo[i].sum = 9;
		}else if(i==1){
			AccountInfo[i].sum = 3;
		}*/
		AccountInfo[i].sum = 0;
		
	}
}


int timeBuyYao(int id){
	if (isBuyYao[id] == 0)
	{
		return 0;
	}
	gt2[id] = dm[id].GetTime();
	if((gt2[id]-gt1[id])/1000/60<60){
		return 0;
	}
	VARIANT x,y;
	int tempX,tempY;
	int index = dm[id].GetNowDict();
	dm[id].UseDict(YAO_DICT);
	int r = 1;
	long t1,t2;
	CStringArray lists;
reOpenMsg:
	AutoPathfindingToShangRen(id,_T("ltr"),_T("������"));
qjk:
	dm[id].FindStr(220,80,560,530,_T("���ҩ��"),_T("ffffff-101010"),1.0,&x,&y);
	if(x.intVal>=0 && y.intVal>=0){
		//�ҵ���ᴫ��NPC
		MoveTo(id,x.intVal,y.intVal+8);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
		dm[id].KeyPress(27);
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		if(Delay(id,60,120)==FALSE)return MAZE;
		goto qjk;
	}
	t1 = dm[id].GetTime();
	CStringToArray(yaoNames,'|',lists);
	for (int i=0;i<lists.GetCount();i++)
	{
reisOpenMsg:
		int ret = dm[id].FindStr(0,0,800,600,_T("��ϸ��|�����"),_T("fff7be-080808"),1.0,&x,&y);
		if (x.intVal>=0 && y.intVal>=0 && ret == 0)
		{
			dm[id].MoveTo(0,0);
			dm[id].Delay(200);
			TRACE(L"��ҩ�괰��\n");
		}else if (x.intVal>=0 && y.intVal>=0 && ret == 1)
		{
			movePosClick(id,x.intVal,y.intVal);
			goto reisOpenMsg;
		}else{
			t2 = dm[id].GetTime();
			if((t2-t1)/1000>=5){
				ExitMsg(4);
				t1 = dm[id].GetTime();
				goto reOpenMsg;
			}
			dm[id].Delay(100);
			goto reisOpenMsg;
		}
		TRACE(_T("����ҩƷ!\n"));
		TRACE(L"ҩƷ����");
		TRACE(lists.GetAt(i));
		TRACE(L"\n");
		dm[id].FindPic(40,100,425,425,lists.GetAt(i),_T("080808"),0.9,0,&x,&y);
		if (x.intVal>=0 && y.intVal>=0)
		{
			TRACE(_T("�ҵ�ҩƷ!\n"));
			tempX = x.intVal;
			tempY = y.intVal;
			movePosClick(id,tempX,tempY);
reSetSum:
			dm[id].Delay(300);
			//��λ��������
			dm[id].FindStr(0,0,800,600,_T("��������"),_T("a8e0ff-080808"),1.0,&x,&y);
			if (x.intVal>=0 && y.intVal>=0)
			{
				dm[id].KeyPressStr(_T("30"),80);
				movePosClick(id,x.intVal+40,y.intVal+75);
				//exitMsg(1);
			}else{
				t2 = dm[id].GetTime();
				if((t2-t1)/1000>=10){
					movePosClick(id,tempX,tempY);
				}
				goto reSetSum;
			}
			dm[id].Delay(500);
		}else{

		}
	}
	gt1[id] = dm[id].GetTime();
	TRACE("��ҩ����\n");
	dm[id].UseDict(index);
	return r;
}

void movePosClick(int id,long x,long y){
	dm[id].Delay(50);
	dm[id].MoveTo(x,y);
	dm[id].Delay(150);
	dm[id].LeftClick();
	dm[id].Delay(100);
	dm[id].MoveTo(0,0);
}


// CfeidusoftDlg �Ի���

CfeidusoftDlg::CfeidusoftDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfeidusoftDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfeidusoftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_UserList);
	DDX_Control(pDX, IDC_LIST2, m_userlist);
	DDX_Control(pDX, IDC_WAIT_USERLIST, m_WaitUserlit);
	DDX_Control(pDX, IDC_BTN_HIDE_SHOW, m_hideWaitUserlist);
	DDX_Control(pDX, IDC_COMBO_PS_TYPE, m_psType);
	DDX_Control(pDX, IDC_COMBO_PS_LINE, m_psLine);
	DDX_Control(pDX, IDC_EDIT_GAME_PATH, m_GamePathValue);
	DDX_Control(pDX, IDC_COMBO_ANS, m_ans);
	DDX_Control(pDX, IDC_EDIT1, m_MachineCode);
	DDX_Control(pDX, IDC_COMBO1, m_paoshangSum);
	DDX_Control(pDX, IDC_CHECK_AUTOHUANGHAO, m_autohuanhao);
	DDX_Control(pDX, IDC_CHECK2, m_buyYao);
}

// CfeidusoftDlg ��Ϣ�������

BOOL CfeidusoftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����״̬��
	m_statusbar.Create(this);
	//�����������//��һ�������1,�ڶ��������2
	UINT nID[] = { 1 };
	m_statusbar.SetIndicators(nID, 1);

	//�������1��Ϣ ,��һ�����,���Ϊ1,������ʾ,���Ϊ100
	
	//�������2��Ϣ,�ڶ������,���Ϊ2,�Զ�������ʾ,�տ�ʼ���Ϊ0

	//����״̬��λ��
	CRect rect;
	GetClientRect(&rect);
	m_statusbar.SetPaneInfo(0, 1, SBPS_NORMAL, rect.Width()-5);
	//rect.bottom - 20��״̬���ĸ߶�λ��,20��״̬�ĸ߶�
	//m_statusbar.MoveWindow(0,rect.bottom - 21,rect.Width()-20, rect.right,20);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);  
	m_statusbar.SetPaneText(0,_T("������ʾ����ϷĿ¼���ô��󽫵��µ��߻��л��ʺ�ʱ�޷���¼��Ϸ��"));

	//���������Ƶ���
	HANDLE hObject = ::CreateMutex(NULL,FALSE, _T("feidusoft_xlyh_paoshangpqiwenpisLKSJDFPQWNPIWERNAPSIFNQPWR2")); 
	if(GetLastError() == ERROR_ALREADY_EXISTS)    
	{    
		CDialogEx::OnOK();
		return FALSE;    
	}  
	//��ʼ��ִ���е�listCtrl
	LONG lStyle; 
	lStyle = GetWindowLong(m_userlist.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	lStyle |= LVS_REPORT; // ����style 
	SetWindowLong(m_userlist.m_hWnd, GWL_STYLE, lStyle);// ����style 
	DWORD dwStyle = m_userlist.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_userlist.SetExtendedStyle(dwStyle); // ������չ���
	m_userlist.DeleteAllItems();
	m_userlist.InsertColumn(0,_T("id"),0,25);
	m_userlist.InsertColumn(1,_T("���"),0,65);
	m_userlist.InsertColumn(2,_T("��ɫID"),0,75);
	m_userlist.InsertColumn(3,_T("��ͼ"),0,100);
	m_userlist.InsertColumn(4,_T("״̬"),0,80);

	//��ʼ���ʺųص�listCtrl
	lStyle = GetWindowLong(m_WaitUserlit.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ
	lStyle |= LVS_REPORT; // ����style
	SetWindowLong(m_WaitUserlit.m_hWnd, GWL_STYLE, lStyle);// ����style 
	dwStyle = m_WaitUserlit.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report����listctrl �� 
	//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_WaitUserlit.SetExtendedStyle(dwStyle); // ������չ���
	m_WaitUserlit.DeleteAllItems();
	m_WaitUserlit.InsertColumn(0,_T("�ʺ�"),0,140);
	m_WaitUserlit.InsertColumn(1,_T("����"),0,100);
	m_WaitUserlit.InsertColumn(2,_T("����"),0,40);
	m_WaitUserlit.InsertColumn(3,_T("��ɫID"),0,75);
	m_WaitUserlit.InsertColumn(4,_T("״̬"),0,50);

	pathConfig = CONFIG_PATH;
	configName = _T("config.ini");
	m_MachineCode.SetWindowText(m_dm.GetMachineCode());
	int reg_ret = m_dm.Reg(_T("jqrfhnwfgd1e888a06309a659b85feefa4bc96528"),_T(""));
	if(reg_ret != 1){
		//MessageBox(_T("Ч��ʧ�ܣ�"));
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		memset(&si,0,sizeof(si));
		si.cb=sizeof(si);
		si.wShowWindow=SW_SHOW;
		si.dwFlags=STARTF_USESHOWWINDOW;
		BOOL fRet=CreateProcess(getFilePos(_T("start.exe")),NULL,NULL,FALSE,NULL,NULL,NULL,NULL,&si,&pi);
		DWORD ErrorCode =  GetLastError();
		if(GetLastError() != 0){
			HLOCAL LocalAddress=NULL;    
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,  
				NULL,ErrorCode,0,(PTSTR)&LocalAddress,0,NULL);    
			MessageBox((LPCTSTR)LocalAddress,_T("��֤ʧ����Ϣ��ʾ��")); 
		}
		CDialogEx::OnOK();
		return FALSE;
	}
	CString post,code,beizhu;
	getConfig(_T("INFO"),_T("code"),_T("Mcode.ini"),code);
	getConfig(_T("INFO"),_T("beizhu"),_T("Mcode.ini"),beizhu);
	post.Format(_T("code=%s&beizhu=%s"),code,beizhu);
	int v_ret = HttpPost("http://v.llyi.cn/Communication/v/softid/5",post);
	if(v_ret != 1){
		CString httpMsg;
		httpMsg.Format(_T("�������ͨ��ʧ�ܣ�����:%d"),v_ret);
		MessageBox(httpMsg,_T("У����ʾ��"));
		CDialogEx::OnOK();
		return FALSE;
	}
	m_dm.SetPath(pathConfig);
	getConfig(_T("INFO"),_T("gamePath"),configName,game_path,_T(""));
	m_GamePathValue.SetWindowText(game_path);
	//��ʼ��������������ѡ���
	m_psType.AddString(_T("ѡ����������"));
	m_psType.AddString(_T("�ͽ�����"));
	m_psType.AddString(_T("��������"));
	CString temp;
	getConfig(_T("PaoShang"),_T("type"),_T("config.ini"),temp);
	m_psType.SetCurSel(CStringToInt(temp));
	TRACE("����ѡ��%s\n",temp);
	//��ʼ������·���趨
	m_psLine.AddString(_T("ѡ������·��"));
	m_psLine.AddString(_T("���ߣ�����-���꣩"));
	m_psLine.AddString(_T("���ߣ�����-������-���أ�"));
	getConfig(_T("PaoShang"),_T("line"),_T("config.ini"),temp);
	m_psLine.SetCurSel(CStringToInt(temp));
	TRACE("��·ѡ��%s\n",temp);

	//��ʼ������·���趨
	m_ans.AddString(_T("ѡ�������"));
	m_ans.AddString(_T("�ð�����"));
	m_ans.AddString(_T("���ٴ���"));
	getConfig(_T("DM"),_T("line"),_T("config.ini"),temp);
	m_ans.SetCurSel(CStringToInt(temp));
	TRACE("��·ѡ��%s\n",temp);

	//��ʼ������·���趨
	m_paoshangSum.AddString(_T("10Ʊ,���껻��"));
	m_paoshangSum.AddString(_T("11Ʊ,���껻��"));
	m_paoshangSum.AddString(_T("12Ʊ,���껻��"));
	m_paoshangSum.AddString(_T("13Ʊ,���껻��"));
	m_paoshangSum.AddString(_T("14Ʊ,���껻��"));
	m_paoshangSum.AddString(_T("15Ʊ,���껻��"));
	m_paoshangSum.AddString(_T("20Ʊ,���껻��"));
	getConfig(_T("PaoShangSum"),_T("PAOSHUANG"),_T("config.ini"),temp);
	m_paoshangSum.SetCurSel(CStringToInt(temp));
	TRACE("��·ѡ��%s\n",temp);
	getConfig(_T("PaoShang"),_T("buyYao"),_T("config.ini"),temp);
	m_buyYao.SetCheck(CStringToInt(temp));

	getConfig(_T("PaoShang"),_T("autoHuanhao"),_T("config.ini"),temp);
	m_autohuanhao.SetCheck(CStringToInt(temp));
	//m_psLine.EnableWindow(FALSE);
	TRACE(m_dm.Ver());
	m_dm.SetShowErrorMsg(0);
	m_dm.SetPath(pathConfig);
	TRACE("%d\n",m_dm.SetPath(pathConfig));
	m_dm.CreateFolder(pathConfig);

	CString MeDir = m_dm.GetDir(4);
	MeDir.Append(_T("\\pic_v"));
	m_dm.CreateFolder(MeDir);
	TRACE(MeDir);

	if(m_dm.IsFileExist(_T("dm_soft.txt"))==0){
		MessageBox(_T("��ʼ����ʧ�� -1"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("price.txt"))==0){
		MessageBox(_T("��ʼ����ʧ�� -2"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("rw_d.txt"))==0){
		MessageBox(_T("��ʼ����ʧ�� -3"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("coordinate.txt"))==0){
		MessageBox(_T("��ʼ����ʧ�� -4"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("reward.txt"))==0){
		MessageBox(_T("��ʼ����ʧ�� -5"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("default.txt"))==0){
		MessageBox(_T("��ʼ����ʧ�� -5"));
		goto exitsoft;
	}
	LPTSTR xuhao = new TCHAR[2];
	for (int i=0;i<=opens;i++)
	{
		dm[i].SetShowErrorMsg(0);
		int dm_ret;
		//int dm_ret = dm[i].SetPicPwd(_T("YIbBQOyw$*"));
		//if(dm_ret == 0){
		//	MessageBox(_T("��ʼ����ʧ�� -12"));
		//	goto exitsoft;
		//}
		dm[i].SetPath(CONFIG_PATH);
		TRACE("����Ŀ¼��%d\n",dm[i].SetPath(CONFIG_PATH));
		UpdateConfigData(i);//����������Ϣ���ڴ�
		isStartThread[i] = FALSE;//��ʼ����δ����״̬
		isMaze[i] = FALSE;
		wsprintf(xuhao, _T("%d"), i+1);
		m_userlist.SetItemText(m_userlist.InsertItem(i,xuhao),4,_T("δ����"));
		dm_ret = dm[i].SetDict(DEFAULT_DICT,_T("dm_soft.txt"));
		if(dm_ret == 0){
			MessageBox(_T("��ʼ����ʧ�� -6"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(NPCMSG_DICT,_T("rw_d.txt"));
		if(dm_ret == 0){
			MessageBox(_T("��ʼ����ʧ�� -7"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(PRICE_DICT,+_T("price.txt"));
		if(dm_ret == 0){
			MessageBox(_T("��ʼ����ʧ�� -8"));
			goto exitsoft;
		}
		//coordinate
		dm_ret = dm[i].SetDict(COORDINATE_DICT,_T("coordinate.txt"));
		if(dm_ret == 0){
			MessageBox(_T("��ʼ����ʧ�� -9"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(REWARD_DICT,_T("reward.txt"));
		if(dm_ret == 0){
			MessageBox(_T("��ʼ����ʧ�� -10"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(YAO_DICT,_T("default.txt"));
		if(dm_ret == 0){
			MessageBox(_T("��ʼ����ʧ�� -10"));
			goto exitsoft;
		}
		dm[i].EnableRealMouse(2,60,120);//��궯��ģ����ʵ����,���ƶ��켣,�Լ������ʱ���. 
		dm[i].EnableRealKeypad(1);//���̶���ģ����ʵ����,�����ʱ���.
		
	}
	m_dm.DisablePowerSave();
	m_dm.DisableScreenSave();
	m_GamePathValue.GetWindowText(game_path);
	//���ش��滻�ʺ��б�
	GetWaitUserLists(_T("usersource.ini"),_T("userlist.ini"));
	initAccount();//��ʼ���ʺ��������ݵ�ֵ
	
	//ע���ȼ�
	RegisterHotKey(m_hWnd,WM_HOTKEY_START_END,0,VK_HOME);  //HOME
	RegisterHotKey(m_hWnd,WM_HOTKEY_START_END,0,VK_END);  //END

	m_hideWaitUserlist.SetWindowTextW(L"<<�����б�");
	isHide = false;
	cwMin = 383;
	ch = 0;
	cw = 0;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
exitsoft:
	CDialogEx::OnOK();
	return FALSE;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CfeidusoftDlg::OnPaint()
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
HCURSOR CfeidusoftDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CfeidusoftDlg::SetUserListMsgAsOneRow( WPARAM wp,LPARAM lp )  
{  
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_LIST2);
	lvUserList *lvUL = (lvUserList*)wp;
	prog->SetItemText(lvUL->nItem,1,lvUL->hwnd);
	prog->SetItemText(lvUL->nItem,2,lvUL->roleId);
	prog->SetItemText(lvUL->nItem,3,lvUL->currentMap);
	prog->SetItemText(lvUL->nItem,4,lvUL->state);
	return 1L;  
}  

LRESULT CfeidusoftDlg::SetUserListMsgAsCell( WPARAM wp,LPARAM lp ){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_LIST2);
	lvUserList *lvUL = (lvUserList*)wp;
	prog->SetItemText(lvUL->nItem,lvUL->nSubItem,lvUL->resultTitle);
	return 0;
}

LRESULT CfeidusoftDlg::GetUserListMsgAsCell( WPARAM wp,LPARAM lp ){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_LIST2);
	lvUserList *lvUL = (lvUserList*)wp;
	lvUL->resultTitle = prog->GetItemText(lvUL->nItem,lvUL->nSubItem);
	return 0;
}

LRESULT CfeidusoftDlg::SetWaitUserListMsgAsOneRow( WPARAM wp,LPARAM lp ){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_WAIT_USERLIST);
	lvWaitUserList *lvWUL = (lvWaitUserList*)wp;
	int t_nItem = prog->InsertItem(0,lvWUL->username);
	prog->SetItemText(t_nItem,1,lvWUL->password);
	LPTSTR sum = new TCHAR[2];
	wsprintf(sum, L"%d", lvWUL->sum);
	prog->SetItemText(t_nItem,2,sum);
	return 0;
}

LRESULT CfeidusoftDlg::SetWaitUserListMsgAsCell( WPARAM wp,LPARAM lp ){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_WAIT_USERLIST);
	lvWaitUserList *lvWUL = (lvWaitUserList*)wp;
	prog->SetItemText(lvWUL->nItem,lvWUL->nSubItem,lvWUL->resultTitle);
	return 0;
}

LRESULT CfeidusoftDlg::GetWaitUserListMsgAsCell( WPARAM wp,LPARAM lp ){
	return 0;
}



void CfeidusoftDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*C7zip zip;
	zip.Unzip(_T("f:\\Desktop.y"),_T("f:\\feidu\\baidhu"),"123456789");
	return;
	Verifiy(0);
	return;*/
	//dm[0].Capture(140,400,640,540,_T("c:\\screen.bmp"));
	//int dm_ret = dm[0].CaptureJpg(0,0,100,50,_T("pic_0.jpg"),80);
	//TRACE(CStr);
	//PublicAnswer(0,str,0,0);
	CString msg;
	if(ConfigData[0].PaoshangLine==1){
		msg.Append(_T("������·�����ߣ����������꣩"));
	}
	if(ConfigData[0].paoshangType == 1){
		msg.Append(_T("���������ͣ��ͽ�����"));
	}else if(ConfigData[0].paoshangType == 2){
		msg.Append(_T("���������ͣ���������"));
	}
	MessageBox(msg);
	return;
	//dm[4].MoveTo(50,50);
	//dm[4].RightClick();
	/*int count = m_userlist.GetItemCount();
	LPTSTR str_count = new TCHAR[5];
	wsprintf(str_count, L"%d", count);
	MessageBox(str_count);*/
	//paoshangThread(0);
	/*lvXY xy1,xy2;
	xy1.x = 61;xy1.y = 51;
	xy2.x = 61;xy2.y = 51;
	compare(xy1,xy2);*/
	//MazeMove(0);
	//LPTSTR str = new TCHAR[5];
	//wsprintf(str,_T("%d"),irand1);
	//TRACE("���ֵ��%d\n",irand1);
	//Delay(-1,800,1600);
	MessageBox(m_dm.GetMachineCodeNoMac());
	MessageBox(m_dm.GetMachineCode());
	return ;
	m_api.SetRoleId(0,_T("��������0"));
	
	m_api.SetRoleId(1,_T("��������1"));
	
	m_api.SetRoleId(2,_T("��������2"));
	m_api.SetRoleId(3,_T("��������3"));
	m_api.SetRoleId(4,_T("��������4"));
	MessageBox(m_api.GetRoleId(0));
	MessageBox(m_api.GetRoleId(1));
	MessageBox(m_api.GetRoleId(2));
	MessageBox(m_api.GetRoleId(3));
	MessageBox(m_api.GetRoleId(4));
}


void CfeidusoftDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(_T("ȷ��Ҫ�˳������"),_T("����˳�ȷ�ϣ�"),MB_OKCANCEL) == IDOK)
	{
		//�û�ȷ���˳������ֱ�ӽ������н��̲�ֹͣ���й���
		for (int i=0;i<=opens;i++)
		{
			isStartThread[i] = FALSE;
			dm[i].UnBindWindow();
		}
	    CDialogEx::OnCancel();
	}
}


// ����һ�����ݣ�����������У�IDֻ�ܴ�0-4
bool CfeidusoftDlg::AddListItem(int id)
{
	if(id>opens){
		return false;
	}
	int index;
	LPTSTR str_id=new TCHAR[2];
	LPTSTR str_hwnd = new TCHAR[8];
	//str_id[0] = (char)id;
	//str_id[1] = '\0';
	wsprintf(str_id, L"%d", id);
	if(hwnd[id]<=0){
		hwnd[id] = 0;
	}
	wsprintf(str_hwnd, L"%d", hwnd[id]);
	//wsprintf(id,str_id,10);
	index = m_userlist.InsertItem(id,str_id);
	m_userlist.SetItemText(index,1,str_hwnd);
	m_userlist.SetItemText(index,2,_T("40954115"));
	m_userlist.SetItemText(index,3,_T("MAPNAME"));
	m_userlist.SetItemText(index,4,_T("�ƶ���"));
	delete(str_hwnd);
	delete(str_id);
	return true;
}


void CfeidusoftDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_userlist.GetFirstSelectedItemPosition();
	int nItem;
	if (pos == NULL){ 
		// û��ѡ�е���
		MessageBox(L"û��ѡ����",L"��Ϣ��ʾ��");
		return ;
	}else 
	{ 
		while (pos) 
		{ 
			nItem = m_userlist.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	/*if(dm[nItem].GetWindowState(hwnd[nItem],0) == 0){
		MessageBox(_T("ָ���е���Ϸ�����Ѳ�����,����HOME�ȼ�ָ�����ڽ��й�������"),_T("��Ϣ��ʾ"));
		return;
	}*/
	CString msg;
	DWORD ThreadID;
	if(isStartThread[nItem] == FALSE){
		UpdateConfigData(nItem);
		CstartSetpHzToJl CsshtjDlg;
		CstartSetpHzToAglToKl csshtatkDlg;
		switch(ConfigData->PaoshangLine)
		{
		case 1:
			CsshtjDlg.startSetpId = nItem;
			CsshtjDlg.DoModal();
			TRACE("ѡ��������:%d\n",StartSetp[nItem]);
			//return;
			break;
		case 2:
			csshtatkDlg.startSetpId = nItem;
			csshtatkDlg.DoModal();
			TRACE("ѡ��������:%d\n",StartSetp[nItem]);
			break;
		default:
			msg.Format(_T("�޷�ʶ���������·,��ɫID��%s��ֹͣ����"),msgUserlist[nItem].roleId);
			MessageBox(msg,_T("������ʾ��"),MB_OK);
			return;
		}
		TRACE("�������̣߳�%d\n",nItem);
		Handle[nItem] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)paoshangThread,(VOID*)nItem,0,&ThreadID);//�����̣߳��̲߳�����;����ֻ��ֹͣ����������
		isStartThread[nItem] = TRUE;
	}else{

	}

}


void CfeidusoftDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	int nItem;
	CString msg;
	POSITION pos = m_userlist.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{
		m_dm.SetWindowState((long)m_hWnd,1);	
		// û��ѡ�е���
		MessageBox(L"��Ҫѡ��һ����Ϣ�����������ܣ�",L"��Ϣ��ʾ��");
	    return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_userlist.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	
	DWORD ThreadID;
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nKey2 == VK_HOME)
	{
		if(isStartThread[nItem] == FALSE){
			UpdateConfigData(nItem);
			CstartSetpHzToJl CsshtjDlg;
			CstartSetpHzToAglToKl csshtatkDlg;
			switch(ConfigData[nItem].PaoshangLine)
			{
			case 1:
				CsshtjDlg.startSetpId = nItem;
				CsshtjDlg.DoModal();
				TRACE("ѡ��������:%d\n",StartSetp[nItem]);
				//return;
				break;
			case 2:
				csshtatkDlg.startSetpId = nItem;
				csshtatkDlg.DoModal();
				TRACE("ѡ��������:%d\n",StartSetp[nItem]);
				break;
			default:
				msg.Format(_T("�޷�ʶ���������·,��ɫID��%s��ֹͣ����"),msgUserlist[nItem].roleId);
				MessageBox(msg,_T("������ʾ��"),MB_OK);
				return;
				break;
			}
			//��ȡ�����ʺ�����
			TRACE("�������̣߳�%d\n",nItem);
			Handle[nItem] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)paoshangThread,(VOID*)nItem,0,&ThreadID);//�����̣߳��̲߳�����;����ֻ��ֹͣ����������
			isStartThread[nItem] = TRUE;
		}else{
			
		}
		//Handle[nItem] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)paoshang,(VOID*)nItem,0,&ThreadID[nItem]);//�����̣߳��̲߳�����;����ֻ��ֹͣ����������
		//�ȼ���HOME����Ķ���
		//MessageBox(L"HOME");
	}
	else if (nKey2 == VK_END)
	{
		//�ȼ���END��Ķ���
		//MessageBox(L"End");
		if (isStartThread[nItem] == TRUE)
		{
			MazeMove(nItem,UP,true);//ֹͣ�ƶ�
			MazeMove(nItem,DOWN,true);//ֹͣ�ƶ�
			m_dm.KeyUp(17);
			m_userlist.SetItemText(nItem,4,_T("��ֹͣ"));
			dm[nItem].UnBindWindow();
			TerminateThread(Handle[nItem],EXIT_FAILURE);
			CloseHandle(Handle[nItem]);
			isStartThread[nItem] = FALSE;
		}
		
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CfeidusoftDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	POSITION pos = m_userlist.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{
		// û��ѡ�е���
		MessageBox(_T("��Ҫѡ��һ����Ϣ�����������ܣ�"),_T("��Ϣ��ʾ��"));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_userlist.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	if (isStartThread[nItem] == TRUE)
	{
		CString msg;
		msg.Format(_T("ȷ��ֹͣ��ɫIDΪ��%s �Ĵ��ڹ�����"),m_userlist.GetItemText(nItem,2));
		if (MessageBox(msg,_T("����ֹͣ��ʾ��"),MB_OKCANCEL) == IDOK)
		{
			MazeMove(nItem,UP,true);//ֹͣ�ƶ�
			MazeMove(nItem,DOWN,true);//ֹͣ�ƶ�
			m_dm.KeyUp(17);
			dm[nItem].UnBindWindow();
			TerminateThread(Handle[nItem],EXIT_FAILURE);
			CloseHandle(Handle[nItem]);
			isStartThread[nItem] = FALSE;
			m_userlist.SetItemText(nItem,4,_T("��ֹͣ"));
		}
	}

}



void CfeidusoftDlg::OnBnClickedBtnUserlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AddUserLists dlg; 
    //dlg.DoModal();
	/*addUser(L"user",L"password");
	int count = m_WaitUserlit.GetItemCount();
	LPTSTR str = new TCHAR[20];
	wsprintf(str,L"%d",count);
	MessageBox(str);*/
	if(isEditWaitUserList() == FALSE)return;
	CaddUser dlg;
	dlg.isModfiy = false;
	dlg.title = _T("����ʺ���Ϣ��");
	dlg.DoModal();
}


void CfeidusoftDlg::addUser(CString user, CString pwd,CString sum,CString role)
{
	int count = m_WaitUserlit.GetItemCount();
	int nItem = m_WaitUserlit.InsertItem(count,user);
	m_WaitUserlit.SetItemText(nItem,1,pwd);
	m_WaitUserlit.SetItemText(nItem,2,sum);
	m_WaitUserlit.SetItemText(nItem,3,role);
	m_WaitUserlit.SetItemText(nItem,4,_T("���Ϻ�"));
	setWaitUserListByOne(user,pwd,sum,role,_T("���Ϻ�"));
}



void CfeidusoftDlg::OnBnClickedBtnModfiyUser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// û��ѡ�е���
		MessageBox(_T("��Ҫѡ��һ����Ϣ��"),_T("��Ϣ��ʾ��"));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	if(isEditWaitUserList() == FALSE)return;
	CaddUser dlg;
	dlg.nItem = nItem;
	dlg.isModfiy = true;
	dlg.title = _T("�޸��ʺ���Ϣ��");
	dlg.DoModal();
}

void CfeidusoftDlg::modfiyUser(int nItem,CString user, CString pwd,CString sum,CString role)
{
	m_WaitUserlit.SetItemText(nItem,0,user);
	m_WaitUserlit.SetItemText(nItem,1,pwd);
	m_WaitUserlit.SetItemText(nItem,2,sum);
	m_WaitUserlit.SetItemText(nItem,3,role);
	CString state = m_WaitUserlit.GetItemText(nItem,4);
	setWaitUserListByOne(user,pwd,sum,role,state);
}


void CfeidusoftDlg::setConfig(CString section,CString key,CString value,CString file)
{
	m_dm.CreateFolder(pathConfig);
	m_dm.SetPath(pathConfig);
	int result = m_dm.WriteIni(section,key,value,file);
	if(result == 0)
	{
		MessageBox(_T("�����ļ�д��ʧ��"));
	}
}

void CfeidusoftDlg::getConfig(CString section,CString key,CString file,CString& value,CString defVal){
	m_dm.SetPath(pathConfig);
	value = m_dm.ReadIni(section,key,file);
	if (value.IsEmpty())
	{
		value = defVal;
	}
}

void CfeidusoftDlg::setWaitUserlist()
{
	int count = m_WaitUserlit.GetItemCount();//��ȡһ���ж���������
	if (count == 0)
	{
		//���û������������Ϊ�պ�ֱ�ӷ���
		setConfig(L"WaitUserList",_T("userlist"),_T(""),_T("config.ini"));
		return;
	}
	CString userlist;
	for (int i=0;i<count;i++)
	{
		if(userlist.IsEmpty()){
			userlist += m_WaitUserlit.GetItemText(i,0);
			userlist += L","+m_WaitUserlit.GetItemText(i,1);
			
		}else{
			userlist += L"||"+m_WaitUserlit.GetItemText(i,0);
			userlist += L","+m_WaitUserlit.GetItemText(i,1);
		}
		//time = CTime::GetCurrentTime()-CTimeSpan( 1, 0, 0, 0 );//�������ڵ�ǰһ��
		//CString last = time.Format("%Y-%m-%d");
	}
	MessageBox(userlist);
}

void CfeidusoftDlg::getWaitUserlist()
{
	CString userlist;
	getConfig(_T("WaitUserList"),_T("userlist"),_T("config.ini"),userlist);
	//userlist.
	CTime time;
	time = CTime::GetCurrentTime();//��������
	CString str = time.Format("%Y-%m-%d");
	CString sum;
	getConfig(str,L"F",L"config.ini",sum);
}


BEGIN_MESSAGE_MAP(CfeidusoftDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START, &CfeidusoftDlg::OnBnClickedBtnStart)
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BTN_STOP, &CfeidusoftDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_USERLIST, &CfeidusoftDlg::OnBnClickedBtnUserlist)
	ON_BN_CLICKED(IDC_BTN_MODFIY_USER, &CfeidusoftDlg::OnBnClickedBtnModfiyUser)
	ON_MESSAGE(WM_USERLIST_SETONEROW_MSG, &CfeidusoftDlg::SetUserListMsgAsOneRow)
	ON_MESSAGE(WM_USERLIST_SETCELL_MSG, &CfeidusoftDlg::SetUserListMsgAsCell)
	ON_MESSAGE(WM_USERLIST_GETCELL_MSG, &CfeidusoftDlg::GetUserListMsgAsCell)
	ON_MESSAGE(WM_WAIT_USERLIST_SETONEROW_MSG, &CfeidusoftDlg::SetWaitUserListMsgAsOneRow)
	ON_MESSAGE(WM_WAIT_USERLIST_SETCELL_MSG, &CfeidusoftDlg::SetWaitUserListMsgAsCell)
	ON_MESSAGE(WM_WAIT_USERLIST_GETCELL_MSG, &CfeidusoftDlg::GetWaitUserListMsgAsCell)
	ON_MESSAGE(WM_WAIT_USERLIST_SETCELL_BY_USERNAME, &CfeidusoftDlg::SetWaitUserAsCellByUsername)
	ON_MESSAGE(WM_WAIT_USERLIST_GET_NEW_USERNAME, &CfeidusoftDlg::GetWaitUserListNewUsername)

	//�жϵ�ǰ��ɫID��Ӧ���ʺ���Ϣ������ڴ����б����Ҳ�����������
	ON_MESSAGE(WM_IS_CURRENT_ROLE_AS_USERNAME, &CfeidusoftDlg::IsCurrentRoleAsUsername)
	
	
	/*
	#define WM_USERLIST_SETONEROW_MSG WM_USER + 1001
	#define WM_USERLIST_SETCELL_MSG WM_USER+1002
	#define WM_USERLIST_GETCELL_MSG WM_USER+1003
	#define WM_WAIT_USERLIST_SETONEROW_MSG WM_USER+1004
	#define WM_WAIT_USERLIST_SETCELL_MSG WM_USER+1005
	#define WM_WAIT_USERLIST_GETCELL_MSG WM_USER+1006
	*/
	ON_BN_CLICKED(IDC_BTN_SET_HOTKEY, &CfeidusoftDlg::OnBnClickedBtnSetHotkey)
	ON_BN_CLICKED(IDC_BTN_ALL_STOP, &CfeidusoftDlg::OnBnClickedBtnAllStop)
	ON_BN_CLICKED(IDC_BTN_SAVE_SETTING, &CfeidusoftDlg::OnBnClickedBtnSaveSetting)
	ON_BN_CLICKED(IDC_BTN_HIDE_SHOW, &CfeidusoftDlg::OnBnClickedBtnHideShow)
	ON_BN_CLICKED(IDC_BTN_TEST, &CfeidusoftDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_MOVE_UP, &CfeidusoftDlg::OnBnClickedBtnMoveUp)
	ON_BN_CLICKED(IDC_BTN_MOVE_DOWN, &CfeidusoftDlg::OnBnClickedBtnMoveDown)
	ON_BN_CLICKED(IDC_BTN_GAME_PATH, &CfeidusoftDlg::OnBnClickedBtnGamePath)
	ON_BN_CLICKED(IDC_BTN_WAIT_USER_DEL, &CfeidusoftDlg::OnBnClickedBtnWaitUserDel)
	ON_BN_CLICKED(IDC_BTN_COPY_CODE, &CfeidusoftDlg::OnBnClickedBtnCopyCode)
END_MESSAGE_MAP()




void CfeidusoftDlg::OnBnClickedBtnSetHotkey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CmodfiyHotKey modfiyHotKeyDlg;
	UnregisterHotKey(m_hWnd,WM_HOTKEY_START_END);//ע���ȼ��ȴ���������
	modfiyHotKeyDlg.DoModal();//
	
}



// �����޸ĵ��ȼ�


void CfeidusoftDlg::UpdateHotKey(CString HotkeyName,UINT modifier, UINT vk)
{
	LPTSTR str = new TCHAR[10];
	wsprintf(str,_T("%d"),modifier);
	setConfig(_T("Hotkey"),_T("_modifier"),str,_T("Config.ini"));
	wsprintf(str,_T("%d"),vk);
	setConfig(_T("Hotkey"),HotkeyName+_T("_vk"),str,_T("Config.ini"));
	RegisterHotKey(m_hWnd,WM_HOTKEY_START_END,modifier,vk);
}


void CfeidusoftDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


void CfeidusoftDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}

void CfeidusoftDlg::OnBnClickedBtnAllStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}




void CfeidusoftDlg::OnBnClickedBtnSaveSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveSetting();
	MessageBox(_T("���ñ���ɹ�,�������Ĵ���������Ч��"),_T("���ø�����ʾ��"),MB_OK);
}

//��������
void CfeidusoftDlg::SaveSetting(void)
{
	
	//CString temp;
	//������������
	//temp.Format(_T("%d"),);
	setConfig(_T("PaoShang"),_T("type"),IntToCString(m_psType.GetCurSel()),_T("config.ini"));
	setConfig(_T("DM"),_T("line"),IntToCString(m_ans.GetCurSel()),_T("config.ini"));//����ѡ��
	//����������·
	//temp.Format(_T("%d"),m_psLine.GetCurSel());
	setConfig(_T("PaoShang"),_T("line"),IntToCString(m_psLine.GetCurSel()),_T("config.ini"));
	setConfig(_T("PaoShangSum"),_T("PAOSHUANG"),IntToCString(m_paoshangSum.GetCurSel()),_T("config.ini"));

	setConfig(_T("PaoShang"),_T("autoHuanhao"),IntToCString(m_autohuanhao.GetCheck()),_T("config.ini"));
	setConfig(_T("PaoShang"),_T("buyYao"),IntToCString(m_buyYao.GetCheck()),_T("config.ini"));
	for (int i=0;i<=opens;i++)
	{
		if(isStartThread[i] == FALSE){//����߳�Ϊ����״̬����ô���ò��ᱻ����
			UpdateConfigData(i);
			//���¸���������Ϣ������������Ч
		}
	}
}


// ����������Ϣ
void CfeidusoftDlg::UpdateConfigData(int id)
{
	CString temp;
	//������������
	getConfig(_T("PaoShang"),_T("type"),_T("config.ini"),temp);
	ConfigData[id].paoshangType = CStringToInt(temp);

	if(ConfigData[id].paoshangType ==1)
		isReward[id] = TRUE;
	else
		isReward[id] = FALSE;

	//��������·��
	getConfig(_T("PaoShang"),_T("line"),_T("config.ini"),temp);
	ConfigData[id].PaoshangLine = CStringToInt(temp);

	getConfig(_T("PaoShang"),_T("buyYao"),_T("config.ini"),temp);
	isBuyYao[id] = CStringToInt(temp);

	getConfig(_T("PaoShang"),_T("autoHuanhao"),_T("config.ini"),temp);
	isAutoHuanhao[id] = CStringToInt(temp);

	//������·
	getConfig(_T("DM"),_T("line"),_T("config.ini"),temp);
	ConfigData[id].DM_line = CStringToInt(temp);

	//������·
	getConfig(_T("PaoShangSum"),_T("PAOSHUANG"),_T("config.ini"),temp);
	int temp_ps_sum = CStringToInt(temp);
	switch(temp_ps_sum)
	{
	case 0://10Ʊ
		ConfigData[id].sum = 10;
		break;
	case 1://11Ʊ
		ConfigData[id].sum = 11;
		break;
	case 2://12Ʊ
		ConfigData[id].sum = 12;
		break;
	case 3://13Ʊ
		ConfigData[id].sum = 13;
		break;
	case 4://14Ʊ
		ConfigData[id].sum = 14;
		break;
	case 5://15Ʊ
		ConfigData[id].sum = 15;
		break;
	case 6://16Ʊ
		ConfigData[id].sum = 20;
		break;
	default:
		ConfigData[id].sum = 0;
		break;//0
	}
	
}


void CfeidusoftDlg::OnBnClickedBtnHideShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(cw == 0 || ch==0){
		VARIANT w;
		VARIANT h;
		m_dm.GetClientSize((long)m_hWnd,&w,&h);
		cw = w.lVal;
		ch = h.lVal;
	}
	if(isHide){
		m_dm.SetClientSize((long)m_hWnd,cw,ch);
		m_hideWaitUserlist.SetWindowTextW(_T("<<�����б�"));
		isHide = false;
	}else{
		m_dm.SetClientSize((long)m_hWnd,cwMin,ch);
		m_hideWaitUserlist.SetWindowTextW(_T("չ���б�>>"));
		isHide = true;
	}
}



// ������������
int CfeidusoftDlg::SetStartSetp(int id,int setp)
{
	StartSetp[id] = setp;
	return 0;
}


void CfeidusoftDlg::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unResPakage(_T("Desktop.y"),_T("C:\\Program Files\\feidu\\"),"123456789");
}


// ����һ�����滻�ʺ�
void CfeidusoftDlg::setWaitUserListByOne(CString username, CString password,CString sum,CString role,CString state){
	CTime time;
	time = CTime::GetCurrentTime();//��������
	CString section = time.Format("%Y-%m-%d");
	setConfig(section,username,sum+_T(",")+state,_T("userlist.ini"));//������滻�ʺ��б�
	setConfig(_T("USERLISTS"),username,password+_T("|")+role,_T("usersource.ini"));//�����ʺ�Դ�б�_��ɫ�ʺ�Ϊ��
}


// ��ȡ���滻�ʺ��б�
void CfeidusoftDlg::GetWaitUserLists(CString section_filename,CString list_filename){
	CTime time;
	CStringArray lists,lists2;
	time = CTime::GetCurrentTime();//��������
	CString section = time.Format("%Y-%m-%d");
	//��ʼ��ǰ�Ƚ����в��ǽ��������ȫ��ɾ��
	CString wait_lists = m_dm.EnumIniSection(list_filename);
	CStringToArray(wait_lists,'|',lists2);
	int len = lists2.GetCount();
	for (int i=0;i<len;i++)
	{
		if (lists2.GetAt(i) == section)
		{
		}else{
			m_dm.DeleteIni(lists2.GetAt(i),_T(""),list_filename);
		}
	}
	CString scoure_lists = m_dm.EnumIniKey(_T("USERLISTS"),section_filename);
	if (scoure_lists.IsEmpty())   return;

	CStringToArray(scoure_lists,'|',lists);
	len = lists.GetCount();
	for (int i=0;i<len;i++)
	{
		CString temp,pwd,sum,role,state;
		CStringArray sum_state,pwd_role;

		int nItem = m_WaitUserlit.InsertItem(0,lists.GetAt(i));

		getConfig(_T("USERLISTS"),lists.GetAt(i),section_filename,temp);
		CStringToArray(temp,'|',pwd_role);
		if(pwd_role.GetCount()>1){
			m_WaitUserlit.SetItemText(nItem,3,pwd_role.GetAt(0));
			m_WaitUserlit.SetItemText(nItem,1,pwd_role.GetAt(1));
		}else{
			m_WaitUserlit.SetItemText(nItem,1,pwd_role.GetAt(0));
		}

		getConfig(section,lists.GetAt(i),list_filename,temp);
		CStringToArray(temp,',',sum_state);
		if(sum_state.GetCount()>1){
			if(sum_state.GetAt(1).IsEmpty())sum = _T("0");
			if(sum_state.GetAt(1).SpanIncluding(_T("0123456789")) == sum_state.GetAt(1)){
				sum = sum_state.GetAt(1);
			}else
				sum = _T("0");

			if(sum_state.GetAt(0).IsEmpty())
				state = _T("���Ϻ�");
			else
				state = sum_state.GetAt(0);
		}else{
			if(sum_state.GetAt(0).IsEmpty())sum = _T("0");
			if(sum_state.GetAt(0).SpanIncluding(_T("0123456789")) == sum_state.GetAt(0)){
				sum = sum_state.GetAt(0);
			}else
				sum = _T("0");
			state = _T("���Ϻ�");
		}
		m_WaitUserlit.SetItemText(nItem,2,sum);
		m_WaitUserlit.SetItemText(nItem,4,state);
		setConfig(section,lists.GetAt(i),sum+_T(",")+state,list_filename);
	}
}



void CfeidusoftDlg::OnBnClickedBtnMoveUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// û��ѡ�е���
		MessageBox(_T("��Ҫѡ��һ����Ϣ��"),_T("��Ϣ��ʾ��"));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	if (nItem == 0)
	{
		return;
	}
	if(isEditWaitUserList() == FALSE)return;
	CString username_temp = m_WaitUserlit.GetItemText(nItem,0);
	CString password_temp = m_WaitUserlit.GetItemText(nItem,1);
	CString sum_temp = m_WaitUserlit.GetItemText(nItem,2);
	CString role_temp = m_WaitUserlit.GetItemText(nItem,3);
	CString state_temp = m_WaitUserlit.GetItemText(nItem,4);
	m_WaitUserlit.DeleteItem(nItem);
	int index = m_WaitUserlit.InsertItem(nItem-1,username_temp);
	m_WaitUserlit.SetItemText(index,1,password_temp);
	m_WaitUserlit.SetItemText(index,2,sum_temp);
	m_WaitUserlit.SetItemText(index,3,role_temp);
	m_WaitUserlit.SetItemText(index,4,state_temp);
	m_WaitUserlit.SetItemState(index,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
	ReSortWaitUserList();
}


void CfeidusoftDlg::OnBnClickedBtnMoveDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// û��ѡ�е���
		MessageBox(_T("��Ҫѡ��һ����Ϣ��"),_T("��Ϣ��ʾ��"));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	int count = m_WaitUserlit.GetItemCount()-1;
	if (nItem >= count)
	{
		return;
	}
	if(isEditWaitUserList() == FALSE)return;
	CString username_temp = m_WaitUserlit.GetItemText(nItem,0);
	CString password_temp = m_WaitUserlit.GetItemText(nItem,1);
	CString sum_temp = m_WaitUserlit.GetItemText(nItem,2);
	CString role_temp = m_WaitUserlit.GetItemText(nItem,3);
	CString state_temp = m_WaitUserlit.GetItemText(nItem,4);
	m_WaitUserlit.DeleteItem(nItem);
	int index = m_WaitUserlit.InsertItem(nItem+1,username_temp);
	m_WaitUserlit.SetItemText(index,1,password_temp);
	m_WaitUserlit.SetItemText(index,2,sum_temp);
	m_WaitUserlit.SetItemText(index,3,role_temp);
	m_WaitUserlit.SetItemText(index,4,state_temp);
	m_WaitUserlit.SetItemState(index,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	ReSortWaitUserList();
}


void CfeidusoftDlg::OnBnClickedBtnGamePath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR pszPath[1024];    
	BROWSEINFO bi;     
	bi.hwndOwner      = this->GetSafeHwnd();    
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = NULL;     
	bi.lpszTitle      = TEXT("��ѡ��ٻŮ�Ļ���Ϸ��װĿ¼");     
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;    
	bi.lpfn           = NULL;     
	bi.lParam         = 0;    
	bi.iImage         = 0;     

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);    
	if (pidl == NULL)    
	{    
		return;    
	}
	if (SHGetPathFromIDList(pidl, pszPath))    
	{    
		this->m_GamePathValue.SetWindowText(pszPath);
		game_path = pszPath;
		setConfig(_T("INFO"),_T("gamePath"),game_path,configName);
	}
}


// ���õ��������б��ʺ�����
void CfeidusoftDlg::SetWaitUserAsOne(CString list_filename, CString username, CString password, int sum,CString role,CString state)
{
	CTime time;
	time = CTime::GetCurrentTime();//��������
	CString section = time.Format("%Y-%m-%d");
	CString sum_s;
	sum_s.Format(_T("%d"),sum);
	setConfig(section,username,sum_s+","+state,list_filename);
}


// ���ʺ�Ҫ����б�����,��һ���������ʺ����͵�ǰ���ʺ����ݣ��ڶ�������ID
LRESULT CfeidusoftDlg::SetWaitUserAsCellByUsername( WPARAM wp,LPARAM lp )
{
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_WAIT_USERLIST);
	lvAccount *lvA = (lvAccount*)wp;
	int id = (int)lp;
	CString username,pwd,sum,role,state;
	username = lvA->username;
	pwd = lvA->password;
	sum = IntToCString(lvA->sum);
	role = lvA->roleId;
	state = lvA->state;
	int count = prog->GetItemCount();//��ȡ�����б�����
	for (int i=0;i<count;i++)
	{
		if(username == prog->GetItemText(i,0)){
			//������ʺţ�����һ�����ݽ��в���
			prog->SetItemText(i,2,sum);
			prog->SetItemText(i,3,role);
			setWaitUserListByOne(username,pwd,sum,role,_T("���Ϻ�"));//���ʺ�д�뱾����Ϣ
			return 0;
		}
	}
	return 0;
}

//����ǰ�̷߳����µ��ʺ�,��һ���������ʺ����ͻ���ǰ���ʺ����ݣ��ڶ�������ID
LRESULT CfeidusoftDlg::GetWaitUserListNewUsername(WPARAM wp,LPARAM lp){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_WAIT_USERLIST);
	lvAccount *lvA = (lvAccount*)wp;
	bool isok = false;
	CString username,pwd,sum,role,state;
	username = lvA->username;
	pwd = lvA->password;
	sum = IntToCString(lvA->sum);
	TRACE("����ǰһ���ʺŵ����̴�����%s\n",sum);
	role = lvA->roleId;
	state = lvA->state;
	int count = prog->GetItemCount();//��ȡ�����б�����
	int id = (int)lp;
	for (int i=0;i<count;i++)
	{
		if(prog->GetItemText(i,0) == username){
			TRACE("���ʺ�����Ϊ����ʱ������new��%d\n",lvA->sum);
			prog->SetItemText(i,4,_T("������"));
			SetWaitUserAsOne(_T("userlist.ini"),username,pwd,lvA->sum,role,_T("������"));//���ʺ�д�뱾����Ϣ
		}
		if(CStringToInt(prog->GetItemText(i,2))<10 && prog->GetItemText(i,4)==_T("���Ϻ�") && isok==false){
			AccountInfo[id].username = prog->GetItemText(i,0);
			AccountInfo[id].password = prog->GetItemText(i,1);
			AccountInfo[id].sum = CStringToInt(prog->GetItemText(i,2));
			prog->SetItemText(i,4,_T("������"));
			SetWaitUserAsOne(_T("userlist.ini"),AccountInfo[id].username,AccountInfo[id].password,AccountInfo[id].sum,AccountInfo[id].roleId,_T("���Ϻ�"));//���ʺ�д�뱾����Ϣ
			AccountInfo[id].state = _T("������");
			isok = true;
		}
	}
	if(isok == false){
		isUserEnd = true;
	}
	return 0;
}


void CfeidusoftDlg::OnBnClickedBtnWaitUserDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// û��ѡ�е���
		MessageBox(_T("��Ҫѡ��һ����Ϣ��"),_T("��Ϣ��ʾ��"));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//��ȡѡ���е�����
		} 
	}
	if(isEditWaitUserList() == FALSE)return;
	CString msg;
	msg.Format(_T("ȷ��Ҫ�Ƴ��ʺš�%s����"),m_WaitUserlit.GetItemText(nItem,0));
	if (MessageBox(msg,_T("�ж��ʺ��Ƴ����ѣ�"),MB_OKCANCEL) == IDOK){
		m_dm.DeleteIni(_T("USERLISTS"),m_WaitUserlit.GetItemText(nItem,0),_T("usersource.ini"));
		m_WaitUserlit.DeleteItem(nItem);
		MessageBox(_T(""),_T("�Ƴ��ɹ�"),MB_OK);
	}
}


// ����������滻�ʺ��б�
void CfeidusoftDlg::ReSortWaitUserList(void)
{
	int count = m_WaitUserlit.GetItemCount();
	CString username,pwd,role;
	//���Դ�ʺ�����
	m_dm.DeleteIni(_T("USERLISTS"),_T(""),_T("usersource.ini"));
	for (int i=0;i<count;i++)
	{
		username = m_WaitUserlit.GetItemText(i,0);
		pwd = m_WaitUserlit.GetItemText(i,1);
		role = m_WaitUserlit.GetItemText(i,3);
		m_dm.WriteIni(_T("USERLISTS"),username,pwd+_T("|")+role,_T("usersource.ini"));
	}
}

LRESULT CfeidusoftDlg::IsCurrentRoleAsUsername(WPARAM wp,LPARAM lp){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_WAIT_USERLIST);
	lvAccount *lvA = (lvAccount*)wp;//��ǰ�ʺ���Ϣ
	int id = (int)lp;
	bool isok = false;
	CString username,pwd,sum,role,state;
	username = lvA->username;
	pwd = lvA->password;
	sum = IntToCString(lvA->sum);
	role = lvA->roleId;
	state = lvA->state;
	//��ʼ�����б�����ʼƥ��

	if(role.IsEmpty()){
		AccountInfo[id].ret = FALSE;
		AccountInfo[id].msg = _T("�ʺ���Ϣ��ȡʧ�ܣ��볢����������");
	}
	
	int count = prog->GetItemCount();
	for (int i=0;i<count;i++)
	{
		if (prog->GetItemText(i,3) == role)
		{
			//TABUSER
			AccountInfo[id].username = prog->GetItemText(i,0);
			AccountInfo[id].password = prog->GetItemText(i,1);
			AccountInfo[id].sum = CStringToInt(prog->GetItemText(i,2));
			AccountInfo[id].state = _T("������");
			TRACE("��ǰ����ŵ����̴�����%d\n",AccountInfo[id].sum);
			if(AccountInfo[id].sum>=10){
				TRACE("���ز�����\n");
				AccountInfo[id].ret = TABUSER;
				return 0;
			}
			prog->SetItemText(i,4,_T("������"));
			isok = true;
		}
	}
	if (isok == false)
	{
		AccountInfo[id].ret = FALSE;
		AccountInfo[id].msg.Format(_T("�ʺ��б���û���ҵ���ɫID��%s����Ӧ���ʺ���Ϣ\n\nע�⣺����ʺ��б����ж�Ӧ�ʺţ����ֶ���д��Ӧ��ɫID�ٳ�����������"),role);
	}
	return 0;
}


// ��ѹ��Դ����ָ��λ��
bool CfeidusoftDlg::unResPakage(CString filename, CString undir, LPCSTR password)
{
	C7zip zip;
	//CString m_FilePath;
	//GetModuleFileName(NULL,m_FilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	//m_FilePath.ReleaseBuffer();
	//int m_iPosIndex;
	//m_iPosIndex = m_FilePath.ReverseFind('\\');
	//m_FilePath = m_FilePath.Left(m_iPosIndex);
	CString file = getFilePos(filename);
	//file.Format(_T("%s\\%s"),m_FilePath,filename);
	TRACE(file);
	int result = zip.Unzip(file,undir,password);
	if(result == 1){
		return true;
	}else{
		CString msg;
		msg.Format(_T("��Դ�ļ���ȡʧ�ܣ�%d"),result);
		MessageBox(msg,_T("������Ϣ��"),MB_OK);
		return false;
	}
	
}


void CfeidusoftDlg::OnBnClickedBtnCopyCode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString data;
	m_MachineCode.GetWindowText(data);
	m_dm.SetClipboard(data);
	MessageBox(_T("�����븴�Ƶ����а�"),_T("���ƻ����룺"));
}
