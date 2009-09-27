#ifndef NEWDVDGUI_H
#define NEWDVDGUI_H

#include <QFrame>
#include <QMap>
#include <QFutureWatcher>
class QLineEdit;
class QLabel;
class QPushButton;

class NewDVDGui : public QFrame
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
	QFutureWatcher<QMap<int, QString> > *m_watcher;
private slots:
	void dvdAdded();
	void dvdRemoved();
	void validateName(const QString &name);
	void rip();
	void loadedTitles();
signals:
	void newDVD(QString name, QMap<int, QString> titles);
};

#endif // NEWDVDGUI_H
