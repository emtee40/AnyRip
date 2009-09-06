#ifndef IMAGEGUI_H
#define IMAGEGUI_H

#include <QProgressBar>
#include <QTime>
class DVDDrive;

class ImageGui : public QProgressBar
{
	Q_OBJECT

public:
    ImageGui();

private:
	QTime m_startTime;
	bool m_first;
	DVDDrive *m_dvdDrive;

private slots:
	void extractProgress(int current, int maximum);
	void startImaging();
};

#endif // IMAGEGUI_H
