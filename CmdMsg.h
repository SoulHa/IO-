#pragma once
#include <zinx.h>
class CmdMsg :
	public UserData
{
public:
	/*��Ա������ʾҪ���Ե��ַ���*/
	std::string szUserData;
	bool isOpen = true;
	bool isCmd = false;
	bool needDatePre = false;
};

