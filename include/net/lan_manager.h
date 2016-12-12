/**
	\file   lan_manager.h
	\brief  Manage finding LAN processes to connect to
	\author Natesh Narain <nnaraindev@gmail.com>
	\date   Dec 11 2016
*/

#ifndef DOTRIX_NET_LAN_MANAGER_H
#define DOTRIX_NET_LAN_MANAGER_H

#include <QObject>

#include "net/discover_server.h"
#include "net/discover_client.h"

class LanManager : public QObject
{
	Q_OBJECT

signals:

	void log(const QString&);

public:

	explicit LanManager(QObject* parent = nullptr) :
		discover_server_(this),
		discover_client_(this)
	{
		connect(&discover_client_, SIGNAL(timeout()), this, SLOT(discoverTimeout()));
		connect(&discover_client_, SIGNAL(serverFound(QHostAddress, quint16)), this, SLOT(serverFound(QHostAddress, quint16)));
	}

	~LanManager()
	{
	}

public:

	DiscoverServer* getDiscoverServer()
	{
		return &discover_server_;
	}

	DiscoverClient* getDiscoverClient()
	{
		return &discover_client_;
	}

public slots:

	void serverFound(QHostAddress, quint16)
	{
		discover_client_.stop();
	}

	void discoverTimeout()
	{
		emit log("Discovery has timed out... Switching to server ");

		discover_client_.stop();
		discover_server_.start();
	}

	void startConnection()
	{
		emit log("Starting search for Dotrix servers...");

		discover_client_.start();
	}

private:
	DiscoverServer discover_server_;
	DiscoverClient discover_client_;

};

#endif // DOTRIX_NET_LAN_MANGER_H
