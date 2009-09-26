#ifndef ENCODEMP4JOB_H
#define ENCODEMP4JOB_H

#include "job.h"
#include "video.h"

class EncodeMP4Job : public Job
{
	Q_OBJECT
public:
	EncodeMP4Job(Video *video, QString encodePath, QString imagePath);
	Video::Jobs jobType() const;
protected:
	bool executeJob();
	QWidget* gui();
private:
	QString m_encodePath;
	QString m_imagePath;
};

#endif // ENCODEMP4JOB_H
