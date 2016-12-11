#include "network_settings.h"
#include "ui_network_settings.h"


NetworkSettings::NetworkSettings(QWidget* parent) :
	QDialog(parent),
	ui_(new Ui::NetworkSettings),
	lan_(new LanManager(this))
{
	ui_->setupUi(this);

	// disable resize
	this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

NetworkSettings::~NetworkSettings()
{
}