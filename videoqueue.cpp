#include "videoqueue.h"
#include "video.h"
#include "jobqueue.h"
#include "job.h"
#include <QDebug>

VideoQueue::VideoQueue(QObject *parent) :
		QObject(parent),
		m_jobQueues(QVector<JobQueue*>(4))
{
	m_jobQueues.replace(Video::DVDImage, new JobQueue(this));
	m_jobQueues.replace(Video::EncodeMP4, new JobQueue(this));
	m_jobQueues.replace(Video::Upload, new JobQueue(this));
	m_jobQueues.replace(Video::TitleLoad, new JobQueue(this));
	foreach (JobQueue *queue, m_jobQueues)
		connect(queue, SIGNAL(runningJob(Job*)), this, SIGNAL(runningJob(Job*)));
}
void VideoQueue::newVideo(Video *video)
{
	connect(video, SIGNAL(jobCompleted(Video::Jobs,bool)), this, SLOT(videoChanged()));
	enqueueVideo(video);
}
void VideoQueue::enqueueVideo(Video *video)
{
	foreach(Job *job, video->availableJobs()) {
		qDebug() << "enqueueing job type" << job->jobType() << "for video" << video->title();
		m_jobQueues.at(job->jobType())->addJob(job);
	}
}
void VideoQueue::videoChanged()
{
	enqueueVideo(qobject_cast<Video*>(sender()));
}
