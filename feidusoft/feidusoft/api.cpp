// api.cpp : 实现文件
//

#include "stdafx.h"
#include "feidusoft.h"
#include "api.h"



// api

IMPLEMENT_DYNAMIC(api, CWnd)

api::api()
{
}

api::~api()
{
}


BEGIN_MESSAGE_MAP(api, CWnd)
END_MESSAGE_MAP()



// 测试方法
int api::test(CString str)
{
	MessageBox(str);
	return 0;
}

CString api::GetRoleId(int id){
	return roleId[id];
}

void api::SetRoleId(int id,CString RoleId){
	roleId[id] = RoleId;
}

CString api::GetRoleState(int id){
	return roleState[id];
}

void api::SetRoleState(int id,int state){
	roleState[id] = GetStateTitle(state);;
}

CString api::GetStateTitle(int state){
	CString title;
	switch(state){
	    case MOVEING://移动中
			title = _T("正在跑商");
		    break;
	    case MAZEIMG://迷宫中
			title = _T("正在迷宫");
		    break;
	    case CHANGEING://换号中
			title = _T("正在换号");
		    break;
		case STOP:
			title = _T("已停止");
		default:
			title = _T("");
	}
	return title;
}
