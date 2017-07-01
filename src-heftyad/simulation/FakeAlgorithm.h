#ifndef FAKEALGORITHM_H
#define FAKEALGORITHM_H

#include "Algorithm.h"

#include "graphics/highlighting/HighlightingTextData.h"

/**
 * The FakeAlgorithm class shouldn't normally exist.
 * But it helps a lot, when it comes to testing the highlighting or clarifying features.
 */
class FakeAlgorithm : public Algorithm
{
private:
    HighlightingTextData data;

public:
    explicit FakeAlgorithm();

    /*
     * When using highlighting data, a model should be defined.
     * Indeed m_highlighter needs to communicate with m_model' view.
     */
    bool requiresAModel() const override;
    bool hasAValidModel() const override;

protected:
    void preExecute() override;
    void execute() override;
    void postExecute() override;
};

#endif // FAKEALGORITHM_H
