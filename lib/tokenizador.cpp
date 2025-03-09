#include "../include/tokenizador.h"
#include <unistd.h>
#include <dirent.h>

char conversion[256] = {0};

void inicializarConversiones() {
    conversion[0xC1] = 'a'; // Á -> a
    conversion[0xC9] = 'e'; // É -> e
    conversion[0xCD] = 'i'; // Í -> i
    conversion[0xD3] = 'o'; // Ó -> o
    conversion[0xDA] = 'u'; // Ú -> u
    conversion[0xDC] = 'u'; // Ü -> u
    
    conversion[0xE1] = 'a'; // á -> a
    conversion[0xE9] = 'e'; // é -> e
    conversion[0xED] = 'i'; // í -> i
    conversion[0xF3] = 'o'; // ó -> o
    conversion[0xFA] = 'u'; // ú -> u
    conversion[0xFC] = 'u'; // ü -> u
    
    conversion[0xC0] = 'a'; // À -> a
    conversion[0xC8] = 'e'; // È -> e
    conversion[0xCC] = 'i'; // Ì -> i
    conversion[0xD2] = 'o'; // Ò -> o
    conversion[0xD9] = 'u'; // Ù -> u
    
    conversion[0xE0] = 'a'; // à -> a
    conversion[0xE8] = 'e'; // è -> e
    conversion[0xEC] = 'i'; // ì -> i
    conversion[0xF2] = 'o'; // ò -> o
    conversion[0xF9] = 'u'; // ù -> u
    
    conversion[0xC2] = 'a'; // Â -> a
    conversion[0xCA] = 'e'; // Ê -> e
    conversion[0xCE] = 'i'; // Î -> i
    conversion[0xD4] = 'o'; // Ô -> o
    conversion[0xDB] = 'u'; // Û -> u
    
    conversion[0xE2] = 'a'; // â -> a
    conversion[0xEA] = 'e'; // ê -> e
    conversion[0xEE] = 'i'; // î -> i
    conversion[0xF4] = 'o'; // ô -> o
    conversion[0xFB] = 'u'; // û -> u
    
    conversion[0xC3] = 'a'; // Ã -> a
    conversion[0xD5] = 'o'; // Õ -> o
    conversion[0xE3] = 'a'; // ã -> a
    conversion[0xF5] = 'o'; // õ -> o
    
    conversion[0xD1] = 'ñ'; // Ñ -> ñ
}

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos){
    // Inicializa delimiters a delimitadoresPalabra filtrando que no se introduzcan delimitadores repetidos 
    // (de izquierda a derecha, en cuyo caso se eliminarian los que hayan sido repetidos por la derecha); 
    // casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos
    inicializarConversiones();
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
    inicializarConversiones();
    // Constructor de copia
    this->delimiters = t.delimiters;
    this->delimitadorSet = t.delimitadorSet;
    this->casosEspeciales = t.casosEspeciales;
    this->pasarAminuscSinAcentos = t.pasarAminuscSinAcentos;
    this->espacio = t.espacio;
}

Tokenizador::Tokenizador(){	
    inicializarConversiones();
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
    string resultado;
    resultado.reserve(texto.size());  // Reservar memoria para evitar reubicaciones

    for (unsigned char c : texto) {  // Usamos unsigned char para manejar correctamente los caracteres
        if (conversion[c]) {
            resultado += conversion[c];
        } else {
            resultado += tolower(c);
        }
    }

    return resultado;
}

//////////////////////////////////////////////////////////
/////////////////// URL
//////////////////////////////////////////////////////////

bool esURL(const string& str, size_t i) {
    if (i + 3 < str.size() && str[i] == 'f' && str[i + 1] == 't' && str[i + 2] == 'p') return true;
    if (i + 4 < str.size() && str[i] == 'h' && str[i + 1] == 't' && str[i + 2] == 't' && str[i + 3] == 'p') {
        if (i + 5 < str.size() && str[i + 4] == 's') return true; // "https"
        return true; // "http"
    }
    return false;
}

string extraerURL(const string& str, size_t& pos, string token, string delimiters) {
    if (pos == string::npos) {
        return token;
    }
    size_t start = pos;
    string especiales = "_:/.?&-=#@";
    for (size_t i = 0; i < delimiters.size(); i++){
        if (especiales.find(delimiters[i]) != string::npos){
            delimiters.erase(i, 1);
            i--;
        }
    }

    while (pos < str.size() && delimiters.find(str[pos]) == string::npos) {
        pos++;
    }

    return (pos == start + 1) ? token : token + str.substr(start, pos - start);
}

//////////////////////////////////////////////////////////
////////////// Acrónimo
//////////////////////////////////////////////////////////

bool esAcronimo(const string &str, size_t &i, string &acronimo, string delimiters) {
    size_t original_i = i;

    //cout << "Entra en acronimo" << endl;
    while (i < str.size() && (delimiters.find(str[i]) == string::npos || (str[i] == '.'))) {
        //cout << "Acronimo: " << acronimo << endl;
        if (i+1 < str.size() && str[i] == '.' && delimiters.find(str[i+1]) != string::npos) {
            i++;
            return true;
        }
        acronimo += str[i];
        i++;
    }

    if (acronimo[acronimo.size() - 1] == '.') 
        acronimo.pop_back();

    return true;
}

//////////////////////////////////////////////////////////
////////////// Multipalabra
//////////////////////////////////////////////////////////

