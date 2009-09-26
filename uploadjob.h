#ifndef UPLOADJOB_H
#define UPLOADJOB_H

#include "job.h"
#include "video.h"

class UploadJob : public Job
{
	Q_OBJECT
public:
	UploadJob(Video *video, QString encodePath);
	Video::Jobs jobType() const;
protected:
	bool executeJob();
	QWidget* gui();
private:
	QString m_encodePath;
};

#endif // UPLOADJOB_H
