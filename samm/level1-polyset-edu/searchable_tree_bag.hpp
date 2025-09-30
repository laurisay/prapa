#ifndef SEARCHABLE_TREE_BAG_HPP
#define SEARCHABLE_TREE_BAG_HPP

#include "tree_bag.hpp"
#include "searchable_bag.hpp"

// PARTE 1:
// Mismo procedimiento que array bag. Solo cambia la funcion "has"

class searchable_tree_bag : public tree_bag, public searchable_bag
{
    public:
    searchable_tree_bag() : tree_bag(){}
    searchable_tree_bag(const searchable_tree_bag& other) : tree_bag(other){}
    ~searchable_tree_bag(){}
    searchable_tree_bag& operator=(const searchable_tree_bag& other)
    {
        if(this!= &other)
        {
            tree_bag::operator= (other);
        }
        return *this;
    }

    // Como es un TREE, es una estructura en "tree_bag", entonces el un tipo de modo TREE.
    // creamos un current (tree que vamos a recorrer)
    // Mientras estemos en tree:
    // - Si el valor es menor al valor actual, nos movemos a la izquierda
    // - Si no, si el valor es mayor al actual, nos vemos a la derecha
    // - Si no, lo encontramos y es TRUE
    // - Y si ya no no hay valores en el arbol actual, no lo encontramos ==> FALSE.
    bool has(int item) const
    {
        node *current =  tree;

        while(current)
        {
            if(item < current->value)
                current = current->l;
            else if(item > current->value)
                current = current->r;
            else
                return true;
        }
        return false;
    }
};

#endif
