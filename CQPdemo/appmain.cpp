/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include<string>
#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载

using namespace std;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;


/* 
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	return EVENT_IGNORE;
}
string old = "";
int64_t qq1 = 0;
int64_t qq2 = 0;
int i = 0;

/*
* Type=2 群消息
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
//			CQ_addLog(ac, CQLOG_ERROR, "口", _i64toa(qq1, ws, 10));
			CQ_sendGroupMsg(ac, 288953819, "抓到了你 复读姬！");
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

