#include "encodemp4job.h"
#include <QWidget>

EncodeMP4Job::EncodeMP4Job(Video *video, QString encodePath, QString imagePath) :
		Job(video),
		m_encodePath(encodePath),
		m_imagePath(imagePath)
{
}
Video::Jobs EncodeMP4Job::jobType() const
{
	return Video::EncodeMP4;
}
bool EncodeMP4Job::executeJob()
{
	//TODO: encode the actual mp4!
	return true;
}
QWidget* EncodeMP4Job::gui()
{
	//TODO: make a gui
	return new QWidget;
}
