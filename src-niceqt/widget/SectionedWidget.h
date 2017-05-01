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
    int m_row; // row of the last section added

public:
    explicit SectionedWidget(QWidget *parent = 0);

    SectionedWidgetElt* addSection();
    void removeSections();

private:
    void resetRow();

signals:

public slots:
};

#endif // SECTIONEDWIDGET_H
