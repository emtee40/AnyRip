#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QBitArray>

class Job;
class DVDDrive;
class Video : public QObject
{
	Q_OBJECT
public:
	Video(DVDDrive *dvdDrive);
	enum Jobs { DVDImage, EncodeMP4, Upload, Subtitle, Poster };
	Job* nextJob();
	bool isJobComplete(Jobs job) const;
private:
	QBitArray m_jobsCompleted;
	QString m_title;
	DVDDrive *m_dvdDrive;
private slots:
	void completedJob(bool success);
};

#endif // VIDEO_H
