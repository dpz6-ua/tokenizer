#include "../include/tokenizador.h"
#include <unistd.h>
#include <dirent.h>

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos){
    // Inicializa delimiters a delimitadoresPalabra filtrando que no se introduzcan delimitadores repetidos 
    // (de izquierda a derecha, en cuyo caso se eliminarian los que hayan sido repetidos por la derecha); 
    // casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos
    for (auto c : delimitadoresPalabra){
        if (delimitadorSet.insert(c).second) { // Si es un nuevo caracter, lo añadimos a delimiters
            delimiters += c;
        }
    }

    if (delimitadorSet.find(' ') == delimitadorSet.end()) {
        if (kcasosEspeciales) {
            delimiters += ' ';
            delimitadorSet.insert(' ');
        }
        espacio = false;
    } else {
        espacio = true;
    }

    casosEspeciales = kcasosEspeciales;
    pasarAminuscSinAcentos = minuscSinAcentos;
}

Tokenizador::Tokenizador(const Tokenizador& t){
    // Constructor de copia
    this->delimiters = t.delimiters;
    this->delimitadorSet = t.delimitadorSet;
    this->casosEspeciales = t.casosEspeciales;
    this->pasarAminuscSinAcentos = t.pasarAminuscSinAcentos;
    this->espacio = t.espacio;
}

Tokenizador::Tokenizador(){	
    // Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t@"; casosEspeciales a true; pasarAminuscSinAcentos a false
    delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t@";
    casosEspeciales = true;
    pasarAminuscSinAcentos = false;
    espacio = true;

    for (char c : delimiters) {
        delimitadorSet.insert(c);
    }
}

Tokenizador::~Tokenizador (){	
    // Pone delimiters=""
    delimiters = "";
    delimitadorSet.clear();
}

Tokenizador &Tokenizador::operator=(const Tokenizador& t){
    if (this != &t){
        this->delimiters = t.delimiters;
        this->delimitadorSet = t.delimitadorSet;
        this->casosEspeciales = t.casosEspeciales;
        this->pasarAminuscSinAcentos = t.pasarAminuscSinAcentos;
        this->espacio = t.espacio;
    }
    return *this;
}

string quitarAcentosYMinusculas(const string& texto) {
    unordered_map<char, char> conversion = {
        {'Á', 'a'}, {'É', 'e'}, {'Í', 'i'}, {'Ó', 'o'}, {'Ú', 'u'}, {'Ü', 'u'},
        {'á', 'a'}, {'é', 'e'}, {'í', 'i'}, {'ó', 'o'}, {'ú', 'u'}, {'ü', 'u'},
        {'À', 'a'}, {'È', 'e'}, {'Ì', 'i'}, {'Ò', 'o'}, {'Ù', 'u'},
        {'à', 'a'}, {'è', 'e'}, {'ì', 'i'}, {'ò', 'o'}, {'ù', 'u'},
        {'Â', 'a'}, {'Ê', 'e'}, {'Î', 'i'}, {'Ô', 'o'}, {'Û', 'u'},
        {'â', 'a'}, {'ê', 'e'}, {'î', 'i'}, {'ô', 'o'}, {'û', 'u'},
        {'Ã', 'a'}, {'Õ', 'o'}, {'ã', 'a'}, {'õ', 'o'}, {'Ñ', 'ñ'}
    };

    string resultado;
    for (char c : texto) {
        if (conversion.count(c)) {
            resultado += conversion[c]; // Sustituir acentuado por versión sin acento
        } else {
            resultado += tolower(c); // Convertir cualquier otro carácter a minúscula
        }
    }
    return resultado;
}

//////////////////////////////////////////////////////////
/////////////////// URL
//////////////////////////////////////////////////////////

