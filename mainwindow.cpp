#include "mainwindow.h"
#include "video.h"
#include "videogui.h"
#include "videoqueue.h"
#include "dvddrive.h"
#include "job.h"
#include "newdvdgui.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow()
{
	m_queue = new VideoQueue;
	connect(m_queue, SIGNAL(runningJob(Job*)), this, SLOT(runningJob(Job*)));
	m_videoGuis = new QVBoxLayout;
	m_jobGuis = new QVBoxLayout;
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(new QLabel(tr("<b>Video Queue</b>")), 0, 0);
	layout->addWidget(new QLabel(tr("<b>Job Queue</b>")), 0, 1);
	layout->addLayout(m_videoGuis, 1, 0);
	layout->addLayout(m_jobGuis, 1, 1);
	NewDVDGui *newDvdGui = new NewDVDGui;
	connect(newDvdGui, SIGNAL(newDVD(QString,QMap<int,QString>)), this, SLOT(newVideoFromDVD(QString,QMap<int,QString>)));
	m_jobGuis->addWidget(newDvdGui);
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	foreach(QString title, settings.childGroups()) {
		Video *video = new Video(title, this);
		if (video->isJobCompleted(Video::DVDImage))
			addVideo(video);
		else
			settings.remove(title);
	}
	setLayout(layout);
}
void MainWindow::addVideo(Video *video)
{
	m_videoGuis->addWidget(video->widget());
	m_queue->newVideo(video);
}
void MainWindow::newVideoFromDVD(QString name, QMap<int, QString> titles)
{
	Video *video = new Video(name);
	video->setDvdTitles(titles);
	addVideo(video);
}
void MainWindow::runningJob(Job *job)
{
	connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
	m_jobGuis->addWidget(job->widget());
}
void MainWindow::completedJob(bool success)
{
	//TODO: do something with [lack of] success
	delete qobject_cast<Job*>(sender())->widget();
}
