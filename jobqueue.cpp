#include "jobqueue.h"
#include "job.h"

#include <QDebug>

JobQueue::JobQueue(QObject *parent) :
		QObject(parent),
		m_jobIsRunning(false)
{
}
void JobQueue::addJob(Job *job)
{
	m_queue.enqueue(job);
	if (!m_jobIsRunning)
		runNextJob();
}
void JobQueue::runNextJob()
{
	if (m_queue.isEmpty())
		return;
	m_jobIsRunning = true;
	Job *job = m_queue.dequeue();
	qDebug() << "running job" << job->jobType() << "for video" << qobject_cast<Video*>(job->parent())->title();
	connect(job, SIGNAL(completed(bool)), this, SLOT(jobCompleted()));
	job->runJob();
}
void JobQueue::jobCompleted()
{
	m_jobIsRunning = false;
	runNextJob();
}
