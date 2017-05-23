#ifndef MODELWORK_H
#define MODELWORK_H

#include "Work.h"

class View;

class ModelWork : public Work
{
    Q_OBJECT

protected:
    /*
     * I try to use std::unique_ptr<View> but I finally abandon since
     *     I was getting the error: invalid application of 'sizeof' to incomplete type 'View'"
     *     and don't want to #include View.h.
     */
    View *m_view;

public:
    /**
     * Create a work.
     * Keeeps ownership of the given view and its asspociated model.
     */
    explicit ModelWork(View *view, QWidget *parent = 0);
    ~ModelWork();

    bool isDirty() const override;
    void setDirty(bool dirty) override;
    ncpp::UndoStack* undoStack() const override;

    bool saveTo(const QString &filePath) override;
    bool loadFrom(const QString &filePath) override;
    QWidget* editionForm() const override;
    bool isEditionEnabled() const override;

    QList<QAction*> toolBarActions() const override;
    void retranslate() override;

protected:
    void execExportDialog() override;

signals:

public slots:
};

#endif // MODELWORK_H
