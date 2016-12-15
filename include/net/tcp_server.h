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

#include <QDebug>

/**
	\class TcpServer
	\brief TCP server
*/
class TcpServer : public QObject
{
	Q_OBJECT

signals:

	void clientConnected();

public:

	TcpServer(QObject* parent = nullptr) :
		QObject(parent),
		server_(new QTcpServer(this)),
		socket_(nullptr),
		is_connected_(false)
	{
		connect(server_, SIGNAL(newConnection()), this, SLOT(newConnection()));
		start();
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
		else
		{
			qDebug() << "TCP Server listening";
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
		QByteArray data = socket_->readAll();
	}

	void disconnected()
	{
		is_connected_ = false;
		socket_->deleteLater();
	}

private:

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
};

#endif // DOTRIX_NET_TCP_SERVER_H
