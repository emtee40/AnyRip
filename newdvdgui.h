#ifndef NEWDVDGUI_H
#define NEWDVDGUI_H

#include <QGroupBox>
#include <QMap>
class QLineEdit;
class QLabel;
class QPushButton;
class TitleLoader;

class NewDVDGui : public QGroupBox
{
	Q_OBJECT
public:
	NewDVDGui();
private:
	QLineEdit *m_dvdName;
	QLabel *m_status;
	QMap<int, QString> m_titles;
	const QString m_insertDvd;
	const QString m_loadingTitles;
	const QString m_ripping;
	QPushButton *m_rip;
	QPushButton *m_eject;
	TitleLoader *m_titleLoader;
private slots:
	void dvdAdded();
	void dvdRemoved();
	void validateName(const QString &name);
	void rip();
	void loadedTitles(QMap<int, QString> titles);
signals:
	void newDVD(QString name, QMap<int, QString> titles);
};

#endif // NEWDVDGUI_H
