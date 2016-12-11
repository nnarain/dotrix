/**
	\file lan_manager.h
	\brief Manage finding LAN processes to connect to
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 11 2016
*/

#ifndef DOTRIX_NET_LAN_MANAGER_H
#define DOTRIX_NET_LAN_MANAGER_H

#include <QObject>

#include "net/discover_server.h"

class LanManager : public QObject
{
	Q_OBJECT

signals:

	void log(const QString&);

public:

	explicit LanManager(QObject* parent = nullptr) :
		discover_server_(this)
	{
	}

	~LanManager()
	{
	}

public slots:

	void startConnection()
	{
		emit log("Searching for Dotrix servers");
	}

private:
	DiscoverServer discover_server_;

};

#endif // DOTRIX_NET_LAN_MANGER_H
