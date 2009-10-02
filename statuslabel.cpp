#include "statuslabel.h"
#include <QStyle>
#include <QLabel>
#include <QHBoxLayout>

StatusLabel::StatusLabel(const QString &text, QWidget *parent) :
		QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout;
	m_pixmap = new QLabel;
	setCompleted(false);
	layout->addWidget(m_pixmap);
	layout->addWidget(new QLabel(text));
	setLayout(layout);
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}
void StatusLabel::setCompleted(bool complete)
{
	if (complete)
		m_pixmap->setPixmap(style()->standardIcon(QStyle::SP_DialogApplyButton).pixmap(QSize(16, 16)));
	else
		m_pixmap->setPixmap(style()->standardIcon(QStyle::SP_DialogCancelButton).pixmap(QSize(16, 16)));
}
