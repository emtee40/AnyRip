#ifndef NEWIMAGEGUI_H
#define NEWIMAGEGUI_H

#include <QGroupBox>
class QLineEdit;
class QPushButton;

class NewImageGui : public QGroupBox
{
	Q_OBJECT
public:
	NewImageGui();
private:
	QLineEdit *m_dvdName;
	QLineEdit *m_imagePath;
	QPushButton *m_importImageButton;
private slots:
	void validatePath(const QString &path);
	void validateName(const QString &name);
	void browse();
	void importImage();
signals:
	void newImage(const QString &path, const QString &name);
};

#endif // NEWIMAGEGUI_H
