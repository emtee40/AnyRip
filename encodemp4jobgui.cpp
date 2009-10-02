#include "encodemp4jobgui.h"
#include "encodemp4job.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStyle>

EncodeMP4JobGui::EncodeMP4JobGui(EncodeMP4Job *job)
{
	m_progressBar = new QProgressBar;
	m_progressLabel = new QLabel;
	QPushButton *cancel = new QPushButton(style()->standardIcon(QStyle::SP_DialogCancelButton), QString());
	connect(cancel, SIGNAL(clicked()), job, SLOT(terminate()));
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *progressCancel = new QHBoxLayout;
	progressCancel->addWidget(m_progressBar);
	progressCancel->addWidget(cancel);
	layout->addLayout(progressCancel);
	layout->addWidget(m_progressLabel);
	setLayout(layout);
	setTitle(tr("Encoding MP4 of %1").arg(job->video()->title()));
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	connect(job, SIGNAL(encodeProgress(int,int,float,float,float,QTime)), this, SLOT(encodeProgress(int,int,float,float,float,QTime)));
}
void EncodeMP4JobGui::encodeProgress(int currentTask, int totalTasks, float percent, float currentFps, float avgFps, QTime timeRemaining)
{
	m_progressBar->setValue((int)(percent * 100) + (currentTask - 1) * 10000);
	m_progressBar->setMaximum(10000 * totalTasks);
	if (!timeRemaining.isNull() && avgFps != -1 && currentFps != -1)
		m_progressLabel->setText(tr("task %1 of %2, %4 fps (avg %5 fps), %3 remaining")
								 .arg(QString::number(currentTask))
								 .arg(QString::number(totalTasks))
								 .arg(timeRemaining.toString())
								 .arg(QString::number(currentFps, 'f', 2))
								 .arg(QString::number(avgFps, 'f', 2)));
	else
		m_progressLabel->setText(tr("task %1 of %2")
								 .arg(QString::number(currentTask))
								 .arg(QString::number(totalTasks)));
}
