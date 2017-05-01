#include "SectionedWidget.h"
#include "SectionedWidgetElt.h"

SectionedWidget::SectionedWidget(QWidget *parent)
    : QWidget(parent)
{
    resetRow();

    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
    /*
     * widgets are stacked on top;
     * useful especially when all sections are closed.
     */
    m_layout.setAlignment(Qt::AlignTop);
}

SectionedWidgetElt* SectionedWidget::addSection()
{
    auto *section = new SectionedWidgetElt;
    m_sections.append(section);
    m_layout.addWidget(section, m_row++, 0);

    /*
     * Ensures that when a section is opened,
     * the sections beneath it aren't moved too far below.
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

    resetRow(); // reset values
}

void SectionedWidget::resetRow()
{
    m_row = 0;
}
