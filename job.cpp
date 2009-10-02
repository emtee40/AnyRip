#include "job.h"
#include "video.h"
#include <QtConcurrentRun>

Job::Job(Video *parent, bool threaded)
		: QObject(parent),
		m_gui(0),
		m_video(parent),
		m_terminated(false)
{
	if (threaded) {
		m_watcher = new QFutureWatcher<bool>;
		m_watcher->setParent(this);
		connect(m_watcher, SIGNAL(finished()), this, SLOT(jobFinished()));
	} else
		m_watcher = 0;
}
Video* Job::video() const
{
	return m_video;
}
void Job::runJob()
{
	m_terminated = false;
	if (m_watcher)
		m_watcher->setFuture(QtConcurrent::run(this, &Job::executeJob));
	else
		executeJob();
}
void Job::jobFinished()
{
	if (!m_terminated)
		emit completed(m_watcher->future().result());
}
QWidget* Job::widget()
{
	if (!m_gui)
		m_gui = gui();
	return m_gui;
}
QFutureWatcher<bool>* Job::watcher() const
{
	return m_watcher;
}
void Job::terminate()
{
	m_terminated = true;
	kill();
	emit completed(false);
}
bool Job::terminated() const
{
	return m_terminated;
}
