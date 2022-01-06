#ifndef QDX_SERIALIZABLEWIDGET_H
#define QDX_SERIALIZABLEWIDGET_H

#include <QWidget>

class QSettings;

namespace QDX {

	class SerializableWidget : public QWidget
	{
		Q_OBJECT
	public:
		using QWidget::QWidget;
		virtual ~SerializableWidget() { }

		virtual void save(const QString &key, QSettings &settings) = 0;
		virtual void load(const QString &key, const QSettings &settings) = 0;
	};

}

#endif // QDX_SERIALIZABLEWIDGET_H
