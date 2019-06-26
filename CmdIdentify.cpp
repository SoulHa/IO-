
#include "CmdMsg.h"
#include "Role.h"
#include "StdInOutChannel.h"
#include "CmdIdentify.h"

#include <iostream>
using namespace std;
CmdIdentify* CmdIdentify::poSingle = new CmdIdentify();
UserData* CmdIdentify::raw2request(std::string _szInput)
{
	if ("exit" == _szInput)
	{
		ZinxKernel::Zinx_Exit();
		return NULL;
	}
	/*ԭʼ����ת��Ϊҵ������*/
	auto pret = new CmdMsg();
	pret->szUserData = _szInput;
	if ("open" == _szInput)
	{
		pret->isCmd = true;
		pret->isOpen = true;
	}
	if ("close" == _szInput)
	{
		pret->isCmd = true;
		pret->isOpen = false;
	}
	if ("date" == _szInput)
	{
		pret->isCmd = true;
		pret->needDatePre = true;
	}
	if ("cleardate" == _szInput)
	{
		pret->isCmd = true;
		pret->needDatePre = false;
	}
	return pret;
}

std::string* CmdIdentify::response2raw(UserData& _oUserData)
{
	GET_REF2DATA(CmdMsg, output, _oUserData);
	return new string(output.szUserData);
}

Irole* CmdIdentify::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{

	auto rolelist = ZinxKernel::Zinx_GetAllRole();
	/*�������ͬ��������ͬ�Ĵ���role����*/
	auto pCmdMsg = dynamic_cast<CmdMsg*>(_oUserDataMsg.poUserData);
	/*��ȡ��ǰ��Ϣ�Ƿ�������*/
	bool isCmd = pCmdMsg->isCmd;
	Irole* pRetRole = NULL;
	for (Irole* prole : rolelist)
	{
		if (isCmd)
		{
			auto pOutCtrl = dynamic_cast<OutputCtrl*>(prole);
			if (NULL != pOutCtrl)
			{
				pRetRole = pOutCtrl;
				break;
			}
		}
		else
		{
			auto pDate = dynamic_cast<DatePreRole*>(prole);
			if (NULL != pDate)
			{
				pRetRole = pDate;
				break;
			}
		}
	}
	//return rolelist.front();
	return pRetRole;
}

Ichannel* CmdIdentify::GetMsgSender(BytesMsg& _oBytes)
{
	return ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
}
