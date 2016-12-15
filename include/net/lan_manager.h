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
#include "net/tcp_server.h"
#include "net/tcp_client.h"

#include <memory>

class LanManager : public QObject
{
	Q_OBJECT

signals:

	void interfaceReady(QObject*);
	void log(const QString&);

public:

	explicit LanManager(QObject* parent = nullptr) :
		discover_server_(this),
		discover_client_(this)
	{
		connect(&discover_client_, SIGNAL(timeout()), this, SLOT(discoverTimeout()));
		connect(&discover_client_, SIGNAL(serverFound(QHostAddress, quint16)), this, SLOT(serverFound(QHostAddress, quint16)));
	}

	virtual ~LanManager()
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

	void serverFound(QHostAddress address, quint16 port)
	{
		// stop server discovery
		discover_client_.stop();

		net_ = std::make_unique<TcpClient>(address, port);

		emit interfaceReady(net_.get());
	}

	void clientConnected()
	{
		emit interfaceReady(net_.get());
	}

	void discoverTimeout()
	{
		emit log("Discovery has timed out...");
		emit log("Switching to server mode");

		discover_client_.stop();
		discover_server_.start();

		net_ = std::make_unique<TcpServer>();
		connect(net_.get(), SIGNAL(clientConnected()), this, SLOT(clientConnected()));
	}

	void startConnection()
	{
		emit log("Starting search for Dotrix servers...");

		discover_client_.start();
	}

private:
	DiscoverServer discover_server_;
	DiscoverClient discover_client_;

	// object that provides signals and slots to communicate with a network
	std::unique_ptr<QObject> net_;
};

#endif // DOTRIX_NET_LAN_MANGER_H
