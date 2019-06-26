#include <zinx.h>
#include <ZinxTCP.h>
#include <iostream>
#include "StdInOutChannel.h"
#include "Role.h"
#include "CmdIdentify.h"
#include "CmdMsg.h"
#include "ZinxTimer.h"
/*三层结构重构*/
/*
1.通道类（传输）
2.协议类 (原始数据和用户数据相互转换)
3.角色类（业务）
*/

class timerxiaowang :public TimerOutProc {
	// 通过 TimerOutProc 继承
	virtual void Proc() override
	{
		auto pchannle = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "xiaowang";
		ZinxKernel::Zinx_SendOut(output, *pchannle);
		TimerOutMng::GetInstance().DelTask(this);
	}
	virtual int GetTimeSec() override
	{
		return 3;
	}
};

class timerxiaohong :public TimerOutProc {
	// 通过 TimerOutProc 继承
	virtual void Proc() override
	{
		auto pchannle = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "xiaohong";
		ZinxKernel::Zinx_SendOut(output, *pchannle);
	}
	virtual int GetTimeSec() override
	{
		return 5;
	}
};

class timerxiaohong233 :public TimerOutProc {
	// 通过 TimerOutProc 继承
	virtual void Proc() override
	{
		auto pchannle = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "xiaohong233";
		ZinxKernel::Zinx_SendOut(output, *pchannle);
	}
	virtual int GetTimeSec() override
	{
		return 1000;
	}
};

int main()
{
	/*1- 初始化框架*/

	ZinxKernel::ZinxKernelInit();
	TimerOutMng::GetInstance().AddTask(new timerxiaowang());
	TimerOutMng::GetInstance().AddTask(new timerxiaohong());
	TimerOutMng::GetInstance().AddTask(new timerxiaohong233());
	/*添加通道*/
	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));

	/*添加角色*/
	ZinxKernel::Zinx_Add_Role(*(new EchoRole()));
	
	ZinxKernel::Zinx_Add_Role(*(new DatePreRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));
	/*5- 运行框架*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}
