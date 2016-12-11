#ifndef DOTRIX_NETWORK_SETTINGS_H
#define DOTRIX_NETWORK_SETTINGS_H

#include <QWidget>
#include <QDialog>

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

private:
	std::unique_ptr<Ui::NetworkSettings> ui_;
};

#endif // DOTRIX_NETWORK_SETTINGS_H
