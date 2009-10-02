#include "videogui.h"
#include "statuslabel.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QMap>
#include <QPushButton>

VideoGui::VideoGui(Video *video) :
		m_video(video)
{
	//TODO: disable first 4 without them going grey
	m_imageStatus = new StatusLabel(tr("Copied DVD ISO"), this);
	m_imageStatus->setCompleted(video->isJobCompleted(Video::DVDImage));
	m_encodeStatus = new StatusLabel(tr("Encoded MP4"), this);
	m_encodeStatus->setCompleted(video->isJobCompleted(Video::EncodeMP4));
	m_uploadStatus = new StatusLabel(tr("Uploaded MP4"), this);
	m_uploadStatus->setCompleted(video->isJobCompleted(Video::Upload));
	m_titleLoadStatus = new StatusLabel(tr("Title-Loaded"), this);
	m_titleLoadStatus->setCompleted(video->isJobCompleted(Video::TitleLoad));
	m_titleSelector = new QComboBox;
	QMapIterator<int, QString> i(video->dvdTitles());
	while (i.hasNext()) {
		i.next();
		m_titleSelector->addItem(tr("Title %1: %2").arg(QString::number(i.key())).arg(i.value()), i.key());
	}
	int currentIndex = m_titleSelector->findData(video->dvdTitle());
	if (currentIndex == -1)
		currentIndex = 0;
	m_titleSelector->setCurrentIndex(currentIndex);
	connect(m_titleSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(titleChanged(int)));
	QGridLayout *statusGrid = new QGridLayout;
	statusGrid->addWidget(m_imageStatus, 0, 0);
	statusGrid->addWidget(m_encodeStatus, 0, 1);
	statusGrid->addWidget(m_uploadStatus, 1, 0);
	statusGrid->addWidget(m_titleLoadStatus, 1, 1);
	statusGrid->addWidget(m_titleSelector, 0, 2);
	statusGrid->addWidget(new QPushButton("&Change Title Information"), 1, 2); //TODO: this is a placeholder
	m_subtitleStatus = new StatusLabel(tr("Found Subtitle"), this);
	m_subtitleStatus->setCompleted(video->isJobCompleted(Video::Subtitle));
	m_posterStatus = new StatusLabel(tr("Found Poster"), this);
	m_posterStatus->setCompleted(video->isJobCompleted(Video::Poster));
	//TODO: actions for checking subtitle and poster check to actually do it
	connect(video, SIGNAL(jobCompleted(Video::Jobs,bool)), this, SLOT(jobCompleted(Video::Jobs,bool)));
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(statusGrid);
	layout->addWidget(m_subtitleStatus);
	layout->addWidget(m_posterStatus);
	setLayout(layout);
	setTitle(video->title());
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
void VideoGui::jobCompleted(Video::Jobs jobType, bool success)
{
	switch (jobType) {
	case Video::DVDImage:
		m_imageStatus->setCompleted(success);
		break;
	case Video::EncodeMP4:
		m_encodeStatus->setCompleted(success);
		break;
	case Video::Upload:
		m_uploadStatus->setCompleted(success);
		break;
	case Video::TitleLoad:
		m_titleLoadStatus->setCompleted(success);
		break;
	case Video::Subtitle:
		m_subtitleStatus->setCompleted(success);
		break;
	case Video::Poster:
		m_posterStatus->setCompleted(success);
		break;
	}
}
void VideoGui::titleChanged(int index)
{
	m_video->setDvdTitle(m_titleSelector->itemData(index).toInt());
}
