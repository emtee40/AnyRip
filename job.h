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
	bool terminated() const;
protected:
	Job(Video *parent, bool threaded);
	virtual bool executeJob() = 0;
	virtual QWidget* gui() = 0;
	virtual void kill() = 0;
	QFutureWatcher<bool>* watcher() const;
private:
	QFutureWatcher<bool> *m_watcher;
	QWidget *m_gui;
	Video *m_video;
	bool m_terminated;
private slots:
	void jobFinished();
public slots:
	void terminate();
signals:
	void completed(bool result);
};

#endif // JOB_H
