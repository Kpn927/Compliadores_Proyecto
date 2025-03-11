#include <iostream>
using namespace std;

template <class T> class Tree;
// Clase para el nodo
template <class T>
class Node {
private:
    T dato;
    typedef Node<T>* nodePtr;
    nodePtr izquierda;
    nodePtr centro;
    nodePtr derecha;

public:
    //nodo(int valor) : dato(valor), izquierda(NULL), centro(NULL), derecha(NULL) {}
    Node(T valor, nodePtr izq=NULL, nodePtr der=NULL, nodePtr cen=NULL){
        dato=valor;
        izquierda=izq;
        derecha=der;
        centro=cen;
    }
    
    Node<T>* getRight(){ return this->derecha;};
    Node<T>* getCenter(){return this->centro;};
    Node<T>* getLeft(){return this->izquierda;};
    void setValue(T value){ this->dato = value;return;};
    T getValue(){ return this->dato;};
    friend class Tree<T>;
};

// Clase del árbol
template <class T>
class Tree {
private:
    typedef Node<T>* nodePtr;
    
    Node<T>* bucleInsertar(nodePtr nodoTemp, int ramaObjetivo, int ramaActual);

    bool isLeaf(nodePtr nodoTemp);

    void bucleBorrar(nodePtr nodoTemp);

    Node<T>* bucleObtenerBorrar(T valor, nodePtr nodoTemp);
public:
    nodePtr raiz;
    nodePtr actual;
    Tree(){
        raiz=NULL;
        actual=raiz;
    }

    ~Tree(){
        this->bucleBorrar(this->getRaiz());
    }

    void insertarOrdenado(T valor, int rama, Node<T>* actual);

    void insertarOrdenado(T valor, int rama);

    void insertar(T valor);

    void orden(nodePtr nodoTemp, bool ordenEleccion);

    void borrar( T valor, bool eliminarHijos);

    Node<T>* getRaiz();
};

template <class T>
Node<T>* Tree<T>::getRaiz(){
    return this->raiz;
}

// Para determinar si es una hoja
template <class T>
bool Tree<T>::isLeaf(nodePtr nodoTemp){
    return  nodoTemp->izquierda==NULL && nodoTemp->centro==NULL && nodoTemp->derecha==NULL;
}
// Insertar eligiendo la rama
template <class T>
void Tree<T>::insertarOrdenado(T valor, int rama, Node<T>* nodoActual){
    nodePtr nuevoNodo = new Node<T>(valor);
    if (raiz == NULL) {
        raiz = nuevoNodo;
        nodoActual = raiz;
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
            } /*else {
                if (rama == 0 && nodoActual->izquierda != NULL) {
                    nodoActual = nodoActual->izquierda;
                } else if (rama == 1 && nodoActual->centro != NULL) {
                    nodoActual = nodoActual->centro;
                } else if (rama == 2 && nodoActual->derecha != NULL) {
                    nodoActual = nodoActual->derecha;
                }
                int eleccion=0;
                rama = eleccion;
            }*/
        }
    }
}

template <class T>
void Tree<T>::insertarOrdenado(T valor, int rama){
    this->insertarOrdenado(valor, rama, this->actual);
}
// Insertar por defecto
template <class T>
void Tree<T>::insertar(T valor){
    nodePtr nuevoNodo = new Node<T>(valor);
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
Node<T>* Tree<T>::bucleInsertar(nodePtr nodoTemp, int ramaObjetivo, int ramaActual){
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
void Tree<T>::orden(nodePtr nodoTemp, bool ordenEleccion) {
        if (nodoTemp == NULL) return;

        if (ordenEleccion==true) cout << nodoTemp->dato << " ";

        orden(nodoTemp->izquierda, ordenEleccion);
        orden(nodoTemp->centro, ordenEleccion);
        orden(nodoTemp->derecha, ordenEleccion);

        if (ordenEleccion==false) cout << nodoTemp->dato << " ";
}

// Para conseguir el nodo padre del que se quiere eliminar
template <class T>
Node<T>* Tree<T>::bucleObtenerBorrar(T valor, nodePtr nodoTemp){
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
void Tree<T>::bucleBorrar(nodePtr nodoTemp){
    if (nodoTemp==NULL) return;
    if (nodoTemp==raiz) raiz = NULL;

    if(nodoTemp->izquierda !=NULL) bucleBorrar(nodoTemp->izquierda);
    if(nodoTemp->centro !=NULL) bucleBorrar(nodoTemp->centro);
    if(nodoTemp->derecha !=NULL) bucleBorrar(nodoTemp->derecha);

    delete nodoTemp;
    nodoTemp ==NULL;
    return;
}

template <class T>
void Tree<T>::borrar(T valor, bool eliminarHijos){
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