
// feidusoftDlg.cpp : 实现文件
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


//全局定义
api m_tapi;//定义全局api
tools dm[opens+1];//全局大漠对象
BOOL isStartThread[opens+1];//线局线程标记
BOOL isStopThread[opens+1];
long hwnd[opens+1];//窗口句柄
BOOL isMaze[opens+1];
tools m_dm;
HANDLE Handle[opens+1];//线程句柄
CString pathConfig;
CString configName;
lvUserList msgUserlist[opens+1];
lvWaitUserList msgWaitUserlist[opens+1];
BOOL isReward[opens+1];
int isAutoHuanhao[opens+1];
int isBuyYao[opens+1];
lvConfig ConfigData[opens+1];//跑商配置信息
int StartSetp[opens+1];//启动进度变量
lvAccount AccountInfo[opens+1];//启动功能帐号信息
bool oneRun = false;//等于false时可以运行下去。true不可以运行下去
CString game_path;
bool isUserEnd = false;//当还有帐号可以更换时返回false,没有帐号更换时返回true
CString bmpDir;
bool is_windows[opens+1];
CString yaoNames;
long gt1[opens+1],gt2[opens+1];


HANDLE m_hMutex = CreateMutex(NULL, FALSE, NULL);//互斥体
CString GetRoleId(int id);//获取角色ID
long GetWindowHwnd(int id);//获取游戏窗口句柄
long BindGameWindow(int id);//绑定游戏窗口
BOOL Delay(int id,long minMs,long maxMs);//延时，封装Sleep和Maze,FALSE进迷宫了,TRUE没进迷宫
void AutoPaoshuang(int id);//初始开启跑商功能
CString CurrentMap(int id);//取当前地图
STATE MoveGhNpc(int id);//将角色移动到帮会NPC
STATE HuiBang(int id);//返回帮会
BOOL paoshangTakeTask(int id);//领取跑商任务
STATE paoshangMove(int id);//移动出帮开始跑商
STATE paoshangGMSP(int id);//购买货物
STATE paoshangCSSP(int id);//出售货物
STATE AutoPathfindingToShangRen(int id,CString keyStr,CString npcName);//自动寻路到商人/货商NPC
STATE GoNextMap(int id,LPTSTR npcName,CString keyStr1,LPTSTR npcMsg,LPTSTR nextMap);//前去下一个地图（NPC方式）
STATE GoNextMapBySpot(int id,LPTSTR npcName,CString keyStr,LPTSTR stopPos,lvXY ClickPos,LPTSTR nextMap);//前去下一个地图（传送点方式）
STATE OpenSHSD(int id);//打开商会商店
STATE paoshangGiveTask(int id);//任务完成，送还任务
BOOL TakeTask(int id);//接跑商
STATE TakeReward(int id);//接赏金
BOOL Maze(int id);//迷宫中
lvXY MazeMove(int id);//迷宫中移动判定
BOOL MazePosition(int id,lvXY inXY);//false方向不对，true方向正确
lvXY MazePos(int id);//获取当前角色处理迷宫位置（字符串坐标）
int isLastPos(int currentPos,int OkPos);
void Verifiy(int id);//验证码检测
void ExitMsg(int id,int s);//清除游戏中所有窗口
void msgExit(int id);//关闭弹出的消息窗口
void KeyPressChar(int id,CString str);//输入文本
void PublicAnswer(int id,LPSTR filename,int x,int y);
void Haoi_Answer(int id,LPSTR filename,int x,int y);
void YunSu_Answer(int id,LPSTR filename,int x,int y);

void movePosClick(int id,long x,long y);
int timeBuyYao(int id);

BOOL MinExit(int id);

BOOL AutoExit(int id,CString username);
BOOL AutoLogin(int id,CString password);
void CancelShangRen(int id);//取消商位

void Log(int id,CString str);//日志文件写入

void setRunState(int id,CString state);//设置跑商状态

void SetWaitUserAsOne(CString list_filename,CString username,CString password,int sum,CString role=_T(""),CString state = _T(""));//更新单条跑商待替列表数据

void UpdateWaitUserDataBySum(int id,lvAccount la);//更新跑商计数
//内线，杭州到金陵
void InLineHzToJl(int id);
void InLineHzToJl(int id){
	//0判断是否在杭州，如果不在，也不在帮则移动到杭州
	//1进帮接任务
	//2离帮出杭州
	//3初始寻找杭州NPC打开商会商店
	//3-1购买商品
	//4前去下一个地图（金陵）
	//5寻找金陵NPC打开商会商店
	//5-1出售商品
	//5-2购买商品
	//6前去下一个地图（杭州）
	//7寻找杭州NPC打开商会商店
	//7-1出售商品
	//7-2购买商品
	//8跳到第4步，直到满票
	//以上每一步都封装好，并有返回值，指定返回值则表示进了迷宫并出来了，然后跳转至上次操作继续跑商
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
		MessageBox(0,_T("无法识别的进度，功能停止"),_T("启动提示："),MB_OK);
		return;
	}
	STATE result;
setp1:
	MoveGhNpc(id);//移动并回帮
setp1_1:
	paoshangGiveTask(id);//接任务|交任务
setp2:
	result = paoshangMove(id);//移动出帮到杭州NPC处买东西
	if (result == MAZE)goto setp2;
setp3:
	result = OpenSHSD(id);//打开商会商店
	if (result == MAZE || result == FALSE)goto setp2;
setp3_1:
	result = paoshangGMSP(id);
	if (result == MAZE)goto setp2;
setp4:
	result = GoNextMap(id,_T("周海发"),_T("zhf"),_T("前去金陵"),_T("金陵"));//去往下个地图
	if (result == MAZE)goto setp4;
setp5:
	result = AutoPathfindingToShangRen(id,_T("jlsr"),_T("金陵商人"));//寻找商人货商NPC
	if (result == MAZE)goto setp5;
setp6:
	result = OpenSHSD(id);//打开商会商店
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
		result = AutoPathfindingToShangRen(id,_T("jlsr"),_T("金陵商人"));//寻找商人货商NPC
		if (result == MAZE)goto setp8_1;
		result = OpenSHSD(id);//打开商会商店
		if (result == FALSE)goto setp8_1;
		else if(result == MAZE)goto setp8_1;
		goto setp8;
	}
setp9:
	result = GoNextMap(id,_T("禇全福"),_T("cqf"),_T("前去杭州"),_T("杭州"));
	if(result == MAZE)goto setp9;
setp10:
	result = AutoPathfindingToShangRen(id,_T("hzsr"),_T("杭州商人"));//寻找商人货商NPC
	if(result == MAZE)goto setp10;
setp11:
	result = OpenSHSD(id);//打开商会商店
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
		result = AutoPathfindingToShangRen(id,_T("hzsr"),_T("杭州商人"));//寻找商人货商NPC
		if(result == MAZE)goto setp13_1;
		result = OpenSHSD(id);//打开商会商店
		if (result == FALSE)goto setp13_1;
		else if(result == MAZE)goto setp13_1;
		goto setp13;
	}
	goto setp4;
give://交票标记
	result = MoveGhNpc(id);
	if(result == MAZE)goto give;
	result = paoshangGiveTask(id);
	if(result == MAZE)goto give;
	else if(result == QUIT)return;
	goto setp2;
	
}

//外线，杭州-阿格拉-昆仑
void OutLineHzToAglToKl(int id);
void OutLineHzToAglToKl(int id){
	lvXY hz2tzhaClickPos;//杭州到台州海岸的点击坐标
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
		MessageBox(0,_T("无法识别的进度，功能停止"),_T("启动提示："),MB_OK);
		return;
	}
	STATE result;
setp1:
	MoveGhNpc(id);//移动并回帮
setp1_1:
	paoshangGiveTask(id);//接任务|交任务
setp2:
	result = paoshangMove(id);//移动出帮到杭州NPC处买东西
	if (result == MAZE)goto setp2;
setp3:
	result = OpenSHSD(id);//打开商会商店
	if (result == MAZE || result == FALSE)goto setp2;
setp3_1:
	result = paoshangGMSP(id);
	if (result == MAZE)goto setp2;
setp4:
	result = GoNextMapBySpot(id,_T("传送到台州海岸"),_T("csd"),HZTOTZHA,hz2tzhaClickPos,_T("台州海岸"));
	if (result == MAZE)goto setp4;
setp5:
	result = GoNextMap(id,_T("徐邙山"),_T("xws"),_T("前往天姥仙山"),_T("天姥仙山"));//去往下个地图
	if (result == MAZE)goto setp5;
setp5_1:
	result = AutoPathfindingToShangRen(id,_T("xssr"),_T("仙山商人"));//寻找商人货商NPC
	if (result == MAZE)goto setp5_1;
setp5_2:
	result = OpenSHSD(id);//打开商会商店
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
		result = AutoPathfindingToShangRen(id,_T("xssr"),_T("仙山商人"));//寻找商人货商NPC
		if (result == MAZE)goto setp5_5;
		result = OpenSHSD(id);//打开商会商店
		if (result == FALSE)goto setp5_5;
		else if(result == MAZE)goto setp5_5;
		goto setp5_4;
	}
setp6:
	result = GoNextMap(id,_T("陈立全"),_T("clq"),_T("上船前去阿格拉"),_T("阿格拉"));
	if (result == MAZE)goto setp6;
setp6_1:
	result = AutoPathfindingToShangRen(id,_T("aglsr"),_T("阿格拉商人"));//寻找商人货商NPC
	if (result == MAZE)goto setp6_1;
setp6_2:
	result = OpenSHSD(id);//打开商会商店
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
		result = AutoPathfindingToShangRen(id,_T("aglsr"),_T("阿格拉商人"));//寻找商人货商NPC
		if (result == MAZE)goto setp6_5;
		result = OpenSHSD(id);//打开商会商店
		if (result == FALSE)goto setp6_5;
		else if(result == MAZE)goto setp6_5;
		goto setp6_4;
	}
setp7:
	result = GoNextMap(id,_T("拉吉"),_T("lj"),_T("加入西域商队"),_T("昆仑荒漠"));
	if (result == MAZE)goto setp7;
