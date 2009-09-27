#include "newdvdgui.h"
#include "dvddrive.h"
#include "encodemp4job.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>
#include <QHBoxLayout>
#include <QtConcurrentRun>

NewDVDGui::NewDVDGui() :
		m_insertDvd(tr("Please insert video DVD.")),
		m_loadingTitles(tr("<i>%1 inserted. Scanning titles...</i>")),
		m_ripping(tr("Created new video from DVD."))
{
	m_watcher = new QFutureWatcher<QMap<int, QString> >;
	m_watcher->setParent(this);
	connect(m_watcher, SIGNAL(finished()), this, SLOT(loadedTitles()));
	m_status = new QLabel;
	m_dvdName = new QLineEdit;
	connect(m_dvdName, SIGNAL(textChanged(QString)), this, SLOT(validateName(QString)));
	m_rip = new QPushButton(tr("&Rip DVD"));
	connect(m_rip, SIGNAL(clicked()), this, SLOT(rip()));
	m_eject = new QPushButton(tr("&Eject"));
	connect(m_eject, SIGNAL(clicked()), DVDDrive::instance(), SLOT(eject()));
	QHBoxLayout *buttons = new QHBoxLayout;
	buttons->addWidget(m_rip);
	buttons->addWidget(m_eject);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_status, 0, Qt::AlignHCenter);
	layout->addWidget(m_dvdName);
	layout->addLayout(buttons);
	if (DVDDrive::instance()->dvdInserted())
		dvdAdded();
	else
		dvdRemoved();
	connect(DVDDrive::instance(), SIGNAL(dvdAdded()), this, SLOT(dvdAdded()));
	connect(DVDDrive::instance(), SIGNAL(dvdRemoved()), this, SLOT(dvdRemoved()));
	setTitle(tr("New Video from DVD"));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	setLayout(layout);
}
void NewDVDGui::dvdAdded()
{
	QString name = DVDDrive::instance()->dvdName();
	m_status->setText(m_loadingTitles.arg(name));
	m_dvdName->setText(name); //Calls validateName
	m_dvdName->setVisible(false);
	m_status->setVisible(true);
	m_rip->setVisible(false);
	m_eject->setVisible(false);
	m_watcher->setFuture(QtConcurrent::run(&EncodeMP4Job::titles, DVDDrive::instance()->dvdDevice()));
}
void NewDVDGui::validateName(const QString &name)
{
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	m_rip->setEnabled(!settings.childGroups().contains(name));
}
void NewDVDGui::loadedTitles()
{
	m_titles = m_watcher->result();
	m_rip->setVisible(true);
	m_eject->setVisible(true);
	m_status->setVisible(false);
	m_dvdName->setVisible(true);
}
void NewDVDGui::rip()
{
	m_rip->setEnabled(false);
	m_dvdName->setVisible(false);
	m_status->setText(m_ripping);
	m_status->setVisible(true);
	emit newDVD(m_dvdName->text(), m_titles);
}

void NewDVDGui::dvdRemoved()
{
	m_status->setText(m_insertDvd);
	m_status->setVisible(true);
	m_dvdName->setVisible(false);
	m_rip->setVisible(false);
	m_eject->setVisible(false);
}
