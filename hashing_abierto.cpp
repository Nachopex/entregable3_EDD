#include "hashing_abierto.h"
#include <iostream>

hashing_abierto::hashing_abierto(int cap) : capacidad(cap), tabla(cap) {}

hashing_abierto::~hashing_abierto() {}

int hashing_abierto::funcionHash(std::string clave) {
    unsigned long hash = 0;
    for (char c : clave) {
        hash += c;
    }
    return hash % capacidad;
}

void hashing_abierto::insertar(std::string clave) {
    int indice = funcionHash(clave);
    tabla[indice].insertar_o_sumar(clave);
}

void hashing_abierto::imprimir_prueba(std::string nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;
    
    int espacios_vacios = 0;
    int maximas_colisiones = 0;
    int muestras_impresas = 0;

    for (size_t i = 0; i < tabla.size(); i++) {
        if (tabla[i].isEmpty()) {
            espacios_vacios++;
        } else {
            if (tabla[i].size() > maximas_colisiones) {
                maximas_colisiones = tabla[i].size();
            }
            if (muestras_impresas < 3) {
                std::cout << "-> En el indice [" << i << "] tenemos " << tabla[i].size() << " usuarios:" << std::endl;
                tabla[i].imprimir_muestra(5);
                muestras_impresas++;
            }
        }
    }

    std::cout << "\n--- ESTADISTICAS ---" << std::endl;
    std::cout << "* Capacidad de la tabla: " << tabla.size() << std::endl;
    std::cout << "* Espacios vacios: " << espacios_vacios << std::endl;
    std::cout << "* Maximas colisiones: " << maximas_colisiones << std::endl;
    std::cout << "========================================\n" << std::endl;
}