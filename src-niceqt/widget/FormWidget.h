#ifndef FORMWIDGET_H
#define FORMWIDGET_H

#include <QFormLayout>
#include <QWidget>

class FormWidget : public QWidget
{
    Q_OBJECT

private:
    QFormLayout m_layout;

public:
    explicit FormWidget(QWidget *parent = 0);

    QFormLayout* formLayout() const;

    void addRow(const QString &labelText, QWidget *field);
    void addRow(const QString &labelText, QLayout *field);

    /**
     * Clears the inner layout of this widget:
     *     removes all QLayoutItem* items from the inner layout and deletes them.
     *
     * This function is useful when dealing with translation:
     *     FormWidget form;
     *     //...
     *     form.clear(); // removes any item
     *     form.addRow(tr("Translated text"), textEdit); // add items back
     *
     * Another way to provide a translation feature would be to:
     *     first get a pointer to an item within the inner layout
     *     then get the widget of that item and qobject_cast it to QLabel*
     *     finally set the text of the label
     */
    void clear();

signals:

public slots:
};

#endif // FORMWIDGET_H
