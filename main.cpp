#include "dvddrive.h"
#include "videogui.h"
#include "video.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QLatin1String("AnyRip"));
	a.setOrganizationName(QLatin1String("AnyClip"));
	a.setOrganizationDomain(QLatin1String("anyclip.com"));
	DVDDrive drive;
	Video v(drive.dvdName());
	v.gui()->show();
	return a.exec();
}
