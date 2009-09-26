#ifndef JOB_H
#define JOB_H

#include "video.h"
#include <QObject>
#include <QFutureWatcher>
class QWidget;

class Job : public QObject
{
	Q_OBJECT
public:
	void runJob();
	virtual Video::Jobs jobType() const = 0;
	QWidget* widget();
	Video* video() const;
protected:
	Job(Video *parent);
	virtual bool executeJob() = 0;
	virtual QWidget* gui() = 0;
private:
	QFutureWatcher<bool> *m_watcher;
	QWidget *m_gui;
	Video *m_video;
private slots:
	void jobFinished();
signals:
	void completed(bool result);
};

#endif // JOB_H
