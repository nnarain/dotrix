#include "network_settings.h"
#include "ui_network_settings.h"


NetworkSettings::NetworkSettings(QWidget* parent) :
	QDialog(parent),
	ui_(new Ui::NetworkSettings)
{
	ui_->setupUi(this);
}

NetworkSettings::~NetworkSettings()
{
}