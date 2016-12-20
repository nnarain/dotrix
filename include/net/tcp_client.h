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

signals:

	void recieved(uint8_t);

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

		auto byte = (uint8_t)data.at(0);

		emit recieved(byte);
	}

	void disconnected()
	{
		socket_->deleteLater();
	}

	void linkReady(uint8_t byte, gb::Link::Mode mode)
	{
		QByteArray data;
		data.append(byte);
		data.append(static_cast<uint8_t>(mode));

		socket_->write(data);
	}

private:
	QTcpSocket* socket_;
	QHostAddress address_;
	quint16 port_;
};

#endif // DOTRIX_NET_TCP_CLIENT_H
