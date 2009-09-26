#ifndef TITLELOADJOB_H
#define TITLELOADJOB_H

#include "job.h"
#include "video.h"

class TitleLoadJob : public Job
{
	Q_OBJECT
public:
	TitleLoadJob(Video *video, QString subtitlePath, QString posterPath);
	Video::Jobs jobType() const;
	QWidget* gui();
protected:
	bool executeJob();
private:
	QString m_subtitlePath;
	QString m_posterPath;
};

#endif // TITLELOADJOB_H
