#include "WidgetSerializer.h"

#include <QSettings>
#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QSplitter>
#include <QComboBox>
#include <QMenu>
#include <QStackedWidget>

QDX::WidgetSerializer::WidgetSerializer(QSettings &settings) : m_settings(settings)
{

}

QDX::WidgetSerializer::~WidgetSerializer()
{

}

QSettings &QDX::WidgetSerializer::settings() const
{
	return m_settings;
}

#define validate(object, name) if (object == nullptr) { return false; } \
	QString key = name; \
	if (key.isEmpty()) { key = object->objectName(); if (key.isEmpty()) { return false; } } \

#define validate_contains(object, name) validate(object, name) \
	if (m_settings.contains(key) == false) { return false; }

bool QDX::WidgetSerializer::save(QCheckBox *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->isChecked());
	return true;
}

bool QDX::WidgetSerializer::load(QCheckBox *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->setChecked(m_settings.value(key).toBool());
	return true;
}

bool QDX::WidgetSerializer::save(QPushButton *widget, const QString &name) const
{
	validate(widget, name);
	if (widget->isCheckable() == false) {
		return false;
	}
	m_settings.setValue(key, widget->isChecked());
	return true;
}

bool QDX::WidgetSerializer::load(QPushButton *widget, const QString &name) const
{
	validate_contains(widget, name);
	if (widget->isCheckable() == false) {
		return false;
	}
	widget->setChecked(m_settings.value(key).toBool());
	return true;
}

bool QDX::WidgetSerializer::save(QRadioButton *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->isChecked());
	return true;
}

bool QDX::WidgetSerializer::load(QRadioButton *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->setChecked(m_settings.value(key).toBool());
	return true;
}

bool QDX::WidgetSerializer::save(QSpinBox *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->value());
	return true;
}

bool QDX::WidgetSerializer::load(QSpinBox *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->setValue(m_settings.value(key).toInt());
	return true;
}

bool QDX::WidgetSerializer::save(QDoubleSpinBox *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->value());
	return true;
}

bool QDX::WidgetSerializer::load(QDoubleSpinBox *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->setValue(m_settings.value(key).toDouble());
	return true;
}

bool QDX::WidgetSerializer::save(QLineEdit *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->text());
	return true;
}

bool QDX::WidgetSerializer::load(QLineEdit *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->setText(m_settings.value(key).toString());
	return true;
}

bool QDX::WidgetSerializer::save(QTabWidget *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->currentIndex());
	return true;
}

bool QDX::WidgetSerializer::load(QTabWidget *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->setCurrentIndex(m_settings.value(key).toInt());
	return true;
}

bool QDX::WidgetSerializer::save(QSplitter *widget, const QString &name) const
{
	validate(widget, name);
	m_settings.setValue(key, widget->saveState());
	return true;
}

bool QDX::WidgetSerializer::load(QSplitter *widget, const QString &name) const
{
	validate_contains(widget, name);
	widget->restoreState(m_settings.value(key).toByteArray());
	return true;
}

static QString &purifyActionName(QString &name)
{
	return name.remove(QRegularExpression("^action", QRegularExpression::CaseInsensitiveOption));
}

bool QDX::WidgetSerializer::save(QAction *action, const QString &name) const
{
	if (action == nullptr || action->isCheckable() == false) {
		return false;
	}
	validate(action, name);
	QString clean_key = key;
	purifyActionName(clean_key);
	m_settings.setValue(clean_key, action->isChecked());
	return true;
}

bool QDX::WidgetSerializer::load(QAction *action, const QString &name) const
{
	if (action == nullptr || action->isCheckable() == false) {
		return false;
	}
	validate(action, name);
	QString clean_key = key;
	purifyActionName(clean_key);
	if (m_settings.contains(clean_key)) {
		action->setChecked(m_settings.value(clean_key).toBool());
		return true;
	}
	return false;
}

bool QDX::WidgetSerializer::save(QActionGroup *group, const QString &name) const
{
	validate(group, name);
	purifyActionName(key);
	QString child_name;
	for (QAction *action : group->actions()) {
		if (action->isCheckable() == false) {
			continue;
		}
		child_name = action->objectName();
		purifyActionName(child_name);
		if (child_name.isEmpty()) {
			continue;
		}
		if (action->isChecked()) {
			m_settings.setValue(key, child_name);
			return true;
		}
	}
	return false;
}

