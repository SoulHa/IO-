#include "Role.h"
#include "CmdMsg.h"
#include "CmdIdentify.h"
#include <iostream>
#include <string>
using namespace std;
bool EchoRole::Init()
{
	return true;
}

UserData* EchoRole::ProcMsg(UserData& _poUserData)
{
	/*回写数据*/
	GET_REF2DATA(CmdMsg, input, _poUserData);

	CmdMsg* pout = new CmdMsg(input);
	ZinxKernel::Zinx_SendOut(*pout, *(CmdIdentify::GetInstance()));
	return nullptr;
}

void EchoRole::Fini()
{
}

bool OutputCtrl::Init()
{
	Irole* pRetRole = NULL;
	/*设置下一个处理环节是date前缀对象*/
	for (auto pRole : ZinxKernel::Zinx_GetAllRole())
	{
		auto pDate = dynamic_cast<DatePreRole*>(pRole);
		if (NULL!=pDate)
		{
			pRetRole = pDate;
		}
	}
	if (NULL != pRetRole)
	{
		SetNextProcessor(*pRetRole);
	}
	return true;
}

UserData* OutputCtrl::ProcMsg(UserData& _poUserData)
{
	/*读取用户设置的开关标志*/
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (true == input.isOpen)
	{
		if (NULL != pOut)
		{
			//开输出
			ZinxKernel::Zinx_Add_Channel(*(pOut));
			pOut = NULL;
		}
	}
	else
	{
		//关输出
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		ZinxKernel::Zinx_Del_Channel(*(pchannel));
		pOut = pchannel;
	}
	return new CmdMsg(input);
	//return nullptr;
}

void OutputCtrl::Fini()
{
}

bool DatePreRole::Init()
{
	Irole* pRetRole = NULL;
	/*设置下一个处理环节是echo对象*/
	for (auto pRole : ZinxKernel::Zinx_GetAllRole())
	{
		auto pEcho = dynamic_cast<EchoRole*>(pRole);
		if (NULL != pEcho)
		{
			pRetRole = pEcho;
			break;
		}

	}
	if (NULL != pRetRole)
	{
		SetNextProcessor(*pRetRole);
	}
	return true;
}

UserData* DatePreRole::ProcMsg(UserData& _poUserData)
{
	/*如果用户是命令--》改变标志位*/
	CmdMsg* pret = NULL;
	GET_REF2DATA(CmdMsg, input, _poUserData);
	if (input.isCmd)
	{
		needAdd = input.needDatePre;
	}
	else
	/*如果是需要回显的内容-》添加字符串并交给回显功能对象*/
	{
		if (true == needAdd)
		{
			time_t tmp;
			time(&tmp);
			string szNew = string(ctime(&tmp)) + input.szUserData;
			pret = new CmdMsg(input);
			pret->szUserData = szNew;
		}
		else
		{
			pret = new CmdMsg(input);
		}
	}
	return pret;


}

void DatePreRole::Fini()
{
}
