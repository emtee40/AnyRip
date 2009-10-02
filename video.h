#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QBitArray>
#include <QList>
#include <QMap>
#include <QMultiHash>

class QIODevice;
class DVDImageJob;
class EncodeMP4Job;
class UploadJob;
class TitleLoadJob;
class Job;
class VideoGui;

class Video : public QObject
{
	Q_OBJECT
public:
	Video(QString title, QObject *parent = 0);
	enum Jobs { DVDImage, EncodeMP4, Upload, TitleLoad, Subtitle, Poster };
	void setSubtitle(const QIODevice &input);
	void setPoster(const QIODevice &input);
	void setDvdImage(const QString &path);
	DVDImageJob* dvdImageJob();
	EncodeMP4Job* encodeMP4Job();
	UploadJob* uploadJob();
	TitleLoadJob* titleLoadJob();
	QList<Job*> availableJobs();
	QString title() const;
	int dvdTitle() const;
	void setDvdTitle(int title);
	VideoGui* widget();
	bool isJobCompleted(Jobs job) const;
	void setDvdTitles(QMap<int, QString> titles);
	QMap<int, QString> dvdTitles() const;
	QString imagePath() const;
	QString encodePath() const;
	QString subtitlePath() const;
	QString posterPath() const;
private:
	QBitArray m_jobsCompleted;
	QMultiHash<Jobs, Job*> m_jobsInProgress;
	QString m_title;
	QString m_rootPath;
	QString m_imagePath;
	QString m_encodePath;
	QString m_subtitlePath;
	QString m_posterPath;
	QString m_settingsKey;
	QMap<int, QString> m_dvdTitles;
	int m_dvdTitle;
	void saveState();
	void terminateJobs(Jobs jobType);
private slots:
	void completedJob(bool success);
signals:
	void jobCompleted(Video::Jobs jobType, bool success);
};

#endif // VIDEO_H
