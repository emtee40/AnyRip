#include "mainwindow.h"
#include "video.h"
#include "videogui.h"
#include "videoqueue.h"
#include "dvddrive.h"
#include "job.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow()
{
	connect(DVDDrive::instance(), SIGNAL(dvdAdded()), this, SLOT(dvdAdded()));
	connect(DVDDrive::instance(), SIGNAL(dvdRemoved()), this, SLOT(dvdRemoved()));
	m_queue = new VideoQueue;
	connect(m_queue, SIGNAL(runningJob(Job*)), this, SLOT(runningJob(Job*)));
	m_videoGuis = new QVBoxLayout;
	m_jobGuis = new QVBoxLayout;
	QHBoxLayout *sides = new QHBoxLayout;
	sides->addLayout(m_videoGuis);
	sides->addLayout(m_jobGuis);
	QHBoxLayout *heading = new QHBoxLayout;
	heading->addWidget(new QLabel(tr("<b>Videos in Queue</b>")), 1);
	m_currentlyInserted = new QPushButton;
	connect(m_currentlyInserted, SIGNAL(clicked()), this, SLOT(newVideoFromDVD()));
	if (DVDDrive::instance()->dvdInserted())
		dvdAdded();
	else
		dvdRemoved();
	heading->addWidget(m_currentlyInserted);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(heading);
	layout->addLayout(sides);
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	foreach(QString title, settings.childGroups()) {
		Video *video = new Video(title, this);
		if (video->isJobCompleted(Video::DVDImage))
			addVideo(video);
		else
			settings.remove(QString("Videos/%1").arg(title));
	}
	setLayout(layout);
}
void MainWindow::dvdAdded()
{
	QSettings settings;
	settings.beginGroup(QLatin1String("Videos"));
	QString dvdName = DVDDrive::instance()->dvdName();
	m_currentlyInserted->setEnabled(!settings.childGroups().contains(dvdName));
	m_currentlyInserted->setText(tr("Rip %1").arg(dvdName));
}
void MainWindow::dvdRemoved()
{
	m_currentlyInserted->setEnabled(false);
	m_currentlyInserted->setText(tr("Insert DVD..."));
}
void MainWindow::addVideo(Video *video)
{
	m_videoGuis->addWidget(video->widget());
	m_queue->newVideo(video);
}
void MainWindow::newVideoFromDVD()
{
	m_currentlyInserted->setEnabled(false);
	addVideo(new Video(DVDDrive::instance()->dvdName(), this));
}
void MainWindow::runningJob(Job *job)
{
	connect(job, SIGNAL(completed(bool)), this, SLOT(completedJob(bool)));
	m_jobGuis->addWidget(job->widget());
}
void MainWindow::completedJob(bool success)
{
	//TODO: do something with success
	delete qobject_cast<Job*>(sender())->widget();
}
