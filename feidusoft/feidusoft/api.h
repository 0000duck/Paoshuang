#pragma once


// api
//正常
#define MOVEING 1
//迷宫中
#define MAZEIMG 2
//换号中
#define CHANGEING  3

#define STOP 4


class api : public CWnd
{
	DECLARE_DYNAMIC(api)

public:
	api();
	virtual ~api();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//测试方法
	int test(CString str);
	//获取角色ID
	CString GetRoleId(int id);
	//设置角色ID
	void SetRoleId(int id,CString RoleId);
	//获取角色状态
	CString GetRoleState(int id);
	//设置角色状态
	void SetRoleState(int id,int state);
	//获取角色当前地图
	CString GetRoleMap(int id);
	//设置角色当前地图
	void SetRoleMap(int id,CString curretMap);
private:
	CString roleId[5];//角色ID
    CString roleState[5];//角色状态
    CString roleCurretMap[5];//角色当前地图
	//获取状态码对应状态
	CString GetStateTitle(int state);
};


