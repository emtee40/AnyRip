#ifndef DVDDRIVE_H
#define DVDDRIVE_H

#include <QObject>
class QDBusInterface;

class DVDDrive : public QObject
{
	Q_OBJECT
public:
	DVDDrive(QObject *parent = 0);
	QString dvdName() const;
	QString dvdDevice() const;
	bool dvdInserted() const;
	void eject();
private:
	QDBusInterface *m_halManager;
	QDBusInterface *m_halDisc;
private slots:
	bool deviceAdded(const QString &device);
	void deviceRemoved(const QString &device);
signals:
	void dvdAdded();
	void dvdRemoved();
};

#endif // DVDDRIVE_H
