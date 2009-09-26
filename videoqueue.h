#ifndef VIDEOQUEUE_H
#define VIDEOQUEUE_H

#include <QObject>
#include <QVector>
class JobQueue;
class Video;

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
};

#endif // VIDEOQUEUE_H
