/*
 * Implementacion de los metodos de la clase hashing_abierto.
 */

#include "hashing_abierto.h"
#include <iostream>

// Constructor que dimensiona el vector para que coincida con la capacidad especificada.
hashing_abierto::hashing_abierto(int cap) : capacidad(cap), tabla(cap) {}

// Destructor vacio, las listas doblemente enlazadas manejan su propia liberacion de memoria.
hashing_abierto::~hashing_abierto() {}

// Calcula la posicion en la tabla sumando el valor ASCII de los caracteres 
// y aplicando la operacion modulo con la capacidad total.
int hashing_abierto::funcionHash(std::string clave) {
    unsigned long hash = 0;
    for (char c : clave) {
        hash += c;
    }
    return hash % capacidad;
}

// Obtiene el indice a traves de la funcion hash y delega la insercion/actualizacion 
// a la lista doblemente enlazada correspondiente a ese indice.
void hashing_abierto::insertar(std::string clave) {
    int indice = funcionHash(clave);
    tabla[indice].insertar_o_sumar(clave);
}

// Imprime un resumen del estado general de la tabla, incluyendo el recuento 
// de colisiones y una muestra de los datos introducidos.
void hashing_abierto::imprimir_prueba(std::string nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;
    
    int espacios_vacios = 0;
    int maximas_colisiones = 0;
    int muestras_impresas = 0;

    // Recorre todo el vector evaluando el estado de las listas asociadas a cada indice
    for (size_t i = 0; i < tabla.size(); i++) {
        if (tabla[i].isEmpty()) {
            espacios_vacios++;
        } else {
            // Actualiza el registro de maxima colision (la lista mas larga)
            if (tabla[i].size() > maximas_colisiones) {
                maximas_colisiones = tabla[i].size();
            }
            // Muestra en consola solo los primeros tres indices poblados
            if (muestras_impresas < 3) {
                std::cout << "-> En el indice [" << i << "] tenemos " << tabla[i].size() << " usuarios:" << std::endl;
                tabla[i].imprimir_muestra(5);
                muestras_impresas++;
            }
        }
    }

    // Muestra los datos recopilados durante la lectura completa de la tabla
    std::cout << "\n--- ESTADISTICAS ---" << std::endl;
    std::cout << "* Capacidad de la tabla: " << tabla.size() << std::endl;
    std::cout << "* Espacios vacios: " << espacios_vacios << std::endl;
    std::cout << "* Maximas colisiones: " << maximas_colisiones << std::endl;
    std::cout << "========================================\n" << std::endl;
}