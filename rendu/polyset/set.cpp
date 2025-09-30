#include "set.hpp"
#include <cstdlib> // pour atoi si nécessaire

set::set(searchable_bag* bag) : _bag(bag) {}

set::set(const set& other) : _bag(other._bag) {}

set& set::operator=(const set& other) {
    if (this != &other) {
        _bag = other._bag;
    }
    return *this;
}

set::~set() {}

void set::insert(const searchable_bag::value_type& value) {
    if (!_bag->contains(value))
        _bag->insert(value);
}

bool set::contains(const searchable_bag::value_type& value) const {
    return _bag->contains(value);
}

void set::erase(const searchable_bag::value_type& value) {
    _bag->erase(value);
}

