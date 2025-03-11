#pragma once
#include "lexico.h"
#include "arbol.h"
#include <iostream>
#include <string>
#include <cctype> //isanum isalpha isdigit
#include <stdexcept> // manejo de errores
#include <algorithm> 
#include <vector>

class Syntax{
    private:

    // Método para obtener el final de la linea plantado por un punto y coma
    int getFinalNumber(int initialNumber, vector<Datos> &datos);

    // Analizador de for
    int forAnalizer(vector<Datos>& datos, int &i);

    // Analizador de if
    int ifAnalizer(vector<Datos>& datos, int &i);
    
    // Método para sintaxis de Begin-End
    void getSyntaxBegin(vector<Datos> &datos, vector<string> &symbols, int &initialNumber, bool isPrincipal, bool isProcedure);

    // Método para crear los árboles
    void recursiveTree(Node<string>* actual, Tree<string>&padre, vector<string> &symbols, vector<Datos> &datos, int initialNumber, int finalNumber);

    // Overload de recursive Tree con menos argumentos
    void recursiveTree(Tree<string>&padre, vector<Datos> &datos, int initialNumber, int finalNumber);

    //Sintaxis de Var en Pascal
    void getSyntaxVar(vector<Datos> &datos, vector<string> &symbols, int &initial);

    //sintaxis de Asignaciones x:=1;
    void getSyntaxAsignation(vector<Datos> &datos, vector<string> &symbols, int &initial);
    
    // sintaxis de la funcion procedure
    void getSyntaxprocedure(vector<Datos> &datos, vector<string> &symbols, int &initial);
    
    // Analizador de writeln
    void getSyntaxwriteln(vector<Datos> &datos, vector<string> &symbols, int &initial);

    // Analizador del readln
    void getSyntaxreadln(vector<Datos> &datos, vector<string> &symbols, int &initial);

    string phraseAnalizer(Node<string> *nodo);

    // Función auxiliar para verificar palabras reservadas o valores inválidos
    bool isReservedOrInvalid(const string &value);


    public:
    Syntax();

    //Método para la obteción principal del Syntax
    void getSyntax(vector<Datos> &datos);
    
};

