#ifndef IMAGEGUI_H
#define IMAGEGUI_H

#include <QProgressBar>
#include <QTime>

class ImageGui : public QProgressBar
{
	Q_OBJECT

public:
    ImageGui();

private:
	QTime m_startTime;
	bool m_first;

private slots:
	void extractProgress(int current, int maximum);
};

#endif // IMAGEGUI_H
