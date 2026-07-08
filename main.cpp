/*
 * Archivo principal del programa.
 * Contiene el punto de entrada, el procesador de archivos CSV y la logica
 * generica de llenado que opera con ambas implementaciones de tablas Hash.
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "hashing_abierto.h"
#include "hashing_cerrado.h"

struct RegistroTweet {
    std::string user_id;
    std::string user_screen_name;
};

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

std::string formatear_bytes(size_t bytes) {
    std::ostringstream salida;
    salida << std::fixed << std::setprecision(2);

    if (bytes >= 1024ULL * 1024ULL) {
        salida << (static_cast<double>(bytes) / (1024.0 * 1024.0)) << " MB";
    } else if (bytes >= 1024ULL) {
        salida << (static_cast<double>(bytes) / 1024.0) << " KB";
    } else {
        salida << bytes << " B";
    }

    return salida.str();
}

double promedio(const std::vector<double>& valores) {
    if (valores.empty()) {
        return 0.0;
    }
    return std::accumulate(valores.begin(), valores.end(), 0.0) / static_cast<double>(valores.size());
}

double desviacion_estandar(const std::vector<double>& valores, double media) {
    if (valores.size() < 2) {
        return 0.0;
    }

    double suma = 0.0;
    for (double valor : valores) {
        double diferencia = valor - media;
        suma += diferencia * diferencia;
    }

    return std::sqrt(suma / static_cast<double>(valores.size() - 1));
}

template <typename Mapa>
size_t estimar_memoria_unordered_map(const Mapa& mapa) {
    size_t memoria = sizeof(mapa);
    memoria += mapa.bucket_count() * sizeof(void*);
    memoria += mapa.size() * sizeof(typename Mapa::value_type);
    return memoria;
}

// Carga un conjunto de registros del CSV para usarlos en los experimentos.
// Se detiene al alcanzar el limite solicitado.
std::vector<RegistroTweet> cargar_registros_csv(const std::string& filename, size_t limite = std::numeric_limits<size_t>::max()) {
    std::vector<RegistroTweet> registros;
    registros.reserve(limite == std::numeric_limits<size_t>::max() ? 0 : limite);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return registros;
    }

    std::string line;
    std::getline(file, line);

    std::string registro_completo;
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
            registros.push_back({columnas[5], columnas[7]});
            if (registros.size() >= limite) {
                break;
            }
        }

        registro_completo.clear();
    }

    return registros;
}

/*
 * Funcion plantilla capaz de procesar un archivo CSV y cargar los datos
 * especificos en cualquier tipo de tabla hash que provea el metodo insertar.
 */
template <typename TipoTabla>
int cargar_tabla(TipoTabla& tabla_ID, TipoTabla& tabla_screen_name, const std::string& filename) {
    std::vector<RegistroTweet> registros = cargar_registros_csv(filename);
    if (registros.empty()) {
        return 1;
    }

    for (const auto& registro : registros) {
        tabla_ID.insertar(registro.user_id);
        tabla_screen_name.insertar(registro.user_screen_name);
    }

    std::cout << "Se cargaron " << registros.size() << " tweets correctamente." << std::endl;
    return 0;
}

template <typename Mapa>
int cargar_mapa(Mapa& mapa_ID, Mapa& mapa_screen_name, const std::string& filename) {
    std::vector<RegistroTweet> registros = cargar_registros_csv(filename);
    if (registros.empty()) {
        return 1;
    }

    for (const auto& registro : registros) {
        mapa_ID[registro.user_id]++;
        mapa_screen_name[registro.user_screen_name]++;
    }

    std::cout << "Se cargaron " << registros.size() << " tweets correctamente." << std::endl;
    return 0;
}

