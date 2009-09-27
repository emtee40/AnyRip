#include "uploadjob.h"
#include <QWidget>
#include <QLabel>

UploadJob::UploadJob(Video *video) :
		Job(video, false)
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
void UploadJob::terminate()
{
	//TODO: implement terminating
}
