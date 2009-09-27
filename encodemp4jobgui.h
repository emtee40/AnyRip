#ifndef ENCODEMP4JOBGUI_H
#define ENCODEMP4JOBGUI_H

#include <QFrame>
#include <QTime>
class QLabel;
class QProgressBar;
class EncodeMP4Job;

class EncodeMP4JobGui : public QFrame
{
	Q_OBJECT

public:
	EncodeMP4JobGui(EncodeMP4Job *job);

private:
	QProgressBar* m_progressBar;
	QLabel* m_progressLabel;
	EncodeMP4Job *m_job;

private slots:
	void encodeProgress(int currentTask, int totalTasks, float percent, float currentFps, float avgFps, QTime timeRemaining);
};

#endif // ENCODEMP4JOBGUI_H
