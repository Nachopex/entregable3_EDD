/*
 * Archivo principal del programa. 
 * Contiene el punto de entrada, el procesador de archivos CSV y la logica 
 * generica de llenado que opera con ambas implementaciones de tablas Hash.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "hashing_cerrado.h"
#include "hashing_abierto.h"

// Analiza una linea de texto con formato CSV, respetando las comas incrustadas
// dentro de campos delimitados por comillas dobles.
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
/*
 * Funcion plantilla (Template) capaz de procesar un archivo CSV y cargar 
 * los datos especificos en cualquier tipo de tabla hash que provea los 
 * metodos requeridos (insertar).
 */
template <typename TipoTabla>
int cargar_tabla(TipoTabla &tabla_ID, TipoTabla &tabla_screen_name, const std::string &filename) {
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return 1;
    }

    std::string line;

    // Se lee y descarta la primera linea correspondiente a las cabeceras del CSV
    std::getline(file, line);

    int lineas_leidas = 0;
    std::string registro_completo = "";
    bool en_comillas = false;

    // Bucle para leer el archivo linea por linea manejando posibles saltos 
    // de linea contenidos dentro de comillas (por ej. en el texto del tweet)
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

        // Se extraen y guardan exclusivamente el ID del usuario y su nombre de pantalla
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

// Punto de entrada del programa. Recibe un argumento por consola que dicta 
// el modo de operacion a utilizar (tipo de tabla Hash a probar).
int main(int argc, char* argv[]) {
    // Validacion del numero de argumentos
    if (argc != 2) {
        std::cerr << "entrada incorrecta" << std::endl;
        return 1;
    }
    
    std::string modo = argv[1];
    
    // Evaluacion del argumento para ejecutar Hashing Abierto
    if(modo == "hashing_abierto"){
            hashing_abierto tablas_ID(1000);
            hashing_abierto tablas_screen_name(1000);
            cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
            tablas_ID.imprimir_prueba("Tabla de User IDs");
            tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    }
    // Evaluacion del argumento para ejecutar Hashing Cerrado
    else if(modo == "hashing_cerrado"){
        hashing_cerrado tablas_ID(1000);
        hashing_cerrado tablas_screen_name(1000);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    }
    // Flujo alternativo
    else{
        
    }

    return 0;
}