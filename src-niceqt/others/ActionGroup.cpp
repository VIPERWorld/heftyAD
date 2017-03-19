#include "ActionGroup.h"

ActionGroup::ActionGroup(QObject *parent)
    : QActionGroup(parent)
{
    m_acceptNoSelection = false;

    m_selectedAction = nullptr;
}

bool ActionGroup::acceptNoSelection() const {return m_acceptNoSelection;}
void ActionGroup::setAcceptNoSelection(bool accept)
{
    if(accept == m_acceptNoSelection) {
        return;
    }

    m_acceptNoSelection = accept;
    if(m_acceptNoSelection) {
        setExclusive(true); // We may let the user does that but doing it costs nothing
        connect(this, &ActionGroup::triggered, this, &ActionGroup::onActionGroupTriggered);
    }
    else {
        disconnect(this, &ActionGroup::triggered, this, &ActionGroup::onActionGroupTriggered);
    }
}

QAction* ActionGroup::selectedAction() const {return m_selectedAction;}
void ActionGroup::setSelectedAction(QAction *action)
{
    if(m_selectedAction && m_selectedAction->isChecked()) {
        m_selectedAction->setChecked(false);
    }

    if(action) {
        for(QAction *act : actions()) {
            if(act == action) {
                act->setChecked(true);
                break;
            }
        }
    }

    m_selectedAction = action;
}

void ActionGroup::onActionGroupTriggered(QAction *action)
{
    if(m_acceptNoSelection) {
        if(action == m_selectedAction) {
            m_selectedAction->setChecked(false);
            m_selectedAction = nullptr;
            return;
        }
    }

    m_selectedAction = action;
}
