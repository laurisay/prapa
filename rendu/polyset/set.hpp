#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"

class set {
private:
    searchable_bag* _bag;

public:
    set(searchable_bag* bag);
    set(const set& other);
    set& operator=(const set& other);
    ~set();

    void insert(const searchable_bag::value_type& value);
    bool contains(const searchable_bag::value_type& value) const;
    void erase(const searchable_bag::value_type& value);
};

#endif

