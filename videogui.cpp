#include "videogui.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

VideoGui::VideoGui(Video *video) :
		m_video(video)
{
	//TODO: disable first 4 without them going grey
	m_imageCheck = new QCheckBox(tr("Copied DVD ISO"), this);
	m_imageCheck->setChecked(video->isJobCompleted(Video::DVDImage));
	m_imageCheck->setEnabled(false);
	m_encodeCheck = new QCheckBox(tr("Encoded MP4"), this);
	m_encodeCheck->setChecked(video->isJobCompleted(Video::EncodeMP4));
	m_encodeCheck->setEnabled(false);
	m_uploadCheck = new QCheckBox(tr("Uploaded MP4"), this);
	m_uploadCheck->setChecked(video->isJobCompleted(Video::Upload));
	m_uploadCheck->setEnabled(false);
	m_titleLoadCheck = new QCheckBox(tr("Title-Loaded"), this);
	m_titleLoadCheck->setChecked(video->isJobCompleted(Video::TitleLoad));
	m_titleLoadCheck->setEnabled(false);
	m_subtitleCheck = new QCheckBox(tr("Found Subtitle"), this);
	m_subtitleCheck->setChecked(video->isJobCompleted(Video::Subtitle));
	m_posterCheck = new QCheckBox(tr("Found Poster"), this);
	m_posterCheck->setChecked(video->isJobCompleted(Video::Poster));
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
	setFrameStyle(QFrame::StyledPanel);
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
