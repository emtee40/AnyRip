#include "ripper.h"
#include <dvdcss/dvdcss.h>
#include <QDebug>
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
	dvdcss_t dvdcss = dvdcss_open("/dev/dvd");
	if (!dvdcss)
		qDebug() << "failed to open dvd";
	else
		qDebug() << "opened dvd";
	const int blocksPerRead = 128;
	uint8_t p_data[DVDCSS_BLOCK_SIZE * (blocksPerRead + 1)];
	uint8_t *p_buffer = p_data + DVDCSS_BLOCK_SIZE - ((intptr_t)p_data & (DVDCSS_BLOCK_SIZE - 1));
	QFile out("out.iso");
	out.open(QIODevice::WriteOnly);
	int blocksOffset = 0;
	int blocksRead;
	int neededTitleKeys = 0;
	while ((blocksRead = dvdcss_read(dvdcss, p_buffer, blocksPerRead, DVDCSS_READ_DECRYPT)) > 0) {
		//Check if key retreval is neccessary for this block sector
		uint8_t *p_scan = p_buffer;
		bool neededAKey = false;
		for (int i = 0; i < blocksRead; ++i) {
			if(((uint8_t*)p_scan)[0x14] & 0x30) {
				dvdcss_seek(dvdcss, blocksOffset + i, DVDCSS_SEEK_KEY);
				++neededTitleKeys;
				neededAKey = true;
				qDebug() << "getting title key" << neededTitleKeys;
			}
			p_scan = p_scan + DVDCSS_BLOCK_SIZE;
		}
		//Reread once we've gotten a key
		if (neededAKey) {
			dvdcss_seek(dvdcss, blocksOffset, DVDCSS_NOFLAGS);
			dvdcss_read(dvdcss, p_buffer, blocksPerRead, DVDCSS_READ_DECRYPT);
		}
		out.write((char*)p_buffer, (quint64)(DVDCSS_BLOCK_SIZE * blocksRead));
		out.flush();
		blocksOffset += blocksRead;
	}
	dvdcss_close(dvdcss);
	out.close();
	return 0;




	QApplication a(argc, argv);
	a.setApplicationName(QLatin1String("AnyRip"));
	a.setOrganizationName(QLatin1String("AnyClip"));
	a.setOrganizationDomain(QLatin1String("anyclip.com"));
	Ripper w;
	w.show();
	return a.exec();
}
