#ifndef SECTIONEDWIDGET_H
#define SECTIONEDWIDGET_H

#include <QGridLayout>
#include <QWidget>

class SectionedWidgetElt;

class SectionedWidget : public QWidget
{
    Q_OBJECT

private:
    QGridLayout m_layout;
    QList<SectionedWidgetElt*> m_sections;
    int m_row, m_col; // row and col of the last section added

public:
    explicit SectionedWidget(QWidget *parent = 0);

    SectionedWidgetElt* addSection();
    void removeSections();

signals:

public slots:
};

#endif // SECTIONEDWIDGET_H
