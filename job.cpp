#include "job.h"
#include "video.h"
#include <QtConcurrentRun>

Job::Job(QObject *parent)
		: QObject(parent)
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
