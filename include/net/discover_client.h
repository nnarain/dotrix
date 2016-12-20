#ifndef DOTRIX_NET_DISCOVER_CLIENT_H
#define DOTRIX_NET_DISCOVER_CLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTimer>
#include <QByteArray>

/**
	
*/
class DiscoverClient : public QObject
{
	Q_OBJECT

signals:

	void serverFound(QHostAddress, quint16);
	void timeout();
	void log(const QString&);


public:

	explicit DiscoverClient(QObject* parent = nullptr):
		QObject(parent),
		socket_(new QUdpSocket(this)),
		broadcast_timer_(new QTimer(this)),
		broadcast_counter_(0),
		searching_(false)
	{
		connect(broadcast_timer_, SIGNAL(timeout()), this, SLOT(sendBroadcast()));
	}

	~DiscoverClient()
	{
	}

	void start()
	{
		if (!searching_)
		{
			socket_->bind(QHostAddress::AnyIPv4, 11411);
			connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));

			broadcast_timer_->start(1000);
			searching_ = true;
		}
	}

	void stop()
	{
		disconnect(socket_);
		socket_->close();

		broadcast_timer_->stop();
		searching_ = false;
	}

private:

	void processPacket(const QString& packet, const QHostAddress& host, quint16 port)
	{
		auto chunks = packet.split("\n");

		if (chunks.size() >= 3)
		{
			auto header  = chunks[0];

			if (header == "DISCOVER_RESPONSE")
			{
				auto address = QHostAddress(chunks[1]);
				auto port = chunks[2].toInt();

				QString msg;
				msg
					.append("Dotrix server @ ")
					.append(address.toString())
					.append(":")
					.append(QString::number(port));

				emit serverFound(address, port);
				emit log(msg);
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

	void sendBroadcast()
	{
		emit log("Requesting discovery...");

		QByteArray datagram;
		datagram.append("DISCOVER_REQUEST");

		socket_->writeDatagram(datagram, QHostAddress::Broadcast, 11411);

		broadcast_counter_++;

		if (broadcast_counter_ == 5)
		{
			broadcast_counter_ = 0;

			emit timeout();
		}
	}

private:
	QUdpSocket* socket_;
	QTimer* broadcast_timer_;
	int broadcast_counter_;
	bool searching_;
};

#endif // DOTRIX_NET_DISCOVER_CLIENT_H
