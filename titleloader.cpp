#include "titleloader.h"

TitleLoader::TitleLoader(const QString &location, QObject *parent) :
		QObject(parent),
		m_location(location)
{
}
TitleLoader::TitleLoader(QObject *parent) :
		QObject(parent),
		m_location(QString())
{
}
void TitleLoader::setLocation(const QString &location)
{
	m_location = location;
}
void TitleLoader::loadTitles()
{
	m_process = new QProcess(this);
	QStringList arguments;
	arguments << "-i" << m_location;
	arguments << "-t" << "0";
	arguments << "--dvdnav";
	connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(terminate()));
	connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
	m_process->start(QLatin1String("./HandBrakeCLI"), arguments, QIODevice::ReadOnly);
}
void TitleLoader::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	QMap<int, QString> titles;
	if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
		static const QRegExp titleDurationPattern(QLatin1String("\\+ title ([0-9]*):\\n[^\\n]*\\n[^\\n]*\\n  \\+ duration: ([0-9]{2}:[0-9]{2}:[0-9]{2})"));
		QString output(m_process->readAllStandardError());
		int matchLocation = 0;
		while ((matchLocation = titleDurationPattern.indexIn(output, matchLocation)) != -1) {
			matchLocation += titleDurationPattern.matchedLength();
			titles.insert(titleDurationPattern.cap(1).toInt(), titleDurationPattern.cap(2));
		}
	}
	disconnect(m_process, 0, 0, 0);
	delete m_process;
	m_process = 0;
	emit loadedTitles(titles);
}
void TitleLoader::terminate()
{
	if (m_process) {
		disconnect(m_process, 0, 0, 0);
		delete m_process;
		m_process = 0;
	}
	emit loadedTitles(QMap<int,QString>());
}
