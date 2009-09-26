#include "uploadjob.h"
#include <QWidget>
#include <QLabel>

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
	sleep(5);
	return true;
}
QWidget* UploadJob::gui()
{
	//TODO: make a gui
	return new QLabel("Endoplasmic sludge in your upload house");
}
