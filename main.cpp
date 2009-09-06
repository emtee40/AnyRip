#include "imagegui.h"
#include "dvddrive.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DVDDrive drive;
	drive.eject();
	a.setApplicationName(QLatin1String("AnyRip"));
	a.setOrganizationName(QLatin1String("AnyClip"));
	a.setOrganizationDomain(QLatin1String("anyclip.com"));
	ImageGui i;
	i.show();
	return a.exec();
}
