#include "set.hpp"
#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"

class set
{
private:
    searchable_bag *bag;

public:
    // Constructeur par défaut public
    set() : bag(0) {}

    // Constructeur avec référence
    set(searchable_bag &other) : bag(&other) {}

    // Constructeur avec pointeur
    set(searchable_bag* other) : bag(other) {}

    // Copy constructor
    set(const set &other)
    {
        if (other.bag)
            bag = other.bag;
        else
            bag = 0;
    }

    ~set() {}

    set &operator=(const set &other)
    {
        if (this != &other)
        {
            if (other.bag)
                bag = other.bag;
            else
                bag = 0;
        }
        return *this;
    }

    void insert(int item)
    {
        if (bag && !has(item))
            (*bag).insert(item);
    }

    void insert(int *items, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (bag && !has(items[i]))
                (*bag).insert(items[i]);
        }
    }

    void print() const
    {
        if (bag)
            (*bag).print();
    }

    void clear()
    {
        if (bag)
            (*bag).clear();
    }

    bool has(int item) const
    {
        return bag ? (*bag).has(item) : false;
    }

    searchable_bag &get_bag() const
    {
        return (*bag);
    }
};

#endif
