#include "job.h"
#include "video.h"
#include <QtConcurrentRun>

Job::Job(QObject *parent)
		: QObject(parent),
		m_gui(0)
{
	m_watcher = new QFutureWatcher<bool>;
	m_watcher->setParent(this);
	connect(m_watcher, SIGNAL(finished()), this, SLOT(jobFinished()));
}
void Job::runJob()
{
	m_watcher->setFuture(QtConcurrent::run(this, &Job::executeJob));
}
void Job::jobFinished()
{
	emit completed(m_watcher->future().result());
}
QWidget* Job::widget()
{
	if (!m_gui)
		m_gui = gui();
	return m_gui;
}