bool QDX::WidgetSerializer::load(QActionGroup *group, const QString &name) const
{
	validate(group, name);
	purifyActionName(key);
	if (m_settings.contains(key) == false) {
		return false;
	}
	QString value = m_settings.value(key).toString();

	QString child_name;
	for (QAction *action : group->actions()) {
		if (action->isCheckable() == false) {
			continue;
		}
		child_name = action->objectName();
		purifyActionName(child_name);
		if (child_name.isEmpty()) {
			continue;
		}
		if (child_name == value) {
			action->setChecked(true);
			action->trigger();
			return true;
		}
	}
	return false;
}

bool QDX::WidgetSerializer::save(QComboBox *widget, const QString &name) const
{
	validate(widget, name);
	if (widget->isEditable()) {
		m_settings.setValue(key, widget->currentText());

		int count = widget->count();
		QStringList items;

		int history = historyLimit(widget);
		if (history > 0) {
			if (this->omitHistory()) {
				return true;
			}

			items.reserve(qMin(count + 1, history));
			QString text = widget->currentText();
			items.append(text);
			--history;
			for (int i = 0; i < count; ++i) {
				if (i >= history) {
					break;
				}
				QString item_text = widget->itemText(i);
				if (text.compare(item_text) != 0) {
					items.append(item_text);
				}
			}
		} else {
			items.reserve(count);
			for (int i = 0; i < count; ++i) {
				items.append(widget->itemText(i));
			}
		}

		m_settings.setValue(key + ".items", items);
	} else {
		m_settings.setValue(key, widget->currentIndex());
	}
	return true;
}

bool QDX::WidgetSerializer::load(QComboBox *widget, const QString &name) const
{
	validate(widget, name);
	if (widget->isEditable()) {
		const QString items_key = key + ".items";
		if (m_settings.contains(items_key)) {
			int history = historyLimit(widget);
			if (history > 0) {
				if (this->omitHistory() == false) {
					widget->clear();
					widget->addItems(m_settings.value(items_key).toStringList().mid(0, history));
				}
			} else {
				widget->clear();
				widget->addItems(m_settings.value(items_key).toStringList());
			}
		}
		if (m_settings.contains(key)) {
			widget->setCurrentText(m_settings.value(key).toString());
		}
	} else {
		if (m_settings.contains(key)) {
			widget->setCurrentIndex(m_settings.value(key).toInt());
		}
	}
	return true;
}

#define saveCast(type, object) if (type *casted_object = qobject_cast<type *>(object)) { if (casted) { *casted = true; } return this->save(casted_object, name); }
#define loadCast(type, object) if (type *casted_object = qobject_cast<type *>(object)) { if (casted) { *casted = true; } return this->load(casted_object, name); }

bool QDX::WidgetSerializer::save(QWidget *widget, const QString &name, bool *casted) const
{
	saveCast(QCheckBox, widget);
	saveCast(QPushButton, widget);
	saveCast(QRadioButton, widget);
	saveCast(QSpinBox, widget);
	saveCast(QDoubleSpinBox, widget);
	saveCast(QLineEdit, widget);
	saveCast(QTabWidget, widget);
	saveCast(QSplitter, widget);
	saveCast(QComboBox, widget);
	if (casted) {
		*casted = false;
	}
	return false;
}

bool QDX::WidgetSerializer::load(QWidget *widget, const QString &name, bool *casted) const
{
	loadCast(QCheckBox, widget);
	loadCast(QPushButton, widget);
	loadCast(QRadioButton, widget);
	loadCast(QSpinBox, widget);
	loadCast(QDoubleSpinBox, widget);
	loadCast(QLineEdit, widget);
	loadCast(QTabWidget, widget);
	loadCast(QSplitter, widget);
	loadCast(QComboBox, widget);
	if (casted) {
		*casted = false;
	}
	return false;
}

