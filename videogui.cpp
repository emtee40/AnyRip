#include "videogui.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

VideoGui::VideoGui(Video *video) :
		m_video(video)
{
	m_imageCheck = new QCheckBox(tr("Copied DVD ISO"), this);
	m_imageCheck->setCheckable(false);
	m_encodeCheck = new QCheckBox(tr("Encoded MP4"), this);
	m_encodeCheck->setCheckable(false);
	m_uploadCheck = new QCheckBox(tr("Uploaded MP4"), this);
	m_uploadCheck->setCheckable(false);
	m_titleLoadCheck = new QCheckBox(tr("Title-Loaded"), this);
	m_titleLoadCheck->setCheckable(false);
	m_subtitleCheck = new QCheckBox(tr("Found Subtitle"), this);
	m_subtitleCheck->setCheckable(false);
	m_posterCheck = new QCheckBox(tr("Found Poster"), this);
	m_posterCheck->setCheckable(false);
	//TODO: actions for checking subtitle and poster check to actually do it
	connect(video, SIGNAL(jobCompleted(Video::Jobs,bool)), this, SLOT(jobCompleted(Video::Jobs,bool)));
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(new QLabel(QString("<b>%1: </b>").arg(video->title())));
	layout->addWidget(m_imageCheck);
	layout->addWidget(m_encodeCheck);
	layout->addWidget(m_uploadCheck);
	layout->addWidget(m_titleLoadCheck);
	layout->addWidget(m_subtitleCheck);
	layout->addWidget(m_posterCheck);
	setLayout(layout);
}
void VideoGui::jobCompleted(Video::Jobs jobType, bool success)
{
	switch (jobType) {
		case Video::DVDImage:
			m_imageCheck->setChecked(success);
			break;
		case Video::EncodeMP4:
			m_encodeCheck->setChecked(success);
			break;
		case Video::Upload:
			m_uploadCheck->setChecked(success);
			break;
		case Video::TitleLoad:
			m_titleLoadCheck->setChecked(success);
			break;
		case Video::Subtitle:
			m_subtitleCheck->setChecked(success);
			break;
		case Video::Poster:
			m_posterCheck->setChecked(success);
			break;
	}
}
