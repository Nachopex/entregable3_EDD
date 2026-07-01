#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lista_doblemente_enlazada.h"

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



template <typename Tabla_hash>
int cargar_tabla(std::vector<Tabla_hash> &tablas_ID, std::vector<Tabla_hash> &tablas_screen_name, const std::string &filename, const std::string &tipo_hash) {
    
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

            if(tipo_hash == "hashing_abierto"){
                std::string user_id = columnas[5];
                std::string user_screen_name = columnas[7];
                int hash_user_id = funcionHash(user_id, tablas_ID.size());
                int hash_user_screen_name = funcionHash(user_screen_name, tablas_screen_name.size());
                tablas_ID[hash_user_id].insertar_o_sumar(user_id);
                tablas_screen_name[hash_user_screen_name].insertar_o_sumar(user_screen_name);
                lineas_leidas++;
            }
            else if(tipo_hash == "linear_probing"){

            }

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
    std::vector<Lista_doblemente_enlazada> tablas_ID(1000);
    std::vector<Lista_doblemente_enlazada> tablas_screen_name(1000);
    cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv", argv[1]);

    return 0;
}