setp7_1:
	result = AutoPathfindingToShangRen(id,_T("klsr"),_T("昆仑商人"));//寻找商人货商NPC
	if (result == MAZE)goto setp7_1;
setp7_2:
	result = OpenSHSD(id);//打开商会商店
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
		result = AutoPathfindingToShangRen(id,_T("klsr"),_T("昆仑商人"));//寻找商人货商NPC
		if (result == MAZE)goto setp7_5;
		result = OpenSHSD(id);//打开商会商店
		if (result == FALSE)goto setp7_5;
		else if(result == MAZE)goto setp7_5;
		goto setp7_4;
	}
setp8:
	result = GoNextMap(id,_T("邓辊"),_T("dk"),_T("加入阿格拉商队"),_T("阿格拉"));
	if (result == MAZE)goto setp8;
	goto setp6_1;
give://交票标记
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
	dm[id].FindStr(250,240,630,400,_T("关闭"),_T("ffffff-101010"),1.0,&x,&y);
	if(x.intVal>=0 && y.intVal>=0){
		MoveTo(id,x.intVal+10,y.intVal+5);
		dm[id].Delay(66);
		LeftClick(id);
	}
	dm[id].UseDict(index_dict);
}

//跑商线程函数 id 线程索引ID
void paoshangThread(int id){//跑商功能
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
		MessageBox(0,_T("未获取到游戏窗口,功能启动失败！"),_T("信息提示："),MB_OK);
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
	msgUserlist[id].state = _T("启动中");
	msgUserlist[id].currentMap = _T("暂无");
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_USERLIST_SETONEROW_MSG,(WPARAM)&msgUserlist[id],0);
	//在这里开始由角色ID判定帐号信息
	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_IS_CURRENT_ROLE_AS_USERNAME,(WPARAM)&AccountInfo[id],id);
	if(AccountInfo[id].ret == FALSE){
		
		CloseHandle(Handle[id]);
		oneRun = false;
		//ReleaseMutex(m_hMutex);
		isStartThread[id] = FALSE;
		MessageBox(0,AccountInfo[id].msg,_T("信息提示："),MB_OK);
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
		msg.Format(_T("无法识别的跑商类型，角色ID：%s 将停止启动"),msgUserlist[id].roleId);
		MessageBox(0,msg,_T("启动提示："),MB_OK);
		goto exitPaoShang;
	}
	int test_bind = BindGameWindow(id);
	TRACE("错误码：%d\n",dm[id].GetLastError());
	TRACE("测试绑定状态%d\n",test_bind);
	isMaze[id] = FALSE;
	//Delay(-1,2000,2500);
	if(AccountInfo[id].ret == TABUSER){
		//切换帐号
		//AccountInfo[id].sum = 0;
		while(oneRun){
			dm[id].Delay(50);
		}
		oneRun = true;
		::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_WAIT_USERLIST_GET_NEW_USERNAME,(WPARAM)&AccountInfo[id],id);
		oneRun = false;
		if(AutoExit(id,AccountInfo[id].username) == QUIT){
			setRunState(id,_T("已跑完"));
			return;
		}
		if(AutoLogin(id,AccountInfo[id].password) == QUIT){
			setRunState(id,_T("已跑完"));
			return;
		}
	}
	//dm[id].UseDict(DICT_DEFAULT);
	setRunState(id,_T("跑商中"));
	TRACE("当前线路：%d\n",ConfigData[id].PaoshangLine);
	switch(ConfigData[id].PaoshangLine)
	{
	case 1:
		InLineHzToJl(id);
		break;
	case 2:
		OutLineHzToAglToKl(id);
		break;
	default:
		setRunState(id,_T("启动失败"));
		msg.Format(_T("无法识别的跑商线路,角色ID：%s将停止启动"),msgUserlist[id].roleId);
		MessageBox(0,msg,_T("启动提示："),MB_OK);
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
/* 小退游戏（切换角色）                                                                     */
/************************************************************************/
BOOL MinExit(int id){
	VARIANT intX,intY,x,y,x2,y2;
	long t1,t2;
	int index_dict = dm[id].GetNowDict();
	dm[id].UseDict(DICT_DEFAULT);
	setRunState(id,_T("小退中"));
	//修改文件目录program\setting.ini  [Login]->[Save]->1 (开启记住帐号)
	//修改文件目录program\setting.ini  [Login]->[LoginUser]->value (默认选中)
reExit:
	t1 = dm[id].GetTime();
	MoveTo(id,784,510);
	dm[id].Delays(100,200);
	LeftClick(id);
ExitOk:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("切换角色"),_T("fff7be-101010"),1.0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
		//切换角色
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
	//退出后继续监视是否已退出
isExit:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("你确定要重新选择角色吗?"),_T("ffffff-101010"),1.0,&intX,&intY);
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
		//确认角色已载入成功
		TRACE("加载角色\n");
		dm[id].Delays(3000,4000);
		dm[id].FindStr(0,0,800,600,_T("正在载入角色中"),_T("ffffff-101010"),1.0,&x2,&y2);
		if (x2.intVal>=0 && x2.intVal>=0)
		{
			TRACE("重新加载角色\n");
			goto reJryx;
		}
		TRACE("加载角色成功\n");
		MoveTo(id,x.intVal+10,y.intVal+5);
		dm[id].Delays(200,300);
		LeftClick(id);
	}else{
		TRACE("待加载角色\n");
		dm[id].Delays(80,120);
		goto reJryx;
	}
