#include "encodemp4job.h"
#include "encodemp4jobgui.h"
#include <QWidget>
#include <QLabel>
#include <QProcess>
#include <QStringList>
#include <QTextStream>
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
	arguments << "--loose-anamorphic" << "--modulus" << "16";
	arguments << "--optimize" << "--decomb" << "--deblock" << "--denoise=\"weak\"";
	arguments << "-x" << "ref=3:mixed-refs:bframes=6:weightb:direct=auto:b-pyramid:me=umh:subme=9:analyse=all:8x8dct:trellis=1:no-fast-pskip:psy-rd=1,1";
	qDebug() << "starting handbreak with arguments:" << arguments;
	//process.setStandardErrorFile("/home/zx2c4/Desktop/error.log");
	//process.setStandardOutputFile("/home/zx2c4/Desktop/out.log");
	process.start(QLatin1String("./HandBrakeCLI"), arguments, QIODevice::ReadOnly);
	if (!process.waitForStarted())
		return false;
	QTextStream reader(&process);
	QRegExp percentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) % (\\(([0-9]*\\.[0-9]*) fps, avg ([0-9]*\\.[0-9]*) fps, ETA ([0-9]{2})h([0-9]{2})m([0-9]{2})s\\))?"));
	QRegExp lessPercentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) %"));
	while (process.waitForReadyRead(-1)) {
		QString line = reader.readLine().trimmed();
		if (percentLinePattern.exactMatch(line)) {
			QStringList captured = percentLinePattern.capturedTexts();
			int currentTask = captured.at(1).toInt();
			int totalTasks = captured.at(2).toInt();
			float percent = captured.at(3).toFloat();
			float currentFps = captured.at(5).toFloat();
			float avgFps = captured.at(6).toFloat();
			QTime timeRemaining(captured.at(7).toInt(), captured.at(8).toInt(), captured.at(9).toInt());
			emit encodeProgress(currentTask, totalTasks, percent, currentFps, avgFps, timeRemaining);
		} else if(lessPercentLinePattern.exactMatch(line)) {
			QStringList captured = lessPercentLinePattern.capturedTexts();
			int currentTask = captured.at(1).toInt();
			int totalTasks = captured.at(2).toInt();
			float percent = captured.at(3).toFloat();
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
