/**
	\file qgameboycore.h
	\brief Qt GameboyCore Wrapper
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 10 2016
*/

#ifndef DOTRIX_QGAMEBOYCORE_H
#define DOTRIX_QGAMEBOYCORE_H

#include <QObject>
#include <QString>
#include <QFile>

#include "core_updater.h"

#include <gameboycore/gameboycore.h>

#include <functional>

/**
	\class QGameboyCore
	\brief Qt wrapper for GameboyCore. 
*/
class QGameboyCore : public QObject
{
	Q_OBJECT

signals:

	void scanline(gb::GPU::Scanline, int line);

public:

	QGameboyCore() : 
		updater_(core_)
	{
		// register Core meta types
		qRegisterMetaType < gb::GPU::Scanline >("gb::GPU::Scanline");
	}

	~QGameboyCore()
	{
	}

	/* Core Callbacks */
private:
	void gpuCallback(gb::GPU::Scanline s, int line)
	{
		emit scanline(s, line);
	}

	/* Convience functions */

public:
	void setupCallbacks()
	{
		// GPU callback
		core_.getGPU()->setRenderCallback(std::bind(&QGameboyCore::gpuCallback, this, std::placeholders::_1, std::placeholders::_2));
	}

	void load(const QString& rom_file_name, const QString& save_file_name)
	{
		// read rom file
		QFile file(rom_file_name);
		std::vector<uint8_t> rom;
		rom.resize(file.size());

		file.open(QIODevice::ReadOnly);
		file.read((char*)&rom[0], file.size());

		core_.loadROM(&rom[0], (uint32_t)rom.size());

		QFile save_file(save_file_name);

		// save file
		if (save_file.exists())
		{
			save_file.open(QIODevice::ReadOnly);
			std::vector<uint8_t> ram;

			if (save_file_name.size() > 0)
			{
				ram.resize(save_file.size());
				save_file.read((char*)&ram[0], ram.size());
				core_.getMMU()->setBatteryRam(ram);
			}
		}
	}

	void save(const QString& save_file_name)
	{
		// save battery ram
		if (save_file_name != "")
		{
			auto ram = core_.getMMU()->getBatteryRam();

			QFile file(save_file_name);
			file.open(QIODevice::WriteOnly);
			file.write((char*)&ram[0], ram.size());
		}
	}

	void start()
	{
		updater_.start();
	}

	void stop()
	{
		updater_.stop();
	}

	/* Accessors */

	bool isRunning()
	{
		return updater_.isRunning();
	}

	gb::GameboyCore& getCore()
	{
		return core_;
	}

private:
	gb::GameboyCore core_;
	CoreUpdater updater_;
};

#endif // DOTRIX_QGAMEBOYCORE_H
