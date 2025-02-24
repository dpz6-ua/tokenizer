#include "../include/tokenizador.h"
#include <unistd.h>
#include <dirent.h>

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos){
    // Inicializa delimiters a delimitadoresPalabra filtrando que no se introduzcan delimitadores repetidos 
    // (de izquierda a derecha, en cuyo caso se eliminarian los que hayan sido repetidos por la derecha); 
    // casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos
    for (auto c : delimitadoresPalabra){
        if (delimiters.find(c) == string::npos){
            delimiters += c;
        }
    }

    if (kcasosEspeciales && delimiters.find(" ") == string::npos){
        delimiters += " ";
    }
    casosEspeciales = kcasosEspeciales;
    pasarAminuscSinAcentos = minuscSinAcentos;
}

Tokenizador::Tokenizador(const Tokenizador& t){
    // Constructor de copia
    this->delimiters = t.delimiters;
    this->casosEspeciales = t.casosEspeciales;
    this->pasarAminuscSinAcentos = t.pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador(){	
    // Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t@"; casosEspeciales a true; pasarAminuscSinAcentos a false
    delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t@";
    casosEspeciales = true;
    pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador (){	
    // Pone delimiters=""
    delimiters = "";
}

Tokenizador &Tokenizador::operator=(const Tokenizador& t){
    if (this != &t){
        this->delimiters = t.delimiters;
        this->casosEspeciales = t.casosEspeciales;
        this->pasarAminuscSinAcentos = t.pasarAminuscSinAcentos;
    }
    return *this;
}

//////////////////////////////////////////////////////////
/////////////////// URL
//////////////////////////////////////////////////////////

bool esURL(const string str, size_t i){
    // Devuelve true si la cadena str es una URL
    return (str.substr(i, 5) == "http:" || str.substr(i, 6) == "https:" || str.substr(i, 4) == "ftp:");
}

string extraerURL(const string& str, size_t& pos, string delimiters) {
    size_t start = pos;
    string especiales = "_:/.?&-=#@";
    string delimitadores = delimiters;
    for (size_t i = 0; i < delimitadores.size(); i++){
        if (especiales.find(delimitadores[i]) != string::npos){
            delimitadores.erase(i, 1);
            i--;
        }
    }

    while (pos < str.size() && delimitadores.find(str[pos]) == string::npos) {
        pos++;
    }

    return str.substr(start, pos - start);
}

//////////////////////////////////////////////////////////
////////////// Multipalabra
//////////////////////////////////////////////////////////

bool esMultipalabra(const string &str, size_t &i, string delimiters, string &multipalabra) {
    size_t original_i = i;

    while (i < str.size() && (delimiters.find(str[i]) == string::npos || (str[i] == '-' && i+1 < str.size() && str[i + 1] != '-'))) {
        if (!isalnum(str[i]) && str[i] != '-') {
            i = original_i;
            multipalabra = "";
            return false;
        }
        multipalabra += str[i];
        i++;
    }

    if (multipalabra[multipalabra.size() - 1] == '-') 
        multipalabra.pop_back();

    return true;
}

//////////////////////////////////////////////////////////
////////////// Acrónimo
//////////////////////////////////////////////////////////

bool esAcronimo(const string &str, size_t &i, string &acronimo, string delimiters) {
    size_t original_i = i;
    //cout << "Entra en acronimo" << endl;
    while (i < str.size() && (delimiters.find(str[i]) == string::npos || (str[i] == '.'))) {
        //cout << "Acronimo: " << acronimo << endl;
        if (i+1 < str.size() && str[i] == '.' && str[i+1] == '.'){
            if (acronimo.find('.') != string::npos){
                return true;
            }
            i = original_i;
            acronimo = "";
            return false;
        }
        acronimo += str[i];
        i++;
    }

    if (acronimo[acronimo.size() - 1] == '.') 
        acronimo.pop_back();
    if (acronimo.find('.') == string::npos){
        i = original_i;
        acronimo = "";
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////
////////////// Números decimales
//////////////////////////////////////////////////////////

bool esNumeroDecimal(const string &str, size_t &i, string &decimal, string delimiters){
    size_t original_i = i;

    if (str[i] == '$' || str[i] == '%'){
        return false;
    }

    // Si el primer elemento es un ./, se añade el 0.
    if ((str[i] == '.' || str[i] == ',')){
        decimal += "0";
        decimal += str[i];
        i++;
    }

    while(i < str.size() && (delimiters.find(str[i]) == string::npos || (str[i] == '.' || str[i] == ',' || str[i] == '$' || str[i] == '%'))){
        if (!isdigit(str[i]) && str[i] != '.' && str[i] != ',' && str[i] != '$' && str[i] != '%'){
            i = original_i;
            decimal = "";
            return false;
        }
        if (str[i] == '.' || str[i] == ','){
            if (str[i-1] == '.' || str[i-1] == ','){
                break;
            }
        }
        
        if (str[i] == '$' || str[i] == '%'){
            if (i+1 < str.size() && str[i+1] == ' ' && decimal.find('.') == string::npos && decimal.find(',') == string::npos){
                decimal += str[i];
                i++;
                break;
            }

            if (i+1 < str.size() && str[i+1] != ' '){
                i = original_i;
                decimal = "";
                return false;
            }
            else{
                break;
            }
        }

        decimal += str[i];
        i++;
    }

    if (decimal[decimal.size() - 1] == '.' || decimal[decimal.size() - 1] == ',') 
        decimal.pop_back();

    return true;
}

//////////////////////////////////////////////////////////
////////////// EMAIL
//////////////////////////////////////////////////////////

bool esEmail(const string &str, size_t &i, string &email, string delimiters){
    size_t original = i;
    string especiales = "@.-_";
    string primarroba = "";
    bool hayarroba = false;
    int arrobacount = 0;

    while (i < str.size() && (delimiters.find(str[i]) == string::npos || (especiales.find(str[i]) != string::npos))) {
        if ((str[i] == '.' || str[i] == '-' || str[i] == '_') && !hayarroba) {
            i = original;
            email = "";
            return false;
        }

        if (str[i] == '.' || str[i] == '-' || str[i] == '_'){
            if (i+1 < str.size() && (str[i+1] == '.' || str[i+1] == '-' || str[i+1] == '_')){
                return true;
            }
        }

        if (str[i] == '@'){
            if (i+1 < str.size() && (str[i+1] == '.' || str[i+1] == '-' || str[i+1] == '_')){
                i++;
                return true;
            }
            hayarroba = true;
            arrobacount++;
            if (arrobacount > 1){
                i = original + primarroba.size();
                email = primarroba;
                return true;
            }
        }

        if (!hayarroba){
            primarroba += str[i];
        }
        email += str[i];
        i++;
    }

    if (email.find('@') == string::npos){
        i = original;
        email = "";
        return false;
    }

    if (email[email.size() - 1] == '.' || email[email.size() - 1] == '-' || email[email.size() - 1] == '_') 
        email.pop_back();

    if (email[email.size() - 1] == '@')
        email.pop_back();
        
    return true;
}

void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const{
    // Tokeniza str devolviendo el resultado en tokens. La lista tokens se
    // vaciará antes de almacenar el resultado de la tokenización.

    /*
    tokens.clear();
    string::size_type lastPos = str.find_first_not_of(delimiters,0);
    string::size_type pos = str.find_first_of(delimiters,lastPos);

    while(string::npos != pos || string::npos != lastPos){
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    */
    tokens.clear();
    size_t i = 0;
    string multipalabra = "";
    string acronimo = "";
    string decimal = "";
    string email = "";
    bool guion = delimiters.find('-') != string::npos;
    bool punto = delimiters.find('.') != string::npos;
    bool coma = delimiters.find(',') != string::npos;
    bool arroba = delimiters.find('@') != string::npos;
    bool puedeSerDecimal = false;

    while (i < str.size()){
        // Saltar delimitadores iniciales a menos que pueda ser decimal
        while (i < str.size() && delimiters.find(str[i]) != string::npos) {
            if ((str[i] == '.' || str[i] == ',') && (i+1 < str.size() && isdigit(str[i+1]))){
                puedeSerDecimal = true;
                break;
            }
            i++;
        }
        if (i == str.size()) 
            break;

        size_t start_token = i;
        if (casosEspeciales){
            if (!puedeSerDecimal && esURL(str, i)){
                string url = extraerURL(str, i, delimiters);
                tokens.push_back(url);
                continue;
            }
            if (punto && coma && esNumeroDecimal(str, i, decimal, delimiters)){
                cout << "Decimal: " << decimal << endl;
                tokens.push_back(decimal);
                puedeSerDecimal = false;
                decimal = "";
                continue;
            }
            if (!puedeSerDecimal && arroba && esEmail(str, i, email, delimiters)){
                cout << "Email: " << email << endl;
                tokens.push_back(email);
                email = "";
                continue;
            }
            if (!puedeSerDecimal && punto && esAcronimo(str, i, acronimo, delimiters)){
                cout << "Acronimo: " << acronimo << endl;
                tokens.push_back(acronimo);
                acronimo = "";
                continue;
            }
            if (!puedeSerDecimal && guion && esMultipalabra(str, i, delimiters, multipalabra)){
                cout << "Multipalabra: " << multipalabra << endl;
                tokens.push_back(multipalabra);
                multipalabra = "";
                continue;
            }
            
        }
        
        //No se ha detectado el punto ni la coma como número decimal
        if (puedeSerDecimal){
            puedeSerDecimal = false;
            i++;
        }
        else{
            while (i < str.size() && delimiters.find(str[i]) == string::npos) {
                i++;
            }
            tokens.push_back(str.substr(start_token, i - start_token));
        }
    }
}

bool Tokenizador::Tokenizar(const string& i, const string& f) const{
    // Tokeniza el fichero i guardando la salida en el fichero f (una
    // palabra en cada linea del fichero). Devolvera true si se realiza la tokenizacion
    // de forma correcta; false en caso contrario enviando a cerr el mensaje correspondiente
    // (p.ej. que no exista el archivo i)
    ifstream entrada;
    ofstream salida;
    string cadena;
    list<string> tokens;

    entrada.open(i.c_str());
    if(!entrada) {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }
    else {
        while(!entrada.eof()){
            cadena="";
            getline(entrada, cadena);
            if(cadena.length() != 0) { 
                Tokenizar(cadena, tokens);
            }
        }
        entrada.close();
        salida.open(f.c_str());
        list<string>::iterator itS;
        for(itS= tokens.begin(); itS!= tokens.end(); itS++)
        {
            salida << (*itS) << endl;
        }
        salida.close();
        return true;
    }

}

bool Tokenizador::Tokenizar (const string & i) const{
    // Tokeniza el fichero i guardando la salida en un fichero de nombre i
    // añadiendole extension .tk (sin eliminar previamente la extension de i por ejemplo,
    // del archivo pp.txt se generaria el resultado en pp.txt.tk), y que contendra una palabra
    // en cada linea del fichero. Devolvera true si se realiza la tokenizacion de forma correcta;
    // false en caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista el archivo i)
    string f = i + ".tk";
    return Tokenizar(i, f);

}

bool Tokenizador::TokenizarListaFicheros(const string& i) const{
    // Tokeniza el fichero i que contiene un nombre de fichero por linea guardando la salida en ficheros 
    // (uno por cada linea de i) cuyo nombre sera el leido en i añadiendole extension .tk, y que contendra 
    // una palabra en cada linea del fichero leido en i. Devolvera true si se realiza la tokenizacion de forma 
    // correcta de todos los archivos que contiene i; devolvera false en caso contrario enviando a cerr el mensaje 
    // correspondiente (p.ej. que no exista el archivo i, o que se trate de un directorio, enviando a "cerr" los archivos 
    //de i que no existan o que sean directorios; luego no se ha de interrumpir la ejecucion si hay algun archivo en i que no exista)
    ifstream entrada;
    string cadena;
    list<string> tokens;
    string f;

    entrada.open(i.c_str());
    if(!entrada) {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }
    else {
        while(!entrada.eof()){
            cadena="";
            getline(entrada, cadena);
            if(cadena.length() != 0) { 
                f = cadena + ".tk";
                if (!Tokenizar(cadena, f)){
                    cerr << "ERROR: No se ha podido tokenizar el archivo: " << cadena << endl;
                }
            }
        }
        entrada.close();
        return true;
    }
}

bool Tokenizador::TokenizarDirectorio(const string& i) const{
    // Tokeniza todos los archivos que contenga el directorio i, incluyendo los de los subdirectorios, 
    // guardando la salida en ficheros cuyo nombre sera el de entrada añadiendole extension .tk, y que 
    // contendra una palabra en cada linea del fichero. Devolvera true si se realiza la tokenizacion de forma 
    // correcta de todos los archivos; devolvera false en caso contrario enviando a cerr el mensaje correspondiente 
    // (p.ej. que no exista el directorio i, o los ficheros que no se hayan podido tokenizar)
    DIR *dir;
    struct dirent *ent;
    string f;
    list<string> tokens;

    if ((dir = opendir(i.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG){
                f = i + "/" + ent->d_name;
                f += ".tk";
                if (!Tokenizar(f, f)){
                    cerr << "ERROR: No se ha podido tokenizar el archivo: " << f << endl;
                }
            }
        }
        closedir(dir);
        return true;
    } else {
        cerr << "ERROR: No existe el directorio: " << i << endl;
        return false;
    }
}

void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters){
    // Inicializa delimiters a nuevoDelimiters, filtrando que no se introduzcan delimitadores repetidos 
    // (de izquierda a derecha, en cuyo caso se eliminarian los que hayan sido repetidos por la derecha)
    delimiters = "";
    bool found_espacio = false;
    for (auto c : nuevoDelimiters){
        if (delimiters.find(c) == string::npos){
            delimiters += c;
            if (c == ' '){
                found_espacio = true;
            }
        }
    }

    if (!found_espacio && casosEspeciales){
        delimiters += " ";
    }

}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){ 
    // Añade al final de "delimiters" los nuevos delimitadores que aparezcan en "nuevoDelimiters" 
    // (no se almacenaran caracteres repetidos)
    for (auto c : nuevoDelimiters){
        if (delimiters.find(c) == string::npos){
            delimiters += c;
        }
    }
}

string Tokenizador::DelimitadoresPalabra() const{
    // Devuelve "delimiters"
    return this->delimiters; 
}

void Tokenizador::CasosEspeciales(const bool& nuevoCasosEspeciales){
    // Cambia la variable privada "casosEspeciales"
    this->casosEspeciales = nuevoCasosEspeciales; 
}

bool Tokenizador::CasosEspeciales(){
    // Devuelve el contenido de la variable privada "casosEspeciales"
    return this->casosEspeciales;
}

void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos){
    // Cambia la variable privada "pasarAminuscSinAcentos". Atencion al formato de codificacion del corpus
    // (comando "file" de Linux). Para la correccion de la practica se utilizara el formato actual (ISO-8859). 

}

bool Tokenizador::PasarAminuscSinAcentos(){
    // Devuelve el contenido de la variable privada "pasarAminuscSinAcentos"
    return this->pasarAminuscSinAcentos;
}

ostream &operator<<(ostream& os, const Tokenizador& t){
    // cout << "DELIMITADORES: " << delimiters << " TRATA CASOS ESPECIALES: " << casosEspeciales << " PASAR A MINUSCULAS 
    // Y SIN ACENTOS: " << pasarAminuscSinAcentos; Aunque se modifique el almacenamiento de los delimitadores por temas 
    // de eficiencia, el campo delimiters se imprimira con el string leido en el tokenizador 
    // (tras las modificaciones y eliminacion de los caracteres repetidos correspondientes)
    os << "DELIMITADORES: " << t.delimiters 
       << " TRATA CASOS ESPECIALES: " << t.casosEspeciales 
       << " PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;
    return os;
}