#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "nicecpplib_global.h"

#include "ConnectionArea.hpp"

namespace ncpp
{
/**
 * To define a finite automaton, we need:
 *         a finite set of input symbols called the alphabet.
 *     Q:  a finite set of states.
 *     Qo: a set of start states (included in Q).
 *     Qf: a set of accept states (included in Q).
 *     t:  a transition function.
 *
 * TODO:
 *     Think about the way this class will be templated.
 *     Also think about how to implement both finite and pushdpwn automata.
 *         pushdown automaton: https://en.wikipedia.org/wiki/Pushdown_automaton
 */
template<typename V, typename E>
class NICECPPLIBSHARED_EXPORT Automaton
{
private:
    ConnectionArea<V, E> m_area;

public:
};
}

#endif // AUTOMATON_H
