#include "Utility.h"

#include <QAbstractScrollArea>
#include <QApplication>
#include <QFrame>
#include <QMenu>
#include <QWidget>

QAction* Utility::execMenuAt(QMenu *menu, const QPoint &pos, QWidget *source)
{
    QAbstractScrollArea *sourceScrollArea = dynamic_cast<QAbstractScrollArea*>(source);

    QWidget *widget = sourceScrollArea ? sourceScrollArea->viewport() : source;
    QPoint globalPos = widget->mapToGlobal(pos);

    return menu->exec(globalPos);
}

QFrame* Utility::lineWidget()
{
    auto* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    return line;
}

QList<QWidget*> Utility::widgetsAt(const QPoint &pos)
{
    QList<QWidget*> widgets;

    QWidget *widget = nullptr;
    do {
        widget = QApplication::widgetAt(pos);
        if(widget) {
            widgets.append(widget);
            // Make widget invisible to further enquiries
            widget->setAttribute(Qt::WA_TransparentForMouseEvents);
        }
    }
    while(widget);

    // Now restore widget attributes
    foreach(QWidget *w, widgets)
        w->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    return widgets;
}
