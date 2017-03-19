#include "SectionedWidget.h"
#include "SectionedWidgetElt.h"

SectionedWidget::SectionedWidget(QWidget *parent)
    : QWidget(parent),
      m_row(-1),
      m_col(-1)
{
    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
}

SectionedWidgetElt* SectionedWidget::addSection()
{
    auto *section = new SectionedWidgetElt;
    m_sections.append(section);
    m_layout.addWidget(section, ++m_row, ++m_col);

    /*
     * widgets are stacked on the top;
     * useful especially when sections are unrolled (opened)
     */
    m_layout.setAlignment(section, Qt::AlignTop);

    return section;
}

void SectionedWidget::removeSections()
{
    while(!m_sections.isEmpty()) {
        SectionedWidgetElt *section = m_sections.takeFirst();
        m_layout.removeWidget(section);
        section->setVisible(false);

        delete section;
    }

    m_row = m_col = -1; // reset values
}
