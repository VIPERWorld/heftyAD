#ifndef NICECPPLIB_GLOBAL_H
#define NICECPPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NICECPPLIB_LIBRARY)
#  define NICECPPLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NICECPPLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NICECPPLIB_GLOBAL_H
