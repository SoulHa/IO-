#include "ZinxTimer.h"
#include <zinx.h>
#include <sys/timerfd.h>
#include <vector>
#include <list>
using namespace std;

bool ZinxTimerChannel::Init()
{
	bool bRet = false;
	int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (0<=iFd)
	{
		struct itimerspec period = {
			{1,0},{1,0}
		};
		if (0==timerfd_settime(iFd,0,&period,NULL))
		{
			bRet = true;
			m_TimerFd = iFd;
		}
	}
	return bRet;
}

bool ZinxTimerChannel::ReadFd(std::string& _input)
{
	bool bRet = false;
	char buff[8] = { 0 };
	if (sizeof(buff) == read(m_TimerFd, buff, sizeof(buff)))
	{
		bRet = true;
		_input.assign(buff, sizeof(buff));
	}
	return bRet;
}

bool ZinxTimerChannel::WriteFd(std::string& _output)
{
	return false;
}

void ZinxTimerChannel::Fini()
{
	close(m_TimerFd);
	m_TimerFd = -1;
}

int ZinxTimerChannel::GetFd()
{
	return m_TimerFd;
}

std::string ZinxTimerChannel::GetChannelInfo()
{
	return "TimerFd";
}

class output_hello :public AZinxHandler {
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		auto pchannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "hello_world";
		ZinxKernel::Zinx_SendOut(output, *pchannel);
		return nullptr;
	}
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return nullptr;
	}
}*pout_hello=new output_hello();
/*处理返回事件的对象*/
AZinxHandler* ZinxTimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return &TimerOutMng::GetInstance();
}
TimerOutMng TimerOutMng::single;

TimerOutMng::TimerOutMng()
{
	for (int i = 0; i < 10; i++)
	{
		 list<TimerOutProc* > tmp;
		 m_timer_wheel.push_back(tmp);
	}
}

IZinxMsg* TimerOutMng::InternelHandle(IZinxMsg& _oInput)
{
	//for (auto task:m_task_list)
	//{
	//	task->iCount--;
	//	if (task->iCount <= 0)
	//	{
	//		task->Proc();
	//		task->iCount = task->GetTimeSec();
	//	}
	//}
	unsigned long iTimeoutCount = 0;
	GET_REF2DATA(BytesMsg, obytes, _oInput);
	obytes.szData.copy((char*)& iTimeoutCount, sizeof(iTimeoutCount));
	cout << endl<<"iTimeoutCount  " << iTimeoutCount << endl;
	while (iTimeoutCount-->0)
	{
		cur_index++;
		cur_index %= 10;
		list<TimerOutProc*> m_cache;
		for (auto itr = m_timer_wheel[cur_index].begin(); itr != m_timer_wheel[cur_index].end();)
		{
			if ((*itr)->iCount <= 0)
			{
				m_cache.push_back(*itr);
				auto ptmp = *itr;
				itr = m_timer_wheel[cur_index].erase(itr);
				AddTask(ptmp);
			}
			else
			{
				(*itr)->iCount--;
				++itr;
			}
		}
		for (auto task : m_cache)
		{
			task->Proc();
		}
	}
	return nullptr;
}

AZinxHandler* TimerOutMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}

void TimerOutMng::AddTask(TimerOutProc* _ptask)
{
	//m_task_list.push_back(_ptask);
	int index = (_ptask->GetTimeSec() + cur_index) % 10;
	m_timer_wheel[index].push_back(_ptask);
	_ptask->iCount = _ptask->GetTimeSec()/10;
}

void TimerOutMng::DelTask(TimerOutProc* _ptask)
{
	//m_task_list.remove(_ptask);
	for (list<TimerOutProc*>& chi : m_timer_wheel)
	{
		for (auto task : chi)
		{
			chi.remove(_ptask);
			return;
		}

	}
}
