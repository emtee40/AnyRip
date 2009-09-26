#include "uploadjob.h"
#include <QWidget>

UploadJob::UploadJob(Video *video, QString encodePath) :
		Job(video),
		m_encodePath(encodePath)
{
}
Video::Jobs UploadJob::jobType() const
{
	return Video::Upload;
}
bool UploadJob::executeJob()
{
	//TODO: upload the mp4!
	return true;
}
QWidget* UploadJob::gui()
{
	//TODO: make a gui
	return new QWidget;
}
