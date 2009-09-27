#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMap>
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
	VideoQueue *m_queue;
	void addVideo(Video *video);
private slots:
	void newVideoFromDVD(QString name, QMap<int, QString> titles);
	void runningJob(Job *job);
	void completedJob(bool success);
};

#endif // MAINWINDOW_H
