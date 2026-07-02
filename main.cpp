#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "hashing_cerrado.h"
#include "hashing_abierto.h"
std::vector<std::string> separarLineaCSV(const std::string& linea) {
    std::vector<std::string> columnas;
    std::string columna_actual;
    bool en_comillas = false;

    for (char caracter : linea) {
        if (caracter == '\"') {
            en_comillas = !en_comillas;
        } else if (caracter == ',' && !en_comillas) {
            columnas.push_back(columna_actual);
            columna_actual.clear();
        } else {
            columna_actual += caracter;
        }
    }
    columnas.push_back(columna_actual);
    
    return columnas;
}


int funcionHash(std::string clave, int capacidad) {
    unsigned long hash = 0;
    for (char c : clave) {
        hash += c;
    }
    return hash % capacidad;
}


template <typename TipoTabla>
int cargar_tabla(TipoTabla &tabla_ID, TipoTabla &tabla_screen_name, const std::string &filename) {
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }

    std::string line;

    std::getline(file, line);

    int lineas_leidas = 0;
    std::string registro_completo = "";
    bool en_comillas = false;

    while (std::getline(file, line)) {
        registro_completo += line;
        
        for (char c : line) {
            if (c == '\"') {
                en_comillas = !en_comillas;
            }
        }
        
        if (en_comillas) {
            registro_completo += "\n";
            continue; 
        }

        std::vector<std::string> columnas = separarLineaCSV(registro_completo);

        if (columnas.size() > 7) {
            std::string user_id = columnas[5];
            std::string user_screen_name = columnas[7];
            tabla_ID.insertar(user_id);
            tabla_screen_name.insertar(user_screen_name);
            lineas_leidas++;
        }

        registro_completo.clear();
    }

    file.close();
    std::cout << "Se cargaron " << lineas_leidas << " tweets correctamente." << std::endl;
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "entrada incorrecta" << std::endl;
        return 1;
    }
    std::string modo = argv[1];
    if(modo == "hashing_abierto"){
            hashing_abierto tablas_ID(1000);
            hashing_abierto tablas_screen_name(1000);
            cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
            tablas_ID.imprimir_prueba("Tabla de User IDs");
            tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    }
    else if(modo == "hashing_cerrado"){
        hashing_cerrado tablas_ID(1000);
        hashing_cerrado tablas_screen_name(1000);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    }

    else{
        
    }


    return 0;
}