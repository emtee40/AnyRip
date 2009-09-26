#ifndef DVDIMAGEJOB_H
#define DVDIMAGEJOB_H

#include "job.h"
class DVDDrive;
class QIODevice;

class DVDImageJob : public Job
{
	Q_OBJECT
public:
	DVDImageJob(Video *video, QString defaultPath);
	bool saveImageToDevice(QIODevice &out);
	bool saveImageToPath(const QString &path);
	Video::Jobs jobType() const;
	QWidget* gui();

private:
	static int cmpvob(const void *p1, const void *p2);
	typedef struct vobfile {
		int32_t start, end;
	} vobfile;
	DVDDrive *m_dvdDrive;
	QString m_defaultPath;

protected:
	bool executeJob();

signals:
	void extractProgress(int current, int total);
};

#endif // DVDIMAGEJOB_H
