#include "network_settings.h"
#include "ui_network_settings.h"


NetworkSettings::NetworkSettings(QWidget* parent) :
	QDialog(parent),
	ui_(new Ui::NetworkSettings),
	lan_(new LanManager(this))
{
	ui_->setupUi(this);

	// disable widgets
	this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	ui_->etOutput->setTextInteractionFlags(Qt::NoTextInteraction);

	// connect
	connect(ui_->bnConnect, SIGNAL(clicked()), lan_, SLOT(startConnection()));

	connect(lan_, SIGNAL(log(const QString&)), ui_->etOutput, SLOT(append(const QString&)));
	connect(lan_->getDiscoverClient(), SIGNAL(log(const QString&)), ui_->etOutput, SLOT(append(const QString&)));

	connect(lan_, SIGNAL(interfaceReady(QObject*)), this, SLOT(interfaceReadySlot(QObject*)));

	qRegisterMetaType<uint8_t>("uint8_t");
}

void NetworkSettings::interfaceReadySlot(QObject* net)
{
	emit interfaceReady(net);
}

NetworkSettings::~NetworkSettings()
{
}