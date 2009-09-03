#ifndef IMAGEGUI_H
#define IMAGEGUI_H

#include <QProgressBar>
class DVDImage;

class ImageGui : public QProgressBar
{
	Q_OBJECT

public:
    ImageGui();

private slots:
	void extractProgress(int current, int maximum);
};

#endif // IMAGEGUI_H
