#ifndef DVDIMAGEJOBGUI_H
#define DVDIMAGEJOBGUI_H

#include <QGroupBox>
#include <QTime>
class QLabel;
class QProgressBar;
class DVDImageJob;

class DVDImageJobGui : public QGroupBox
{
	Q_OBJECT

public:
	DVDImageJobGui(DVDImageJob *job);

private:
	QProgressBar* m_progressBar;
	QLabel* m_progressLabel;
	QTime m_startTime;
	bool m_first;
	DVDImageJob *m_job;

private slots:
	void extractProgress(int current, int maximum);
};

#endif // DVDIMAGEJOBGUI_H
