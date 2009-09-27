#include "newimagegui.h"
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
	m_dvdName = new QLineEdit;
	m_dvdName->setVisible(false);
	connect(m_dvdName, SIGNAL(textChanged(QString)), this, SLOT(validateName(QString)));
	m_imagePath = new QLineEdit(tr("Select a DVD image"));
	m_imagePath->setEnabled(false);
	connect(m_imagePath, SIGNAL(textChanged(QString)), this, SLOT(validatePath(QString)));
	QPushButton *browseButton = new QPushButton(tr("&Browse"));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	m_importImageButton = new QPushButton(tr("&Import Image"));
	m_importImageButton->setEnabled(false);
	connect(m_importImageButton, SIGNAL(clicked()), this, SLOT(importImage()));
	QHBoxLayout *pathArea = new QHBoxLayout;
	pathArea->addWidget(m_imagePath);
	pathArea->addWidget(browseButton);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(pathArea);
	layout->addWidget(m_dvdName);
	layout->addWidget(m_importImageButton);
	setLayout(layout);
	setTitle(tr("New Video from DVD Image"));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
void NewImageGui::validatePath(const QString &path)
{
	if (path.toLower().endsWith(".iso") && QFile::exists(path)) {
		m_dvdName->setText(QFileInfo(path).baseName());
		m_dvdName->setVisible(true);
	} else {
		m_dvdName->setText(QString());
		m_dvdName->setVisible(false);
		m_importImageButton->setEnabled(false);
	}
}
void NewImageGui::validateName(const QString &name)
{
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	m_importImageButton->setEnabled(!settings.childGroups().contains(name));
}
void NewImageGui::browse()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select DVD Image"), QString(), tr("DVD Images (*.iso)"));
	if (!path.isNull())
		m_imagePath->setText(path);
}
void NewImageGui::importImage()
{
	m_importImageButton->setEnabled(false);
	emit newImage(m_imagePath->text(), m_dvdName->text());
}
