#include "encodemp4job.h"
#include <QWidget>
#include <QLabel>
#include <QProcess>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>

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
	arguments << "-E" << "faac";
	arguments << "-B" << "96";
	arguments << "-R" << "auto";
	arguments << "-6" << "stereo";
	arguments << "-N" << "eng" << "--native-dub";
	arguments << "-f" << "mp4";
	arguments << "-P" << "-2" << "-T";
	arguments << "--optimize" << "--decomb" << "--deblock" << "--denoise=\"weak\"";
	arguments << "-x" << "ref=3:mixed-refs:bframes=6:weightb:direct=auto:b-pyramid:me=umh:subme=9:analyse=all:8x8dct:trellis=1:no-fast-pskip:psy-rd=1,1";
	process.start(QLatin1String("HandBrakeCLI"), arguments, QIODevice::ReadOnly);
	if (!process.waitForStarted())
		return false;
	QTextStream reader(&process);
	QRegExp percentLinePattern(QLatin1String("^Encoding: task ([0-9]*) of ([0-9]*), ([0-9]*\\.[0-9]*) %( \\(([0-9]*\\.[0-9]*) fps, avg ([0-9]*\\.[0-9]*) fps, ETA ([0-9]{2})h([0-9]{2})m([0-9]{2})s\\))?"));
	while (process.waitForReadyRead(-1)) {
		if (percentLinePattern.exactMatch(reader.readLine())) {
			qDebug() << percentLinePattern.capturedTexts(); //cap(n)
		}
	}
	return process.waitForFinished(-1);
}
QWidget* EncodeMP4Job::gui()
{
	//TODO: make a gui
	return new QLabel("Encode mp4 place holder, foo!");
}
