#include "titleloadjob.h"
#include <QWidget>

TitleLoadJob::TitleLoadJob(Video *video, QString subtitlePath, QString posterPath) :
		Job(video),
		m_subtitlePath(subtitlePath),
		m_posterPath(posterPath)
{
}
Video::Jobs TitleLoadJob::jobType() const
{
	return Video::TitleLoad;
}
bool TitleLoadJob::executeJob()
{
	//TODO: title load the whole thing
	return true;
}
QWidget* TitleLoadJob::gui()
{
	//TODO: make a gui
	return new QWidget;
}
