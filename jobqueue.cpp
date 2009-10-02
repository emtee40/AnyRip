#include "jobqueue.h"
#include "job.h"

#include <QDebug>

JobQueue::JobQueue(QObject *parent) :
		QObject(parent),
		m_jobIsRunning(false),
		m_currentJob(0)
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
	m_currentJob = job;
	qDebug() << "running job" << job->jobType() << "for video" << qobject_cast<Video*>(job->parent())->title();
	connect(job, SIGNAL(completed(bool)), this, SLOT(jobCompleted()));
	emit runningJob(job);
	job->runJob();
}
void JobQueue::jobCompleted()
{
	Job *job = qobject_cast<Job*>(sender());
	if (job == m_currentJob) {
		m_jobIsRunning = false;
		m_currentJob = 0;
		runNextJob();
	} else
		m_queue.removeAll(job);
}
