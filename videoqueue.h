#ifndef VIDEOQUEUE_H
#define VIDEOQUEUE_H

#include <QObject>
#include <QVector>
class JobQueue;
class Video;
class Job;

class VideoQueue : public QObject
{
	Q_OBJECT
public:
	VideoQueue(QObject *parent = 0);
	void newVideo(Video *video);
private:
	QVector<JobQueue*> m_jobQueues;
	void enqueueVideo(Video *video);
private slots:
	void videoChanged();
signals:
	void runningJob(Job *job);
};

#endif // VIDEOQUEUE_H
