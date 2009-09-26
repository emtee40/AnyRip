#ifndef VIDEOGUI_H
#define VIDEOGUI_H

#include <QWidget>
#include "video.h"
class QCheckBox;
class Video;

class VideoGui : public QWidget
{
	Q_OBJECT
public:
	VideoGui(Video *video);
private:
	QCheckBox *m_imageCheck;
	QCheckBox *m_encodeCheck;
	QCheckBox *m_uploadCheck;
	QCheckBox *m_titleLoadCheck;
	QCheckBox *m_subtitleCheck;
	QCheckBox *m_posterCheck;
	Video *m_video;
private slots:
	void jobCompleted(Video::Jobs jobType, bool success);
};

#endif // VIDEOGUI_H
