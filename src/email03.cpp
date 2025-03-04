#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"

using namespace std;

///////// Comprobación de que vacíe la lista resultado

void imprimirListaSTL(int i, const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
        cout<<i<<" ";
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

Tokenizador a("-#", true, false); 
list<string> tokens; 

a.DelimitadoresPalabra("@.&");
a.Tokenizar("catedraTelefonicaUA@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(1,tokens);

a.Tokenizar("pal1 @iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(2,tokens);

a.DelimitadoresPalabra("&.");
a.Tokenizar("catedraTelefonicaUA@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(3,tokens);

a.Tokenizar("pal1 @iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(4,tokens);

a.Tokenizar("pal1&@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(5,tokens);

a.Tokenizar("pal1&catedra@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(6,tokens);

a.PasarAminuscSinAcentos(true);
a.Tokenizar("catedraTelefonicaUA@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(7,tokens);


a.DelimitadoresPalabra("@.&");
a.CasosEspeciales (false);
a.Tokenizar("catedraTelefonicaUA@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(8,tokens);


}
