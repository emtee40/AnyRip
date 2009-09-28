#include "newimagegui.h"
#include "titleloader.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>

NewImageGui::NewImageGui()
{
	m_titleLoader = new TitleLoader(this);
	connect(m_titleLoader, SIGNAL(loadedTitles(QMap<int,QString>)), this, SLOT(loadedTitles(QMap<int,QString>)));
	m_dvdName = new QLineEdit;
	connect(m_dvdName, SIGNAL(textChanged(QString)), this, SLOT(validateName(QString)));
	m_status = new QLabel;
	m_status->setText(tr("Scanning titles..."));
	m_browseButton = new QPushButton(tr("&Select File"));
	connect(m_browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	m_importImageButton = new QPushButton(tr("&Import Image"));
	connect(m_importImageButton, SIGNAL(clicked()), this, SLOT(importImage()));
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_status);
	layout->addWidget(m_dvdName);
	layout->addWidget(m_browseButton);
	layout->addWidget(m_importImageButton);
	initialState();
	setLayout(layout);
	setTitle(tr("New Video from DVD Image"));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
void NewImageGui::initialState()
{
	m_status->setVisible(false);
	m_dvdName->setVisible(false);
	m_importImageButton->setVisible(false);
	m_browseButton->setVisible(true);
}
void NewImageGui::validateName(const QString &name)
{
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	m_importImageButton->setEnabled(!settings.childGroups().contains(QString(name).replace(QChar('/'), QChar('-'))) && !name.trimmed().isEmpty());
}
void NewImageGui::browse()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select DVD Image"), QString(), tr("DVD Images (*.iso)"));
	if (!path.isNull()) {
		m_path = path;
		m_browseButton->setVisible(false);
		m_titleLoader->setLocation(path);
		m_status->setVisible(true);
		m_titleLoader->loadTitles();
	}
}
void NewImageGui::loadedTitles(QMap<int, QString> titles)
{
		m_dvdName->setVisible(true);
		m_dvdName->setText(QFileInfo(m_path).baseName());
		m_status->setVisible(false);
		m_importImageButton->setVisible(true);
		m_titles = titles;
}
void NewImageGui::importImage()
{
	initialState();
	emit newImage(m_path, m_dvdName->text(), m_titles);
}
