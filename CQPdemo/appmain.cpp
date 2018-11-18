/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include<string>
#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����

using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;


/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return EVENT_IGNORE;
}
string old = "";
int64_t qq1 = 0;
int64_t qq2 = 0;
int i = 0;

/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	if (fromGroup == 288953819) {
		char ws[65];
		string s = msg;
		if (old != msg&&i<=1) {
			old = msg;
			qq1 = fromQQ;
			qq2 = 0;
			i = 0;
		}
		else if(old==msg) {
			i++;
			if (qq2 == 0) {
				qq2 = fromQQ;
			}
			else {
				qq1 = qq2;
				qq2 = fromQQ;
			}
		}
		else {
//			CQ_sendPrivateMsg(ac, 2101784264, _i64toa(qq1, ws, 10));
//			CQ_addLog(ac, CQLOG_ERROR, "��", _i64toa(qq1, ws, 10));
			CQ_sendGroupMsg(ac, 288953819, "ץ������ ��������");
			switch (rand()%3)
			{
			case 0:
				if (i > 4) {
					CQ_setGroupBan(ac, 288953819, qq1, 300);
				}
				else {
					CQ_setGroupBan(ac, 288953819, qq1, (i + 1) * 60);
				}
				break;
			case 1:
				if (i > 4) {
					CQ_setGroupBan(ac, 288953819, qq2, 300);
				}
				else {
					CQ_setGroupBan(ac, 288953819, qq2, (i + 1) * 60);
				}
				break;
			case 2:
				if (i > 4) {
					CQ_setGroupBan(ac, 288953819, fromQQ, 300);
				}
				else {
					CQ_setGroupBan(ac, 288953819, fromQQ, (i + 1) * 60);
				}
				break;
			}
			qq2 = 0;
			i = 0;
			old = "";
		}
		//string c = "old=" + old + "; qq1=" + _i64toa(qq1, ws, 10) + "; qq2=" + _i64toa(qq2, ws, 10)+"i="+ to_string(i);
		//CQ_sendPrivateMsg(ac, 2101784264,c.c_str());
	}
	return EVENT_IGNORE;
}

