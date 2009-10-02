#ifndef VIDEOGUI_H
#define VIDEOGUI_H

#include <QGroupBox>
#include "video.h"
class StatusLabel;
class QComboBox;
class Video;

class VideoGui : public QGroupBox
{
	Q_OBJECT
public:
	VideoGui(Video *video);
private:
	StatusLabel *m_imageStatus;
	StatusLabel *m_encodeStatus;
	StatusLabel *m_uploadStatus;
	StatusLabel *m_titleLoadStatus;
	StatusLabel *m_subtitleStatus;
	StatusLabel *m_posterStatus;
	QComboBox *m_titleSelector;
	Video *m_video;
private slots:
	void jobCompleted(Video::Jobs jobType, bool success);
	void titleChanged(int index);
};

#endif // VIDEOGUI_H
