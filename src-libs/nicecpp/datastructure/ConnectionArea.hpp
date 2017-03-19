#ifndef CONNECTIONAREA_H
#define CONNECTIONAREA_H

#include "nicecpplib_global.h"

#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include <vector>

namespace ncpp
{
/**
 * N: Node
 * C: Connection
 */
template<typename N, typename C>
class NICECPPLIBSHARED_EXPORT ConnectionArea
{
protected:
    template<typename Key, typename Value>
    struct connection_property_t {
    private:
        std::string m_name;
        std::map<Key, Value> m_values;

    public:
        std::string name() const noexcept {return m_name;}
        void setName(const std::string &name) noexcept {m_name = name;}

        bool contains(const Key &key) const noexcept {return m_values.count(key) == 1;}
        const Value& get(const Key &key) const {return m_values.at(key);}
        void set(const Key &key, const Value &value) noexcept {m_values[key] = value;}
        void remove(const Key &key) noexcept {m_values.erase(key);}
    };

#define ensureAttributeContainsKey(attributeName, key) \
    if(!attributeName.contains(key)) { \
        std::stringstream ss; \
        ss << __func__ << " -> unable to find property for the given key"; \
        throw std::invalid_argument(ss.str()); \
    }

#define CONNECTION_PROPERTY(key_t, value_t, attributeName) \
    private: \
        connection_property_t<key_t, value_t> attributeName;

#define CONNECTION_PROPERTY_GET(key_t, value_t, attributeName, getterNameSuffix) \
    CONNECTION_PROPERTY(key_t, value_t, attributeName) \
    public: \
        const value_t& getProperty##getterNameSuffix(const key_t &key) const { \
            ensureAttributeContainsKey(attributeName, key) \
            return attributeName.get(key); \
        }

#define CONNECTION_PROPERTY_GET_SET(key_t, value_t, attributeName, getterNameSuffix, setterNameSuffix) \
    CONNECTION_PROPERTY_GET(key_t, value_t, attributeName, getterNameSuffix) \
    public: \
        void setProperty##setterNameSuffix(const key_t &key, const value_t &value) const { \
            ensureAttributeContainsKey(attributeName, key) \
            attributeName.set(key, value); \
        }

private:
    std::vector<N> m_nodes;
    std::vector<C> m_connections;

    CONNECTION_PROPERTY_GET(N, std::vector<N>, m_prop_nodeNeighbors, NodeNeighbors)

    CONNECTION_PROPERTY_GET(C, N, m_prop_connectionSource, ConnectionSource)
    CONNECTION_PROPERTY_GET(C, N, m_prop_connectionTarget, ConnectionTarget)

