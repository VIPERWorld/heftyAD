#ifndef MAINDATA_H
#define MAINDATA_H

namespace ncpp
{
class UndoStack;
}

/**
 * The MainData class gives access to some useful non gui data.
 */

class MainData
{
private:
    static ncpp::UndoStack *m_currentUndoStack;

public:
    MainData() = delete;
    MainData(const MainData&) = delete;
    MainData& operator=(const MainData&) = delete;
    MainData(const MainData&&) = delete;
    ~MainData() = delete;

    static ncpp::UndoStack* currentUndoStack();
    static void setCurrentUndoStack(ncpp::UndoStack *stack);
};

#endif // MAINDATA_H
