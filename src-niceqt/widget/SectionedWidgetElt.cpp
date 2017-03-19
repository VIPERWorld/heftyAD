#include "SectionedWidgetElt.h"

SectionedWidgetElt::SectionedWidgetElt(QWidget *parent)
    : GridWidget(parent),
      m_unrolled(false)
{
    addWidget(&m_bar, 0, 0);
    adjustBar();

    connect(&m_bar, &SectionedWidgetEltBar::pressed,  this, &SectionedWidgetElt::unroll);
    connect(this,   &SectionedWidgetElt::widgetAdded, this, &SectionedWidgetElt::onWidgetAdded);
}

bool SectionedWidgetElt::isUnrolled() const {return m_unrolled;}
SectionedWidgetEltBar* SectionedWidgetElt::bar() const {return const_cast<SectionedWidgetEltBar*>(&m_bar);}

QString SectionedWidgetElt::barTitle() const {return m_bar.text();}
void SectionedWidgetElt::setBarTitle(const QString &title) {m_bar.setText(title);}
QString SectionedWidgetElt::barStyleSheet() const {return m_bar.styleSheet();}
void SectionedWidgetElt::setBarStyleSheet(const QString &styleSheet) {m_bar.setStyleSheet(styleSheet);}

void SectionedWidgetElt::unroll()
{
    m_unrolled = !m_unrolled;
    for(QWidget *widget : widgets()) {
        if(widget != &m_bar) {
            widget->setVisible(m_unrolled);
        }
    }
    adjustBar();

    emit unrolled();
}

void SectionedWidgetElt::adjustBar()
{
    m_bar.m_widgetIsUnrolled = m_unrolled;
    m_bar.update();
}

void SectionedWidgetElt::onWidgetAdded(QWidget *widget)
{
    widget->setVisible(m_unrolled);
}
