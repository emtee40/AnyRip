#include "encodemp4jobgui.h"
#include "encodemp4job.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

EncodeMP4JobGui::EncodeMP4JobGui(EncodeMP4Job *job)
{
	m_progressBar = new QProgressBar;
	m_progressBar->setMaximum(10000);
	m_progressLabel = new QLabel;
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel(tr("<b>Encoding MP4</b> of <i>%1</i>").arg(job->video()->title())));
	layout->addWidget(m_progressBar);
	layout->addWidget(m_progressLabel);
	setLayout(layout);
	setFrameStyle(QFrame::StyledPanel);
	connect(job, SIGNAL(encodeProgress(int,int,float,float,float,QTime)), this, SLOT(encodeProgress(int,int,float,float,float,QTime)));
}
void EncodeMP4JobGui::encodeProgress(int currentTask, int totalTasks, float percent, float currentFps, float avgFps, QTime timeRemaining)
{
	m_progressBar->setValue((int)(percent * 100));
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
