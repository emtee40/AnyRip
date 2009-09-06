#include "video.h"
#include "job.h"
#include "dvdimagejob.h"
#include "dvddrive.h"

Video::Video(DVDDrive *dvdDrive) :
		m_jobsCompleted(QBitArray(5)),
		m_dvdDrive(dvdDrive)
{
	m_title = dvdDrive->dvdName();
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
		job = new DVDImageJob(this, m_dvdDrive);
	} else {
		job = 0; //Other jobs...
	}
	connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
	return job;
}
bool Video::isJobComplete(Jobs job) const
{
	return m_jobsCompleted.at(job);
}