bool esURL(const string& str, size_t i) {
    return (str.substr(i, 5) == "http:"  && i + 6 < str.size()) ||
           (str.substr(i, 6) == "https:" && i + 7 < str.size()) ||
           (str.substr(i, 4) == "ftp:"   && i + 5 < str.size());
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

    string strstr = pasarAminuscSinAcentos ? quitarAcentosYMinusculas(str) : str;

    tokens.clear();
    size_t i = 0;
    string tok = "";
    bool guion = delimitadorSet.count('-');
    bool punto = delimitadorSet.count('.');
    bool coma = delimitadorSet.count(',');
    bool arroba = delimitadorSet.count('@');
    bool puedeSerDecimal = false;

    while (i < strstr.size()){
        // Saltar delimitadores iniciales a menos que pueda ser decimal
        while (i < strstr.size() && delimitadorSet.count(strstr[i])) {
            if ((strstr[i] == '.' || strstr[i] == ',') && (i+1 < strstr.size() && isdigit(strstr[i+1]))){
                puedeSerDecimal = true;
                break;
            }
            i++;
        }

        // Salir del bucle cuando se llegue al final del string
        if (i == strstr.size()) 
            break;

        size_t start_token = i;
        if (casosEspeciales){
            if (!puedeSerDecimal && esURL(strstr, i)){
                string url = extraerURL(strstr, i, delimiters);
                tokens.push_back(url);
                continue;
            }
            if (punto && coma && esNumeroDecimal(strstr, i, tok, delimiters)){
                tokens.push_back(tok);
                puedeSerDecimal = false;
                tok = "";
                continue;
            }
            if (!puedeSerDecimal && arroba && esEmail(strstr, i, tok, delimiters)){
                tokens.push_back(tok);
                tok = "";
                continue;
            }
            if (!puedeSerDecimal && punto && esAcronimo(strstr, i, tok, delimiters)){
                tokens.push_back(tok);
                tok = "";
                continue;
            }
            if (!puedeSerDecimal && guion && esMultipalabra(strstr, i, delimiters, tok)){
                tokens.push_back(tok);
                tok = "";
                continue;
            }
            
        }
        
        //No se ha detectado el punto ni la coma como número decimal
        if (puedeSerDecimal){
            puedeSerDecimal = false;
            i++;
        }
        else{
            while (i < strstr.size() && !delimitadorSet.count(strstr[i])) {
                i++;
            }
            tokens.push_back(strstr.substr(start_token, i - start_token));
        }
    }
}

bool Tokenizador::Tokenizar(const string& i, const string& f) const {
    ifstream entrada(i);
    if (!entrada) {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }

    list<string> tokens;
    list<string> tokens2;
    string cadena;

    while (getline(entrada, cadena)) {
        if (!cadena.empty()) { 
            Tokenizar(cadena, tokens);
            tokens2.splice(tokens2.end(), tokens);
        }
    }
    entrada.close();

    if (!tokens.empty()) {
        ofstream salida(f);
        for (const auto& token : tokens2) {
            salida << token << '\n';
        }
    }

    return true;
}

bool Tokenizador::Tokenizar(const string& i) const {
    return Tokenizar(i, i + ".tk");
}

bool Tokenizador::TokenizarListaFicheros(const string& i) const {
    ifstream entrada(i);
    if (!entrada) {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }

    string cadena;
    bool resultado = true;

    while (getline(entrada, cadena)) {
        if (!cadena.empty()) {
            string f = cadena + ".tk";
            if (!Tokenizar(cadena, f)) {
                cerr << "ERROR: No se ha podido tokenizar el archivo: " << cadena << endl;
                resultado = false;
            }
        }
    }

    return resultado;
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
    delimitadorSet.clear();
    espacio = false;

    for (auto c : nuevoDelimiters){
        if (delimitadorSet.insert(c).second) {  // Solo inserta si no está repetido
            delimiters += c;
            if (c == ' ') {
                espacio = true;
            }
        }
    }

    if (!espacio && casosEspeciales) {
        delimiters += ' ';
        delimitadorSet.insert(' ');
    }
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){ 
    // Añade al final de "delimiters" los nuevos delimitadores que aparezcan en "nuevoDelimiters" 
    // (no se almacenaran caracteres repetidos)
    for (char c : nuevoDelimiters) {
        if (delimitadorSet.insert(c).second) {  // Solo inserta si no está repetido
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
    if (!this->espacio && !nuevoCasosEspeciales) {
        auto it = delimitadorSet.find(' ');
        if (it != delimitadorSet.end()) {
            delimitadorSet.erase(it);  // Eliminar el espacio del set
            delimiters.erase(std::remove(delimiters.begin(), delimiters.end(), ' '), delimiters.end());  // Eliminarlo del string
        }
    }
}

bool Tokenizador::CasosEspeciales(){
    // Devuelve el contenido de la variable privada "casosEspeciales"
    return this->casosEspeciales;
}

void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos){
    // Cambia la variable privada "pasarAminuscSinAcentos". Atencion al formato de codificacion del corpus
    // (comando "file" de Linux). Para la correccion de la practica se utilizara el formato actual (ISO-8859). 
    this->pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
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