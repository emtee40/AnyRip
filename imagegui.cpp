#include "imagegui.h"
#include "dvdimage.h"
#include <dvdcss/dvdcss.h>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QFile>
#include <QtConcurrentRun>

ImageGui::ImageGui()
{
	DVDImage *dvdImage = new DVDImage;
	m_first = true;
	connect(dvdImage, SIGNAL(extractProgress(int,int)), this, SLOT(extractProgress(int,int)));
	QtConcurrent::run(dvdImage, &DVDImage::saveImageToPath, QLatin1String("/dev/dvd"), QLatin1String("image.iso"));
}
void ImageGui::extractProgress(int current, int maximum)
{
	if (m_first) {
		m_startTime.start();
		m_first = false;
	} else {
		long long currentBytes = (long long)current * DVDCSS_BLOCK_SIZE;
		long long maximumBytes = (long long)maximum * DVDCSS_BLOCK_SIZE;
		double bytesPerMillisecond = (double)currentBytes / (double)m_startTime.elapsed();
		long long millisecondsRemaining = (long long)((maximumBytes - currentBytes) / bytesPerMillisecond);
		setWindowTitle(QString("AnyRip: %1 of %2 megabytes (%3 mb/s, %4 remaining)")
							 .arg(QString::number((double)currentBytes / 1024.0 / 1024.0, 'f', 2))
							 .arg(QString::number((double)maximumBytes / 1024.0 / 1024.0, 'f', 2))
							 .arg(QString::number(bytesPerMillisecond * (1000.0 / 1024.0 / 1024.0), 'f', 2))
							 .arg(QTime().addMSecs(millisecondsRemaining).toString()));
	}
	setMaximum(maximum);
	setValue(current);
}
