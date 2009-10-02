#include "encodemp4job.h"
#include "encodemp4jobgui.h"
#include <QWidget>
#include <QLabel>
#include <QStringList>
#include <QRegExp>
#include <QTime>
#include <QFile>

EncodeMP4Job::EncodeMP4Job(Video *video) :
		Job(video, false),
		m_process(0)
{
}
EncodeMP4Job::~EncodeMP4Job()
{
	disconnect(this, 0, 0, 0);
	kill();
}
Video::Jobs EncodeMP4Job::jobType() const
{
	return Video::EncodeMP4;
}
bool EncodeMP4Job::executeJob()
{
	m_process = new QProcess(this);
	connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error()));
	connect(m_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
	connect(m_process, SIGNAL(readyRead()), this, SLOT(readyRead()));
	QStringList arguments;
	arguments << "-i" << video()->imagePath();
	m_encodePath = video()->encodePath();
	arguments << "-o" << m_encodePath;
	arguments << "-e" << "x264";
	arguments << "-b" << "500";
	arguments << "-2" << "-T";
	arguments << "-E" << "faac";
	arguments << "-B" << "96";
	arguments << "-R" << "auto";
	arguments << "-6" << "stereo";
	arguments << "-N" << "eng" << "--native-dub";
	arguments << "-f" << "mp4";
	arguments << "--dvdnav" << "-t" << QString::number(video()->dvdTitle());
	arguments << "--loose-anamorphic" << "--modulus" << "16";
	arguments << "--optimize" << "--decomb" << "--deblock" << "--denoise=\"weak\"";
	arguments << "-x" << "ref=3:mixed-refs:bframes=6:weightb:direct=auto:b-pyramid:me=umh:subme=9:analyse=all:8x8dct:trellis=1:no-fast-pskip:psy-rd=1,1";
	qDebug() << "starting handbreak with arguments:" << arguments;
	//m_process->setStandardErrorFile("/home/zx2c4/Desktop/error.log");
	//m_process->setStandardOutputFile("/home/zx2c4/Desktop/out.log");
	m_process->start(QLatin1String("./HandBrakeCLI"), arguments, QIODevice::ReadOnly);
	return true;
}
void EncodeMP4Job::readyRead()
{
	static const QRegExp percentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) % (\\(([0-9]*\\.[0-9]*) fps, avg ([0-9]*\\.[0-9]*) fps, ETA ([0-9]{2})h([0-9]{2})m([0-9]{2})s\\))?"));
	static const QRegExp lessPercentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) %"));
	QByteArray byteLine;
	while (!(byteLine = m_process->readLine()).isEmpty()) {
		QString line = QString(byteLine).trimmed();
		if (percentLinePattern.exactMatch(line)) {
			int currentTask = percentLinePattern.cap(1).toInt();
			int totalTasks = percentLinePattern.cap(2).toInt();
			float percent = percentLinePattern.cap(3).toFloat();
			float currentFps = percentLinePattern.cap(5).toFloat();
			float avgFps = percentLinePattern.cap(6).toFloat();
			QTime timeRemaining(percentLinePattern.cap(7).toInt(), percentLinePattern.cap(8).toInt(), percentLinePattern.cap(9).toInt());
			emit encodeProgress(currentTask, totalTasks, percent, currentFps, avgFps, timeRemaining);
		} else if(lessPercentLinePattern.exactMatch(line)) {
			int currentTask = lessPercentLinePattern.cap(1).toInt();
			int totalTasks = lessPercentLinePattern.cap(2).toInt();
			float percent = lessPercentLinePattern.cap(3).toFloat();
			emit encodeProgress(currentTask, totalTasks, percent, -1, -1, QTime());
		}
	}
}
void EncodeMP4Job::kill()
{
	if (m_process) {
		disconnect(m_process, 0, 0, 0);
		m_process->terminate();
		m_process->deleteLater();
		m_process = 0;
		QFile::remove(m_encodePath);
	}
}
void EncodeMP4Job::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (!m_process) return;
	if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
		disconnect(m_process, 0, 0, 0);
		m_process->terminate();
		m_process->deleteLater();
		m_process = 0;
		emit completed(true);
	}
	else
		terminate();
}
void EncodeMP4Job::error()
{
	kill();
	emit completed(false);
}
QWidget* EncodeMP4Job::gui()
{
	return new EncodeMP4JobGui(this);
}
