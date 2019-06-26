#pragma once
#include <zinx.h>
class CmdMsg :
	public UserData
{
public:
	/*成员变量表示要回显的字符串*/
	std::string szUserData;
	bool isOpen = true;
	bool isCmd = false;
	bool needDatePre = false;
};

