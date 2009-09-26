#include "video.h"
#include "job.h"
#include "dvdimagejob.h"
#include "encodemp4job.h"
#include "uploadjob.h"
#include "titleloadjob.h"

#include <QDir>

Video::Video(QString title, QObject *parent) :
		QObject(parent),
		m_jobsCompleted(QBitArray(6)),
		m_jobsInProgress(QBitArray(6))
{
	m_title = title;
	m_rootPath = QString("%1/AnyRip/%2").arg(QDir::homePath()).arg(m_title);
	m_imagePath = QString("%1/%2 - Image.iso").arg(m_rootPath).arg(m_title);
	m_encodePath = QString("%1/%2 - Encode.mp4").arg(m_rootPath).arg(m_title);
	m_subtitlePath = QString("%1/%2 - Subtitles.srt").arg(m_rootPath).arg(m_title);
	m_posterPath = QString("%1/%2 - Poster.jpg").arg(m_rootPath).arg(m_title);
}
void Video::completedJob(bool success)
{
	Job *job = qobject_cast<Job*>(sender());
	qDebug() << "Job code" << job->jobType() << "completed" << success;
	m_jobsCompleted.setBit(job->jobType(), success);
	m_jobsInProgress.setBit(job->jobType(), false);
	emit jobCompleted(job->jobType(), success);
	delete job;
}
DVDImageJob* Video::dvdImageJob()
{
	if (!m_jobsInProgress.at(Video::DVDImage) && !m_jobsCompleted.at(Video::DVDImage)) {
		DVDImageJob *job = new DVDImageJob(this, m_imagePath);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::DVDImage, true);
		return job;
	}
	return 0;
}
EncodeMP4Job* Video::encodeMP4Job()
{
	if (!m_jobsInProgress.at(Video::EncodeMP4) && !m_jobsCompleted.at(Video::EncodeMP4) && m_jobsCompleted.at(Video::DVDImage)) {
		EncodeMP4Job *job = new EncodeMP4Job(this, m_encodePath, m_imagePath);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::EncodeMP4, true);
		return job;
	}
	return 0;
}
UploadJob* Video::uploadJob()
{
	if (!m_jobsInProgress.at(Video::Upload) && !m_jobsCompleted.at(Video::Upload) && m_jobsCompleted.at(Video::EncodeMP4)) {
		UploadJob *job = new UploadJob(this, m_encodePath);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::Upload, true);
		return job;
	}
	return 0;
}
TitleLoadJob* Video::titleLoadJob()
{
	if (!m_jobsInProgress.at(Video::TitleLoad) && !m_jobsCompleted.at(Video::TitleLoad) && m_jobsCompleted.at(Video::Upload) && m_jobsCompleted.at(Video::Subtitle) && m_jobsCompleted.at(Video::Poster)) {
		TitleLoadJob *job = new TitleLoadJob(this, m_subtitlePath, m_posterPath);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::TitleLoad, true);
		return job;
	}
	return 0;
}
QList<Job*> Video::availableJobs()
{
	QList<Job*> jobs;
	Job *job;
	if ((job = dvdImageJob()))
		jobs.append(job);
	if ((job = encodeMP4Job()))
		jobs.append(job);
	if ((job = uploadJob()))
		jobs.append(job);
	if ((job = titleLoadJob()))
		jobs.append(job);
	return jobs;
}
void Video::setSubtitle(const QIODevice &input)
{
	m_jobsInProgress.setBit(Video::Subtitle, true);
	//TODO: save input to subtitle file
	m_jobsCompleted.setBit(Video::Subtitle, true);
	m_jobsInProgress.setBit(Video::Subtitle, false);
	emit jobCompleted(Video::Subtitle, true);
}
void Video::setPoster(const QIODevice &input)
{
	m_jobsInProgress.setBit(Video::Poster, true);
	//TODO: save input to subtitle file
	m_jobsCompleted.setBit(Video::Poster, true);
	m_jobsInProgress.setBit(Video::Poster, false);
	emit jobCompleted(Video::Poster, true);
}
QString Video::title() const
{
	return m_title;
}
