#ifndef DVDIMAGEJOB_H
#define DVDIMAGEJOB_H

#include "job.h"
class DVDDrive;
class QIODevice;

class DVDImageJob : public Job
{
	Q_OBJECT
public:
	DVDImageJob(Video *video, DVDDrive *dvdDrive);
	bool saveImageToDevice(QIODevice &out);
	bool saveImageToPath(const QString &path);
	Video::Jobs jobType();

private:
	static int cmpvob(const void *p1, const void *p2);
	typedef struct vobfile {
		int32_t start, end;
	} vobfile;
	DVDDrive *m_dvdDrive;

protected:
	bool executeJob();

signals:
	void extractProgress(int current, int total);
};

#endif // DVDIMAGEJOB_H
