#ifndef ALGORITHMPLUGININTERFACE_H
#define ALGORITHMPLUGININTERFACE_H

#include "PluginInterface.h"

class Algorithm;

/**
 * First of all
 * ============
 *
 * If you have never written plugins for heftyAD,
 * please first read the documentation about the PluginInterface class.
 *
 * About this class
 * ================
 *
 * The AlgorithmPluginInterface class is the base class for all heftyAD algorithm plugins.
 */

class AlgorithmPluginInterface : public PluginInterface
{
public:
    AlgorithmPluginInterface() = default;

    /**
     * Returns a new algorithm.
     * For instance if MyAlgorithm is a concret Algorithm, this function may merly return new MyAlgorithm().
     */
    virtual Algorithm* algorithmInstance() const = 0;
};

#define AlgorithmPluginInterface_ID "org.heftyAD.plugins.AlgorithmPluginInterface"

Q_DECLARE_INTERFACE(AlgorithmPluginInterface, AlgorithmPluginInterface_ID)

#endif // ALGORITHMPLUGININTERFACE_H
