#include "video.h"
#include "job.h"
#include "dvdimagejob.h"

Video::Video() : m_jobsCompleted(QBitArray(5))
{
	//Do something
}
void Video::completedJob(bool success)
{
	Job *job = qobject_cast<Job*>(sender());
	qDebug() << "Job code" << job->jobType() << "completed" << success;
	m_jobsCompleted.setBit(job->jobType(), success);
	delete job;

	if (success) {
		//queue next job function
		;
	} else {
		//failure mechanism
		;
	}
}
Job* Video::nextJob()
{
	Job *job;
	if (!m_jobsCompleted.at(Video::DVDImage)) {
		job = new DVDImageJob(this);
	} else {
		return job = 0; //Other jobs...
	}
	connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
	return job;
}
