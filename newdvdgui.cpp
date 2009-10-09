#include "newdvdgui.h"
#include "dvddrive.h"
#include "encodemp4job.h"
#include "titleloader.h"
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
	m_titleLoader = new TitleLoader(DVDDrive::instance()->dvdDevice(), this);
	connect(m_titleLoader, SIGNAL(loadedTitles(QMap<int,QString>)), this, SLOT(loadedTitles(QMap<int,QString>)));
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
	QString name = DVDDrive::instance()->dvdName().replace(QLatin1Char('_'), QLatin1Char(' ')).toLower();
	int i = 0;
	do {
		name[i] = name[i].toUpper();
		i = name.indexOf(QLatin1Char(' '), i) + 1;
	} while (i);
	m_status->setText(m_loadingTitles.arg(name));
	m_dvdName->setText(name);
	m_dvdName->setVisible(false);
	m_status->setVisible(true);
	m_rip->setVisible(false);
	m_eject->setVisible(false);
	m_titleLoader->loadTitles();
}
void NewDVDGui::validateName(const QString &name)
{
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	m_rip->setEnabled(!settings.childGroups().contains(QString(name).replace(QChar('/'), QChar('-'))) && !name.trimmed().isEmpty());
}
void NewDVDGui::loadedTitles(QMap<int, QString> titles)
{
	m_titles = titles;
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
