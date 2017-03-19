#ifndef ARRAYFORM_H
#define ARRAYFORM_H

#include "ViewForm.h"

#include <QLineEdit>

class ArrayView;

class ArrayForm : public ViewForm
{
    Q_OBJECT

private:
    ArrayView *m_view;

    // model widgets

    FormWidget m_arrayForm;
        QLineEdit m_arraySpacing;

    // item widgets

public:
    explicit ArrayForm(ArrayView *view, QWidget *parent = 0);

    void retranslate() override;

protected:
    void onSceneSelectionChanged() override;

signals:

public slots:
};

#endif // ARRAYFORM_H
