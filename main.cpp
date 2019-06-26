#include <zinx.h>
#include <ZinxTCP.h>
#include <iostream>
#include "StdInOutChannel.h"
#include "Role.h"
#include "CmdIdentify.h"
#include "CmdMsg.h"
#include "ZinxTimer.h"
/*����ṹ�ع�*/
/*
1.ͨ���ࣨ���䣩
2.Э���� (ԭʼ���ݺ��û������໥ת��)
3.��ɫ�ࣨҵ��
*/

class timerxiaowang :public TimerOutProc {
	// ͨ�� TimerOutProc �̳�
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
	// ͨ�� TimerOutProc �̳�
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
	// ͨ�� TimerOutProc �̳�
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
	/*1- ��ʼ�����*/

	ZinxKernel::ZinxKernelInit();
	TimerOutMng::GetInstance().AddTask(new timerxiaowang());
	TimerOutMng::GetInstance().AddTask(new timerxiaohong());
	TimerOutMng::GetInstance().AddTask(new timerxiaohong233());
	/*���ͨ��*/
	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));

	/*��ӽ�ɫ*/
	ZinxKernel::Zinx_Add_Role(*(new EchoRole()));
	
	ZinxKernel::Zinx_Add_Role(*(new DatePreRole()));
	ZinxKernel::Zinx_Add_Role(*(new OutputCtrl()));
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimerChannel()));
	/*5- ���п��*/
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}
