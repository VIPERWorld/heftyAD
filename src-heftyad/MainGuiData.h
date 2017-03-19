#ifndef MAINGUIDATA_H
#define MAINGUIDATA_H

/**
 * The MainGuiData class gives access to some useful gui data.
 */

#include <QMetaObject>

class EditMenu;
class Work;

class MainGuiData
{
private:
    static EditMenu *m_editMenu;
    //
    static Work *m_currentWork;
    static QMetaObject::Connection m_currentWorkConn;

public:
    MainGuiData() = delete;
    MainGuiData(const MainGuiData&) = delete;
    MainGuiData& operator=(const MainGuiData&) = delete;
    MainGuiData(const MainGuiData&&) = delete;
    ~MainGuiData() = delete;

    static void setEditMenu(EditMenu *menu);

    static Work* currentWork();
    static void setCurrentWork(Work *work);
};

#endif // MAINGUIDATA_H
