#ifndef NEWIMAGEGUI_H
#define NEWIMAGEGUI_H

#include <QGroupBox>
#include <QMap>
class QLineEdit;
class QPushButton;
class QLabel;
class TitleLoader;

class NewImageGui : public QGroupBox
{
	Q_OBJECT
public:
	NewImageGui();
private:
	QLineEdit *m_dvdName;
	QLabel *m_status;
	QPushButton *m_importImageButton;
	QPushButton *m_browseButton;
	TitleLoader *m_titleLoader;
	QString m_path;
	QMap<int, QString> m_titles;
	void initialState();
private slots:
	void validateName(const QString &name);
	void browse();
	void importImage();
	void loadedTitles(QMap<int, QString> titles);
signals:
	void newImage(const QString &path, const QString &name, QMap<int, QString> titles);
};

#endif // NEWIMAGEGUI_H