bool esMultipalabra(const string &str, size_t &i, string delimiters, string &multipalabra) {
    size_t original_i = i;

    //cout << "Entra en acronimo" << endl;
    while (i < str.size() && (delimiters.find(str[i]) == string::npos || (str[i] == '-'))) {
        //cout << "Acronimo: " << acronimo << endl;
        if (i+1 < str.size() && str[i] == '-' && delimiters.find(str[i+1]) != string::npos) {
            i++;
            return true;
        }
        multipalabra += str[i];
        i++;
    }

    if (multipalabra[multipalabra.size() - 1] == '-') 
        multipalabra.pop_back();

    return true;
}

//////////////////////////////////////////////////////////
////////////// EMAIL
//////////////////////////////////////////////////////////

bool esEmail(const string &str, size_t &i, string &email, string delimiters){
    size_t original = i;
    string especiales = "@.-_";
    string tok = email;

    if (i+1 < str.size() && (str[i+1] == '.' || str[i+1] == '-' || str[i+1] == '_')){
        i += 2; // saltarse arroba y elemento
        return true;
    }

    //añadir arroba
    email += str[i];
    i++;

    while (i < str.size() && (delimiters.find(str[i]) == string::npos || especiales.find(str[i]) != string::npos)){
        if (str[i] == '@'){
            i = original + 1; //saltarse la arroba
            email = tok;
            break;
        }
        if (str[i] == '.' || str[i] == '-' || str[i] == '_'){
            if (i+1 < str.size() && (str[i+1] == '.' || str[i+1] == '-' || str[i+1] == '_')){
                i++;
                break;
            }
        }
        email += str[i];
        i++;
    }

    if (email[email.size() - 1] == '.' || email[email.size() - 1] == '-' || email[email.size() - 1] == '_') 
        email.pop_back();

    if (email[email.size() - 1] == '@')
        email.pop_back();
        
    return true;
}

//////////////////////////////////////////////////////////
////////////// Números decimales
//////////////////////////////////////////////////////////

bool esNumeroDecimal(const string &str, size_t &i, string &decimal, size_t lastPos, string delimiters){
    size_t original_i = i;
    string deciOriginal = decimal;
    bool puntocoma = false;
    size_t auxPos = i;

    // Si los elementos acumulados no son numeros, se devuelve false.
    for (char c : decimal){
        if (!isdigit(c)){
            return false;
        }
    }
    // Si el primer elemento es un ./, se añade el 0.
    if (lastPos > 0 && (str[lastPos - 1] == '.' || str[lastPos - 1] == ',')) {
        decimal = "0" + std::string(1, str[lastPos - 1]) + decimal;
    }

    while(i < str.size() && (delimiters.find(str[i]) == string::npos || str[i] == '.' || str[i] == ',' || str[i] == '$' || str[i] == '%')){
        if (!isdigit(str[i]) && str[i] != '.' && str[i] != ',' && str[i] != '$' && str[i] != '%'){
            i = original_i;
            decimal = deciOriginal;
            return false;
        }

        if (str[i] == '.' || str[i] == ','){
            puntocoma = true;
            auxPos = i;
            if (i+1 < str.size() && delimiters.find(str[i+1]) != string::npos){
                i++;
                break;
            }
        }

        if (str[i] == '$' || str[i] == '%'){
            if (i+1 == str.size()){
                break;
            }
            if (i+1 < str.size() && str[i+1] == ' ' && !puntocoma){
                decimal += str[i];
                i++;
                break;
            }
            if (i+1 < str.size()){
                if (str[i+1] == ' ')
                    break;
                decimal = str.substr(original_i - 1, auxPos - (original_i - 1));
                i = auxPos;
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
////////////// Tokenizar
//////////////////////////////////////////////////////////

void Tokenizador::Tokenizar(const string& str, list<string>& tokens) const {
    tokens.clear();
    string strstr = pasarAminuscSinAcentos ? quitarAcentosYMinusculas(str) : str;
    bool anadido = false;

    string::size_type lastPos = strstr.find_first_not_of(delimiters, 0);
    string::size_type pos = strstr.find_first_of(delimiters, lastPos);

    while (lastPos != string::npos) {
        string tok = strstr.substr(lastPos, pos - lastPos);
        anadido = false;

        if (casosEspeciales) {

            if (esURL(strstr, lastPos)) {
                tokens.push_back(extraerURL(strstr, pos, tok, delimiters));
                anadido = true;
            }

            if (((strstr[pos] == '.' || strstr[pos] == ',')
                || (lastPos > 0 && (strstr[lastPos - 1] == '.' || strstr[lastPos - 1] == ',')))
                && !anadido && esNumeroDecimal(strstr, pos, tok, lastPos, delimiters)) {
                tokens.push_back(tok);
                anadido = true;
            }

            if (strstr[pos] == '@' && !anadido && esEmail(strstr, pos, tok, delimiters)) {
                tokens.push_back(tok);
                anadido = true;
            }

            if (strstr[pos] == '.' && !anadido && esAcronimo(strstr, pos, tok, delimiters)) {
                tokens.push_back(tok);
                anadido = true;
            }

            if (strstr[pos] == '-' && !anadido && esMultipalabra(strstr, pos, delimiters, tok)) {
                tokens.push_back(tok);
                anadido = true;
            }

            if (!anadido){
                tokens.push_back(tok);
            }

        } else {
            tokens.push_back(tok);
        }

        // Avanzar al siguiente token
        lastPos = strstr.find_first_not_of(delimiters, pos);
        pos = strstr.find_first_of(delimiters, lastPos);
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

    if (!tokens2.empty()) {
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
    os << "DELIMITADORES: " << t.delimiters 
       << " TRATA CASOS ESPECIALES: " << t.casosEspeciales 
       << " PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;
    return os;
}