#ifndef SECTIONWIDGETGROUP_H
#define SECTIONWIDGETGROUP_H

#include <QGridLayout>
#include <QPointer>
#include <QWidget>

class SectionWidget;

class SectionWidgetGroup : public QWidget
{
    Q_OBJECT

private:
    int m_openedSections; // number of opned sections
    int m_openedSectionLimit; // opened sections limit

    bool m_openedSectionExclusive;
    QPointer<SectionWidget> m_currentlyOpenedSection; // use only when opened-section-exclusive is enabled

    QGridLayout m_layout;
    QList<SectionWidget*> m_sections;
    int m_row; // row of the last section added

public:
    explicit SectionWidgetGroup(QWidget *parent = 0);

    QList<SectionWidget*> sections() const;
    SectionWidget* addSection();
    void removeSections();

    int openedSections() const;
    int openedSectionLimit() const;
    /**
     * Sets the maximum number of opened sections allowed.
     * Once the given limit is reached no more sections would open.
     * When the given limit is negative no restrictions are applied.
     *
     * This function does nothing when opened-section-exclusive is enabled.
     * This function relies on the signal SectionWidget::unrolled which shouldn't be blocked.
     */
    void setOpenedSectionLimit(int limit);

    bool openedSectionExclusive() const;
    /**
     * Sets whether opening sections should be handled in an exclusive manner.
     * When enabled, opening a section triggers the closure of the one currently opened.
     *
     * This function does nothing when opened-section-limit >= 0.
     * * This function relies on the signal SectionWidget::unrolled which shouldn't be blocked.
     */
    void setOpenedSectionExclusive(bool exclusive);

private:
    void resetValues();

signals:

private slots:
    void onSectionUnrolled();
};

#endif // SECTIONWIDGETGROUP_H
