#ifndef TITLELOADER_H
#define TITLELOADER_H

#include <QObject>
#include <QMap>
#include <QProcess>

class TitleLoader : public QObject
{
	Q_OBJECT
public:
	TitleLoader(const QString &location, QObject *parent = 0);
	TitleLoader(QObject *parent = 0);
	void loadTitles();
	void setLocation(const QString &location);
private:
	QString m_location;
	QProcess *m_process;
private slots:
	void finished(int exitCode ,QProcess::ExitStatus exitStatus);
public slots:
	void terminate();
signals:
	void loadedTitles(QMap<int, QString> titles);
};

#endif // TITLELOADER_H
