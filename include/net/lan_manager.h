/**
	\file lan_manager.h
	\brief Manage finding LAN processes to connect to
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 11 2016
*/

#ifndef DOTRIX_NET_LAN_MANAGER_H
#define DOTRIX_NET_LAN_MANAGER_H

#include <QObject>

#include <memory>

class LanManager : public QObject
{
	Q_OBJECT

public:

	explicit LanManager(QObject* parent = nullptr)
	{
	}

	~LanManager()
	{
	}

private:

};

#endif // DOTRIX_NET_LAN_MANGER_H