template <typename Mapa>
void imprimir_reporte_mapa(const Mapa& mapa, const std::string& nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;

    int muestras_impresas = 0;
    for (const auto& par : mapa) {
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

void imprimir_reporte_unordered_map(const std::unordered_map<std::string, int>& mapa, const std::string& nombre_tabla) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "REPORTE DE PRUEBA: " << nombre_tabla << std::endl;
    std::cout << "========================================" << std::endl;

    int muestras_impresas = 0;
    for (const auto& par : mapa) {
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

template <typename Estructura, typename Factory, typename Inserter, typename Extractor, typename MemoryFn>
void ejecutar_serie_benchmark(
    const std::vector<RegistroTweet>& registros,
    const std::vector<size_t>& tamanos,
    int repeticiones,
    const std::string& dataset,
    const std::string& nombre_estructura,
    const std::string& nombre_clave,
    std::ofstream& csv,
    int& numero_experimento,
    Factory factory,
    Inserter inserter,
    Extractor extractor,
    MemoryFn memory_fn
) {
    std::cout << "\n[" << nombre_estructura << " | " << nombre_clave << "]" << std::endl;

    for (size_t cantidad : tamanos) {
        std::vector<double> tiempos_ms;
        size_t memoria_bytes = 0;
        tiempos_ms.reserve(static_cast<size_t>(repeticiones));

        for (int repeticion = 1; repeticion <= repeticiones; ++repeticion) {
            Estructura estructura = factory();

            auto inicio = std::chrono::steady_clock::now();
            for (size_t i = 0; i < cantidad; ++i) {
                inserter(estructura, extractor(registros[i]));
            }
            auto fin = std::chrono::steady_clock::now();

            double tiempo_ms = std::chrono::duration<double, std::milli>(fin - inicio).count();
            tiempos_ms.push_back(tiempo_ms);
            memoria_bytes = memory_fn(estructura);

            csv << numero_experimento++ << ';'
                << dataset << ';'
                << nombre_estructura << ';'
                << nombre_clave << ';'
                << cantidad << ';'
                << repeticion << ';'
                << std::fixed << std::setprecision(3) << tiempo_ms << ';'
                << memoria_bytes << '\n';
        }

        double media = promedio(tiempos_ms);
        double desv = desviacion_estandar(tiempos_ms, media);

        std::cout << "  " << std::setw(7) << cantidad << " tweets"
                  << " | promedio: " << std::setw(10) << std::fixed << std::setprecision(3) << media << " ms"
                  << " | desv: " << std::setw(10) << std::fixed << std::setprecision(3) << desv << " ms"
                  << " | memoria: " << formatear_bytes(memoria_bytes)
                  << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "entrada incorrecta" << std::endl;
        return 1;
    }

    std::string modo = argv[1];

    if (modo == "hashing_abierto") {
        hashing_abierto tablas_ID(1000, USER_ID);
        hashing_abierto tablas_screen_name(1000, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    } else if (modo == "hashing_cerrado_linear") {
        hashing_cerrado tablas_ID(1000, LINEAR, USER_ID);
        hashing_cerrado tablas_screen_name(1000, LINEAR, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names");
    } else if (modo == "hashing_cerrado_quadratic") {
        hashing_cerrado tablas_ID(1000, QUADRATIC, USER_ID);
        hashing_cerrado tablas_screen_name(1000, QUADRATIC, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs (Quadratic Probing)");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names (Quadratic Probing)");
    } else if (modo == "hashing_cerrado_double") {
        hashing_cerrado tablas_ID(1000, DOUBLE, USER_ID);
        hashing_cerrado tablas_screen_name(1000, DOUBLE, USER_SCREEN_NAME);
        cargar_tabla(tablas_ID, tablas_screen_name, "auspol2019.csv");
        tablas_ID.imprimir_prueba("Tabla de User IDs (Double Hashing)");
        tablas_screen_name.imprimir_prueba("Tabla de Screen Names (Double Hashing)");
    } else if (modo == "unordered_map") {
        std::unordered_map<std::string, int> tablas_ID;
        std::unordered_map<std::string, int> tablas_screen_name;
        cargar_mapa(tablas_ID, tablas_screen_name, "auspol2019.csv");
        imprimir_reporte_unordered_map(tablas_ID, "Tabla de User IDs (unordered_map)");
        imprimir_reporte_unordered_map(tablas_screen_name, "Tabla de Screen Names (unordered_map)");
    } else if (modo == "benchmark") {
        const std::string dataset = "auspol2019.csv";
        const int repeticiones = 20;
        const size_t max_tweets = 180000;
        const size_t paso = 10000;

        std::vector<RegistroTweet> registros = cargar_registros_csv(dataset, max_tweets);
        if (registros.size() < paso) {
            std::cerr << "El dataset no contiene suficientes registros para el benchmark." << std::endl;
            return 1;
        }

        std::vector<size_t> tamanos;
        for (size_t cantidad = paso; cantidad <= std::min(max_tweets, registros.size()); cantidad += paso) {
            tamanos.push_back(cantidad);
        }

        std::ofstream csv("resultados_experimentos.csv");
        if (!csv.is_open()) {
            std::cerr << "No fue posible crear el archivo resultados_experimentos.csv" << std::endl;
            return 1;
        }

        csv << "experimento;dataset;estructura_datos;clave;cantidad_tweets;repeticion;tiempo_ms;memoria_bytes\n";

        int numero_experimento = 1;

        auto insertar_hash = [](auto& estructura, const std::string& clave) {
            estructura.insertar(clave);
        };

        auto insertar_mapa = [](auto& mapa, const std::string& clave) {
            mapa[clave]++;
        };

        auto extraer_user_id = [](const RegistroTweet& registro) -> const std::string& {
            return registro.user_id;
        };

        auto extraer_screen_name = [](const RegistroTweet& registro) -> const std::string& {
            return registro.user_screen_name;
        };

        ejecutar_serie_benchmark<hashing_abierto>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_abierto",
            "user_id",
            csv,
            numero_experimento,
            []() { return hashing_abierto(1000, USER_ID); },
            insertar_hash,
            extraer_user_id,
            [](hashing_abierto& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_abierto>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_abierto",
            "user_screen_name",
            csv,
            numero_experimento,
            []() { return hashing_abierto(1000, USER_SCREEN_NAME); },
            insertar_hash,
            extraer_screen_name,
            [](hashing_abierto& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_cerrado>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_cerrado_linear",
            "user_id",
            csv,
            numero_experimento,
            []() { return hashing_cerrado(1000, LINEAR, USER_ID); },
            insertar_hash,
            extraer_user_id,
            [](hashing_cerrado& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_cerrado>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_cerrado_linear",
            "user_screen_name",
            csv,
            numero_experimento,
            []() { return hashing_cerrado(1000, LINEAR, USER_SCREEN_NAME); },
            insertar_hash,
            extraer_screen_name,
            [](hashing_cerrado& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_cerrado>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_cerrado_quadratic",
            "user_id",
            csv,
            numero_experimento,
            []() { return hashing_cerrado(1000, QUADRATIC, USER_ID); },
            insertar_hash,
            extraer_user_id,
            [](hashing_cerrado& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_cerrado>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_cerrado_quadratic",
            "user_screen_name",
            csv,
            numero_experimento,
            []() { return hashing_cerrado(1000, QUADRATIC, USER_SCREEN_NAME); },
            insertar_hash,
            extraer_screen_name,
            [](hashing_cerrado& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_cerrado>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_cerrado_double",
            "user_id",
            csv,
            numero_experimento,
            []() { return hashing_cerrado(1000, DOUBLE, USER_ID); },
            insertar_hash,
            extraer_user_id,
            [](hashing_cerrado& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<hashing_cerrado>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "hashing_cerrado_double",
            "user_screen_name",
            csv,
            numero_experimento,
            []() { return hashing_cerrado(1000, DOUBLE, USER_SCREEN_NAME); },
            insertar_hash,
            extraer_screen_name,
            [](hashing_cerrado& estructura) { return estructura.memoria_aproximada_bytes(); }
        );

        ejecutar_serie_benchmark<std::unordered_map<std::string, int>>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "unordered_map",
            "user_id",
            csv,
            numero_experimento,
            []() { return std::unordered_map<std::string, int>{}; },
            insertar_mapa,
            extraer_user_id,
            [](const std::unordered_map<std::string, int>& mapa) { return estimar_memoria_unordered_map(mapa); }
        );

        ejecutar_serie_benchmark<std::unordered_map<std::string, int>>(
            registros,
            tamanos,
            repeticiones,
            dataset,
            "unordered_map",
            "user_screen_name",
            csv,
            numero_experimento,
            []() { return std::unordered_map<std::string, int>{}; },
            insertar_mapa,
            extraer_screen_name,
            [](const std::unordered_map<std::string, int>& mapa) { return estimar_memoria_unordered_map(mapa); }
        );

        std::cout << "\nArchivo CSV generado: resultados_experimentos.csv" << std::endl;
        std::cout << "Cada fila incluye experimento, dataset, estructura, clave, cantidad de tweets, repeticion, tiempo y memoria estimada." << std::endl;
    } else {
        std::cerr << "Modo desconocido. Use: hashing_abierto, hashing_cerrado_linear, "
                  << "hashing_cerrado_quadratic, hashing_cerrado_double, unordered_map o benchmark." << std::endl;
        return 1;
    }

    return 0;
}
