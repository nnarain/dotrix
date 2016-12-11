/**
	\file discover_server.h
	\brief Response to discover request with network connection information
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 12 2016
*/

#ifndef DOTRIX_NET_DISCOVER_SERVER_H
#define DOTRIX_NET_DISCOVER_SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QByteArray>
#include <QString>
#include <QList>
#include <QNetworkInterface>

#include <QDebug>

class DiscoverServer : public QObject
{
	Q_OBJECT

public:

	explicit DiscoverServer(QObject* parent = nullptr) : 
		QObject(parent),
		socket_(new QUdpSocket(this))
	{
	}

	~DiscoverServer()
	{
	}

	void serve()
	{
		socket_->bind(QHostAddress::AnyIPv4, 11411);
		connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));


	}

	void stop()
	{
		disconnect(socket_);
		socket_->close();
	}

private:

	void processPacket(const QString& msg, const QHostAddress& host, quint16 port)
	{
		// check for discovery request header
		if(msg == "DISCOVER_REQUEST")
		{
			// build response text
			QString response;
			response.append("DISCOVER_RESPONSE\n")
					.append(getIpAddress())
					.append("\n")
					.append(QString::number(11412));

			QByteArray datagram;
			datagram.append(response);

			socket_->writeDatagram(datagram, host, port);
		}
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
	}

public slots:

	void readyRead()
	{
		// process recieve packets

		while (socket_->hasPendingDatagrams())
		{
			QByteArray buffer;
			buffer.resize(socket_->pendingDatagramSize());

			QHostAddress sender_addr;
			quint16      sender_port;

			socket_->readDatagram(buffer.data(), buffer.size(), &sender_addr, &sender_port);

			processPacket(QString::fromStdString(buffer.toStdString()), sender_addr, sender_port);
		}
	}

private:
	QUdpSocket* socket_;
};

#endif // DOTRIX_NET_DISCOVER_SERVER_H