bool QDX::WidgetSerializer::save(QObject *object, const QString &name, bool *casted) const
{
	saveCast(QActionGroup, object);
	saveCast(QAction, object);
	if (object->isWidgetType()) {
		return this->save(qobject_cast<QWidget *>(object), name, casted);
	}
	if (casted) {
		*casted = false;
	}
	return false;
}

bool QDX::WidgetSerializer::load(QObject *object, const QString &name, bool *casted) const
{
	loadCast(QActionGroup, object);
	loadCast(QAction, object);
	if (object->isWidgetType()) {
		return this->load(qobject_cast<QWidget *>(object), name, casted);
	}
	if (casted) {
		*casted = false;
	}
	return false;
}

bool QDX::WidgetSerializer::saveCascade(QObject *object, const QString &group_name) const
{
	return this->performCascade(object, false, group_name);
}

bool QDX::WidgetSerializer::loadCascade(QObject *object, const QString &group_name) const
{
	return this->performCascade(object, true, group_name);
}

bool QDX::WidgetSerializer::saveChildren(QObject *object, const QString &group_name) const
{
	return this->performChildren(object, false, group_name);
}

bool QDX::WidgetSerializer::loadChildren(QObject *object, const QString &group_name) const
{
	return this->performChildren(object, true, group_name);
}

bool QDX::WidgetSerializer::performCascade(QObject *object, bool is_load, const QString &group_name) const
{
	if (object == nullptr) {
		return 0;
	}

	bool group_opened = false;

	QWidget *widget = qobject_cast<QWidget *>(object);

	if (group_name.isEmpty() == false || (widget && widget->isWindow())) {
		m_settings.beginGroup(group_name.isEmpty() ? object->objectName() : group_name);
		group_opened = true;
	}

	if (this->omitWindow() == false) {
		if (is_load) {
			this->loadWindow(widget);
		} else {
			this->saveWindow(widget);
		}
	}

	this->performCascade(object, is_load);

	if (group_opened) {
		m_settings.endGroup();
	}

	return true;
}

bool QDX::WidgetSerializer::performCascade(QObject *object, bool is_load) const
{
	if (is_load) {
		this->load(object);
	} else {
		this->save(object);
	}

	if (qobject_cast<QMenu *>(object)) {
		return true;
	}

	QVariant cascadable = object->property(CASCADABLE);
	if (cascadable.isValid() && cascadable.toBool() == false) {
		return true;
	}

	return this->performChildren(object, is_load);
}

bool QDX::WidgetSerializer::performChildren(QObject *object, bool is_load, const QString &group_name) const
{
	if (object == nullptr) {
		return 0;
	}

	bool group_opened = false;

	QWidget *widget = qobject_cast<QWidget *>(object);

	if (group_name.isEmpty() == false || (widget && widget->isWindow())) {
		m_settings.beginGroup(group_name.isEmpty() ? object->objectName() : group_name);
		group_opened = true;
	}

	this->performChildren(object, is_load);

	if (group_opened) {
		m_settings.endGroup();
	}

	return true;
}

bool QDX::WidgetSerializer::performChildren(QObject *object, bool is_load) const
{
	if (object == nullptr) {
		return false;
	}
	if (QTabWidget *tabs = qobject_cast<QTabWidget *>(object)) {
		return this->performChildren(tabs->findChild<QStackedWidget *>("qt_tabwidget_stackedwidget"), is_load);
	}
	QVariant serializable;
	for (QObject *child : object->children()) {
		if (child->objectName().startsWith("qt_")) {
			continue;
		}
		serializable = child->property(SERIALIZABLE);
		if (serializable.isValid() && serializable.toBool() == false) {
			continue;
		}

		this->performCascade(child, is_load);
	}
	return true;
}

bool QDX::WidgetSerializer::saveWindow(QWidget *widget) const
{
	if (widget == nullptr || widget->isWindow() == false) {
		return false;
	}
	if (widget->windowType() == Qt::Dialog) {
		m_settings.setValue("_position", widget->pos());
		m_settings.setValue("_size", widget->size());
	} else {
		m_settings.setValue("_geometry", widget->saveGeometry());
	}
	if (const QMainWindow *window = qobject_cast<const QMainWindow *>(widget)) {
		m_settings.setValue("_state", window->saveState());
	}
	return true;
}