isLoad:
	dm[id].FindStr(0,450,185,570,_T("聊天框外置"),_T("a8e0ff-101010"),1.0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		TRACE("进入到游戏\n");
		ExitMsg(id,2);
		dm[id].Delays(500,800);
		MoveTo(id,x.intVal+102,y.intVal-22);
		dm[id].Delays(80,130);
		LeftClick(id);
	}else{
		TRACE("还没有进入到游戏\n");
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
	setRunState(id,_T("跑商中"));
	dm[id].UseDict(index_dict);
	return TRUE;
}
/************************************************************************/
/* 自动退出游戏重新登录                                                                     */
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
	setRunState(id,_T("切换帐号"));
	//修改文件目录program\setting.ini  [Login]->[Save]->1 (开启记住帐号)
	//修改文件目录program\setting.ini  [Login]->[LoginUser]->value (默认选中)
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
	dm[id].FindStr(220,180,535,418,_T("重新登录"),_T("fff7be-101010"),1.0,&intX,&intY);
	if (intX.intVal>=0 && intY.intVal>=0)
	{
		//退出游戏时将帐号写入配置文件默认帐号
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
	//退出后继续监视是否已退出
isExit:
	dm[id].Delays(100,200);
	dm[id].FindStr(220,180,535,418,_T("你确定要重新登录吗?"),_T("ffffff-101010"),1.0,&intX,&intY);
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
/*自动登录(自动上号)
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
		KeyPressChar(id,password);//输入密码
		dm[id].Delays(50,100);
		MoveTo(id,x.intVal+106,y.intVal+102);
		dm[id].Delays(50,100);
		LeftClick(id);//点击确认
		//检测出现的验证码
	}
re_v:
	dm[id].Delays(100,230);
	dm[id].FindStr(0,0,800,600,_T("请输入图片中的字符"),_T("ffffff-101010"),1.0,&x,&y);
	if (x.intVal>=0 && x.intVal>=0)
	{
		CString MeDir = dm[id].GetDir(4);
		MeDir.AppendFormat(_T("\\pic_v\\v_%d.jpg"),id);
		LPSTR filename = CStringToLpstr(MeDir);
		dm[id].CaptureJpg(x.intVal,y.intVal,x.intVal+267,y.intVal+98,MeDir,80);
		dm[id].Delays(200,300);
		PublicAnswer(id,filename,x.intVal+66,y.intVal+115);
		dm[id].DeleteFile(MeDir);
		//等待1秒后检查是否输入错误
		dm[id].Delays(1000,1300);
		dm[id].FindStr(0,0,800,600,_T("验证码输入错误"),_T("ffffff-101010"),1.0,&x2,&y2);
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
	//检测验证码
reJryx:
	TRACE("准备进入游戏\n");
	bmpDir.Format(_T("%s%s"),pathConfig,_T("jryx.bmp"));
	dm[id].FindPic(0,0,800,600,bmpDir,_T("101010"),0.95,0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		//确认角色已载入成功
		TRACE("加载角色\n");
		dm[id].Delays(3000,4000);
		dm[id].FindStr(0,0,800,600,_T("正在载入角色中"),_T("ffffff-101010"),1.0,&x2,&y2);
		if (x2.intVal>=0 && x2.intVal>=0)
		{
			TRACE("重新加载角色\n");
			goto reJryx;
		}
		TRACE("加载角色成功\n");
		MoveTo(id,x.intVal+10,y.intVal+5);
		dm[id].Delays(200,300);
		LeftClick(id);
	}else{
		TRACE("待加载角色\n");
		dm[id].Delays(80,120);
		goto reJryx;
	}
isLoad:
	dm[id].FindStr(0,450,185,570,_T("聊天框外置"),_T("a8e0ff-101010"),1.0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		TRACE("进入游戏了\n");
		ExitMsg(id,2);
		dm[id].Delays(500,800);
		MoveTo(id,x.intVal+102,y.intVal-22);
		dm[id].Delays(80,130);
		LeftClick(id);
	}else{
		TRACE("还没有进入游戏\n");
		dm[id].Delays(300,500);
		goto isLoad;
	}
	AccountInfo[id].roleId = GetRoleId(id);
	//更新数据
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
	setRunState(id,_T("跑商中"));
	dm[id].UseDict(index_dict);
	return TRUE;
}

/************************************************************************/
/*获取指定线程当前地图名称
/************************************************************************/
CString CurrentMap(int id){
	return _T('');
}

//1,or end将角色移动到帮会NPC
STATE MoveGhNpc(int id){
	VARIANT intX,intY;
	dm[id].UseDict(DEFAULT_DICT);
	if(Delay(id,60,120)==FALSE)return MAZE;
	//杭州回帮过程开始
	int dm_ret = dm[id].FindStr(645,0,800,34,_T("杭州|金陵|阿格拉|昆仑"),_T("66ff00-101010|ff2d2d-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0 && dm_ret == 0){
		//人在杭州
		if(HuiBang(id)==MAZE)return MAZE;
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 1)
	{
		//人在金陵,先回杭州
		if(AutoPathfindingToShangRen(id,_T("cqf"),_T("禇全福"))==MAZE)return MAZE;
tabMapBtn:
        dm[id].UseDict(DEFAULT_DICT);
		if(Delay(id,100,180)==FALSE)return MAZE;
		dm[id].UseDict(DICT_NPCMSG);
		dm[id].FindStr(220,80,560,530,_T("前去杭州"),_T("ffffff-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal+10,intY.intVal+5);
			if(Delay(id,60,120)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,120)==FALSE)return MAZE;
		}else{
			TRACE("查找NPC对话框\n");

			goto tabMapBtn;
		}
		dm[id].UseDict(DICT_DEFAULT);
tabMap:
		if(Delay(id,60,120)==FALSE)return MAZE;;
		dm[id].FindStr(645,0,800,34,_T("杭州"),_T("66ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			ExitMsg(id,4);
			Verifiy(id);
			//goto nextMap;
		}else{
			goto tabMap;
		}
		if(HuiBang(id)==MAZE)return MAZE;
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 2){
		//人在阿格拉
		if(GoNextMap(id,_T("布鲁沃"),_T("blw"),_T("上船前去杭州"),_T("杭州"))==MAZE)return MAZE;
		if(HuiBang(id)==MAZE)return MAZE;
	}else if (intX.intVal>=0 && intY.intVal>=0 && dm_ret == 3){
		//人在昆仑
		 if(GoNextMap(id,_T("邓辊"),_T("dk"),_T("加入阿格拉商队"),_T("阿格拉"))==MAZE)return MAZE;
		 if(GoNextMap(id,_T("布鲁沃"),_T("blw"),_T("上船前去杭州"),_T("杭州"))==MAZE)return MAZE;
		 if(HuiBang(id)==MAZE)return MAZE;
	}
	return TRUE;
}


STATE HuiBang(int id){
	TRACE("\n回帮调用\n");
	VARIANT intX,intY;
bhjbxx:
	if(Delay(id,60,120)==FALSE)return MAZE;
	MoveTo(id,785,481);
	if(Delay(id,60,120)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,600,800)==FALSE)return MAZE;
	dm[id].FindStr(0,0,400,285,_T("基本信息"),_T("c9d2fd-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		if(Delay(id,60,120)==FALSE)return MAZE;
		MoveTo(id,intX.intVal+35,intY.intVal+10);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,600,800)==FALSE)return MAZE;
		dm[id].FindStr(430,460,800,600,_T("回帮"),_T("fff7be-101010"),1.0,&intX,&intY);
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
bhNpc://找询帮会传送使
		dm[id].FindStr(645,0,800,34,_T("276,136"),_T("ff2d2d-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			if(Delay(id,60,120)==FALSE)return MAZE;
			ExitMsg(id,3);
			if(Delay(id,400,600)==FALSE)return MAZE;;
			//进入帮会后移动一点再找帮会传送使
			AutoPathfindingToShangRen(id,_T("fyz"),_T("冯云子"));
qjk:
				dm[id].FindStr(220,80,560,530,_T("去金库"),_T("ffffff-101010"),1.0,&intX,&intY);
				if(intX.intVal>=0 && intY.intVal>=0){
					//找到帮会传送NPC
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

//2-1,找跑商NPC并接取任务
BOOL paoshangTakeTask(int id){
	VARIANT intX,intY;
	int i = 0;
	MoveTo(id,780,130);
	Delay(-1,70,110);
	LeftClick(id);
	Delay(-1,400,600);;
	//找NPC并找开对话框300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("搜索"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		Delay(-1,70,110);
		LeftClick(id);
		Delay(-1,70,110);
		KeyPressChar(id,_T("ley"));
		i = 0;
souso:
		Delay(id,400,600);
		dm[id].FindStr(220,80,560,530,_T("李遏云"),_T("00ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal,intY.intVal+8);
			Delay(-1,70,110);
			LeftClick(id);
			Delay(-1,70,110);
		}else{
			i++;
			//没有找到帮会的指定NPC，停止功能
			if(i>=10){
				//MessageBox(0,_T("寻找跑商NPC失败！软件停止此角色跑商"),_T("消息提示："),MB_OK);
				return FALSE;
			}else{
				goto souso;
			}
		}
	}
sr:
	Delay(-1,60,80);
	//申请成为商人
	dm[id].FindStr(190,215,600,540,_T("申请成为商人"),_T("ffffff-101010"),1.0,&intX,&intY);
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
	//接跑商任务
	dm[id].FindStr(190,215,600,540,_T("商会任务之跑商"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		Delay(-1,70,110);
		LeftClick(id);
		Delay(-1,1100,1600);
	}else{
		goto jrw;
	}
	dm[id].FindStr(0,0,800,600,_T("接受"),_T("fff7be-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+10,intY.intVal+5);
		Delay(-1,70,110);
		LeftClick(id);
		Delay(-1,70,110);
		Verifiy(id);
	}
	return TRUE;
}


//3,领取任务后移动出帮至杭州商会NPC处
STATE paoshangMove(int id){
	VARIANT intX,intY;
	long t1,t2;
reMove:
	ExitMsg(id,5);
	//清空全部窗口后打开任务窗口
	MoveTo(id,785,307);
	if(Delay(id,60,120)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,60,120)==FALSE)return MAZE;
	t1 = dm[id].GetTime();
	dm[id].UseDict(DICT_NPCMSG);
rwhz:
	if(Delay(id,60,80)==FALSE)return MAZE;
	dm[id].FindStr(0,0,800,600,_T("杭州"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+90,intY.intVal+5);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
	}else{
		//10次ESC键位取消全部窗口
		t2 = dm[id].GetTime();
		if ((t2-t1)/1000>20)
		{
			ExitMsg(id,5);
		    //清空全部窗口后打开任务窗口
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
	int x,y;//相对坐标
	if(Delay(id,120,140)==FALSE)return MAZE;
	dm[id].FindStr(0,0,800,600,_T("跑商货物"),_T("a8e0ff-101010"),1.0,&intX,&intY);
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
			//购买make+1格子中的商品
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
			//判断判断NPC有没有出现在眼前
			bmpDir.Format(_T("%s%s"),pathConfig,_T("psmp.bmp"));
			dm[id].FindPic(200,402,332,465,bmpDir,_T("101010"),0.95,0,&intX,&intY);
			if(intX.intVal>=0 && intY.intVal>=0){
				TRACE("满票，可以交任务了\n");
				return FALSE;
			}
			//判断判断NPC有没有出现在眼前
			bmpDir.Format(_T("%s%s"),pathConfig,_T("spbj.bmp"));
			dm[id].FindPic(20,402,332,465,bmpDir,_T("101010"),0.95,0,&intX,&intY);
			if(intX.intVal>=0 && intY.intVal>=0){
				TRACE("商品变价了，刷新一下\n");
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

//出售货物
STATE paoshangCSSP(int id){
	VARIANT intX,intY;
cssp:
	int x,y;//相对坐标
	if(Delay(id,120,140)==FALSE)return MAZE;
	dm[id].FindStr(0,0,800,600,_T("道具行囊"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		x = intX.intVal;
		y = intY.intVal;
		for (int i=0;i<3;i++)
		{
			//购买make+1格子中的商品
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
			//判断判断NPC有没有出现在眼前
			bmpDir.Format(_T("%s%s"),pathConfig,_T("spbj.bmp"));
			dm[id].FindPic(20,402,332,465,bmpDir,_T("101010"),0.95,0,&intX,&intY);
			if(intX.intVal>=0 && intY.intVal>=0){
				TRACE("商品变价了，刷新一下\n");
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

//前往下一个地图（传送点方式）
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
		TRACE("查找NPC对话框");
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

//前往下一个地图（NPC方式）
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
		TRACE("查找NPC对话框");
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


//自动寻路到商人/货商
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
	//找NPC并找开对话框300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("搜索"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,20,30)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,20,30)==FALSE)return MAZE;
		KeyPressChar(id,keyStr);
		if(Delay(id,400,600)==FALSE)return MAZE;;
		//查找金库选项
		dm[id].FindStr(220,80,560,530,npcName,_T("00ff00-101010|ff9900-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			//找到帮会传送NPC
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
		Verifiy(id);//扫描验证码
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

//点击打开商会货物窗口
STATE OpenSHSD(int id){
	VARIANT intX,intY;
	long start_time,end_time,ms;
	start_time = dm[id].GetTime();
dkshsd://打开商会商店
	msgExit(id);
	if(Delay(id,60,120)==FALSE)return MAZE;
	dm[id].FindStr(190,215,600,540,_T("打开商会商店"),_T("ffffff-101010"),1.0,&intX,&intY);
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
	Verifiy(id);//扫描验证码
	if(Delay(id,60,120)==FALSE)return MAZE;
	return TRUE;
}

//任务完成，送还任务
STATE paoshangGiveTask(int id){
	VARIANT intX,intY;
	int i = 0;
	int dm_ret;
reopen:
	TRACE("接还任务\n");
	MoveTo(id,780,130);
	dm[id].Delay(100);
	dm[id].GetCursorPos(&intX,&intY);
	TRACE("坐标：%d,%d\n",intX.intVal,intY.intVal);
	dm[id].CaptureJpg(intX.intVal-15,intY.intVal-15,intX.intVal+15,intY.intVal+15,_T("C:\\pos.jpg"),80);
	if(Delay(id,80,150)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,400,500)==FALSE)return MAZE;
	//找NPC并找开对话框300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("搜索"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	dm[id].CaptureJpg(140,400,640,540,_T("C:\\tttt.jpg"),80);
	TRACE("坐标：%d，%d\n",intX.intVal,intY.intVal);
	if(intX.intVal>=0 && intY.intVal>=0){
		TRACE("找到\n");
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,60,120)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,120)==FALSE)return MAZE;
		KeyPressChar(id,_T("ley"));
		i = 0;
souso:
		if(Delay(id,400,500)==FALSE)return MAZE;
		dm[id].FindStr(220,80,560,530,_T("李遏云"),_T("00ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal,intY.intVal+8);
			if(Delay(id,60,120)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,120)==FALSE)return MAZE;
		}else{
			i++;
			//没有找到帮会的指定NPC，停止功能
			if(i>=10){
				MessageBox(0,_T("寻找李遏云失败！软件停止此角色跑商"),_T("消息提示："),MB_OK);
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
	//申请成为商人
	dm[id].FindStr(190,215,600,540,_T("申请成为商人"),_T("ffffff-101010"),1.0,&intX,&intY);
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

//领取跑商任务
STATE TakeTask(int id){
	VARIANT intX,intY;
	int dm_ret,i=0;
	if(Delay(id,200,400)==FALSE)return MAZE;
jrw:
	msgExit(id);
	if(Delay(id,60,80)==FALSE)return MAZE;
	//接跑商任务
	dm[id].FindStr(190,215,600,540,_T("商会任务之跑商"),_T("ffffff-101010"),1.0,&intX,&intY);
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
	dm_ret = dm[id].FindStr(0,0,800,600,_T("接受|完成"),_T("fff7be-101010"),1.0,&intX,&intY);
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
		//重置进入迷宫状态
		isMaze[id] = FALSE;
		MoveTo(id,intX.intVal+10,intY.intVal+5);
		if(Delay(id,60,80)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,80)==FALSE)return MAZE;
		ExitMsg(id,4);
		timeBuyYao(id);
		AccountInfo[id].sum++;
		CString djp;
		djp.Format(_T("刚交的是第：%d票"),AccountInfo[id].sum);
		Log(id,djp);
		::SendMessage(
			AfxGetApp()->m_pMainWnd->GetSafeHwnd()
			,WM_WAIT_USERLIST_SETCELL_BY_USERNAME
			,(WPARAM)&AccountInfo[id]
		    ,id);
		//这里添加跑完一个号后换号的过程
		if (AccountInfo[id].sum>=ConfigData[id].sum)
		{
			CancelShangRen(id);
			Log(id,_T("满十票"));
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
				setRunState(id,_T("已跑完"));
				return QUIT;
			}
			if(isUserEnd==false){
				if(AutoLogin(id,AccountInfo[id].password) == QUIT){
					setRunState(id,_T("已跑完"));
					return QUIT;
				}
			}else{
				setRunState(id,_T("无号可换"));
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
	AutoPathfindingToShangRen(id,_T("ley"),_T("李遏云"));
	//取消商位  我暂时不想做商人了  你的商人职位被取消了(确定：100,70)（区域：180，220|600,380）
re:
	msgExit(id);
	dm_ret = dm[id].FindStr(190,215,600,540,_T("我暂时不想做商人了"),_T("ffffff-101010"),1.0,&intX,&intY);
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
	dm_ret = dm[id].FindStr(180,220,600,380,_T("你的商人职位被取消了"),_T("ffffff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+100,intY.intVal+70);
		dm[id].Delays(50,80);
		LeftClick(id);
		dm[id].Delays(50,80);
	}
	ExitMsg(id,4);
}

//今取赏金任务
STATE TakeReward(int id){
	VARIANT intX,intY,x,y;
	int dm_ret,i=0;
	if(Delay(id,60,80)==FALSE)return MAZE;;
	MoveTo(id,780,130);
	if(Delay(id,60,80)==FALSE)return MAZE;
	LeftClick(id);
	if(Delay(id,400,600)==FALSE)return MAZE;;
	//找NPC并找开对话框300,160,615,415
	dm[id].FindStr(140,400,640,540,_T("搜索"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MoveTo(id,intX.intVal+65,intY.intVal+5);
		if(Delay(id,60,80)==FALSE)return MAZE;
		LeftClick(id);
		if(Delay(id,60,80)==FALSE)return MAZE;
		KeyPressChar(id,_T("cmy"));
		i = 0;
souso:
		if(Delay(id,300,400)==FALSE)return MAZE;
		dm[id].FindStr(220,80,560,530,_T("陈明允"),_T("00ff00-101010"),1.0,&intX,&intY);
		if(intX.intVal>=0 && intY.intVal>=0){
			MoveTo(id,intX.intVal,intY.intVal+8);
			if(Delay(id,60,80)==FALSE)return MAZE;
			LeftClick(id);
			if(Delay(id,60,80)==FALSE)return MAZE;
			i=0;
		}else{
			i++;
			//没有找到帮会的指定NPC，停止功能
			if(i>=10){
				MessageBox(0,_T("寻找赏金NPC失败！软件停止此角色跑商"),_T("消息提示："),MB_OK);
				return FALSE;
			}else{
				goto souso;
			}
		}
	}
npcmsg:
	msgExit(id);
	//打开接赏金界面
	dm[id].FindStr(190,215,600,540,_T("我来找点事情做"),_T("ffffff-101010"),1.0,&intX,&intY);
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
fbr://找发布人
	dm[id].FindStr(210,60,650,800,_T("发布人"),_T("a8e0ff-101010"),1.0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		dm[id].UseDict(DICT_REWARD);
		dm[id].FindStr(intX.intVal+120,intY.intVal+30,intX.intVal+208,intY.intVal+48,_T("/"),_T("f6f6ce-101010"),1.0,&x,&y);
		if(x.intVal>=0 && y.intVal>=0){//有赏金任务
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

				CString str = dm[id].Ocr(intX.intVal+120,intY.intVal+30+y_pos,intX.intVal+208,intY.intVal+48+y_pos,_T("f6f6ce-101010"),1.0);//获取坐标字符串，有可能会出现多个逗号","
				Log(id,str);
				lvXY rewa= getXY(str,'/');
				if(rewa.x<rewa.y){
					Log(id,_T("领取赏金"));
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
				//证明没有赏金任务
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
	dm[id].FindStr(0,0,800,600,_T("领取任务"),_T("f6f6ce-101010"),1.0,&intX,&intY);
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

void MazeMove(int id,int pos,bool isFree);//向指定方向移动角色
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


//出迷宫（NEW）
BOOL Maze(int id){
	if(isMaze[id] == TRUE)return TRUE;
	VARIANT x,y,x1,y2;
	long t1,t2;
	//判断是否处于迷宫中
	dm[id].FindStr(645,0,800,34,_T("跑商迷宫"),_T("ff2d2d-101010"),1.0,&x,&y);
	if (x.intVal>=0 && y.intVal>=0)
	{
		setRunState(id,_T("进入迷宫"));
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

	int Position = UP;//当前要走的方向
	int isOK = -1;//最后一次正确方向
	int LastPos = -1;//上次走的方向
	lvXY currentXY;
	BOOL MoveResult;
Loop:
	t2 = dm[id].GetTime();
	if((t2-t1)/1000>=240){
		msgExit(id);
		setRunState(id,_T("超时小退"));
		MinExit(id);
		if (is_windows[id]==true)oneRun=false;
		return FALSE;
	}
	setRunState(id,_T("找出口"));
	currentXY = MazePos(id);
	switch(Position)
	{
	case UP:
		MazeMove(id,UP);//开始移动
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = UP;
			Position = isLastPos(UP,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(UP,isOK);
			if (Position == 100)
			{
				//可以退出游戏重新上号了
				MinExit(id);
				return FALSE;
			}
		}else if (MoveResult == MAZE)
		{
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,UP,true);//停止移动
		break;
	case DOWN:
		MazeMove(id,DOWN);//开始移动
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = DOWN;
			Position = isLastPos(DOWN,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(DOWN,isOK);
			if (Position == 100)
			{
				//可以退出游戏重新上号了
				MinExit(id);
				return FALSE;
			}
		}else if(MoveResult == MAZE){
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,DOWN,true);//停止移动
		break;
	case LEFT:
		MazeMove(id,LEFT);//开始移动
		MoveResult = MazePosition(id,currentXY);
		if(MoveResult == TRUE){
			isOK = LEFT;
			Position = isLastPos(LEFT,isOK);
		}else if(MoveResult == FALSE){
			Position = isLastPos(LEFT,isOK);
			if (Position == 100)
			{
				//可以退出游戏重新上号了
				MinExit(id);
				return FALSE;
			}
		}else if(MoveResult == MAZE){
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,LEFT,true);//停止移动
		break;
	case RIGHT:
		MazeMove(id,RIGHT);//开始移动
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
				//可以退出游戏重新上号了
			}
		}else if(MoveResult == MAZE){
			if (is_windows[id]==true)oneRun=false;
			return FALSE;
		}
		MazeMove(id,RIGHT,true);//停止移动
		break;
	}
	goto Loop;
}


BOOL MazePosition(int id,lvXY inXY){
	VARIANT intX,intY,x,y;
	lvXY currentXY,lastXY,moveXY;
	bool lock = false;
	int i = 0;
	TRACE("定位坐标：%d,%d\n",inXY.x,inXY.y);
getpos:
	msgExit(id);
	bmpDir.Format(_T("%s%s"),pathConfig,_T("mgnpc.bmp"));
	dm[id].FindPic(242,84,570,400,bmpDir,_T("101010"),1.0,0,&intX,&intY);
	if(intX.intVal>=0 && intY.intVal>=0){
		MazeMove(id,UP,true);//停止移动
		MazeMove(id,DOWN,true);//停止移动
		dm[id].UseDict(DEFAULT_DICT);
		TRACE("找到迷宫离开NPC");
		MoveTo(id,intX.intVal+30,intY.intVal+30,false);
		dm[id].Delays(300,500);
		LeftClick(id,false);
		dm[id].Delays(1500,2000);
		msgExit(id);
		dm[id].FindStr(0,0,800,600,_T("离开迷宫"),_T("ffffff-101010"),1.0,&x,&y);
		if (x.intVal>=0 && y.intVal>=0)
		{
			setRunState(id,_T("离开迷宫"));
			MoveTo(id,x.intVal+10,y.intVal+5,false);
			dm[id].Delays(50,80);
			LeftClick(id,false);
waitOutMaze:
			dm[id].Delays(50,80);
			dm[id].FindStr(645,0,800,34,_T("杭州|金陵|昆仑|阿格拉|台州海岸|仙山"),_T("66ff00-101010|ff2d2d-101010"),1.0,&intX,&intY);
			if (intX.intVal>=0 && intY.intVal>=0)
			{
				setRunState(id,_T("跑商中"));
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
	
	if(lock == false){//将当前坐标定位
		lastXY = inXY;
		lock = true;
		currentXY = inXY;
	}
	moveXY = currentXY;
	TRACE("移动前坐标：%d,%d\n",moveXY.x,moveXY.y);
	currentXY = MazePos(id);//获取当前坐标
	TRACE("当前坐标：%d,%d\n",currentXY.x,currentXY.y);
	if(compare(currentXY,moveXY) == STOP_MOVE){//没有移动了
		//当前坐标和上次定位坐标判断，是否正确路线
		if(abs((currentXY.x-lastXY.x))>5 || abs((currentXY.y-lastXY.y))>5){
			TRACE("没有移动了，并坐标超过5\n");
			//正确路线
			i++;
			if(i>12){
				TRACE("方向正确\n");
				return TRUE;
			}
		}else{
			//错误路线
			TRACE("没有移动了，并坐标没超过5\n");
			i++;
			if(i>12){
				TRACE("方向错误\n");
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


//使用当前走向和最后一次正确走向判断下次应该走哪个方向
int isLastPos(int currentPos,int OkPos)
{
	int pos;//返回下次走的方向
	TRACE("判断方向前正常方向：%d\n",OkPos);
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
			TRACE("找不到方向，可以退出游戏了！");
		}
		break;
	}
	TRACE("判断方向后准备移动的方向：%d\n",pos);
	return pos;
}

//验证码检查功能
//此功能出现场景出下：打开
void Verifiy(int id){
	VARIANT intX,intY;
	long start_time,end_time;
	start_time = dm[id].GetTime();
	//方式一的特征码：1b93f82a；方式二：6561614a
	int topX=320,topY=250,BottomX=590,bottomY=430;//扫描验证码的区域
	//检测验证码是否出现的几个时机：1打开商店时，2移动时，暂进先加入这两个时机
	int setpX=35,setpY=15,secondX,secondY,x,y;
	CString mouse_tz;
	long double tempX,tempY;
	tempX = (BottomX-topX)/setpX;
	tempY = (bottomY-topY)/setpY;
	secondX = ceil(tempX);
	secondY = ceil(tempY);
	dm[id].EnableRealMouse(0,5,35);
	//开始扫描
	
	for (int i=0;i<=secondY;i++)
	{
		y = i*setpY+topY;
		for(int j=0;j<=secondX;j++){
			x = j*setpX+topX;
			MoveTo(id,x,y);
			dm[id].Delay(5);
			mouse_tz = dm[id].GetCursorShape();
			if(mouse_tz == _T("1b93f82a")){
				setRunState(id,_T("验证中"));
				Log(id,_T("找到验证码"));
				TRACE("找到验证码！\n");
				CString filename = dm[id].GetDir(4);
				filename.AppendFormat(_T("\\pic_v\\v_%d.jpg"),id);
				LPSTR lpFilename = CStringToLpstr(filename);
				TRACE(lpFilename);
				//开始执行答题动作
				dm[id].GetCursorPos(&intX,&intY);
				//切图保存
				int dm_ret = dm[id].CaptureJpg(intX.intVal-147,intY.intVal-126,intX.intVal+100,intY.intVal,filename,80);
				TRACE("%d",dm_ret);
				if(dm_ret == 1){
					Log(id,_T("切图成功"));
				}else{
					Log(id,_T("切图失败"));
				}
				dm[id].Delay(666);
				Log(id,_T("准备发送验证数据"));
				PublicAnswer(id,lpFilename,intX.intVal+6,intY.intVal+3);
				Log(id,_T("完成答题操作"));
				dm[id].DeleteFile(filename);
				goto result;
			}
		}
	}
result:
	dm[id].EnableRealMouse(2,60,120);//鼠标动作模拟真实操作,带移动轨迹,以及点击延时随机. 
	end_time = dm[id].GetTime();
	setRunState(id,_T("跑商中"));
	TRACE("扫描验证码用时：%d ms\n",end_time-start_time);
}

void Haoi_Answer(int id,LPSTR filename,int x,int y){
	
	char Reply[512];//储存答案变量
	int i;
	char TID[512];//储存题目编号变量
	CString log;
rePost:
	SendFile("jqrfhnwfg|DB47D229725C456B","3004",filename,55,0,"",TID);
	if (IsRight(TID)){
		TRACE("获取答案中1\n");
		Log(id,_T("好爱：发题成功，等待答案"));
		//Go on GetAnswer 继续
	}else{
		
		log.Format(_T("发题失败，一秒后重新发送，失败返回值:%s"),TID);
		Log(id,log);
		dm[id].Delay(1000);
		goto rePost;
	}
	CString strTid(TID);
	TRACE("获取答案:%S\n",strTid);

	for(i=1;i<65;i++){	

		Sleep(1000);//延迟1秒以上，必须延迟哦
		TRACE("获取答案中3\n");
		GetAnswer(TID,Reply);//利用流水编号 获取答案
		CString strAns(Reply);
		log = _T("");
		log.Format(_T("获取答案，等待答案，值：%s"),strAns);
		Log(id,log);
		TRACE("获取答案:%S\n",strAns);
		if (!strAns.IsEmpty())
		{
			goto isok;
		}
		
	}
	isok:
	if (IsRight(Reply)){
		TRACE("有答案了");
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
		log.Format(_T("得到答案，答案：%s"),answer);
		Log(id,log);
		KeyPressChar(id,answer);
		TRACE("答案：%S\n",answer);
		dm[id].Delays(100,180);
		dm[id].KeyPress(13);
	}else{
		CString answer(Reply);
		log = _T("");
		log.Format(_T("取答案错误，返回：%s"),answer);
		Log(id,log);;
		TRACE("不是正确答案");
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
	//MessageBox(0,ans,_T("答案"),MB_OK);
	//return;
	
	int start = ans.Find(_T("<Result>"));
	int end = ans.Find(_T("</Result>"));
	
	if (ret == TRUE)
	{
		log = _T("");
		log.Format(_T("云速：数据错误，返回：%s"),ans);
		Log(id,log);
		return;
	}
	log = _T("");
	log.Format(_T("云速：取到数据，返回：%s"),ans);
	Log(id,log);

	TRACE("有答案了");
	if(dm[id].GetWindowState(hwnd[id],1)==0){
		dm[id].SetWindowState(hwnd[id],1);
	}
	dm[id].Delays(100,150);
	MoveTo(id,x,y);
	dm[id].Delays(100,150);
	LeftClick(id);
	dm[id].Delays(100,150);
	KeyPressChar(id,ans.Mid(start+8,end-9));
	TRACE("答案：%S\n",ans.Mid(start+8,end-9));
	dm[id].Delays(100,180);
	dm[id].KeyPress(13);
}

lvXY MazePos(int id){
	//切换字库
	//Sleep(3000);
	//VARIANT intX,intY;
	long dict_index = dm[id].GetNowDict();
	dm[id].UseDict(DICT_COORDINATE);
	CString str = dm[id].Ocr(650,0,785,26,_T("ff2d2d-101010"),1.0);//获取坐标字符串，有可能会出现多个逗号","
	//dm[id].CaptureJpg(650,0,785,26,_T("c:\\pos.jpg"),80);
	//TRACE("当前使用字库%d\n",dm[id].GetNowDict());
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
/*获取指定线程的当前角色ID
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
	long h = dm[id].FindWindowEx(0,_T(""),_T("新倩女幽魂Online"));
	for (int i=0;i<=opens;i++)
	{
		if (hwnd[i] == h && i!=id && hwnd[i]!=0)
		{
			CloseHandle(Handle[id]);
			isStartThread[id] = FALSE;
			MessageBox(0,_T("启动信息已存在！"),_T("信息提示："),MB_OK);
			return 0;
		}
	}
	return h;
}

//绑定指定游戏窗口
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


//更新待替换列表真实数据
void SetWaitUserAsOne(CString list_filename,CString username,CString password,int sum){
	CTime time;
	time = CTime::GetCurrentTime();//当天日期
	CString section = time.Format("%Y-%m-%d");
	CString sum_s;
	sum_s.Format(_T("%d"),sum);
	m_dm.WriteIni(section,username,password+"|"+sum_s,list_filename);
}

//更新待替换列表表面数据
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
	time = CTime::GetCurrentTime();//当天日期
	CString section = time.Format("%Y-%m-%d %H:%M:%S");
	log.Format(_T("%s   %s\r\n"),section,str);
	CString filename;
	filename.Format(_T("c:\\feidu_log_%d.txt"),id);
	m_dm.WriteFile(filename,log);
}

//判断是否可以编辑待替列表帐号信息
BOOL isEditWaitUserList(){
	for (int i =0;i<=opens;i++)
	{
		if(isStartThread[i] == TRUE){
			MessageBox(0,_T("功能运行时不能对待替换帐号列表进行编辑等相关操作！"),_T("提示信息："),MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}
//初始化全部帐号数据
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
	AutoPathfindingToShangRen(id,_T("ltr"),_T("刘天若"));
qjk:
	dm[id].FindStr(220,80,560,530,_T("帮会药店"),_T("ffffff-101010"),1.0,&x,&y);
	if(x.intVal>=0 && y.intVal>=0){
		//找到帮会传送NPC
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
		int ret = dm[id].FindStr(0,0,800,600,_T("明细版|精简版"),_T("fff7be-080808"),1.0,&x,&y);
		if (x.intVal>=0 && y.intVal>=0 && ret == 0)
		{
			dm[id].MoveTo(0,0);
			dm[id].Delay(200);
			TRACE(L"打开药店窗口\n");
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
		TRACE(_T("查找药品!\n"));
		TRACE(L"药品名：");
		TRACE(lists.GetAt(i));
		TRACE(L"\n");
		dm[id].FindPic(40,100,425,425,lists.GetAt(i),_T("080808"),0.9,0,&x,&y);
		if (x.intVal>=0 && y.intVal>=0)
		{
			TRACE(_T("找到药品!\n"));
			tempX = x.intVal;
			tempY = y.intVal;
			movePosClick(id,tempX,tempY);
reSetSum:
			dm[id].Delay(300);
			//定位购买数量
			dm[id].FindStr(0,0,800,600,_T("购买数量"),_T("a8e0ff-080808"),1.0,&x,&y);
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
	TRACE("购药结束\n");
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


// CfeidusoftDlg 对话框

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

// CfeidusoftDlg 消息处理程序

BOOL CfeidusoftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	//创建状态栏
	m_statusbar.Create(this);
	//创建两个面板//第一个面板是1,第二个面板是2
	UINT nID[] = { 1 };
	m_statusbar.SetIndicators(nID, 1);

	//设置面板1信息 ,第一个面板,编号为1,正常显示,宽度为100
	
	//设置面板2信息,第二个面板,编号为2,自动拉伸显示,刚开始宽度为0

	//设置状态栏位置
	CRect rect;
	GetClientRect(&rect);
	m_statusbar.SetPaneInfo(0, 1, SBPS_NORMAL, rect.Width()-5);
	//rect.bottom - 20是状态栏的高度位置,20是状态的高度
	//m_statusbar.MoveWindow(0,rect.bottom - 21,rect.Width()-20, rect.right,20);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);  
	m_statusbar.SetPaneText(0,_T("友情提示：游戏目录设置错误将导致掉线或切换帐号时无法登录游戏！"));

	//互斥体限制单开
	HANDLE hObject = ::CreateMutex(NULL,FALSE, _T("feidusoft_xlyh_paoshangpqiwenpisLKSJDFPQWNPIWERNAPSIFNQPWR2")); 
	if(GetLastError() == ERROR_ALREADY_EXISTS)    
	{    
		CDialogEx::OnOK();
		return FALSE;    
	}  
	//初始化执行中的listCtrl
	LONG lStyle; 
	lStyle = GetWindowLong(m_userlist.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
	lStyle |= LVS_REPORT; // 设置style 
	SetWindowLong(m_userlist.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	DWORD dwStyle = m_userlist.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_userlist.SetExtendedStyle(dwStyle); // 设置扩展风格
	m_userlist.DeleteAllItems();
	m_userlist.InsertColumn(0,_T("id"),0,25);
	m_userlist.InsertColumn(1,_T("句柄"),0,65);
	m_userlist.InsertColumn(2,_T("角色ID"),0,75);
	m_userlist.InsertColumn(3,_T("地图"),0,100);
	m_userlist.InsertColumn(4,_T("状态"),0,80);

	//初始化帐号池的listCtrl
	lStyle = GetWindowLong(m_WaitUserlit.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位
	lStyle |= LVS_REPORT; // 设置style
	SetWindowLong(m_WaitUserlit.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	dwStyle = m_WaitUserlit.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report风格的listctrl ） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_WaitUserlit.SetExtendedStyle(dwStyle); // 设置扩展风格
	m_WaitUserlit.DeleteAllItems();
	m_WaitUserlit.InsertColumn(0,_T("帐号"),0,140);
	m_WaitUserlit.InsertColumn(1,_T("密码"),0,100);
	m_WaitUserlit.InsertColumn(2,_T("已跑"),0,40);
	m_WaitUserlit.InsertColumn(3,_T("角色ID"),0,75);
	m_WaitUserlit.InsertColumn(4,_T("状态"),0,50);

	pathConfig = CONFIG_PATH;
	configName = _T("config.ini");
	m_MachineCode.SetWindowText(m_dm.GetMachineCode());
	int reg_ret = m_dm.Reg(_T("jqrfhnwfgd1e888a06309a659b85feefa4bc96528"),_T(""));
	if(reg_ret != 1){
		//MessageBox(_T("效验失败！"));
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
			MessageBox((LPCTSTR)LocalAddress,_T("验证失败信息提示：")); 
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
		httpMsg.Format(_T("与服务器通信失败，返回:%d"),v_ret);
		MessageBox(httpMsg,_T("校验提示："));
		CDialogEx::OnOK();
		return FALSE;
	}
	m_dm.SetPath(pathConfig);
	getConfig(_T("INFO"),_T("gamePath"),configName,game_path,_T(""));
	m_GamePathValue.SetWindowText(game_path);
	//初始化跑商类型下拉选择框
	m_psType.AddString(_T("选择跑商类型"));
	m_psType.AddString(_T("赏金任务"));
	m_psType.AddString(_T("跑商任务"));
	CString temp;
	getConfig(_T("PaoShang"),_T("type"),_T("config.ini"),temp);
	m_psType.SetCurSel(CStringToInt(temp));
	TRACE("类型选中%s\n",temp);
	//初始化跑商路线设定
	m_psLine.AddString(_T("选择跑商路线"));
	m_psLine.AddString(_T("内线（杭州-金陵）"));
	m_psLine.AddString(_T("外线（杭州-阿格拉-昆仑）"));
	getConfig(_T("PaoShang"),_T("line"),_T("config.ini"),temp);
	m_psLine.SetCurSel(CStringToInt(temp));
	TRACE("线路选中%s\n",temp);

	//初始化跑商路线设定
	m_ans.AddString(_T("选择答题室"));
	m_ans.AddString(_T("好爱代答"));
	m_ans.AddString(_T("云速打码"));
	getConfig(_T("DM"),_T("line"),_T("config.ini"),temp);
	m_ans.SetCurSel(CStringToInt(temp));
	TRACE("线路选中%s\n",temp);

	//初始化跑商路线设定
	m_paoshangSum.AddString(_T("10票,跑完换号"));
	m_paoshangSum.AddString(_T("11票,跑完换号"));
	m_paoshangSum.AddString(_T("12票,跑完换号"));
	m_paoshangSum.AddString(_T("13票,跑完换号"));
	m_paoshangSum.AddString(_T("14票,跑完换号"));
	m_paoshangSum.AddString(_T("15票,跑完换号"));
	m_paoshangSum.AddString(_T("20票,跑完换号"));
	getConfig(_T("PaoShangSum"),_T("PAOSHUANG"),_T("config.ini"),temp);
	m_paoshangSum.SetCurSel(CStringToInt(temp));
	TRACE("线路选中%s\n",temp);
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
		MessageBox(_T("初始加载失败 -1"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("price.txt"))==0){
		MessageBox(_T("初始加载失败 -2"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("rw_d.txt"))==0){
		MessageBox(_T("初始加载失败 -3"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("coordinate.txt"))==0){
		MessageBox(_T("初始加载失败 -4"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("reward.txt"))==0){
		MessageBox(_T("初始加载失败 -5"));
		goto exitsoft;
	}
	if(m_dm.IsFileExist(_T("default.txt"))==0){
		MessageBox(_T("初始加载失败 -5"));
		goto exitsoft;
	}
	LPTSTR xuhao = new TCHAR[2];
	for (int i=0;i<=opens;i++)
	{
		dm[i].SetShowErrorMsg(0);
		int dm_ret;
		//int dm_ret = dm[i].SetPicPwd(_T("YIbBQOyw$*"));
		//if(dm_ret == 0){
		//	MessageBox(_T("初始加载失败 -12"));
		//	goto exitsoft;
		//}
		dm[i].SetPath(CONFIG_PATH);
		TRACE("设置目录：%d\n",dm[i].SetPath(CONFIG_PATH));
		UpdateConfigData(i);//更新配置信息至内存
		isStartThread[i] = FALSE;//初始化至未启动状态
		isMaze[i] = FALSE;
		wsprintf(xuhao, _T("%d"), i+1);
		m_userlist.SetItemText(m_userlist.InsertItem(i,xuhao),4,_T("未启动"));
		dm_ret = dm[i].SetDict(DEFAULT_DICT,_T("dm_soft.txt"));
		if(dm_ret == 0){
			MessageBox(_T("初始加载失败 -6"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(NPCMSG_DICT,_T("rw_d.txt"));
		if(dm_ret == 0){
			MessageBox(_T("初始加载失败 -7"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(PRICE_DICT,+_T("price.txt"));
		if(dm_ret == 0){
			MessageBox(_T("初始加载失败 -8"));
			goto exitsoft;
		}
		//coordinate
		dm_ret = dm[i].SetDict(COORDINATE_DICT,_T("coordinate.txt"));
		if(dm_ret == 0){
			MessageBox(_T("初始加载失败 -9"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(REWARD_DICT,_T("reward.txt"));
		if(dm_ret == 0){
			MessageBox(_T("初始加载失败 -10"));
			goto exitsoft;
		}
		dm_ret = dm[i].SetDict(YAO_DICT,_T("default.txt"));
		if(dm_ret == 0){
			MessageBox(_T("初始加载失败 -10"));
			goto exitsoft;
		}
		dm[i].EnableRealMouse(2,60,120);//鼠标动作模拟真实操作,带移动轨迹,以及点击延时随机. 
		dm[i].EnableRealKeypad(1);//键盘动作模拟真实操作,点击延时随机.
		
	}
	m_dm.DisablePowerSave();
	m_dm.DisableScreenSave();
	m_GamePathValue.GetWindowText(game_path);
	//加载待替换帐号列表
	GetWaitUserLists(_T("usersource.ini"),_T("userlist.ini"));
	initAccount();//初始化帐号类型数据的值
	
	//注册热键
	RegisterHotKey(m_hWnd,WM_HOTKEY_START_END,0,VK_HOME);  //HOME
	RegisterHotKey(m_hWnd,WM_HOTKEY_START_END,0,VK_END);  //END

	m_hideWaitUserlist.SetWindowTextW(L"<<收起列表");
	isHide = false;
	cwMin = 383;
	ch = 0;
	cw = 0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
exitsoft:
	CDialogEx::OnOK();
	return FALSE;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfeidusoftDlg::OnPaint()
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
	// TODO: 在此添加控件通知处理程序代码
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
		msg.Append(_T("跑商线路：内线（杭州至金陵）"));
	}
	if(ConfigData[0].paoshangType == 1){
		msg.Append(_T("，任务类型：赏金任务！"));
	}else if(ConfigData[0].paoshangType == 2){
		msg.Append(_T("，任务类型：跑商任务！"));
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
	//TRACE("随机值：%d\n",irand1);
	//Delay(-1,800,1600);
	MessageBox(m_dm.GetMachineCodeNoMac());
	MessageBox(m_dm.GetMachineCode());
	return ;
	m_api.SetRoleId(0,_T("弹出窗口0"));
	
	m_api.SetRoleId(1,_T("弹出窗口1"));
	
	m_api.SetRoleId(2,_T("弹出窗口2"));
	m_api.SetRoleId(3,_T("弹出窗口3"));
	m_api.SetRoleId(4,_T("弹出窗口4"));
	MessageBox(m_api.GetRoleId(0));
	MessageBox(m_api.GetRoleId(1));
	MessageBox(m_api.GetRoleId(2));
	MessageBox(m_api.GetRoleId(3));
	MessageBox(m_api.GetRoleId(4));
}


void CfeidusoftDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(_T("确定要退出软件吗？"),_T("软件退出确认："),MB_OKCANCEL) == IDOK)
	{
		//用户确定退出软件后直接结束所有进程并停止所有功能
		for (int i=0;i<=opens;i++)
		{
			isStartThread[i] = FALSE;
			dm[i].UnBindWindow();
		}
	    CDialogEx::OnCancel();
	}
}


// 新增一行数据，最多上限五行，ID只能从0-4
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
	m_userlist.SetItemText(index,4,_T("移动中"));
	delete(str_hwnd);
	delete(str_id);
	return true;
}


void CfeidusoftDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_userlist.GetFirstSelectedItemPosition();
	int nItem;
	if (pos == NULL){ 
		// 没有选中的行
		MessageBox(L"没有选中行",L"信息提示：");
		return ;
	}else 
	{ 
		while (pos) 
		{ 
			nItem = m_userlist.GetNextSelectedItem(pos);//获取选中行的索引
		} 
	}
	/*if(dm[nItem].GetWindowState(hwnd[nItem],0) == 0){
		MessageBox(_T("指定行的游戏窗口已不存在,请用HOME热键指定窗口进行功能启动"),_T("信息提示"));
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
			TRACE("选择步骤索引:%d\n",StartSetp[nItem]);
			//return;
			break;
		case 2:
			csshtatkDlg.startSetpId = nItem;
			csshtatkDlg.DoModal();
			TRACE("选择步骤索引:%d\n",StartSetp[nItem]);
			break;
		default:
			msg.Format(_T("无法识别的跑商线路,角色ID：%s将停止启动"),msgUserlist[nItem].roleId);
			MessageBox(msg,_T("启动提示："),MB_OK);
			return;
		}
		TRACE("启动了线程：%d\n",nItem);
		Handle[nItem] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)paoshangThread,(VOID*)nItem,0,&ThreadID);//创建线程，线程不可中途挂起，只可停止和重新启动
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
		// 没有选中的行
		MessageBox(L"需要选择一条信息才能启动功能！",L"信息提示：");
	    return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_userlist.GetNextSelectedItem(pos);//获取选中行的索引
		} 
	}
	
	DWORD ThreadID;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
				TRACE("选择步骤索引:%d\n",StartSetp[nItem]);
				//return;
				break;
			case 2:
				csshtatkDlg.startSetpId = nItem;
				csshtatkDlg.DoModal();
				TRACE("选择步骤索引:%d\n",StartSetp[nItem]);
				break;
			default:
				msg.Format(_T("无法识别的跑商线路,角色ID：%s将停止启动"),msgUserlist[nItem].roleId);
				MessageBox(msg,_T("启动提示："),MB_OK);
				return;
				break;
			}
			//获取进程帐号数据
			TRACE("启动了线程：%d\n",nItem);
			Handle[nItem] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)paoshangThread,(VOID*)nItem,0,&ThreadID);//创建线程，线程不可中途挂起，只可停止和重新启动
			isStartThread[nItem] = TRUE;
		}else{
			
		}
		//Handle[nItem] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)paoshang,(VOID*)nItem,0,&ThreadID[nItem]);//创建线程，线程不可中途挂起，只可停止和重新启动
		//热键按HOME键后的动作
		//MessageBox(L"HOME");
	}
	else if (nKey2 == VK_END)
	{
		//热键按END后的动作
		//MessageBox(L"End");
		if (isStartThread[nItem] == TRUE)
		{
			MazeMove(nItem,UP,true);//停止移动
			MazeMove(nItem,DOWN,true);//停止移动
			m_dm.KeyUp(17);
			m_userlist.SetItemText(nItem,4,_T("已停止"));
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
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos = m_userlist.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{
		// 没有选中的行
		MessageBox(_T("需要选择一条信息才能启动功能！"),_T("信息提示："));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_userlist.GetNextSelectedItem(pos);//获取选中行的索引
		} 
	}
	if (isStartThread[nItem] == TRUE)
	{
		CString msg;
		msg.Format(_T("确定停止角色ID为：%s 的窗口功能吗？"),m_userlist.GetItemText(nItem,2));
		if (MessageBox(msg,_T("功能停止提示："),MB_OKCANCEL) == IDOK)
		{
			MazeMove(nItem,UP,true);//停止移动
			MazeMove(nItem,DOWN,true);//停止移动
			m_dm.KeyUp(17);
			dm[nItem].UnBindWindow();
			TerminateThread(Handle[nItem],EXIT_FAILURE);
			CloseHandle(Handle[nItem]);
			isStartThread[nItem] = FALSE;
			m_userlist.SetItemText(nItem,4,_T("已停止"));
		}
	}

}



void CfeidusoftDlg::OnBnClickedBtnUserlist()
{
	// TODO: 在此添加控件通知处理程序代码
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
	dlg.title = _T("添加帐号信息：");
	dlg.DoModal();
}


void CfeidusoftDlg::addUser(CString user, CString pwd,CString sum,CString role)
{
	int count = m_WaitUserlit.GetItemCount();
	int nItem = m_WaitUserlit.InsertItem(count,user);
	m_WaitUserlit.SetItemText(nItem,1,pwd);
	m_WaitUserlit.SetItemText(nItem,2,sum);
	m_WaitUserlit.SetItemText(nItem,3,role);
	m_WaitUserlit.SetItemText(nItem,4,_T("待上号"));
	setWaitUserListByOne(user,pwd,sum,role,_T("待上号"));
}



void CfeidusoftDlg::OnBnClickedBtnModfiyUser()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// 没有选中的行
		MessageBox(_T("需要选择一条信息！"),_T("信息提示："));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//获取选中行的索引
		} 
	}
	if(isEditWaitUserList() == FALSE)return;
	CaddUser dlg;
	dlg.nItem = nItem;
	dlg.isModfiy = true;
	dlg.title = _T("修改帐号信息：");
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
		MessageBox(_T("配置文件写入失败"));
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
	int count = m_WaitUserlit.GetItemCount();//获取一共有多少条数据
	if (count == 0)
	{
		//如果没有数据则设置为空后直接返回
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
		//time = CTime::GetCurrentTime()-CTimeSpan( 1, 0, 0, 0 );//当天日期的前一天
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
	time = CTime::GetCurrentTime();//当天日期
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

	//判断当前角色ID对应的帐号信息，如果在待替列表中找不到择不让启动
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
	// TODO: 在此添加控件通知处理程序代码
	CmodfiyHotKey modfiyHotKeyDlg;
	UnregisterHotKey(m_hWnd,WM_HOTKEY_START_END);//注消热键等待重新设置
	modfiyHotKeyDlg.DoModal();//
	
}



// 更新修改的热键


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
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}


void CfeidusoftDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void CfeidusoftDlg::OnBnClickedBtnAllStop()
{
	// TODO: 在此添加控件通知处理程序代码
}




void CfeidusoftDlg::OnBnClickedBtnSaveSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveSetting();
	MessageBox(_T("配置保存成功,已启动的窗口重启生效！"),_T("配置更新提示："),MB_OK);
}

//保存设置
void CfeidusoftDlg::SaveSetting(void)
{
	
	//CString temp;
	//保存跑商类型
	//temp.Format(_T("%d"),);
	setConfig(_T("PaoShang"),_T("type"),IntToCString(m_psType.GetCurSel()),_T("config.ini"));
	setConfig(_T("DM"),_T("line"),IntToCString(m_ans.GetCurSel()),_T("config.ini"));//打码选择
	//设置跑商线路
	//temp.Format(_T("%d"),m_psLine.GetCurSel());
	setConfig(_T("PaoShang"),_T("line"),IntToCString(m_psLine.GetCurSel()),_T("config.ini"));
	setConfig(_T("PaoShangSum"),_T("PAOSHUANG"),IntToCString(m_paoshangSum.GetCurSel()),_T("config.ini"));

	setConfig(_T("PaoShang"),_T("autoHuanhao"),IntToCString(m_autohuanhao.GetCheck()),_T("config.ini"));
	setConfig(_T("PaoShang"),_T("buyYao"),IntToCString(m_buyYao.GetCheck()),_T("config.ini"));
	for (int i=0;i<=opens;i++)
	{
		if(isStartThread[i] == FALSE){//如果线程为启动状态，那么配置不会被更新
			UpdateConfigData(i);
			//重新更新配置信息，重启功能生效
		}
	}
}


// 更新配置信息
void CfeidusoftDlg::UpdateConfigData(int id)
{
	CString temp;
	//保存跑商类型
	getConfig(_T("PaoShang"),_T("type"),_T("config.ini"),temp);
	ConfigData[id].paoshangType = CStringToInt(temp);

	if(ConfigData[id].paoshangType ==1)
		isReward[id] = TRUE;
	else
		isReward[id] = FALSE;

	//保存跑商路线
	getConfig(_T("PaoShang"),_T("line"),_T("config.ini"),temp);
	ConfigData[id].PaoshangLine = CStringToInt(temp);

	getConfig(_T("PaoShang"),_T("buyYao"),_T("config.ini"),temp);
	isBuyYao[id] = CStringToInt(temp);

	getConfig(_T("PaoShang"),_T("autoHuanhao"),_T("config.ini"),temp);
	isAutoHuanhao[id] = CStringToInt(temp);

	//打码线路
	getConfig(_T("DM"),_T("line"),_T("config.ini"),temp);
	ConfigData[id].DM_line = CStringToInt(temp);

	//打码线路
	getConfig(_T("PaoShangSum"),_T("PAOSHUANG"),_T("config.ini"),temp);
	int temp_ps_sum = CStringToInt(temp);
	switch(temp_ps_sum)
	{
	case 0://10票
		ConfigData[id].sum = 10;
		break;
	case 1://11票
		ConfigData[id].sum = 11;
		break;
	case 2://12票
		ConfigData[id].sum = 12;
		break;
	case 3://13票
		ConfigData[id].sum = 13;
		break;
	case 4://14票
		ConfigData[id].sum = 14;
		break;
	case 5://15票
		ConfigData[id].sum = 15;
		break;
	case 6://16票
		ConfigData[id].sum = 20;
		break;
	default:
		ConfigData[id].sum = 0;
		break;//0
	}
	
}


void CfeidusoftDlg::OnBnClickedBtnHideShow()
{
	// TODO: 在此添加控件通知处理程序代码
	if(cw == 0 || ch==0){
		VARIANT w;
		VARIANT h;
		m_dm.GetClientSize((long)m_hWnd,&w,&h);
		cw = w.lVal;
		ch = h.lVal;
	}
	if(isHide){
		m_dm.SetClientSize((long)m_hWnd,cw,ch);
		m_hideWaitUserlist.SetWindowTextW(_T("<<收起列表"));
		isHide = false;
	}else{
		m_dm.SetClientSize((long)m_hWnd,cwMin,ch);
		m_hideWaitUserlist.SetWindowTextW(_T("展开列表>>"));
		isHide = true;
	}
}



// 设置启动进度
int CfeidusoftDlg::SetStartSetp(int id,int setp)
{
	StartSetp[id] = setp;
	return 0;
}


void CfeidusoftDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	unResPakage(_T("Desktop.y"),_T("C:\\Program Files\\feidu\\"),"123456789");
}


// 设置一条待替换帐号
void CfeidusoftDlg::setWaitUserListByOne(CString username, CString password,CString sum,CString role,CString state){
	CTime time;
	time = CTime::GetCurrentTime();//当天日期
	CString section = time.Format("%Y-%m-%d");
	setConfig(section,username,sum+_T(",")+state,_T("userlist.ini"));//保存待替换帐号列表
	setConfig(_T("USERLISTS"),username,password+_T("|")+role,_T("usersource.ini"));//保存帐号源列表_角色帐号为键
}


// 获取待替换帐号列表
void CfeidusoftDlg::GetWaitUserLists(CString section_filename,CString list_filename){
	CTime time;
	CStringArray lists,lists2;
	time = CTime::GetCurrentTime();//当天日期
	CString section = time.Format("%Y-%m-%d");
	//初始化前先将所有不是今天的数据全部删除
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
				state = _T("待上号");
			else
				state = sum_state.GetAt(0);
		}else{
			if(sum_state.GetAt(0).IsEmpty())sum = _T("0");
			if(sum_state.GetAt(0).SpanIncluding(_T("0123456789")) == sum_state.GetAt(0)){
				sum = sum_state.GetAt(0);
			}else
				sum = _T("0");
			state = _T("待上号");
		}
		m_WaitUserlit.SetItemText(nItem,2,sum);
		m_WaitUserlit.SetItemText(nItem,4,state);
		setConfig(section,lists.GetAt(i),sum+_T(",")+state,list_filename);
	}
}



void CfeidusoftDlg::OnBnClickedBtnMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// 没有选中的行
		MessageBox(_T("需要选择一条信息！"),_T("信息提示："));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//获取选中行的索引
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
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// 没有选中的行
		MessageBox(_T("需要选择一条信息！"),_T("信息提示："));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//获取选中行的索引
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
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[1024];    
	BROWSEINFO bi;     
	bi.hwndOwner      = this->GetSafeHwnd();    
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = NULL;     
	bi.lpszTitle      = TEXT("请选择倩女幽魂游戏安装目录");     
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


// 设置单条跑商列表帐号数据
void CfeidusoftDlg::SetWaitUserAsOne(CString list_filename, CString username, CString password, int sum,CString role,CString state)
{
	CTime time;
	time = CTime::GetCurrentTime();//当天日期
	CString section = time.Format("%Y-%m-%d");
	CString sum_s;
	sum_s.Format(_T("%d"),sum);
	setConfig(section,username,sum_s+","+state,list_filename);
}


// 按帐号要变更列表数据,第一个参数是帐号类型当前的帐号数据，第二个操作ID
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
	int count = prog->GetItemCount();//获取所有列表数量
	for (int i=0;i<count;i++)
	{
		if(username == prog->GetItemText(i,0)){
			//是这个帐号，对这一行数据进行操作
			prog->SetItemText(i,2,sum);
			prog->SetItemText(i,3,role);
			setWaitUserListByOne(username,pwd,sum,role,_T("待上号"));//将帐号写入本地信息
			return 0;
		}
	}
	return 0;
}

//给当前线程分配新的帐号,第一个参数是帐号类型换号前的帐号数据，第二个操作ID
LRESULT CfeidusoftDlg::GetWaitUserListNewUsername(WPARAM wp,LPARAM lp){
	CListCtrl *prog = (CListCtrl *)GetDlgItem(IDC_WAIT_USERLIST);
	lvAccount *lvA = (lvAccount*)wp;
	bool isok = false;
	CString username,pwd,sum,role,state;
	username = lvA->username;
	pwd = lvA->password;
	sum = IntToCString(lvA->sum);
	TRACE("换号前一个帐号的跑商次数：%s\n",sum);
	role = lvA->roleId;
	state = lvA->state;
	int count = prog->GetItemCount();//获取所有列表数量
	int id = (int)lp;
	for (int i=0;i<count;i++)
	{
		if(prog->GetItemText(i,0) == username){
			TRACE("将帐号设置为跑完时的数量new：%d\n",lvA->sum);
			prog->SetItemText(i,4,_T("已跑完"));
			SetWaitUserAsOne(_T("userlist.ini"),username,pwd,lvA->sum,role,_T("已跑完"));//将帐号写入本地信息
		}
		if(CStringToInt(prog->GetItemText(i,2))<10 && prog->GetItemText(i,4)==_T("待上号") && isok==false){
			AccountInfo[id].username = prog->GetItemText(i,0);
			AccountInfo[id].password = prog->GetItemText(i,1);
			AccountInfo[id].sum = CStringToInt(prog->GetItemText(i,2));
			prog->SetItemText(i,4,_T("跑商中"));
			SetWaitUserAsOne(_T("userlist.ini"),AccountInfo[id].username,AccountInfo[id].password,AccountInfo[id].sum,AccountInfo[id].roleId,_T("待上号"));//将帐号写入本地信息
			AccountInfo[id].state = _T("跑商中");
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
	// TODO: 在此添加控件通知处理程序代码
	int nItem;
	POSITION pos = m_WaitUserlit.GetFirstSelectedItemPosition(); 
	if (pos == NULL) 
	{	
		// 没有选中的行
		MessageBox(_T("需要选择一条信息！"),_T("信息提示："));
		return ;
	}
	else 
	{ 
		while (pos) 
		{ 
			nItem = m_WaitUserlit.GetNextSelectedItem(pos);//获取选中行的索引
		} 
	}
	if(isEditWaitUserList() == FALSE)return;
	CString msg;
	msg.Format(_T("确定要移除帐号【%s】吗"),m_WaitUserlit.GetItemText(nItem,0));
	if (MessageBox(msg,_T("列队帐号移除提醒："),MB_OKCANCEL) == IDOK){
		m_dm.DeleteIni(_T("USERLISTS"),m_WaitUserlit.GetItemText(nItem,0),_T("usersource.ini"));
		m_WaitUserlit.DeleteItem(nItem);
		MessageBox(_T(""),_T("移除成功"),MB_OK);
	}
}


// 重新排序待替换帐号列表
void CfeidusoftDlg::ReSortWaitUserList(void)
{
	int count = m_WaitUserlit.GetItemCount();
	CString username,pwd,role;
	//清空源帐号数据
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
	lvAccount *lvA = (lvAccount*)wp;//当前帐号信息
	int id = (int)lp;
	bool isok = false;
	CString username,pwd,sum,role,state;
	username = lvA->username;
	pwd = lvA->password;
	sum = IntToCString(lvA->sum);
	role = lvA->roleId;
	state = lvA->state;
	//开始查找列表，并开始匹配

	if(role.IsEmpty()){
		AccountInfo[id].ret = FALSE;
		AccountInfo[id].msg = _T("帐号信息获取失败，请尝试重新启动");
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
			AccountInfo[id].state = _T("跑商中");
			TRACE("当前这个号的跑商次数：%d\n",AccountInfo[id].sum);
			if(AccountInfo[id].sum>=10){
				TRACE("返回并换号\n");
				AccountInfo[id].ret = TABUSER;
				return 0;
			}
			prog->SetItemText(i,4,_T("跑商中"));
			isok = true;
		}
	}
	if (isok == false)
	{
		AccountInfo[id].ret = FALSE;
		AccountInfo[id].msg.Format(_T("帐号列表中没有找到角色ID【%s】对应的帐号信息\n\n注意：如果帐号列表中有对应帐号，请手动填写对应角色ID再尝试重新启动"),role);
	}
	return 0;
}


// 解压资源包到指定位置
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
		msg.Format(_T("资源文件读取失败！%d"),result);
		MessageBox(msg,_T("错误信息："),MB_OK);
		return false;
	}
	
}


void CfeidusoftDlg::OnBnClickedBtnCopyCode()
{
	// TODO: 在此添加控件通知处理程序代码
	CString data;
	m_MachineCode.GetWindowText(data);
	m_dm.SetClipboard(data);
	MessageBox(_T("机器码复制到剪切板"),_T("复制机器码："));
}
