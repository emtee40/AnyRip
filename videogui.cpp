#include "videogui.h"
#include <QCheckBox>

VideoGui::VideoGui(Video *video) :
		m_video(video)
{
	m_imageCheck = new QCheckBox(tr("Copied DVD ISO"), this);
	m_imageCheck->setEnabled(false);
	m_encodeCheck = new QCheckBox(tr("Encoded MP4"), this);
	m_encodeCheck->setEnabled(false);
	m_uploadCheck = new QCheckBox(tr("Uploaded MP4"), this);
	m_uploadCheck->setEnabled(false);
	m_titleLoadCheck = new QCheckBox(tr("Title-loaded"), this);
	m_titleLoadCheck->setEnabled(false);
	m_subtitleCheck = new QCheckBox(tr("Found Subtitle"), this);
	m_posterCheck = new QCheckBox(tr("Found Poster"), this);
	//TODO: actions for checking subtitle and poster check to actually do it
	connect(video, SIGNAL(jobCompleted(Video::Jobs,bool)), this, SLOT(jobCompleted(Video::Jobs,bool)));
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
