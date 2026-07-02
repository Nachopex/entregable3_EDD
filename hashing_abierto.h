#ifndef HASHING_ABIERTO_H
#define HASHING_ABIERTO_H

#include <vector>
#include <string>
#include "Lista_doblemente_enlazada.h"

class hashing_abierto {
    private:
        std::vector<Lista_doblemente_enlazada> tabla;
        int capacidad;
        int funcionHash(std::string clave);

    public:
        hashing_abierto(int cap);
        ~hashing_abierto();

        void insertar(std::string clave);
        void imprimir_prueba(std::string nombre_tabla);
};

#endif