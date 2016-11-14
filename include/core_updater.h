
#ifndef DOTRIX_CORE_UPDATER_H
#define DOTRIX_CORE_UPDATER_H

#include <QThread>

#include <gameboycore/gameboycore.h>

class CoreUpdater : public QThread
{
public:
	Q_OBJECT

public:
	CoreUpdater(gb::GameboyCore& core) :
		core_(core),
		running_(false)
	{
	}

	void run()
	{
		while (running_)
		{
			core_.update(512);
		}
	}

	void start()
	{
		running_ = true;
		QThread::start();
	}

	void stop()
	{
		running_ = false;
	}

	~CoreUpdater()
	{
	}

private:
	gb::GameboyCore& core_;
	volatile bool running_;
};

#endif // 
