#include "video.h"
#include "job.h"
#include "dvdimagejob.h"
#include "encodemp4job.h"
#include "uploadjob.h"
#include "titleloadjob.h"
#include "videogui.h"
#include "dvddrive.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>

Video::Video(QString title, QObject *parent) :
		QObject(parent),
		m_jobsCompleted(QBitArray(6)),
		m_jobsInProgress(QBitArray(6)),
		m_dvdTitle(1)
{
	title.replace(QChar('/'), QChar('-'));
	m_settingsKey = QString("Videos/%1/%2").arg(title);
	QSettings settings;
	settings.beginGroup("Videos");
	if (settings.childGroups().contains(title)) {
		settings.endGroup();
		m_jobsCompleted = settings.value(m_settingsKey.arg("Jobs Completed")).toBitArray();
		if (m_jobsCompleted.size() != 6)
			m_jobsCompleted.resize(6);
		m_dvdTitle = settings.value(m_settingsKey.arg("DVD Title"), 1).toInt();
		settings.beginGroup(m_settingsKey.arg("DVD Titles"));
		foreach(QString titleNumber, settings.childKeys())
			m_dvdTitles.insert(titleNumber.toInt(), settings.value(titleNumber).toString());
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
	settings.setValue(m_settingsKey.arg("Jobs Completed"), m_jobsCompleted);
	settings.setValue(m_settingsKey.arg("DVD Title"), m_dvdTitle);
	settings.remove(m_settingsKey.arg("DVD Titles"));
	QMapIterator<int, QString> i(m_dvdTitles);
	while (i.hasNext()) {
		i.next();
		settings.setValue(QString("%1/%2").arg(m_settingsKey.arg("DVD Titles")).arg(QString::number(i.key())), i.value());
	}
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
		DVDImageJob *job = new DVDImageJob(this);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::DVDImage, true);
		return job;
	}
	return 0;
}
EncodeMP4Job* Video::encodeMP4Job()
{
	if (!m_jobsInProgress.at(Video::EncodeMP4) && !m_jobsCompleted.at(Video::EncodeMP4) && m_jobsCompleted.at(Video::DVDImage)) {
		EncodeMP4Job *job = new EncodeMP4Job(this);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::EncodeMP4, true);
		return job;
	}
	return 0;
}
UploadJob* Video::uploadJob()
{
	if (!m_jobsInProgress.at(Video::Upload) && !m_jobsCompleted.at(Video::Upload) && m_jobsCompleted.at(Video::EncodeMP4)) {
		UploadJob *job = new UploadJob(this);
		connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
		m_jobsInProgress.setBit(Video::Upload, true);
		return job;
	}
	return 0;
}
TitleLoadJob* Video::titleLoadJob()
{
	if (!m_jobsInProgress.at(Video::TitleLoad) && !m_jobsCompleted.at(Video::TitleLoad) && m_jobsCompleted.at(Video::Upload) && m_jobsCompleted.at(Video::Subtitle) && m_jobsCompleted.at(Video::Poster)) {
		TitleLoadJob *job = new TitleLoadJob(this);
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
void Video::setDvdImage(const QString &path)
{
	m_jobsInProgress.setBit(Video::DVDImage, true);
	QFileInfo image(m_imagePath);
	if (image.exists() && image != QFileInfo(path))
		QFile::remove(m_imagePath);
	QFile::rename(path, m_imagePath);
	m_jobsCompleted.setBit(Video::DVDImage, true);
	saveState();
	m_jobsInProgress.setBit(Video::DVDImage, false);
	emit jobCompleted(Video::DVDImage, true);
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
int Video::dvdTitle() const
{
	return m_dvdTitle;
}
void Video::setDvdTitle(int title)
{
	if (m_dvdTitle == title)
		return;
	m_dvdTitle = title;
	if (m_jobsCompleted.at(Video::EncodeMP4)) {
		m_jobsCompleted.setBit(Video::EncodeMP4, false);
		emit jobCompleted(Video::EncodeMP4, false);
		if(m_jobsCompleted.at(Video::Upload)) {
			m_jobsCompleted.setBit(Video::Upload, false);
			emit jobCompleted(Video::Upload, false);
		}
		//TODO: if its already encoding or uploading, cancel those jobs
	}
	saveState();
}
QMap<int, QString> Video::dvdTitles() const
{
	return m_dvdTitles;
}
void Video::setDvdTitles(QMap<int, QString> titles)
{
	m_dvdTitles = titles;
	saveState();
}
QString Video::imagePath() const
{
	return m_imagePath;
}
QString Video::encodePath() const
{
	return m_encodePath;
}
QString Video::posterPath() const
{
	return m_posterPath;
}
QString Video::subtitlePath() const
{
	return m_subtitlePath;
}
