/*
 * Archivo principal del programa. 
 * Contiene el punto de entrada, el procesador de archivos CSV y la logica 
 * generica de llenado que opera con ambas implementaciones de tablas Hash.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
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

template <typename Mapa>
int cargar_mapa(Mapa &mapa_ID, Mapa &mapa_screen_name, const std::string &filename) {
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
            mapa_ID[user_id]++;
            mapa_screen_name[user_screen_name]++;
            lineas_leidas++;
        }

        registro_completo.clear();
    }

    file.close();
    std::cout << "Se cargaron " << lineas_leidas << " tweets correctamente." << std::endl;
    return 0;
}

template <typename Mapa>
void imprimir_reporte_mapa(const Mapa &mapa, const std::string &nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;

    int muestras_impresas = 0;
    for (const auto &par : mapa) {
        if (muestras_impresas < 5) {
            std::cout << "-> Clave: " << par.first << " | Tweets: " << par.second << std::endl;
            muestras_impresas++;
        } else {
            break;
        }
    }

    std::cout << "\n--- ESTADISTICAS ---" << std::endl;
    std::cout << "* Elementos unicos: " << mapa.size() << std::endl;
    std::cout << "========================================\n" << std::endl;
}

void imprimir_reporte_unordered_map(const std::unordered_map<std::string, int> &mapa, const std::string &nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;

    int muestras_impresas = 0;
    for (const auto &par : mapa) {
        if (muestras_impresas < 5) {
            std::cout << "-> Clave: " << par.first << " | Tweets: " << par.second << std::endl;
            muestras_impresas++;
        } else {
            break;
        }
    }

    std::cout << "\n--- ESTADISTICAS ---" << std::endl;
    std::cout << "* Elementos unicos: " << mapa.size() << std::endl;
    std::cout << "* Buckets: " << mapa.bucket_count() << std::endl;
    std::cout << "* Factor de carga: " << mapa.load_factor() << std::endl;
    std::cout << "========================================\n" << std::endl;
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
            hashing_abierto tablas_ID(1000, USER_ID);
            hashing_abierto tablas_screen_name(1000, USER_SCREEN_NAME);
            cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
            tablas_ID.imprimir_prueba("Tabla de User IDs");
            tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    }
    // Evaluacion de los argumentos para ejecutar Hashing Cerrado con cada una
    // de las 3 estrategias de manejo de colisiones solicitadas en el entregable.
    else if(modo == "hashing_cerrado_linear"){
        hashing_cerrado tablas_ID(1000, LINEAR, USER_ID);
        hashing_cerrado tablas_screen_name(1000, LINEAR, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    }
    else if(modo == "hashing_cerrado_quadratic"){
        hashing_cerrado tablas_ID(1000, QUADRATIC, USER_ID);
        hashing_cerrado tablas_screen_name(1000, QUADRATIC, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs (Quadratic Probing)");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names (Quadratic Probing)");
    }
    else if(modo == "hashing_cerrado_double"){
        hashing_cerrado tablas_ID(1000, DOUBLE, USER_ID);
        hashing_cerrado tablas_screen_name(1000, DOUBLE, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs (Double Hashing)");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names (Double Hashing)");
    }
    else if(modo == "unordered_map"){
        std::unordered_map<std::string, int> tablas_ID;
        std::unordered_map<std::string, int> tablas_screen_name;
        cargar_mapa(tablas_ID, tablas_screen_name, "auspol2019.csv");
        imprimir_reporte_unordered_map(tablas_ID, "Tabla de User IDs (unordered_map)");
        imprimir_reporte_unordered_map(tablas_screen_name, "Tabla de Screen Names (unordered_map)");
    }
    // Flujo alternativo
    else{
        std::cerr << "Modo desconocido. Use: hashing_abierto, hashing_cerrado_linear, "
                  << "hashing_cerrado_quadratic, hashing_cerrado_double o unordered_map." << std::endl;
        return 1;
    }

    return 0;
}
