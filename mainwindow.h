#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "dvddrive.h"
class QVBoxLayout;
class QPushButton;
class Video;
class VideoQueue;
class Job;

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow();
private:
	QVBoxLayout *m_videoGuis;
	QVBoxLayout *m_jobGuis;
	QPushButton *m_currentlyInserted;
	VideoQueue *m_queue;
	void addVideo(Video *video);
private slots:
	void dvdAdded();
	void dvdRemoved();
	void newVideoFromDVD();
	void runningJob(Job *job);
	void completedJob(bool success);
};

#endif // MAINWINDOW_H
