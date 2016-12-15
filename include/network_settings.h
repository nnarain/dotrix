#ifndef DOTRIX_NETWORK_SETTINGS_H
#define DOTRIX_NETWORK_SETTINGS_H

#include <QWidget>
#include <QDialog>

#include "net/lan_manager.h"

#include <memory>

namespace Ui{
	class NetworkSettings;
}

class NetworkSettings : public QDialog
{
	Q_OBJECT

signals:

	void interfaceReady(QObject*);

public:

	explicit NetworkSettings(QWidget* parent = nullptr);
	~NetworkSettings();

public slots:


	void interfaceReadySlot(QObject* net);

private:
	std::unique_ptr<Ui::NetworkSettings> ui_;

	LanManager* lan_;
};

#endif // DOTRIX_NETWORK_SETTINGS_H
