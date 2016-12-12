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

public:

	explicit NetworkSettings(QWidget* parent = nullptr);
	~NetworkSettings();

public slots:


private:
	std::unique_ptr<Ui::NetworkSettings> ui_;

	LanManager* lan_;
};

#endif // DOTRIX_NETWORK_SETTINGS_H
