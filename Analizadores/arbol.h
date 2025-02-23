#include <iostream>
using namespace std;

template <class T> class arbol;
// Clase para el nodo
template <class T>
class nodo {
private:
    T dato;
    typedef nodo<T>* nodePtr;
    nodePtr izquierda;
    nodePtr centro;
    nodePtr derecha;

public:
    //nodo(int valor) : dato(valor), izquierda(NULL), centro(NULL), derecha(NULL) {}
    nodo(T valor, nodePtr izq=NULL, nodePtr der=NULL, nodePtr cen=NULL){
        dato=valor;
        izquierda=izq;
        derecha=der;
        centro=cen;
    }
    friend class arbol<T>;
};

// Clase del árbol
template <class T>
class arbol {
private:
    typedef nodo<T>* nodePtr;
    
    nodo<T>* bucleInsertar(nodePtr nodoTemp, int ramaObjetivo, int ramaActual);

    bool isLeaf(nodePtr nodoTemp);

    void bucleBorrar(nodePtr nodoTemp);

    nodo<T>* bucleObtenerBorrar(T valor, nodePtr nodoTemp);
public:
    nodePtr raiz;
    arbol(){
        raiz=NULL;
    }

    void insertarOrdenado(T valor, int rama, nodo<T>* actual);

    void insertar(T valor);

    void orden(nodePtr nodoTemp, bool ordenEleccion);

    void borrar( T valor, bool eliminarHijos);

    nodo<T>* getRaiz();

    void gotoRight(nodo<T>* actual);
    void gotoCenter(nodo<T>* actual);
    void gotoLeft(nodo<T>* actual);
};

template <class T>
nodo<T>* arbol<T>::getRaiz(){
    return this->raiz;
}

template <class T>
void arbol<T>::gotoRight(nodo<T>* actual){
    return this->actual = this->actual->derecha;
}

template <class T>
void arbol<T>::gotoCenter(nodo<T>* actual){
    return this->actual = this->actual->centro;
}

template <class T>
void arbol<T>::gotoLeft(nodo<T>* actual){
    return this->actual = this->actual->izquierda;
}

// Para determinar si es una hoja
template <class T>
bool arbol<T>::isLeaf(nodePtr nodoTemp){
    return  nodoTemp->izquierda==NULL && nodoTemp->centro==NULL && nodoTemp->derecha==NULL;
}
// Insertar eligiendo la rama
template <class T>
void arbol<T>::insertarOrdenado(T valor, int rama, nodo<T>* nodoActual){
    nodePtr nuevoNodo = new nodo<T>(valor);
    if (raiz == NULL) {
        raiz = nuevoNodo;
    } else {
        if (nodoActual == NULL) nodoActual = raiz;
        while (true) {
            if (rama == 0 && nodoActual->izquierda == NULL) {
                nodoActual->izquierda = nuevoNodo;
                break;
            } else if (rama == 1 && nodoActual->centro == NULL) {
                nodoActual->centro = nuevoNodo;
                break;
            } else if (rama == 2 && nodoActual->derecha == NULL) {
                nodoActual->derecha = nuevoNodo;
                break;
            } else {
                if (rama == 0 && nodoActual->izquierda != NULL) {
                    nodoActual = nodoActual->izquierda;
                } else if (rama == 1 && nodoActual->centro != NULL) {
                    nodoActual = nodoActual->centro;
                } else if (rama == 2 && nodoActual->derecha != NULL) {
                    nodoActual = nodoActual->derecha;
                }
                int eleccion=0;
                //cout << "La rama seleccionada existe, seleccione la siguiente rama (0, 1 o 2): ";
                //cin >> eleccion;
                rama = eleccion;
            }
        }
    }
}

// Insertar por defecto
template <class T>
void arbol<T>::insertar(T valor){
    nodePtr nuevoNodo = new nodo<T>(valor);
    if (raiz == NULL) {
        raiz = nuevoNodo;
    } else {
        nodePtr nodoActual = raiz;
        int nivelRamaIzquierda = 0;
        int nivelRamaDerecha = 0;
        
        while(nodoActual->derecha != NULL){
            nodoActual = nodoActual->derecha;
            nivelRamaDerecha++;
        }
        nodoActual=raiz;
        while(nodoActual->izquierda != NULL){
            nodoActual = nodoActual->izquierda;
            nivelRamaIzquierda++;
        }

        if(nivelRamaDerecha==nivelRamaIzquierda){
            nodoActual->izquierda= nuevoNodo;
        } else {
            nodePtr nodoTemp;
            int ramaActual=0;
            
            nodoTemp = bucleInsertar(raiz, nivelRamaDerecha, ramaActual);

            if (nodoTemp->izquierda==NULL){
                nodoTemp->izquierda = nuevoNodo;
            } else if (nodoTemp->centro==NULL){
                nodoTemp->centro = nuevoNodo;
            } else if (nodoTemp->derecha==NULL){
                nodoTemp->derecha = nuevoNodo;
            }
        }
    }
}

