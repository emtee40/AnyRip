#include "encodemp4job.h"
#include <QWidget>
#include <QLabel>

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
	sleep(5);
	return true;
}
QWidget* EncodeMP4Job::gui()
{
	//TODO: make a gui
	return new QLabel("Encode mp4 place holder, foo!");
}
