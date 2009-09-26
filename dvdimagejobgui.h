#ifndef DVDIMAGEJOBGUI_H
#define DVDIMAGEJOBGUI_H

#include <QProgressBar>
#include <QTime>
class DVDImageJob;

class DVDImageJobGui : public QProgressBar
{
	Q_OBJECT

public:
	DVDImageJobGui(DVDImageJob *job);

private:
	QTime m_startTime;
	bool m_first;
	DVDImageJob *m_job;

private slots:
	void extractProgress(int current, int maximum);
};

#endif // DVDIMAGEJOBGUI_H
