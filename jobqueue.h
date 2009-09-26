#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <QObject>
#include <QQueue>
class Job;

class JobQueue : public QObject
{
	Q_OBJECT

public:
	JobQueue(QObject *parent = 0);
	void addJob(Job *job);
private:
	QQueue<Job*> m_queue;
	bool m_jobIsRunning;
	void runNextJob();
private slots:
	void jobCompleted();
};

#endif // JOBQUEUE_H
