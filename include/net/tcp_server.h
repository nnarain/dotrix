/**
	\file tcp_server.h
	\brief TCP Server
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 12 2016
*/


#ifndef DOTRIX_NET_TCP_SERVER_H
#define DOTRIX_NET_TCP_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QByteArray>

#include <gameboycore/link_cable.h>

#include <QDebug>

/**
	\class TcpServer
	\brief TCP server
*/
class TcpServer : public QObject
{
	Q_OBJECT

		signals :

	void clientConnected();

	void recieved(uint8_t);

public:

	TcpServer(QObject* parent = nullptr) :
		QObject(parent),
		server_(new QTcpServer(this)),
		socket_(nullptr),
		is_connected_(false)
	{
		connect(server_, SIGNAL(newConnection()), this, SLOT(newConnection()));
		start();

		cable_.setLink1RecieveCallback(std::bind(&TcpServer::link1RecieveCallback, this, std::placeholders::_1));
		cable_.setLink2RecieveCallback(std::bind(&TcpServer::link2RecieveCallback, this, std::placeholders::_1));
	}

	virtual ~TcpServer()
	{
		if (server_->isListening())
		{
			server_->close();
		}
	}

	void start()
	{
		const QHostAddress address(getIpAddress());
		quint16 port(11412);

		if (!server_->listen(address, port))
		{
			qDebug() << "Could not bind to " << address << ":" << QString(port);
		}
	}

	public slots:

	void newConnection()
	{
		qDebug() << "Handling new connection";

		// can only handle one connection at a time
		if (is_connected_) return;

		socket_ = server_->nextPendingConnection();

		connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
		connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnected()));

		is_connected_ = true;

		emit clientConnected();
	}

	void readyRead()
	{
		qDebug() << "S: client <- server";

		QByteArray data = socket_->readAll();

		if (data.size() >= 2)
		{
			auto byte = (uint8_t)data.at(0);
			auto mode = static_cast<gb::Link::Mode>(data.at(1));

			cable_.link2ReadyCallback(byte, mode);
		}
	}

	void disconnected()
	{
		is_connected_ = false;
		socket_->deleteLater();
	}

	void linkReady(uint8_t byte, gb::Link::Mode mode)
	{
		cable_.link1ReadyCallback(byte, mode);
	}

private:

	void link2RecieveCallback(uint8_t byte)
	{
		qDebug() << "S: server -> client";

		QByteArray data;
		data.append(byte);

		socket_->write(data);
	}

	void link1RecieveCallback(uint8_t byte)
	{
		emit recieved(byte);
	}

	QString getIpAddress() const
	{
		auto addresses = QNetworkInterface::allAddresses();

		for (auto iter = addresses.begin(); iter != addresses.end(); ++iter)
		{
			const auto address = *iter;

			if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback())
			{
				return address.toString();
			}
		}

		return QString();
	}

private:
	QTcpServer* server_;
	QTcpSocket* socket_;

	bool is_connected_;

	gb::LinkCable cable_;
};

#endif // DOTRIX_NET_TCP_SERVER_H
