#pragma once


// api
//����
#define MOVEING 1
//�Թ���
#define MAZEIMG 2
//������
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
	//���Է���
	int test(CString str);
	//��ȡ��ɫID
	CString GetRoleId(int id);
	//���ý�ɫID
	void SetRoleId(int id,CString RoleId);
	//��ȡ��ɫ״̬
	CString GetRoleState(int id);
	//���ý�ɫ״̬
	void SetRoleState(int id,int state);
	//��ȡ��ɫ��ǰ��ͼ
	CString GetRoleMap(int id);
	//���ý�ɫ��ǰ��ͼ
	void SetRoleMap(int id,CString curretMap);
private:
	CString roleId[5];//��ɫID
    CString roleState[5];//��ɫ״̬
    CString roleCurretMap[5];//��ɫ��ǰ��ͼ
	//��ȡ״̬���Ӧ״̬
	CString GetStateTitle(int state);
};


