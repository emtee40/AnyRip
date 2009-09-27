#ifndef ENCODEMP4JOB_H
#define ENCODEMP4JOB_H

#include "job.h"
#include "video.h"
#include <QTime>
#include <QMap>
#include <QProcess>

class EncodeMP4Job : public Job
{
	Q_OBJECT
public:
	EncodeMP4Job(Video *video);
	~EncodeMP4Job();
	Video::Jobs jobType() const;
protected:
	bool executeJob();
	QWidget* gui();
private:
	QProcess *m_process;
	QString m_encodePath;
private slots:
	void finished(int exitCode, QProcess::ExitStatus exitStats);
	void readyRead();
public slots:
	void terminate();
signals:
	void encodeProgress(int currentTask, int totalTasks, float percent, float currentFps, float avgFps, QTime timeRemaining);
};

#endif // ENCODEMP4JOB_H
