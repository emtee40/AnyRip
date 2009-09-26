#include "video.h"
#include "job.h"
#include "dvdimagejob.h"
#include "encodemp4job.h"
#include "uploadjob.h"
#include "titleloadjob.h"
#include "videogui.h"
#include "dvddrive.h"
#include <QDir>
#include <QSettings>

Video::Video(QString title, QObject *parent) :
		QObject(parent),
		m_jobsCompleted(QBitArray(6)),
		m_jobsInProgress(QBitArray(6))
{
	m_settingsKey = QString("Videos/%1/Jobs Completed").arg(title.replace(QChar('/'), QChar('-')));
	QSettings settings;
	if (settings.contains(m_settingsKey)) {
		m_jobsCompleted = settings.value(m_settingsKey).toBitArray();
		if (m_jobsCompleted.size() != 6)
			m_jobsCompleted.resize(6);
	}
	m_title = title;
	m_rootPath = QString("%1/AnyRip/%2").arg(QDir::homePath()).arg(m_title);
	QDir().mkpath(m_rootPath);
	m_imagePath = QString("%1/%2 - Image.iso").arg(m_rootPath).arg(m_title);
	m_encodePath = QString("%1/%2 - Encode.mp4").arg(m_rootPath).arg(m_title);
	m_subtitlePath = QString("%1/%2 - Subtitles.srt").arg(m_rootPath).arg(m_title);
	m_posterPath = QString("%1/%2 - Poster.jpg").arg(m_rootPath).arg(m_title);
}
void Video::saveState()
{
	QSettings settings;
	settings.setValue(m_settingsKey, m_jobsCompleted);
}
void Video::completedJob(bool success)
{
	Job *job = qobject_cast<Job*>(sender());
	qDebug() << "Job code" << job->jobType() << "completed" << success;
	m_jobsCompleted.setBit(job->jobType(), success);
	saveState();
	m_jobsInProgress.setBit(job->jobType(), false);
	emit jobCompleted(job->jobType(), success);
	job->deleteLater();
}
DVDImageJob* Video::dvdImageJob()
{
	if (!m_jobsInProgress.at(Video::DVDImage) && !m_jobsCompleted.at(Video::DVDImage) && DVDDrive::instance()->dvdInserted()) {
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
	saveState();
	m_jobsInProgress.setBit(Video::Subtitle, false);
	emit jobCompleted(Video::Subtitle, true);
}
void Video::setPoster(const QIODevice &input)
{
	m_jobsInProgress.setBit(Video::Poster, true);
	//TODO: save input to subtitle file
	m_jobsCompleted.setBit(Video::Poster, true);
	saveState();
	m_jobsInProgress.setBit(Video::Poster, false);
	emit jobCompleted(Video::Poster, true);
}
QString Video::title() const
{
	return m_title;
}
VideoGui* Video::widget()
{
	return new VideoGui(this);
}
bool Video::isJobCompleted(Video::Jobs job) const
{
	return m_jobsCompleted.at(job);
}
