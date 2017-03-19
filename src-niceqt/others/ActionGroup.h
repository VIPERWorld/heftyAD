#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include <QActionGroup>

class ActionGroup : public QActionGroup
{
    Q_OBJECT

protected:
    bool m_acceptNoSelection;

    QAction *m_selectedAction;

public:
    explicit ActionGroup(QObject *parent = 0);

    bool acceptNoSelection() const;
    /**
     * Sets whether this action group allows no-selection (having 0 actions checked).
     *
     * When this group accepts no-selection,
     * the following should be done to check an action:
     *     action.setChecked(true);
     *     action.triger(); // to make sure this group registers action as the one currently selected.
     *
     * Another way to check an action is to use the convenient setSelectedAction function.
     * Doing it that way may sometimes suits your needs the most,
     * especially if your code already reacts to your action' triggered signal.
     */
    void setAcceptNoSelection(bool accept);

    QAction* selectedAction() const;
    /**
     * Registers the given action as the one selected.
     *
     * The previously selected action (if any) is first unchecked (if QAction::isChecked() returns true).
     * The given action is then checked only if it is not null and is contained in this groups's actions.
     */
    void setSelectedAction(QAction *action);

signals:

public slots:
    void onActionGroupTriggered(QAction *action);
};

#endif // ACTIONGROUP_H
