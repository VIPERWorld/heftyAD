#include "SectionWidget.h"

SectionWidget::SectionWidget(QWidget *parent)
    : GridWidget(parent),
      m_unrolled(false)
{
    addWidget(&m_bar, 0, 0);
    adjustBar();

    connect(&m_bar, &SectionWidgetBar::pressed,  this, &SectionWidget::unroll);
    connect(this,   &SectionWidget::widgetAdded, this, &SectionWidget::onWidgetAdded);
}

bool SectionWidget::isUnrolled() const {return m_unrolled;}
bool SectionWidget::isOpened() const {return isUnrolled();}
bool SectionWidget::isClosedz() const {return !isOpened();}

SectionWidgetBar* SectionWidget::bar() const {return const_cast<SectionWidgetBar*>(&m_bar);}

QString SectionWidget::barTitle() const {return m_bar.text();}
void SectionWidget::setBarTitle(const QString &title) {m_bar.setText(title);}
QString SectionWidget::barStyleSheet() const {return m_bar.styleSheet();}
void SectionWidget::setBarStyleSheet(const QString &styleSheet) {m_bar.setStyleSheet(styleSheet);}

void SectionWidget::unroll()
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

void SectionWidget::open()
{
    if(!isOpened()) {
        unroll();
    }
}

void SectionWidget::closez()
{
    if(!isClosedz()) {
        unroll();
    }
}

void SectionWidget::adjustBar()
{
    m_bar.m_widgetIsUnrolled = m_unrolled;
    m_bar.update();
}

void SectionWidget::onWidgetAdded(QWidget *widget)
{
    widget->setVisible(m_unrolled);
}
