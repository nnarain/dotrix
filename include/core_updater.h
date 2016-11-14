
#ifndef DOTRIX_CORE_UPDATER_H
#define DOTRIX_CORE_UPDATER_H

#include <QThread>

#include <gameboycore/gameboycore.h>

#include <memory>

class CoreUpdater
{
public:
	class Worker : public QThread
	{
	public:
		Worker(gb::GameboyCore& core) : core_(core), running_(false)
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

		void wait()
		{
			running_ = false;
			QThread::wait();
		}

	private:
		gb::GameboyCore& core_;
		volatile bool running_;
	};

	CoreUpdater(gb::GameboyCore& core) :
		core_(core)
	{
	}

	void start()
	{
		worker_.reset(new Worker(core_));
		worker_->start();
	}

	void stop()
	{
		worker_->wait();
	}

	bool isRunning()
	{
		return worker_ && worker_->isRunning();
	}

	~CoreUpdater()
	{
	}

private:
	gb::GameboyCore& core_;
	std::unique_ptr<Worker> worker_;
};

#endif // 
