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

        a.DelimitadoresPalabra("@.&");
        a.Tokenizar("U.S.A p1 e.g. p2. La", lt1);
        // La lista de tokens a devolver debería contener: "U.S.A, p1, e.g, p2, La"
        cout << "1" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("U..S.A p1 e..g. p2. La", lt1);
        // La lista de tokens a devolver debería contener (al encontrar el segundo
        // punto seguido no lo considera como acrónimo, por lo que extrae el primer token
        // “U”; el siguiente token sí lo detecta como acrónimo “S.A”): "U, S.A, p1, e, g,
        // p2, La"
        cout << "2" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("U.S....A.BC.D ", lt1);
        // La lista de tokens a devolver debería contener (al encontrar el segundo
        // punto seguido no lo considera como acrónimo, por lo que extrae el primer
        // acrónimo “U.S”; el siguiente token sí lo detecta como acrónimo “A.BC.D”):
        // "U.S, A.BC.D"
        cout << "3" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("...U.S.A p1 e..g. p2. La", lt1);
        // La lista de tokens a devolver debería contener: "U.S.A, p1, e, g, p2, La"
        cout << "4" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("...U.S.A... p1 e..g. p2. La", lt1);
        // La lista de tokens a devolver debería contener: "U.S.A, p1, e, g, p2, La"
        cout << "5" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("...U.S.A@p1 e..g-p2. La", lt1);
        // La lista de tokens a devolver debería contener (el acrónimo U.S.A finaliza
        // al encontrar el @ que es delimitador; “g-p2” no lo detecta como multipalabra
        // ya que el guion no es delimitador, pero tras detectar el segundo punto que
        // hace que no sea acrónimo y quita el punto delimitador del final): "U.S.A, p1,
        // e, g-p2, La"
        cout << "6" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("Hack.4.Good p1 ", lt1);
        // La lista de tokens a devolver debería contener: "Hack.4.Good, p1"
        cout << "7" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.DelimitadoresPalabra(""); // Pero al estar activados los casos especiales el blanco sí se considerará separador
        a.Tokenizar("U.S.A .U.S.A .p1 p1 e.g. p2. La", lt1);
        // La lista de tokens a devolver debería contener (no detecta ningún acrónimo
        // al no ser el . un delimitador): "U.S.A, .U.S.A, .p1, p1, e.g., p2., La"
        cout << "8" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("U..S.A p1 e..g. p2. La", lt1);
        // La lista de tokens a devolver debería contener (no lo detecta como
        // acrónimo, ya que el . no es delimitador): "U..S.A, p1, e..g., p2., La"
        cout << "9" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("...U.S.A p1 e..g. p2. La", lt1);
        // La lista de tokens a devolver debería contener: "...U.S.A, p1, e..g., p2., La"
        cout << "10" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.Tokenizar("a&U.S.A p1 e.g. p2. La", lt1);
        // La lista de tokens a devolver debería contener: "a&U.S.A, p1, e.g., p2., La"
        cout << "11" << endl;
        imprimirListaSTL(lt1);
        cout << endl;

        a.DelimitadoresPalabra("&");
        a.Tokenizar("a&U.S.A p1 e.g. p2. La", lt1);
        // La lista de tokens a devolver debería contener (porque & es delimitador,
        // pero “U.S.A” no lo detecta como acrónimo ya que el punto no es delimitador):
        // "a, U.S.A, p1, e.g., p2., La"
        cout << "12" << endl;
        imprimirListaSTL(lt1);
        cout << endl;
        

}