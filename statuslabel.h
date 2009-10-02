#ifndef STATUSLABEL_H
#define STATUSLABEL_H

#include <QWidget>
class QLabel;

class StatusLabel : public QWidget
{
public:
	StatusLabel(const QString &text, QWidget *parent = 0);
	void setCompleted(bool complete);
private:
	QLabel *m_pixmap;
};

#endif // STATUSLABEL_H
