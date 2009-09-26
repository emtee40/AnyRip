#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QBitArray>
#include <QList>

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
	DVDImageJob* dvdImageJob();
	EncodeMP4Job* encodeMP4Job();
	UploadJob* uploadJob();
	TitleLoadJob* titleLoadJob();
	QList<Job*> availableJobs();
	QString title() const;
	VideoGui* gui();
private:
	QBitArray m_jobsCompleted;
	QBitArray m_jobsInProgress;
	QString m_title;
	QString m_rootPath;
	QString m_imagePath;
	QString m_encodePath;
	QString m_subtitlePath;
	QString m_posterPath;
private slots:
	void completedJob(bool success);
signals:
	void jobCompleted(Video::Jobs job, bool success);
};

#endif // VIDEO_H
