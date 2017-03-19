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
     * Returns a new instance of a subclass of Algorithm.
     * For instance, if MyAlgorithm is a concret Algorithm, this method may merly return new MyAlgorithm().
     */
    virtual Algorithm* algorithmInstance() const = 0;
};

Q_DECLARE_INTERFACE(AlgorithmPluginInterface, "org.heftyAD.plugins.AlgorithmPluginInterface")

#endif // ALGORITHMPLUGININTERFACE_H
