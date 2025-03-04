#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"

using namespace std;

///////// Comprobaci�n de que vac�e la lista resultado

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

int main(){
    Tokenizador a(",", true, false);
    list<string> tokens;
    string s = "p0 http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es p1 p2";

    a.Tokenizar(s, tokens);
    imprimirListaSTL(1, tokens);

    a.DelimitadoresPalabra("@");

    a.Tokenizar(s, tokens);
    imprimirListaSTL(2, tokens);
    
    a.DelimitadoresPalabra("/ ");

    a.Tokenizar(s, tokens);
    imprimirListaSTL(3, tokens);

    a.DelimitadoresPalabra("_:/.?&-=#@");

    a.Tokenizar(s, tokens);
    imprimirListaSTL(4, tokens);

    a.DelimitadoresPalabra("/&");

    s = "p0 hhttp://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013 p1 p2";

    a.Tokenizar(s, tokens);
    imprimirListaSTL(5, tokens);

    s = "p0 Http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013 p1 p2";
    
    a.Tokenizar(s, tokens);
    imprimirListaSTL(6, tokens);

    a.Tokenizar("http:////ab.", tokens);
    imprimirListaSTL(7, tokens);

    a.PasarAminuscSinAcentos(true);

    a.Tokenizar(s, tokens);
    imprimirListaSTL(8, tokens);

    return 0;
}
