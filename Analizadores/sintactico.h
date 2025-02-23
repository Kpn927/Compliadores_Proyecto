#include "lexico.h"
#include "arbol.h"
#include <iostream>
#include <string>
#include <cctype> //isanum isalpha isdigit
#include <stdexcept> // manejo de errores
#include <algorithm>

class Syntax{
    public:
    Syntax(){};
        void getsyntax(vector<string> &datos){
            
            // WORKS (⊙_⊙;)
            if (!programOpener(datos)) {
                cout << "ERROR: El programa debe iniciar con la palabra 'program'." << endl;
                return;
            }
            
            int iterator = 0;
            vector<string> temp_vector;
            
            while(iterator < datos.size()){
                
                if (!lineaLeida(datos, temp_vector, iterator)) {
                    return;
                }

                temp_vector.clear();
            }

            /*int countId = std::count(identifiers.begin(), identifiers.end(), datos);
            if (countId > 0) {
                cout << " condicional Identifier " << endl;
            }*/

        };

        bool lineaLeida(const vector<string>& datos, vector<string>& temp, int& i) {
            while (i < datos.size() && datos[i] != ";") {
                temp.push_back(datos[i]);
                i++;
            }
            if (i < datos.size() && datos[i] == ";") {
                temp.push_back(datos[i]);
                i++;
                return true; 
            } else {
                cout << "ERROR: Falta de ';' en la linea." << endl;
                return false;
            }
        }

        bool programOpener(const vector<string>& datos) {
            if (datos.empty() || datos[0] != "program") {
                return false; // No comienza con "program"
            }
            return true;
        }

        void getsyntaxvar(vector<string> &datos)
        {
            /*try{
                bool flag = false;
                int i = 0;

                if(datos == ":")
                {
                    cout << " condicional : " << endl;
                    // agregar nodo del medio
                }

                int count = std::count(reserved.begin(), reserved.end(), datos);
                if (count > 0) {
                    cout << " condicional reservada " << endl;
                }

                int countType = std::count(dataType.begin(), dataType.end(), datos);
                if (countType > 0) {
                    cout << " condicional dataType " << endl;
                }

                int countId = std::count(identifiers.begin(), identifiers.end(), datos);
                if (countId > 0) {
                    cout << " condicional Identifier " << endl;
                }

            }
            catch(const runtime_error& e)
            {
                cout << " ERROR: ARBOL DE VARIABLES";
            }
        }*/
    }
};

