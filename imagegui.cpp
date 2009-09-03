#include "imagegui.h"
#include "dvdimage.h"
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QFile>
#include <QtConcurrentRun>

ImageGui::ImageGui()
{
	DVDImage *dvdImage = new DVDImage;
	connect(dvdImage, SIGNAL(extractProgress(int,int)), this, SLOT(extractProgress(int,int)));
	QtConcurrent::run(dvdImage, &DVDImage::saveImageToPath, QLatin1String("/dev/dvd"), QLatin1String("image.iso"));
}
void ImageGui::extractProgress(int current, int maximum)
{
	setMaximum(maximum);
	setValue(current);
}
