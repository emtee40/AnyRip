#ifndef VIDEO_H
#define VIDEO_H


#include <QObject>
#include <QBitArray>

class Job;
class Video : public QObject
{
	Q_OBJECT
public:
    Video();
	enum Jobs { DVDImage, EncodeMP4, Upload, Subtitle, Poster };
	Job* nextJob();
private:
	QBitArray m_jobsCompleted;
private slots:
	void completedJob(bool success);
};

#endif // VIDEO_H
