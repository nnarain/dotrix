#ifndef DOTRIX_NETWORK_SETTINGS_H
#define DOTRIX_NETWORK_SETTINGS_H

#include <QWidget>
#include <QDialog>

namespace Ui{
	class NetworkSettings;
}

class NetworkSettings : public QDialog
{
	Q_OBJECT

public:

	explicit NetworkSettings(QWidget* parent = nullptr);
	~NetworkSettings();

private:
	Ui::NetworkSettings* ui_;
};

#endif // DOTRIX_NETWORK_SETTINGS_H
