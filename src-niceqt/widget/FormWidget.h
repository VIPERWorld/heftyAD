#ifndef FORMWIDGET_H
#define FORMWIDGET_H

#include <QFormLayout>
#include <QWidget>

class FormWidget : public QWidget
{
    Q_OBJECT

private:
    QFormLayout m_layout;
    QList<QLayout*> m_newedLayouts;

public:
    explicit FormWidget(QWidget *parent = 0);
    ~FormWidget();

    QFormLayout* formLayout() const;

    void addRow(QWidget *label, QWidget *field);
    void addRow(QWidget *label, QLayout *field);
    void addRow(const QString &labelText, QWidget *field);
    void addRow(const QString &labelText, QLayout *field);
    void addRow(QWidget *widget);
    void addRow(QLayout *layout);

    void addRow(QWidget *label,           const QWidgetList &fields,  Qt::Orientation orient, int spacing = -1, int margin = -1);
    void addRow(const QString &labelText, const QWidgetList &fields,  Qt::Orientation orient, int spacing = -1, int margin = -1);
    void addRow(                          const QWidgetList &widgets, Qt::Orientation orient, int spacing = -1, int margin = -1);

    QLayout* newLayout(                   const QWidgetList &widgets, Qt::Orientation orient, int spacing = -1, int margin = -1);

    /**
     * Clears the inner layout of this widget.
     * This function merely:
     *     removes all QLayoutItem* items from the inner layout
     *     and deletes them.
     * Widgets are not deleted so they can be added back using addRow.
     *
     * When iterating through items, if a label happens to be a widget of an inner item,
     * that is item->widget() is a QLabel, then the text of that label is set to "".
     * So if a label is added to this form, label->text() will return
     * an empty string after a call to this function.
     *
     * This function is useful when dealing with translation,
     * especially when you want "behind scene" labels to be translated.
     * Here is a snippet which could be taken as a guide:
     *     FormWidget form;
     *
     *     // simulate translation
     *
     *     form.clear(); // removes items (if any)
     *     form.addRow(tr("Name"), nameTextEdit);
     *     form.addRow(tr("Help"), helpLabel);
     *     helpLabel->setText(tr("This is a hint to guide you.")); // to make sure label text is not empty
     *
     * Another way to provide a translation feature (without clearing this form) would be to:
     *     first get a pointer to an item within the inner layout
     *     then get the widget of that item and qobject_cast it to QLabel*, QTextEdit* or whatever else
     *     finally set the right text whenever translation is needed.
     * But that is a little bit tedious.
     */
    void clear();

signals:

public slots:
};

#endif // FORMWIDGET_H
