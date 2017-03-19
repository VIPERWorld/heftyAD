#ifndef ARRAY_H
#define ARRAY_H

#include "nicecpplib_global.h"

#include <functional>
#include <stdexcept>
#include <sstream>
#include <vector>

namespace ncpp
{
/**
 *
 */
template<typename T>
class NICECPPLIBSHARED_EXPORT Array
{
private:
    std::vector<T> m_values;

public:
    Array() = default;

    /*
     ===================
     Querying this array
     ===================
     */

    bool isValidIndex(int index) const noexcept {return index>=0 && index<=size()-1;}

    T* data() {return m_values.data();}
    const T* data() const {return m_values.data();}

    std::vector<T> values() const noexcept {return m_values;}

    std::vector<T> values(std::function<bool (const T&)> predicate) const noexcept
    {
        std::vector<T> res;
        for(const auto &val : m_values) {
            if(predicate(val)) {
                res.push_back(val);
            }
        }

        return res;
    }

    int size() const noexcept {return m_values.size();}

    bool isEmpty() const noexcept {return m_values.size()==0;}

    bool contains(std::function<bool (const T&)> predicate) const noexcept
    {
        return !values(predicate).empty();
    }

    bool contains(const T &value) const noexcept
    {
        return contains([&] (const T &elem) -> bool {return elem==value;});
    }

    int indexOf(const T &value) const noexcept
    {
        auto index = -1;

        for(auto i=0, s=size(); i<s; i++) {
            if(m_values[i] == value) {
                index = i;
                break;
            }
        }

        return index;
    }

    std::vector<int> indexesOf(const T &value) const noexcept
    {
        std::vector<int> res;
        for(auto i=0, s=size(); i<s; i++) {
            if(m_values[i] == value) {
                res.push_back(i);
            }
        }

        return res;
    }

    T& operator[](int index)
    {
        if(!isValidIndex(index)) {
            throw std::out_of_range("operator[] -> out of range exception");
        }

        return m_values[index];
    }

    const T& at(int index) const
    {
        if(!isValidIndex(index)) {
            throw std::out_of_range("at -> out of range exception");
        }

        return m_values.at(index);
    }

    void swap(int i, int j)
    {
        if(!isValidIndex(i) || !isValidIndex(j)) {
            throw std::out_of_range("swap -> out of range exception");
        }

        if(i != j) {
            auto &arr(*this);
            T tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }

    /*
     =============================
     Adding elements to this array
     =============================
     */

    void add(const T &value) noexcept {m_values.push_back(value);}

    void add(const T &value, int index)
    {
        if(index<0 || (isEmpty() && index!=0) || (!isEmpty() && index>size()-1)) {
            throw std::out_of_range("add -> out of range exception");
        }

        typename std::vector<T>::iterator it = m_values.begin();
        while(index-- > 0) {
            it++;
        }
        m_values.insert(it, value);
    }

    /*
     =================================
     Removing elements from this array
     =================================
     */

    bool removeAt(int index)
    {
        if(!isValidIndex(index)) {
            throw std::out_of_range("removeAt -> out of range exception");
        }

        typename std::vector<T>::iterator it = m_values.begin();
        while(index-- > 0) {
            it++;
        }
        m_values.erase(it);

        return true;
    }

    bool removeFirst(std::function<bool (const T&)> predicate) noexcept
    {
        // Search for the first element e such that predicate(e) returns true

        auto containsValue = false; // We need this boolean to make sure the iterator given below as argument to erase is always valid.
                                    // Indeed this iterator may remain invalid if the for-loop isn't entered.
                                    // In that case, the iterator will hold an invalid value.

        typename std::vector<T>::iterator it;
        for(it=m_values.begin(); it!=m_values.end(); it++) {
            if(predicate(*it)) {
                containsValue = true;
                break;
            }
        }

        // Remove that element

        if(containsValue) {
            m_values.erase(it);
        }

        return containsValue;
    }

    bool removeFirst(const T &value) noexcept
    {
        return removeFirst([&] (const T &elem) -> bool {return elem==value;});
    }

    bool removeLast(std::function<bool (const T&)> predicate) noexcept
    {
        // Search for the last element e such that predicate(e) returns true

        auto containsValue = false;
        auto c = 0; // We need this variable because erase doesnt't applay to a reverse iterator

        typename std::vector<T>::reverse_iterator rit;
        for(rit=m_values.rbegin(); rit!=m_values.rend(); rit++) {
            c++;
            if(predicate(*rit)) {
                containsValue = true;
                break;
            }
        }

        // Remove that element

        if(containsValue) {
            const auto &k = size()-c;
            m_values.erase(m_values.begin()+k);
        }

        return containsValue;
    }

    bool removeLast(const T &value) noexcept
    {
        return removeLast([&] (const T &elem) -> bool {return elem==value;});
    }

    int remove(std::function<bool (const T&)> predicate) noexcept
    {
        // Find all elements e for which predicate(e) returns true

        std::vector<T> matches;
        for(const T& value : m_values) {
            if(predicate(value)) {
                matches.push_back(value);
            }
        }

        // Remove those elements

        for(const T &value : matches) {
            removeFirst(value);
        }

        return matches.size();
    }

    int remove(const T &value) noexcept
    {
        return remove([&] (const T &elem) -> bool {return elem==value;});
    }

    void clear() noexcept {m_values.clear();}

    /*
     ==========================================
     Gets a string-representation of this array
     ==========================================
     */

    /**
     * Returns a customizable string representation of this array.
     * Note: operator<< should be defined for type T.
     */
    std::string toString(const std::string &delStart = "[", const std::string &delEnd = "]",
                         const std::string &sep = ", ") const noexcept
    {
        std::stringstream stream;
        const int &s(size());
        int c = 0;

        stream << delStart;
        for(const auto &value : m_values) {
            stream << value;
            if(++c != s) { // Make sure that it is not the last value.
                stream << sep;
            }
        }
        stream << delEnd;

        return stream.str();
    }
};
}

#endif // ARRAY_H
