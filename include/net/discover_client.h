#ifndef DOTRIX_NET_DISCOVER_CLIENT_H
#define DOTRIX_NET_DISCOVER_CLIENT_H

#include <QObject>
#include <QHostAddress>

class DiscoverClient : public QObject
{
	Q_OBJECT

signals:

	void serverFound(QHostAddress, quint16);


public:

	explicit DiscoverClient(QObject* parent = nullptr):
		QObject(parent)
	{
	}

	~DiscoverClient()
	{
	}

private:
};

#endif // DOTRIX_NET_DISCOVER_CLIENT_H
