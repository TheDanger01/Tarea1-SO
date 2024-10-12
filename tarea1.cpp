#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <mutex>
#include <iomanip>

// Mutex para sincronizar la salida a la consola
std::mutex mtx;
int puestoActual = 1; // Para llevar el control del puesto de llegada

// Funcion que simula la carrera de un auto
void correrAuto(int id, int M, std::vector<int>& resultados) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distanciaDist(1, 10);   // Distancia aleatoria (1-10 metros)
    std::uniform_int_distribution<> velocidadDist(100, 500); // Velocidad aleatoria (100-500 ms)

    int distanciaRecorrida = 0;

    while (distanciaRecorrida < M) {
        int avance = distanciaDist(gen);
        distanciaRecorrida += avance;
        if (distanciaRecorrida > M) {
            avance -= (distanciaRecorrida - M); // Ajustar el avance para que no exceda la meta
            distanciaRecorrida = M;
        }

        // Imprimir progreso en la consola
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Auto" << id << " avanza " << avance << " metros (total: "
                      << distanciaRecorrida << " metros)\n";
        }

        // Esperar un tiempo aleatorio simulando la velocidad
        std::this_thread::sleep_for(std::chrono::milliseconds(velocidadDist(gen)));
    }

    // Almacenar el resultado final (llegada) y asignar puesto
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Auto" << id << " termina la carrera en el lugar "
                  << puestoActual << "!\n";
        resultados.push_back(id);  // Guardar el ID en el orden de llegada
        puestoActual++; // Incrementar el puesto para el próximo auto que termine
    }
}

// Funcion principal
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total_M> <numero_autos_N>\n";
        return 1;
    }

    int M = std::stoi(argv[1]); // Distancia total de la carrera
    int N = std::stoi(argv[2]); // Numero de autos

    std::vector<std::thread> autos;
    std::vector<int> resultados; // Almacena el orden de llegada de los autos

    std::cout << "Distancia total carrera: " << M << " metros\n\n";

    // Iniciar la carrera (lanzar las hebras)
    for (int i = 1; i <= N; ++i) {
        autos.push_back(std::thread(correrAuto, i, M, std::ref(resultados)));
    }

    // Esperar a que todas las hebras terminen
    for (auto& autoThread : autos) {
        autoThread.join();
    }

    // Mostrar resultados finales en forma de tabla
    std::cout << "\nLugar\tAuto\n";
    std::cout << "-----------------\n";
    for (size_t i = 0; i < resultados.size(); ++i) {
        std::cout << (i + 1) << "\tAuto" << resultados[i] << "\n";
    }

    return 0;
}