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
    Tokenizador a("-#", true, false);
    list<string> tokens;
    a.Tokenizar("MS-DOS p1 p2 UN-DOS-TRES", tokens);
    // La lista de tokens a devolver debería contener: "MS-DOS, p1, p2, UN-DOS-TRES"
    cout << "1" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 -MS-DOS p1 p2", tokens);
    // La lista de tokens a devolver debería contener (multipalabra MS-DOS y quitaría el primer – 
    // porque es delimitador y no está por el medio de la palabra): "pal1, MS-DOS, p1, p2"
    cout << "2" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 -MS-DOS- p1 p2", tokens);
    // La lista de tokens a devolver debería contener (multipalabra MS-DOS y
    // quitaría el primer y último – porque es delimitador y no está por el medio de
    // la palabra): "pal1, MS-DOS, p1, p2"
    cout << "3" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 MS-DOS--TRES p1 p2", tokens);
    // La lista de tokens a devolver debería contener (multipalabra MS-DOS y
    // separaría por -– porque es delimitador y no está por el medio de la palabra):
    // "pal1, MS-DOS, TRES, p1, p2"
    cout << "4" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 MS-DOS-TRES--- p1 p2", tokens);
    // La lista de tokens a devolver debería contener (multipalabra MS-DOS-TRES y
    // separaría por -–- porque es delimitador y no está por el medio de la palabra):
    // "pal1, MS-DOS-TRES, p1, p2"
    cout << "5" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 MS-DOS#p3 p1 p2", tokens);
    // La lista de tokens a devolver debería contener: "pal1, MS-DOS, p3, p1, p2"
    cout << "6" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1#MS-DOS#p3 p1 p2", tokens);
    // La lista de tokens a devolver debería contener: "pal1, MS-DOS, p3, p1, p2"
    cout << "7" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1#MS- DOS#p3 p1 p2", tokens);
    // La lista de tokens a devolver debería contener: "pal1, MS, DOS, p3, p1, p2"
    cout << "8" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1#MS -DOS#p3 p1 p2", tokens);
    // La lista de tokens a devolver debería contener: "pal1, MS, DOS, p3, p1, p2"
    cout << "9" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("p1 p2 pal1 MS-DOS-TRES---", tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    //multipalabra, sino como token normal ya que el – no es delimitador): "pal1,
    // MS-DOS-TRES---, p1, p2"
    cout << "17" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.DelimitadoresPalabra("/ ");
    a.Tokenizar("MS-DOS p1 p2", tokens);
    // La lista de tokens a devolver debería contener (MS-DOS no se detectaría
    // como multipalabra, sino como token normal ya que el – no es delimitador): "MS-
    // DOS, p1, p2"
    cout << "10" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 -MS-DOS p1 p2", tokens);
    cout << "11" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 MS-DOS#p3 p1 p2", tokens);
    // La lista de tokens a devolver debería contener (no separa #p3 porque # no
    // está entre los delimitadores): "pal1, MS-DOS#p3, p1, p2"
    cout << "12" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1#MS-DOS#p3 p1 p2", tokens);
    // La lista de tokens a devolver debería contener: "pal1#MS-DOS#p3, p1, p2"
    cout << "13" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 -MS-DOS- p1 p2", tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    // multipalabra, sino como token normal ya que el – no es delimitador): "pal1,
    // -MS-DOS-, p1, p2"
    cout << "14" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 MS-DOS--TRES p1 p2", tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    // multipalabra, sino como token normal ya que el – no es delimitador): "pal1,
    // MS-DOS--TRES, p1, p2"
    cout << "15" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

    a.Tokenizar("pal1 MS-DOS-TRES--- p1 p2", tokens);
    // La lista de tokens a devolver debería contener (no se detectaría como
    //multipalabra, sino como token normal ya que el – no es delimitador): "pal1,
    // MS-DOS-TRES---, p1, p2"
    cout << "16" << endl;
    imprimirListaSTL(tokens);
    cout << endl;

}