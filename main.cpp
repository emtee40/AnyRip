#include "mainwindow.h"
#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(QLatin1String("AnyRip"));
	a.setOrganizationName(QLatin1String("AnyClip"));
	a.setOrganizationDomain(QLatin1String("anyclip.com"));
	MainWindow mw;
	mw.show();
	return a.exec();
}
