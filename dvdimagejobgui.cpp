#include "dvdimagejobgui.h"
#include "dvdimagejob.h"
#include "dvddrive.h"
#include <dvdcss/dvdcss.h>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>

DVDImageJobGui::DVDImageJobGui(DVDImageJob *job) :
		m_first(true)
{
	m_progressBar = new QProgressBar;
	m_progressLabel = new QLabel;
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(new QLabel(tr("<b>Copying DVD ISO</b> of <i>%1</i>").arg(job->video()->title())));
	layout->addWidget(m_progressBar);
	layout->addWidget(m_progressLabel);
	setLayout(layout);
	connect(job, SIGNAL(extractProgress(int,int)), this, SLOT(extractProgress(int,int)));
}
void DVDImageJobGui::extractProgress(int current, int maximum)
{
	if (m_first) {
		m_startTime.start();
		m_first = false;
	} else {
		long long currentBytes = (long long)current * DVDCSS_BLOCK_SIZE;
		long long maximumBytes = (long long)maximum * DVDCSS_BLOCK_SIZE;
		double bytesPerMillisecond = (double)currentBytes / (double)m_startTime.elapsed();
		long long millisecondsRemaining = (long long)((maximumBytes - currentBytes) / bytesPerMillisecond);
		m_progressLabel->setText(QString("%1 of %2 megabytes (%3 mb/s, %4 remaining)")
							.arg(QString::number((double)currentBytes / 1024.0 / 1024.0, 'f', 2))
							.arg(QString::number((double)maximumBytes / 1024.0 / 1024.0, 'f', 2))
							.arg(QString::number(bytesPerMillisecond * (1000.0 / 1024.0 / 1024.0), 'f', 2))
							.arg(QTime().addMSecs(millisecondsRemaining).toString()));
	}
	m_progressBar->setMaximum(maximum);
	m_progressBar->setValue(current);
}
