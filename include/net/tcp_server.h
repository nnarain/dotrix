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

class TcpServer : public QObject
{
	Q_OBJECT

public:

	TcpServer(QObject* parent = nullptr) :
		QObject(parent),
		server_(new QTcpServer(this)),
		socket_(nullptr),
		is_connected_(false)
	{
		connect(server_, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}

	~TcpServer()
	{
		if (server_->isListening())
		{
			server_->close();
		}
	}

	void start(const QHostAddress& address, quint16 port)
	{
		if (!server_->listen(address, port))
		{
			qDebug() << "Could not bind to " << address << ":" << QString(port);
		}
	}

public slots:

	void newConnection()
	{
		// can only handle one connection at a time
		if (is_connected_) return;

		socket_ = server_->nextPendingConnection();

		connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
		connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnected()));

		is_connected_ = true;
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
	QTcpServer* server_;
	QTcpSocket* socket_;

	bool is_connected_;
};

#endif // DOTRIX_NET_TCP_SERVER_H
