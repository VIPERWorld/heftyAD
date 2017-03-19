#include "ExtraToolBar.h"
#include "ToolButton.h"

#include <QAction>

ExtraToolBar::ExtraToolBar(Qt::Orientation orientation, QWidget *parent)
    : GridWidget(parent),
      m_orientation(orientation)
{
    m_iconSize = 20;

    QLayout *layout = gridLayout();
    switch(m_orientation) {
        case Qt::Horizontal:
            setFixedHeight(m_iconSize);
            layout->setAlignment(Qt::AlignLeft);
            break;

        case Qt::Vertical:
            setFixedWidth(m_iconSize);
            layout->setAlignment(Qt::AlignTop);
            break;
    }

    layout->setSpacing(3);
}

ExtraToolBar::~ExtraToolBar()
{
    clearToolBarActions();
}

QList<QToolButton*> ExtraToolBar::toolButtons(void) const {return m_toolButtons;}

void ExtraToolBar::setToolBarActions(const QList<QAction*> &actions)
{
    clearToolBarActions();

    auto numberOfActions = 0;
    auto numberOfSeparators = 0;
    for(auto *action : actions) {
        // Create a button

        auto *button = new ToolButton;
        button->setIconSize(QSize(m_iconSize, m_iconSize));
        button->setDefaultAction(action);
        if(action->isSeparator()) {
            numberOfSeparators++;
            button->setUpdatesEnabled(false);
        }

        // Register it

        switch(m_orientation) {
            case Qt::Horizontal: addWidget(button, 0, numberOfActions++); break;
            case Qt::Vertical:   addWidget(button, numberOfActions++, 0); break;
        }
        m_toolButtons.append(button);
    }

    setVisible(numberOfActions!=0 && numberOfActions!=numberOfSeparators);
}

void ExtraToolBar::clearToolBarActions(void)
{
    for(QToolButton *button : m_toolButtons) {
        removeWidget(button);
        delete button;
    }
    m_toolButtons.clear();

    setVisible(false);
}
