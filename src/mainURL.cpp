#include <iostream>
#include "../include/tokenizador.h"

using namespace std;

void imprimirListaSTL(const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
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
    cout << "1" << endl;
    imprimirListaSTL(tokens);

    a.DelimitadoresPalabra("@");
    a.Tokenizar(s, tokens);
    cout << "2" << endl;
    imprimirListaSTL(tokens);

    a.DelimitadoresPalabra("/ ");
    a.Tokenizar(s, tokens);
    cout << "3" << endl;
    imprimirListaSTL(tokens);

    a.DelimitadoresPalabra("_:/.?&-=#@");
    a.Tokenizar(s, tokens);
    cout << "4" << endl;
    imprimirListaSTL(tokens);

    a.DelimitadoresPalabra("/&");
    s = "p0 hhttp://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013 p1 p2";
    a.Tokenizar(s, tokens);
    cout << "5" << endl;
    imprimirListaSTL(tokens);

    s = "p0 Http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013 p1 p2";
    a.Tokenizar(s, tokens);
    cout << "6" << endl;
    imprimirListaSTL(tokens);

    a.Tokenizar("http:////ab.", tokens);
    cout << "7" << endl;
    imprimirListaSTL(tokens);

}