#ifndef UPLOADJOB_H
#define UPLOADJOB_H

#include "job.h"
#include "video.h"

class UploadJob : public Job
{
	Q_OBJECT
public:
	UploadJob(Video *video);
	Video::Jobs jobType() const;
protected:
	bool executeJob();
	QWidget* gui();
public slots:
	void kill();
};

#endif // UPLOADJOB_H
