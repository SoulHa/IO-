#pragma once
#include <zinx.h>
#include <sys/timerfd.h>
#include <vector>
#include <list>

class ZinxTimerChannel :
	public Ichannel
{
	int m_TimerFd = -1;
public:
	// Í¨¹ý Ichannel ¼Ì³Ð
	virtual bool Init() override;
	virtual bool ReadFd(std::string& _input) override;
	virtual bool WriteFd(std::string& _output) override;
	virtual void Fini() override;
	virtual int GetFd() override;
	virtual std::string GetChannelInfo() override;
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class TimerOutProc {
public:
	virtual void Proc() = 0;
	virtual int GetTimeSec() = 0;
	int iCount = -1;
};

class TimerOutMng :public AZinxHandler {
	//std::list<TimerOutProc*>m_task_list;
	std::vector<std::list<TimerOutProc*>>m_timer_wheel;
	int cur_index = 0;
	static TimerOutMng single;
	TimerOutMng();

public:
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;

	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
	void AddTask(TimerOutProc* _ptask);
	void DelTask(TimerOutProc* _ptask);
	static TimerOutMng& GetInstance() {
		return single;
	}
};
