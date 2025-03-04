#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include "../include/tokenizador.h"

using namespace std;
using namespace std::chrono;

int main() {
    auto start = high_resolution_clock::now();

    Tokenizador a("\t ,;:.-+/*_`'{}[]()!?&#\"\\<>", true, true);
    a.TokenizarListaFicheros("listaFicheros.txt"); // TODO EL CORPUS

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    cout << "Ha tardado " << elapsed.count() << " segundos" << endl;

    return 0;
}
