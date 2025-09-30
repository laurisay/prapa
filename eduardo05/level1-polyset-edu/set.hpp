#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"

class set
{
private:
    // SEGUNDA PARTE DEL EJERCICIO:
    // Es implementar un SET, nos basamos de la clase BAG.

    // el set comienza con un bag = 0, y llamalos en parametro searchable bag
    set() : bag(0) {}
    searchable_bag *bag;

public:
    // Ortodox
    set(searchable_bag &other) : bag(&other) {}

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


    // Funciones SET, ya llamadas en en la clase BAG.

    // 1. El insert basico, si el bag existe y no existe el item.
    void insert(int item)
    {
        if (bag && !has(item))
            (*bag).insert(item);
    }

    // 2. El insert mismo del anterior, pero con posicion.
    void insert(int *items, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (bag && !has(items[i]))
                (*bag).insert(items[i]);
        }
    }

    // 3. llamar la print de la clase bag (pura virtual, es const)
    void print() const
    {
        (*bag).print();
    }

    // 4. llamar el clear de la clase bag.
    void clear()
    {
        (*bag).clear();
    }

    // 5. llamar el has de la clase bag (pura virtual, es const)
    bool has(int item) const
    {
        return (*bag).has(item);
    }

    // 6. El getter del bag (es pura virtual, es const)
    searchable_bag &get_bag() const
    {
        return (*bag);
    }
};

#endif
