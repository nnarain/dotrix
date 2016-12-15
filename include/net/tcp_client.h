/**
	\file tcp_client.h
	\brief TCP Client
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 12 2016
*/

#ifndef DOTRIX_NET_TCP_CLIENT_H
#define DOTRIX_NET_TCP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class TcpClient : public QObject
{
	Q_OBJECT

public:

	TcpClient(QHostAddress addr, quint16 port, QObject* parent = nullptr) :
		QObject(parent),
		address_(addr),
		port_(port),
		socket_(new QTcpSocket(this))
	{
		connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
		start();
	}

	virtual ~TcpClient()
	{
		if (socket_->isOpen())
		{
			socket_->close();
		}
	}

	void start()
	{
		socket_->connectToHost(address_, port_);

		if (!socket_->isOpen())
		{
			qDebug() << "TCP client could not bind";
		}
		else
		{
			qDebug() << "TCP client bound";
		}
	}

public slots:

	void readyRead()
	{
		QByteArray data = socket_->readAll();
	}

	void disconnected()
	{
		socket_->deleteLater();
	}


private:
	QTcpSocket* socket_;
	QHostAddress address_;
	quint16 port_;
};

#endif // DOTRIX_NET_TCP_CLIENT_H
