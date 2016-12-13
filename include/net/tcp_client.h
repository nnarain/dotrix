/**
	\file tcp_client.h
	\brief TCP Client
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 12 2016
*/

#ifndef DOTRIX_NET_TCP_CLIENT_H
#define DOTRIX_NET_TCP_CLIENT_H

#include <QObject>

class TcpClient : public QObject
{
	Q_OBJECT

public:

	TcpClient(QObject* parent = nullptr) :
		QObject(parent)
	{
	}

	~TcpClient()
	{
	}
};

#endif // DOTRIX_NET_TCP_CLIENT_H
