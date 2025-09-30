#ifndef SEARCHABLE_ARRAY_BAG_HPP
#define SEARCHABLE_ARRAY_BAG_HPP

#include "array_bag.hpp"
#include "searchable_bag.hpp"

// PARTE 1
// Te piden hacer la clase que busque el array_bag y el tree_bag (son distintos)
// metodos de almacenar una lista de objetos.
// Como en el subject no se entiende nada, lo explico aca:
// Se tienen que implementar las clases para array y tree, metodo orthodox.
// Lo bueno es que ya nos dan las clases de array class, tree class implementadas, solo debemos
// Hacer la clase de busqueda.
// Comenzamos con el array, metodo orthodox.

// 1. Recordar que las dos clases heredaras deben ser publicas.
class searchable_array_bag : public array_bag, public searchable_bag
{
public:
    searchable_array_bag() : array_bag() {}
    searchable_array_bag(const searchable_array_bag &other) : array_bag(other) {}
    ~searchable_array_bag() {}

    // como array_bag ya tiene un 'operator =' lo la llamamos solo aca.
    searchable_array_bag &operator=(const searchable_array_bag &other) // recordar que esta con &
    {
        if (this != &other) // recordar la instancia a other => &other
            array_bag::operator=(other);
        return *this;
    }

    // 2. Aca el metodo 'has' ya esta implementado en la clase vietual "array_bag" entonces es CONST.
    // como es un array de datos (data esta en array_bag, es solo recorrerlo)
    bool has(int item) const
    {

        for (int i = 0; i < size; i++)
        {
            if (data[i] == item) // ==, recordar.
                return true;
        }
        return false;
    }

    // Yo lo implementaria todo en el header y en el cpp solo llamaria la cabecera, para no perder tiempo.
};

#endif
