#include "Iterable.h"

void Iterable::postIterations() {}

void Iterable::run()
{
    preIterations();

    while(canIterate()) {
        iterate();
        postIterate();
    }

    postIterations();
}
