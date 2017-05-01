#include "TabStackWidget.h"

#include <QTabWidget>

TabStackWidget::TabStackWidget(QWidget *parent)
    : QWidget(parent),
      m_mode(Tabbed),
      m_tab(nullptr)
{
    setLayout(&m_layout);
    addWidget(&m_modeSwitcher);
    m_modeSwitcher.setText("Stacked");

    connect(&m_modeSwitcher, &QCheckBox::toggled, [this](bool checked) {
        setMode(checked ? Stacked : Tabbed);
    });
    connect(&m_comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
        m_stack.setCurrentIndex(index);
    });
    connect(this, &TabStackWidget::modeChanged, this, &TabStackWidget::onModeChanged);

    onModeChanged(); // to validate the initial mode
}

TabStackWidget::Mode TabStackWidget::mode() const {return m_mode;}
void TabStackWidget::setMode(TabStackWidget::Mode mode)
{
    if(m_mode != mode) {
        m_mode = mode;
        emit modeChanged();
    }
}

void TabStackWidget::setModeSwitcherVisible(bool visible) {m_modeSwitcher.setVisible(visible);}
void TabStackWidget::setModeSwitcherEnabled(bool enabled) {m_modeSwitcher.setEnabled(enabled);}

void TabStackWidget::setTabWidget(QTabWidget *tab)
{
    if(m_tab != tab) {
        moveContentToTab(); // move content back to the tab before we set the new one
        removeWidget(m_tab);
        m_tab = tab;

        onModeChanged(); // to make sure the new tab is shown if necessary
    }
}

void TabStackWidget::moveContentToTab()
{
    /*
     * Move widgets from stack to tab (which is empty)
     * Clear combobox
     */

    if(m_tab) {
        const bool tw = m_tab->blockSignals(true);
        const bool cb = m_comboBox.blockSignals(true);
        m_tab->setUpdatesEnabled(false);

        const int curr = m_comboBox.currentIndex();

        int i = 0;
        while(m_stack.widget(0)) {
            m_tab->addTab(m_stack.widget(0), m_comboBox.itemText(i++));
        }
        m_comboBox.clear();

        m_tab->setCurrentIndex(curr);

        m_tab->blockSignals(tw);
        m_comboBox.blockSignals(cb);
        m_tab->setUpdatesEnabled(true);
    }
}

void TabStackWidget::moveContentToStack()
{
    /*
     * Move widgets from tab to stack (which is empty)
     * Fulfil combobox (which is empty)
     */

    if(m_tab) {
        const bool tw = m_tab->blockSignals(true);
        const bool cb = m_comboBox.blockSignals(true);

        const int curr = m_tab->currentIndex();

        while(m_tab->widget(0)) {
            m_comboBox.addItem(m_tab->tabText(0));
            m_stack.addWidget(m_tab->widget(0));
        }

        m_comboBox.setCurrentIndex(curr);
        m_stack.setCurrentIndex(curr);

        m_tab->blockSignals(tw);
        m_comboBox.blockSignals(cb);
    }
}

void TabStackWidget::removeWidget(QWidget *widget)
{
    if(widget != nullptr) {
        m_layout.removeWidget(widget);
        widget->setVisible(false);
    }
}

void TabStackWidget::addWidget(QWidget *widget)
{
    if(widget != nullptr) {
        m_layout.addWidget(widget);
        widget->setVisible(true); // in case if it were previously removed
    }
}

void TabStackWidget::onModeChanged()
{
    // remove all widgets but mode switcher

    removeWidget(m_tab);
    removeWidget(&m_comboBox);
    removeWidget(&m_stack);

    // then only add the widgets we need

    switch(m_mode) {
    case Stacked:
        addWidget(&m_comboBox);
        addWidget(&m_stack);
        moveContentToStack();
        break;

    case Tabbed:
        addWidget(m_tab);
        moveContentToTab();
        break;
    }

    // finally check mode switcher if needed

    const bool b = m_modeSwitcher.blockSignals(true);
    m_modeSwitcher.setChecked(m_mode == Stacked);
    m_modeSwitcher.blockSignals(b);
}
