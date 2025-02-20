#include <iostream> 
#include <string>
#include <list> 
#include "../include/tokenizador.h"

using namespace std;

///////// Comprobaci�n de que vac�e la lista resultado

void imprimirListaSTL(const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
        for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
        {
                cout << (*itCadena) << ", ";
        }
        cout << endl;
}

int
main(void)
{
	bool kCasosEspeciales = true, kpasarAminusculas = false;

	list<string> lt1, lt2;

Tokenizador a("-#:/.", true, false); 
list<string> tokens; 

a.Tokenizar("a http:", tokens);
	imprimirListaSTL(tokens);


a.Tokenizar("a http:////ab/ s", tokens);
	imprimirListaSTL(tokens);

a.Tokenizar("http:////ab.yyeees a", tokens);
	imprimirListaSTL(tokens);


}
