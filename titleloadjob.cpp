#include "titleloadjob.h"
#include <QWidget>
#include <QLabel>

TitleLoadJob::TitleLoadJob(Video *video) :
		Job(video, false)
{
}
Video::Jobs TitleLoadJob::jobType() const
{
	return Video::TitleLoad;
}
bool TitleLoadJob::executeJob()
{
	//TODO: title load the whole thing
	sleep(5);
	return true;
}
QWidget* TitleLoadJob::gui()
{
	//TODO: make a gui
	return new QLabel("Bet your ass this is title loading.");
}
void TitleLoadJob::terminate()
{
	//TODO: implement terminating
}
