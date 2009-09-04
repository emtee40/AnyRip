#ifndef JOB_H
#define JOB_H

#include "video.h"
#include <QObject>
#include <QFutureWatcher>

class Job : public QObject
{
	Q_OBJECT
public:
	Job(QObject *parent = 0);
	void runJob();
	virtual Video::Jobs jobType() = 0;
protected:
	virtual bool executeJob() = 0;
private:
	QFutureWatcher<bool> *m_watcher;
private slots:
	void jobFinished();
signals:
	void completed(bool result);
};

#endif // JOB_H
