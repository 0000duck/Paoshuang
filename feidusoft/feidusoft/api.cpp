// api.cpp : ʵ���ļ�
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



// ���Է���
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
	    case MOVEING://�ƶ���
			title = _T("��������");
		    break;
	    case MAZEIMG://�Թ���
			title = _T("�����Թ�");
		    break;
	    case CHANGEING://������
			title = _T("���ڻ���");
		    break;
		case STOP:
			title = _T("��ֹͣ");
		default:
			title = _T("");
	}
	return title;
}
