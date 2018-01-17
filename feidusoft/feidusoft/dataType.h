#pragma once

struct lvUserList{//�����е���Ϣ�б���Ϣ��������
	int nItem;
	int nSubItem;
	CString hwnd;
	CString roleId;
	CString currentMap;
	CString state;
	CString resultTitle;
};

struct lvWaitUserList{//�ȴ��е��ʺ���Ϣ�б���������
	int nItem;
	int nSubItem;
	CString username;
	CString password;
	int sum;
	CString resultTitle;
};

//��������
struct lvXY{
	int x;//��������X
	int y;//��������Y
	CString sx;//�ַ�������X����
	CString sy;//�ַ�������Y����
};

//ǰ����һ����ͼ����������
struct lvMoveData{
	CString shNpcName;//��ǰ��ͼ���̻�NPC����
	CString shNpcKeyStr;//��ǰ��ͼ�̻�NPC������ƴ������ĸ
	CString npcName;//��ǰ��ͼǰ����һ����ͼ��NPC����
	CString npcKeyStr;//��ǰ��ͼǰ����һ����ͼNPC������ƴ������ĸ
	CString mapName;//��ǰ��ͼ��
	CString nextMapName;//�¸���ͼ��
	CString tabMapMsg;//�л���ͼ��ʱ����NPC�Ի���Ť
};

//���������Ϣ
struct lvConfig{
	int paoshangType;//��������
	int PaoshangLine;//������·
	int DM_line;//���빤����
	int sum;//����ָ��Ʊ���󻻺�
};

//�ʺ����ͣ����ڹ���ǰ�������ܵ��ʺ���Ϣ
struct lvAccount{
	int nItem;//�����ʺ��б������
	CString username;//�ʺ�
	CString password;//����
	CString roleId;//��ɫID
	int sum;//�������̴���
	CString state;//������״̬
	BOOL ret;//�ر��������ֵ��falseʱ��״̬���ܾ�������trueʱͨ������
	CString msg;//�ܾ�����ʱ����Ϣ
};

struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};

#define DEFAULT_DICT 0//Ĭ���ֿ�
#define NPCMSG_DICT 1//ר��NPC��Ϣ���ֿ�
#define PRICE_DICT 2//ר�ü۸��ֿ�
#define COORDINATE_DICT 3//ר�������ֿ�
#define REWARD_DICT 4//����ͽ�����ר���ֿ�
#define YAO_DICT 5

#define DICT_DEFAULT DEFAULT_DICT
#define DICT_NPCMSG NPCMSG_DICT
#define DICT_PRICE PRICE_DICT
#define DICT_COORDINATE COORDINATE_DICT
#define DICT_REWARD REWARD_DICT

//�Թ��ƶ��ж�����ֵ
#define BEYOND 0//�����޶�
#define BEYOND_NOT 1//δ�����޶�
#define STOP_MOVE 2//����ϴ�û���ƶ�

#define UP 0//��
#define DOWN 1//��
#define LEFT 2//��
#define RIGHT 3//��

//����ִ�й����и��ز���ķ���ֵ����
typedef int STATE;
//����ִ�й����еķ���ֵ
#define MAZE 2//�Թ�������
#define QUIT 3//�˳�����(�˳���ǰ�߳�)
#define RELOAD 4//������Ϸ
#define TABUSER 5//�ػ��ʺ�

#define HZTOTZHA _T("83,367")//���͵�̨�ݺ���ͣ����