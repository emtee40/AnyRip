#ifndef DVDIMAGE_H
#define DVDIMAGE_H

#include <QObject>
class QIODevice;

class DVDImage : public QObject
{
	Q_OBJECT
public:
	bool saveImageToDevice(const QString &dvdDevice, QIODevice &out);
	bool saveImageToPath(const QString &dvdDevice, const QString &path);

private:
	static int cmpvob(const void *p1, const void *p2);
	typedef struct vobfile {
		int32_t start, end;
	} vobfile;

signals:
	void extractProgress(int current, int total);
};

#endif // DVDIMAGE_H
