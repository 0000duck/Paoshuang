#pragma once

struct lvUserList{//跑商中的信息列表消息数据类型
	int nItem;
	int nSubItem;
	CString hwnd;
	CString roleId;
	CString currentMap;
	CString state;
	CString resultTitle;
};

struct lvWaitUserList{//等待中的帐号信息列表数据类型
	int nItem;
	int nSubItem;
	CString username;
	CString password;
	int sum;
	CString resultTitle;
};

//坐标类型
struct lvXY{
	int x;//操作坐标X
	int y;//操作坐标Y
	CString sx;//字符串类型X坐标
	CString sy;//字符串类型Y坐标
};

//前往下一个地图的数据类型
struct lvMoveData{
	CString shNpcName;//当前地图的商会NPC名称
	CString shNpcKeyStr;//当前地图商会NPC的名称拼音首字母
	CString npcName;//当前地图前往下一个地图的NPC名称
	CString npcKeyStr;//当前地图前往下一个地图NPC的名称拼音首字母
	CString mapName;//当前地图名
	CString nextMapName;//下个地图名
	CString tabMapMsg;//切换地图换时传送NPC对话框按扭
};

//软件配置信息
struct lvConfig{
	int paoshangType;//跑商类型
	int PaoshangLine;//跑商线路
	int DM_line;//打码工作室
	int sum;//跑完指定票数后换号
};

//帐号类型，用于管理当前启动功能的帐号信息
struct lvAccount{
	int nItem;//待换帐号列表的索引
	CString username;//帐号
	CString password;//密码
	CString roleId;//角色ID
	int sum;//当天跑商次数
	CString state;//进行中状态
	BOOL ret;//特别情况返回值，false时该状态将拒绝启动，true时通过启动
	CString msg;//拒绝启动时的消息
};

struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};

#define DEFAULT_DICT 0//默认字库
#define NPCMSG_DICT 1//专用NPC消息框字库
#define PRICE_DICT 2//专用价格字库
#define COORDINATE_DICT 3//专用坐标字库
#define REWARD_DICT 4//检测赏金任务专用字库
#define YAO_DICT 5

#define DICT_DEFAULT DEFAULT_DICT
#define DICT_NPCMSG NPCMSG_DICT
#define DICT_PRICE PRICE_DICT
#define DICT_COORDINATE COORDINATE_DICT
#define DICT_REWARD REWARD_DICT

//迷宫移动判定常量值
#define BEYOND 0//超出限定
#define BEYOND_NOT 1//未超出限定
#define STOP_MOVE 2//相较上次没有移动

#define UP 0//上
#define DOWN 1//下
#define LEFT 2//左
#define RIGHT 3//右

//定义执行过程中个地步骤的返回值类型
typedef int STATE;
//定义执行过程中的返回值
#define MAZE 2//迷宫出来后
#define QUIT 3//退出程序(退出当前线程)
#define RELOAD 4//重载游戏
#define TABUSER 5//重换帐号

#define HZTOTZHA _T("83,367")//传送到台州海岸停坐标