// Función utilizada en la función Insertar
template <class T>
nodo<T>* arbol<T>::bucleInsertar(nodePtr nodoTemp, int ramaObjetivo, int ramaActual){
    if (nodoTemp==NULL) return NULL;

    if (ramaObjetivo==ramaActual){
        if(nodoTemp->izquierda==NULL || nodoTemp->centro==NULL || nodoTemp->derecha==NULL){
            return nodoTemp;
        }
        return NULL;
    }
    ramaActual++;

    nodePtr nodoLeido=NULL;;
    nodoLeido = bucleInsertar(nodoTemp->izquierda, ramaObjetivo, ramaActual);
    if (nodoLeido!=NULL) return nodoLeido;

    nodoLeido = bucleInsertar(nodoTemp->centro, ramaObjetivo, ramaActual);
    if (nodoLeido!=NULL) return nodoLeido;

    nodoLeido = bucleInsertar(nodoTemp->derecha, ramaObjetivo, ramaActual);
    if (nodoLeido!=NULL) return nodoLeido;
    return NULL;
}

// Impresión Orden del árbol
template <class T>
void arbol<T>::orden(nodePtr nodoTemp, bool ordenEleccion) {
        if (nodoTemp == NULL) return;

        if (ordenEleccion==true) cout << nodoTemp->dato << " ";

        orden(nodoTemp->izquierda, ordenEleccion);
        orden(nodoTemp->centro, ordenEleccion);
        orden(nodoTemp->derecha, ordenEleccion);

        if (ordenEleccion==false) cout << nodoTemp->dato << " ";
}

// Para conseguir el nodo padre del que se quiere eliminar
template <class T>
nodo<T>* arbol<T>::bucleObtenerBorrar(T valor, nodePtr nodoTemp){
    if (nodoTemp==NULL) return NULL;

    // Tener presente la importancia de verificación de NULL
    if (nodoTemp->izquierda != NULL || nodoTemp->centro != NULL || nodoTemp->derecha != NULL){
        if (nodoTemp->izquierda->dato == valor || nodoTemp->centro->dato == valor || nodoTemp->derecha->dato == valor){
            return nodoTemp;
        }
    }

    nodePtr nodoLeido=NULL;;
    nodoLeido = bucleObtenerBorrar(valor, nodoTemp->izquierda);
    if (nodoLeido != NULL) return nodoLeido;

    nodoLeido = bucleObtenerBorrar(valor, nodoTemp->centro);
    if (nodoLeido != NULL) return nodoLeido;

    nodoLeido = bucleObtenerBorrar(valor, nodoTemp->derecha);
    if (nodoLeido != NULL) return nodoLeido;
    
    return NULL;
}

//Para borrar los hijos del que se quiere borrar
template <class T>
void arbol<T>::bucleBorrar(nodePtr nodoTemp){
    if (nodoTemp==NULL) return;

    if(nodoTemp->izquierda != NULL && nodoTemp->centro != NULL && nodoTemp->derecha != NULL){
        bucleBorrar(nodoTemp->izquierda);
        bucleBorrar(nodoTemp->centro);
        bucleBorrar(nodoTemp->derecha);
    }

    delete nodoTemp;
    return;
}

template <class T>
void arbol<T>::borrar(T valor, bool eliminarHijos){
    nodePtr padre = NULL;
    nodePtr deleteNode= NULL;

    
    padre = bucleObtenerBorrar(valor, raiz);
    
    // Tener presente la importancia de verificación de NULL
    if(padre==NULL){
        cout<<"El valor no fue encontrado"<<endl;
        return;
    }

    if(padre->izquierda->dato == valor){
        deleteNode = padre->izquierda;
        padre->izquierda=NULL;
    } else if(padre->centro->dato == valor){
        deleteNode = padre->centro;
        padre->centro=NULL;
    }else if(padre->derecha->dato == valor){
        deleteNode = padre->derecha;
        padre->derecha=NULL;
    }
    
    if (eliminarHijos==true){
        bucleBorrar(deleteNode);
    } else {
        // Codigo para obtener una rama a preguntar
        // ¿Se obtiene una y se borran las demás o se insertan?
        delete deleteNode;
    }

}