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
    list<string> lt1, lt2;

    a.DelimitadoresPalabra("@.,&");
    a.Tokenizar("pal1 10.000,34 10,000.34 10.000.123.456.789.009,34 10,000,123,456,789,009.34 20.03 40,03 2005 10. 20, 10.0 20,0 La 20,12.456,7.8.9,", lt1);
    cout << "1" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar(".34 ,56", lt1);
    // La lista de tokens a devolver debería contener: "0.34 0,56"
    cout << "2" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar("pal1 10.35% 10,35% 23.000,3% 23,5$ 23.05$ 23,05$ 11.1$ 11.05$ 3.4% 4,3% 9$ 7% 9,56%@ 9,56% @", lt1);
    cout << "3" << endl;
    imprimirListaSTL(lt1);
    cout << endl;
    
    a.Tokenizar("pal1 10.00a 10.000.a.000 10/12/85 1,23E+10", lt1);
    // La lista de tokens a devolver debería contener (no extraería números sino
    //acrónimos): "pal1 10.00a 10.000.a.000 10/12/85 1 23E+10"
    cout << "4" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar("pal1&10.00@10.000&abc@10/12/85", lt1);
    cout << "5" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar(".34@@&,56", lt1);
    // La lista de tokens a devolver debería contener: "0.34 0,56"
    cout << "6" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar("...10.000.a.000 ,,23.05 10/12/85 1,23E+10", lt1);
    cout << "7" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar("...10.000.000 ,,23.05 10/12/85 1,23E+10", lt1);
    cout << "8" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar("3..2 4,,,,5 ..35", lt1);
    cout << "9" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.DelimitadoresPalabra("");
    a.Tokenizar("...10.000.a.000 ,,23.05 10/12/85 1,23E+10", lt1);
    // La lista de tokens a devolver debería contener (no quita las , ni los .
    // porque ahora no son delimitadores, por lo que no detecta acrónimos ni
    // números): "...10.000.a.000 ,,23.05 10/12/85 1,23E+10"
    cout << "10" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    a.Tokenizar("3..2 4,,,,5 ..35", lt1);
    // La lista de tokens a devolver debería contener (no quita las , ni los .
    // porque ahora no son delimitadores, por lo que no detecta acrónimos ni
    // números): "3..2 4,,,,5 ..35"
    cout << "11" << endl;
    imprimirListaSTL(lt1);
    cout << endl;

    return 0;

}