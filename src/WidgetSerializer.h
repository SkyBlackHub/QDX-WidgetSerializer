#ifndef QDX_WIDGETSERIALIZER_H
#define QDX_WIDGETSERIALIZER_H

class QObject;
class QSettings;
class QWidget;
class QMainWindow;
class QCheckBox;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QDoubleSpinBox;
class QLineEdit;
class QTabWidget;
class QSplitter;
class QComboBox;
class QMenu;
class QStackedWidget;
class QAction;
class QActionGroup;

#include <QString>

namespace QDX {

	class SerializableWidget;

	class WidgetSerializer
	{
	public:
		WidgetSerializer(QSettings &settings);
		virtual ~WidgetSerializer();

		static constexpr const char* SERIALIZABLE = "serializable";
		static constexpr const char* HISTORY = "history";
		static constexpr const char* CASCADABLE = "cascadable";

		QSettings &settings() const;

		virtual bool save(QCheckBox *widget, const QString &name = QString()) const;
		virtual bool load(QCheckBox *widget, const QString &name = QString()) const;

		virtual bool save(QPushButton *widget, const QString &name = QString()) const;
		virtual bool load(QPushButton *widget, const QString &name = QString()) const;

		virtual bool save(QRadioButton *widget, const QString &name = QString()) const;
		virtual bool load(QRadioButton *widget, const QString &name = QString()) const;

		virtual bool save(QSpinBox *widget, const QString &name = QString()) const;
		virtual bool load(QSpinBox *widget, const QString &name = QString()) const;

		virtual bool save(QDoubleSpinBox *widget, const QString &name = QString()) const;
		virtual bool load(QDoubleSpinBox *widget, const QString &name = QString()) const;

		virtual bool save(QLineEdit *widget, const QString &name = QString()) const;
		virtual bool load(QLineEdit *widget, const QString &name = QString()) const;

		virtual bool save(QTabWidget *widget, const QString &name = QString()) const;
		virtual bool load(QTabWidget *widget, const QString &name = QString()) const;

		virtual bool save(QSplitter *widget, const QString &name = QString()) const;
		virtual bool load(QSplitter *widget, const QString &name = QString()) const;

		virtual bool save(QAction *action, const QString &name = QString()) const;
		virtual bool load(QAction *action, const QString &name = QString()) const;

		virtual bool save(QActionGroup *group, const QString &name = QString()) const;
		virtual bool load(QActionGroup *group, const QString &name = QString()) const;

		virtual bool save(QComboBox *widget, const QString &name = QString()) const;
		virtual bool load(QComboBox *widget, const QString &name = QString()) const;

		virtual bool save(SerializableWidget *widget, const QString &name = QString()) const;
		virtual bool load(SerializableWidget *widget, const QString &name = QString()) const;

		virtual bool save(QWidget *widget, const QString &name = QString(), bool *casted = nullptr) const;
		virtual bool load(QWidget *widget, const QString &name = QString(), bool *casted = nullptr) const;

		virtual bool save(QObject *object, const QString &name = QString(), bool *casted = nullptr) const;
		virtual bool load(QObject *object, const QString &name = QString(), bool *casted = nullptr) const;

		virtual bool saveCascade(QObject *object, const QString &group_name = QString()) const;
		virtual bool loadCascade(QObject *object, const QString &group_name = QString()) const;

		virtual bool saveChildren(QObject *object, const QString &group_name = QString()) const;
		virtual bool loadChildren(QObject *object, const QString &group_name = QString()) const;

		virtual bool saveWindow(QWidget *widget) const;
		virtual bool loadWindow(QWidget *widget) const;

		bool omitHistory() const;
		void setOmitHistory(bool omit_history);

		bool omitWindow() const;
		void setOmitWindow(bool omit_window);

		static void disableSerialization(QWidget *widget);
		static void disableSerialization(const QList<QWidget *> &widgets);

		static void enableSerialization(QWidget *widget);
		static void enableSerialization(const QList<QWidget *> &widgets);

		static void toggleSerialization(QWidget *widget, bool serializable);
		static void toggleSerialization(const QList<QWidget *> &widgets, bool serializable);

		static void disableCascadable(QWidget *widget);
		static void disableCascadable(const QList<QWidget *> &widgets);

		static void enableCascadable(QWidget *widget);
		static void enableCascadable(const QList<QWidget *> &widgets);

		static void toggleCascadable(QWidget *widget, bool cascadable);
		static void toggleCascadable(const QList<QWidget *> &widgets, bool cascadable);

		static void enableHistory(QWidget *widget, int limit = 100);
		static void enableHistory(const QList<QWidget *> &widgets, int limit = 100);

		static void disableHistory(QWidget *widget);
		static void disableHistory(const QList<QWidget *> &widgets);

		static int historyLimit(QWidget *widget);

	private:
		QSettings &m_settings;

		bool m_omit_history = false;
		bool m_omit_window = false;

		bool performCascade(QObject *object, bool is_load, const QString &group_name) const;
		bool performCascade(QObject *object, bool is_load) const;
		bool performChildren(QObject *object, bool is_load, const QString &group_name) const;
		bool performChildren(QObject *object, bool is_load) const;
	};

} // namespace QDX

#endif // QDX_WIDGETSERIALIZER_H
