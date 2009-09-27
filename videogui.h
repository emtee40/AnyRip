#ifndef VIDEOGUI_H
#define VIDEOGUI_H

#include <QGroupBox>
#include "video.h"
class QCheckBox;
class QComboBox;
class Video;

class VideoGui : public QGroupBox
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
	QComboBox *m_titleSelector;
	Video *m_video;
private slots:
	void jobCompleted(Video::Jobs jobType, bool success);
	void titleChanged(int index);
};

#endif // VIDEOGUI_H
