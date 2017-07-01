#ifndef SIMULATIONCONFIGWIDGET_H
#define SIMULATIONCONFIGWIDGET_H

#include "AlgorithmPluginForm.h"

#include "widget/GridWidget.h"
#include "widget/PushButton.h"

#include <QLineEdit>
#include <QPointer>

class Algorithm;
class Model;
class SimulationAlgorithmPluginPicker;
class View;

class SimulationConfigWidget : public GridWidget
{
    Q_OBJECT

protected:
    SimulationAlgorithmPluginPicker *m_diag;

    QPointer<Algorithm> m_algorithm;
    QPointer<Model> m_model;
    QPointer<View> m_view;

    QString m_algorithmPath;
    QString m_modelPath;

    QLineEdit m_algorithmLineEdit; PushButton m_chooseAlgorithm;
    QLineEdit m_modelLineEdit;     PushButton m_chooseModel;
    AlgorithmPluginForm m_form;

public:
    explicit SimulationConfigWidget(QWidget *parent = 0);
    ~SimulationConfigWidget();

    Algorithm* algorithm() const;
    Model* model() const;
    View* modelView() const;

    QString algorithmPath() const;
    QString modePath() const;

    void retranslate();

private:
    void deleteAlgorithm();

signals:
    void algorithmChanged();
    void modelChanged();

public slots:

protected slots:
    void onChooseAlgorithmButtonPressed();
    void onChooseModelButtonPressed();
};

#endif // SIMULATIONCONFIGWIDGET_H