public:
    ConnectionArea() {
        m_prop_nodeNeighbors.setName("neighbors");

        m_prop_connectionSource.setName("source");
        m_prop_connectionTarget.setName("target");
    }

    /*
     =============================
     Querying this connection area
     =============================
     */

    std::vector<N> nodes() const noexcept {return m_nodes;}

    std::vector<N> nodes(std::function<bool (const N&)> predicate) const noexcept {
        std::vector<N> res;
        for(const N &node : m_nodes) {
            if(predicate(node)) {
                res.push_back(node);
            }
        }

        return res;
    }

    /**
     * Returns the list of nodes "source" such that
     * there is a connection from "source" to target.
     */
    std::vector<N> nodeSources(const N &target) const noexcept {
        std::vector<N> res;
        for(const C &conn : m_connections) {
            if(m_prop_connectionTarget.get(conn) == target) {
                res.push_back(m_prop_connectionSource.get(conn));
            }
        }

        return res;
    }

    /**
     * Returns the list of nodes "target" such that
     * there is a connection to "target" from source.
     */
    std::vector<N> nodeTargets(const N &source) const noexcept {
        std::vector<N> res;
        for(const C &conn : m_connections) {
            if(m_prop_connectionSource.get(conn) == source) {
                res.push_back(m_prop_connectionTarget.get(conn));
            }
        }

        return res;
    }

    int nodeCount() const noexcept {return m_nodes.size();}

    std::vector<C> connections() const noexcept {return m_connections;}

    std::vector<C> connections(std::function<bool (const C&)> predicate) const noexcept {
        std::vector<C> res;
        for(const C &conn : m_connections) {
            if(predicate(conn)) {
                res.push_back(conn);
            }
        }

        return res;
    }

    /**
     * Returns the connections going from the source node to the target node.
     */
    std::vector<C> connections(const N &source, const N &target) const noexcept {
        std::vector<C> res;
        for(const C &conn : m_connections) {
            if(m_prop_connectionSource.get(conn) == source && m_prop_connectionTarget.get(conn) == target) {
                res.push_back(conn);
            }
        }

        return res;
    }

    /**
     * Returns the list of connections
     *     from node (if from ONLY)
     *     to   node (if to ONLY)
     *     with node (if from AND to)
     */
    std::vector<C> connections(bool from, bool to, const N &node) const noexcept {
        return connections([&] (const C &conn) -> bool {
            const bool &ok1 = from && m_prop_connectionSource.get(conn) == node;
            const bool &ok2 = to   && m_prop_connectionTarget.get(conn) == node;
            return ok1 || ok2;
        });
    }

    int connectionCount() const noexcept {return m_connections.size();}

    int connectionCount(std::function<bool (const C&)> predicate) const noexcept {
        return connections(predicate).size();
    }

    int connectionCount(const N &source, const N &target) const noexcept {
        return connections(source, target).size();
    }

    int connectionCount(bool from, bool to, const N &node) const noexcept {
        return connections(from, to, node).size();
    }

    bool containsNode(const N &node) const noexcept {
        for(const N &n : m_nodes) {
            if(n == node) {
                return true;
            }
        }
        return false;
    }

    bool containsConnection(const C &connection) const noexcept {
        for(const C &conn : m_connections) {
            if(conn == connection) {
                return true;
            }
        }
        return false;
    }

    /**
     * Tells whether there is a connection from the source node to the target node.
     */
    bool connected(const N &source, const N &target) const noexcept {
        auto res = false;
        for(const C &conn : m_connections) {
            res = m_prop_connectionSource.get(conn) == source && m_prop_connectionTarget.get(conn) == target;
            if(res) {
                break;
            }
        }

        return res;
    }

    /*
     =======================================
     Adding elements to this connection area
     =======================================
     */

    bool addNode(const N &node) {
        const bool &ignore = containsNode(node);
        if(!ignore) {
            m_prop_nodeNeighbors.set(node, std::vector<N>());
            m_nodes.push_back(node);
        }

        return !ignore;
    }

    bool addConnection(const C &connection, const N &source, const N &target) {
        if(containsConnection(connection)) {
            return false;
        }

        if(!containsNode(source)) {
            throw std::invalid_argument("addConnection -> source node doesn't belong to connection area");
        }
        if(!containsNode(target)) {
            throw std::invalid_argument("addConnection -> target node doesn't belong to connection area");
        }

        if(!connected(source, target)) {
            std::vector<N> sourceNeighbors = m_prop_nodeNeighbors.get(source);
            sourceNeighbors.push_back(target);
            m_prop_nodeNeighbors.set(source, sourceNeighbors);
        }

        m_prop_connectionSource.set(connection, source);
        m_prop_connectionTarget.set(connection, target);
        m_connections.push_back(connection);
        return true;
    }

    /*
     ===========================================
     Removing elements from this connection area
     ===========================================
     */

    bool removeNode(const N &node) {
        auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
        if(it != m_nodes.end()) {
            // Gather connections from & to node

            std::vector<C> vect;
            for(const C &conn : m_connections) {
                if(m_prop_connectionSource.get(conn) == node || m_prop_connectionTarget.get(conn) == node) {
                    vect.push_back(conn);
                }
            }

            // Remove those connections

            while(!vect.empty()) {
                auto it = vect.begin();
                removeConnection(*it);
                vect.erase(it);
            }

            // Remove node

            m_prop_nodeNeighbors.remove(node);
            m_nodes.erase(it);
            return true;
        }

        return false;
    }

    bool removeConnection(const C &connection) {
        auto it = std::find(m_connections.begin(), m_connections.end(), connection);
        if(it != m_connections.end()) {
            const N &source = m_prop_connectionSource.get(connection);
            const N &target = m_prop_connectionTarget.get(connection);

            // Remove target from source neighbors only if needed
            if(connectionCount(source, target) == 1) {
                std::vector<N> sourceNeighbors = m_prop_nodeNeighbors.get(source);
                auto it2 = std::find(sourceNeighbors.begin(), sourceNeighbors.end(), target);
                if(it2 != sourceNeighbors.end()) {
                    sourceNeighbors.erase(it2);
                }
                m_prop_nodeNeighbors.set(source, sourceNeighbors);
            }

            m_prop_connectionSource.remove(connection);
            m_prop_connectionTarget.remove(connection);
            m_connections.erase(it);

            return true;
        }

        return false;
    }

    void clear() {
        while(!m_nodes.empty()) {
            auto it = m_nodes.begin();
            removeNode(*it);
            // m_nodes.erase(it); // shouldn't be done since the node is removed (see previous statement)
        }

        // No need to remove connections since they are removed gradually as nodes are.
    }
};
}

#endif // CONNECTIONAREA_H
