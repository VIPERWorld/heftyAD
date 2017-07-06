#ifndef JSALGORITHM_H
#define JSALGORITHM_H

#include "Algorithm.h"

#include <QJSValue>

class JSFileParser;

class JSAlgorithm : public Algorithm
{
    Q_OBJECT

private:
    JSFileParser &m_parser;

public:
    explicit JSAlgorithm(JSFileParser &parser);

    bool requiresAModel() const override;
    bool hasAValidModel() const override;

    void preExecute() override;
    void execute() override;
    void postExecute() override;

private:
    void exposeAttributes();
    void callJSFunction(QJSValue &jsFunction) const;

    void onModelChanged();
    void onParserReseted();

signals:

public slots:
};

#endif // JSALGORITHM_H
