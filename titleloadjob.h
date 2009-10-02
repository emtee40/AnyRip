#ifndef TITLELOADJOB_H
#define TITLELOADJOB_H

#include "job.h"
#include "video.h"

class TitleLoadJob : public Job
{
	Q_OBJECT
public:
	TitleLoadJob(Video *video);
	Video::Jobs jobType() const;
protected:
	bool executeJob();
	QWidget* gui();
public slots:
	void kill();
};

#endif // TITLELOADJOB_H
