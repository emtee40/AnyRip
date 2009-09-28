#include "mainwindow.h"
#include "video.h"
#include "videogui.h"
#include "videoqueue.h"
#include "dvddrive.h"
#include "job.h"
#include "newdvdgui.h"
#include "newimagegui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QDebug>

MainWindow::MainWindow()
{
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	//We delay adding because addVideo needs things to be contructed.
	//We need to remove bad entries though because other constructors here rely on our settings
	QList<Video*> toAdd;
	foreach(QString title, settings.childGroups()) {
		Video *video = new Video(title, this);
		if (video->isJobCompleted(Video::DVDImage))
			toAdd.append(video);
		else
			settings.remove(title);
	}
	m_queue = new VideoQueue;
	connect(m_queue, SIGNAL(runningJob(Job*)), this, SLOT(runningJob(Job*)));
	m_videoGuis = new QVBoxLayout;
	m_jobGuis = new QVBoxLayout;
	QGroupBox *videoGuiBox = new QGroupBox;
	videoGuiBox->setTitle(tr("Video Queue"));
	videoGuiBox->setLayout(m_videoGuis);
	QGroupBox *jobsGuiBox = new QGroupBox;
	jobsGuiBox->setTitle(tr("Job Queue"));
	jobsGuiBox->setLayout(m_jobGuis);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(videoGuiBox);
	layout->addWidget(jobsGuiBox);
	NewDVDGui *newDvdGui = new NewDVDGui;
	connect(newDvdGui, SIGNAL(newDVD(QString,QMap<int,QString>)), this, SLOT(newVideoFromDVD(QString,QMap<int,QString>)));
	m_jobGuis->addWidget(newDvdGui, 0, Qt::AlignTop);
	NewImageGui *newImageGui = new NewImageGui;
	connect(newImageGui, SIGNAL(newImage(QString,QString,QMap<int,QString>)), this, SLOT(newVideoFromImage(QString,QString,QMap<int,QString>)));
	m_jobGuis->addWidget(newImageGui, 0, Qt::AlignTop);
	foreach (Video *video, toAdd)
		addVideo(video);
	setLayout(layout);
}
void MainWindow::addVideo(Video *video)
{
	m_videoGuis->addWidget(video->widget(), 0, Qt::AlignTop);
	m_queue->newVideo(video);
}
void MainWindow::newVideoFromDVD(const QString &name, QMap<int, QString> titles) //TODO: use const QString &name
{
	Video *video = new Video(name, this);
	video->setDvdTitles(titles);
	addVideo(video);
}
void MainWindow::newVideoFromImage(const QString &path, const QString &name, QMap<int, QString> titles)
{
	Video *video = new Video(name, this);
	video->setDvdImage(path);
	video->setDvdTitles(titles);
	addVideo(video);
}
void MainWindow::runningJob(Job *job)
{
	connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
	m_jobGuis->addWidget(job->widget(), 0, Qt::AlignTop);
}
void MainWindow::completedJob(bool success)
{
	//TODO: do something with [lack of] success
	delete qobject_cast<Job*>(sender())->widget();
}
