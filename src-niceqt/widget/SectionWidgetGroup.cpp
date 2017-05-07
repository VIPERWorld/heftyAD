#include "SectionWidgetGroup.h"
#include "SectionWidget.h"

SectionWidgetGroup::SectionWidgetGroup(QWidget *parent)
    : QWidget(parent)
{
    m_openedSectionLimit = -1;
    m_openedSectionExclusive = false;
    m_currentlyOpenedSection = nullptr;

    resetValues();

    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
    /*
     * widgets are stacked on top;
     * useful especially when all sections are closed.
     */
    m_layout.setAlignment(Qt::AlignTop);
}

QList<SectionWidget*> SectionWidgetGroup::sections() const {return m_sections;}

SectionWidget* SectionWidgetGroup::addSection()
{
    auto *section = new SectionWidget;
    m_sections.append(section);
    m_layout.addWidget(section, m_row++, 0);

    /*
     * Ensures that when a section is opened,
     * the sections beneath it aren't moved too far below.
     */
    m_layout.setAlignment(section, Qt::AlignTop);

    connect(section, &SectionWidget::unrolled, this, &SectionWidgetGroup::onSectionUnrolled);

    return section;
}

void SectionWidgetGroup::removeSections()
{
    while(!m_sections.isEmpty()) {
        SectionWidget *section = m_sections.takeFirst();
        m_layout.removeWidget(section);
        section->setVisible(false);

        disconnect(section, &SectionWidget::unrolled, this, &SectionWidgetGroup::onSectionUnrolled);

        delete section;
    }

    resetValues(); // reset values
}

int SectionWidgetGroup::openedSections() const {return m_openedSections;}
int SectionWidgetGroup::openedSectionLimit() const {return m_openedSectionLimit;}
void SectionWidgetGroup::setOpenedSectionLimit(int limit)
{
    if(m_openedSectionExclusive || m_openedSectionLimit == limit) {
        return;
    }

    m_openedSectionLimit = limit;
    if(m_openedSectionLimit < 0) {
        return;
    }

    // make sure sections are hidden if necessary

    for(SectionWidget *section : m_sections) {
        if(m_openedSections <= m_openedSectionLimit) {
            break;
        }

        section->closez(); // signal urolled will be emitted
    }
}

bool SectionWidgetGroup::openedSectionExclusive() const {return m_openedSectionExclusive;}
void SectionWidgetGroup::setOpenedSectionExclusive(bool exclusive)
{
    if(m_openedSectionLimit >= 0 || m_openedSectionExclusive == exclusive) {
        return;
    }

    m_openedSectionExclusive = exclusive;
    m_currentlyOpenedSection = nullptr;

    if(m_openedSectionExclusive) {
        // set current opened section and make sure it's the only one that is opened
        for(SectionWidget *section : m_sections) {
            if(section->isOpened()) {
                if(m_currentlyOpenedSection == nullptr) {
                    m_currentlyOpenedSection = section;
                }
                else {
                    section->closez();
                }
            }
        }
    }
}

void SectionWidgetGroup::resetValues()
{
    m_row = 0;
    m_openedSections = 0;
}

void SectionWidgetGroup::onSectionUnrolled()
{
    QObject *emitter = sender();
    auto *section = qobject_cast<SectionWidget*>(emitter);
    if(!section) {
        return;
    }

    // close sections to make sure limit is not exceeded

    if(section->isOpened()) {
        m_openedSections++;
        const bool cancelOpen = m_openedSectionLimit>=0 && m_openedSections>m_openedSectionLimit;
        if(cancelOpen) {
            section->closez();
        }
    }
    else {
        if(m_openedSections > 0) {
            m_openedSections--;
        }
    }

    // handle exlusion if enabled

    if(m_openedSectionExclusive) { // surely opened-section-limit is negative too
        if(section->isOpened()) {
            if(m_currentlyOpenedSection) {
                m_currentlyOpenedSection->closez();
            }
            m_currentlyOpenedSection = section;
        }
        else {
            if(m_currentlyOpenedSection == section) {
                m_currentlyOpenedSection = nullptr;
            }
        }
    }
}
