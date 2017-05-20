#ifndef ARRAYFORM_H
#define ARRAYFORM_H

#include "ViewForm.h"

#include <QLabel>
#include <QSpinBox>

class ArrayView;

class ArrayForm : public ViewForm
{
    Q_OBJECT

private:
    ArrayView *m_view;

    // model widgets

    SectionWidget *m_arraySection;
        FormWidget m_arrayForm;
            QLabel m_arraySpacingLabel; QSpinBox m_arraySpacing;

    // item widgets

public:
    explicit ArrayForm(ArrayView *view, QWidget *parent = 0);

    void retranslate() override;

protected:
    void onSceneSelectionChanged() override;

signals:

private slots:
    void onArraySpacingChanged();
};

#endif // ARRAYFORM_H