bool QDX::WidgetSerializer::loadWindow(QWidget *widget) const
{
	if (widget == nullptr || widget->isWindow() == false) {
		return false;
	}
	if (widget->windowType() == Qt::Dialog) {
		if (m_settings.contains("_position")) {
			widget->move(m_settings.value("_position").toPoint());
		}
		if (m_settings.contains("_size")) {
			widget->resize(m_settings.value("_size").toSize());
		}
	} else {
		if (m_settings.contains("_geometry")) {
			widget->restoreGeometry(m_settings.value("_geometry").toByteArray());
		}
	}
	if (QMainWindow *window = qobject_cast<QMainWindow *>(widget)) {
		if (m_settings.contains("_state")) {
			window->restoreState(m_settings.value("_state").toByteArray());
		}
	}
	return true;
}

bool QDX::WidgetSerializer::omitHistory() const
{
	return m_omit_history;
}

void QDX::WidgetSerializer::setOmitHistory(bool omit_history)
{
	m_omit_history = omit_history;
}

bool QDX::WidgetSerializer::omitWindow() const
{
	return m_omit_window;
}

void QDX::WidgetSerializer::setOmitWindow(bool omit_window)
{
	m_omit_window = omit_window;
}

void QDX::WidgetSerializer::disableSerialization(QWidget *widget)
{
	toggleSerialization(widget, false);
}

void QDX::WidgetSerializer::disableSerialization(const QList<QWidget *> &widgets)
{
	toggleSerialization(widgets, false);
}

void QDX::WidgetSerializer::enableSerialization(QWidget *widget)
{
	toggleSerialization(widget, true);
}

void QDX::WidgetSerializer::enableSerialization(const QList<QWidget *> &widgets)
{
	toggleSerialization(widgets, true);
}

void QDX::WidgetSerializer::toggleSerialization(QWidget *widget, bool serializable)
{
	if (widget) {
		widget->setProperty(SERIALIZABLE, serializable);
	}
}

void QDX::WidgetSerializer::toggleSerialization(const QList<QWidget *> &widgets, bool serializable)
{
	for (QWidget *widget : widgets) {
		if (widget) {
			widget->setProperty(SERIALIZABLE, serializable);
		}
	}
}

void QDX::WidgetSerializer::disableCascadable(QWidget *widget)
{
	toggleCascadable(widget, false);
}

void QDX::WidgetSerializer::disableCascadable(const QList<QWidget *> &widgets)
{
 toggleCascadable(widgets, false);
}

void QDX::WidgetSerializer::enableCascadable(QWidget *widget)
{
	toggleCascadable(widget, true);
}

void QDX::WidgetSerializer::enableCascadable(const QList<QWidget *> &widgets)
{
	toggleCascadable(widgets, true);
}

void QDX::WidgetSerializer::toggleCascadable(QWidget *widget, bool cascadable)
{
	if (widget) {
		widget->setProperty(CASCADABLE, cascadable);
	}
}

void QDX::WidgetSerializer::toggleCascadable(const QList<QWidget *> &widgets, bool cascadable)
{
	for (QWidget *widget : widgets) {
		if (widget) {
			widget->setProperty(CASCADABLE, cascadable);
		}
	}
}

void QDX::WidgetSerializer::enableHistory(QWidget *widget, int limit)
{
	if (widget && limit > 0) {
		widget->setProperty(HISTORY, limit);
	}
}

void QDX::WidgetSerializer::enableHistory(const QList<QWidget *> &widgets, int limit)
{
	if (limit <= 0) {
		return;
	}
	for (QWidget *widget : widgets) {
		if (widget) {
			widget->setProperty(HISTORY, limit);
		}
	}
}

void QDX::WidgetSerializer::disableHistory(QWidget *widget)
{
	enableHistory(widget, 0);
}

void QDX::WidgetSerializer::disableHistory(const QList<QWidget *> &widgets)
{
	enableHistory(widgets, 0);
}

int QDX::WidgetSerializer::historyLimit(QWidget *widget)
{
	return widget ? widget->property(HISTORY).toInt() : 0;
}