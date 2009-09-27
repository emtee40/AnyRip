#include "encodemp4job.h"
#include "encodemp4jobgui.h"
#include <QWidget>
#include <QLabel>
#include <QProcess>
#include <QStringList>
#include <QRegExp>
#include <QTime>

EncodeMP4Job::EncodeMP4Job(Video *video, QString encodePath, QString imagePath) :
		Job(video),
		m_encodePath(encodePath),
		m_imagePath(imagePath)
{
}
Video::Jobs EncodeMP4Job::jobType() const
{
	return Video::EncodeMP4;
}
bool EncodeMP4Job::executeJob()
{
	QProcess process;
	QStringList arguments;
	arguments << "-i" << m_imagePath;
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
	//process.setStandardErrorFile("/home/zx2c4/Desktop/error.log");
	//process.setStandardOutputFile("/home/zx2c4/Desktop/out.log");
	process.start(QLatin1String("./HandBrakeCLI"), arguments, QIODevice::ReadOnly);
	if (!process.waitForStarted())
		return false;
	QRegExp percentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) % (\\(([0-9]*\\.[0-9]*) fps, avg ([0-9]*\\.[0-9]*) fps, ETA ([0-9]{2})h([0-9]{2})m([0-9]{2})s\\))?"));
	QRegExp lessPercentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) %"));
	while (process.waitForReadyRead(-1)) {
		QString line = QString(process.readLine()).trimmed();
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
	process.waitForFinished(-1);
	return process.exitStatus() == QProcess::NormalExit;
}
QWidget* EncodeMP4Job::gui()
{
	return new EncodeMP4JobGui(this);
}
QMap<int, QString> EncodeMP4Job::titles() const
{
	return titles(m_encodePath);
}
QMap<int, QString> EncodeMP4Job::titles(const QString &location)
{
	QMap<int, QString> titles;
	QProcess process;
	QStringList arguments;
	arguments << "-i" << location;
	arguments << "-t" << "0";
	arguments << "--dvdnav";
	process.start(QLatin1String("./HandBrakeCLI"), arguments, QIODevice::ReadOnly);
	if (!process.waitForStarted())
		return titles;
	process.waitForFinished(-1);
	QRegExp titleDurationPattern(QLatin1String("\\+ title ([0-9]*):\\n[^\\n]*\\n[^\\n]*\\n  \\+ duration: ([0-9]{2}:[0-9]{2}:[0-9]{2})"));
	QString output(process.readAllStandardError());
	int matchLocation = 0;
	while ((matchLocation = titleDurationPattern.indexIn(output, matchLocation)) != -1) {
		matchLocation += titleDurationPattern.matchedLength();
		titles.insert(titleDurationPattern.cap(1).toInt(), titleDurationPattern.cap(2));
	}
	return titles;
}
