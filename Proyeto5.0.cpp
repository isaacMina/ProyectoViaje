#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

struct Pasajero {
    char nombre[50];
    int Cedula;
    int id;
};

struct Viaje {
    char identificador[50];
    char matricula[50];
    char nombreEmbarcacion[50];
    char destino[50];
    int precio;
    int capacidad;
    int dia, mes, anio;
    Pasajero pasajeros[5]; // Arreglo para almacenar hasta 5 pasajeros
    int cantidadPasajeros = 0; // Variable para llevar la cuenta de pasajeros
};

struct nodo {
    Viaje viaje;
    struct nodo *izq;
    struct nodo *der;
    int altura;
};

struct nodo *raiz = NULL;

int obtenerAltura(struct nodo *n) {
    if (n == NULL) {
        return 0;
    }
    return n->altura;
}

int mayor(int a, int b) {
    return (a > b) ? a : b;
}

struct nodo* rotarDerecha(struct nodo *y) {
    struct nodo *x = y->izq;
    struct nodo *T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    return x;
}

struct nodo* rotarIzquierda(struct nodo *x) {
    struct nodo *y = x->der;
    struct nodo *T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = mayor(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = mayor(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    return y;
}

int obtenerBalance(struct nodo *n) {
    if (n == NULL) {
        return 0;
    }
    return obtenerAltura(n->izq) - obtenerAltura(n->der);
}

struct nodo* insertar(struct nodo* nodo, struct Viaje v) {
    if (nodo == NULL) {
        struct nodo* nuevoNodo = (struct nodo*)malloc(sizeof(struct nodo));
        if (nuevoNodo == NULL) {
            cout << "Error al asignar memoria." << endl;
            exit(1);
        }
        nuevoNodo->viaje = v;
        nuevoNodo->izq = NULL;
        nuevoNodo->der = NULL;
        nuevoNodo->altura = 1;
        return nuevoNodo;
    }

    if (strcmp(v.identificador, nodo->viaje.identificador) < 0) {
        nodo->izq = insertar(nodo->izq, v);
    } else if (strcmp(v.identificador, nodo->viaje.identificador) > 0) {
        nodo->der = insertar(nodo->der, v);
    } else {
        return nodo;
    }

    nodo->altura = 1 + mayor(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && strcmp(v.identificador, nodo->izq->viaje.identificador) < 0) {
        return rotarDerecha(nodo);
    }

    if (balance < -1 && strcmp(v.identificador, nodo->der->viaje.identificador) > 0) {
        return rotarIzquierda(nodo);
    }

    if (balance > 1 && strcmp(v.identificador, nodo->izq->viaje.identificador) > 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && strcmp(v.identificador, nodo->der->viaje.identificador) < 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void inOrden(struct nodo *nodo) {
    if (nodo != NULL) {
        inOrden(nodo->izq);
        cout << nodo->viaje.identificador << " ";
        inOrden(nodo->der);
    }
}

Viaje crearViaje() {
    Viaje v;
    cout << "Ingrese la matricula de la embarcacion: ";
    cin >> v.matricula;
    cout << "Ingrese el nombre de la embarcacion: ";
    cin >> v.nombreEmbarcacion;
    cout << "Ingrese el destino del viaje: ";
    cin >> v.destino;
    cout << "Ingrese el precio del viaje: ";
    cin >> v.precio;
    cout << "Ingrese la capacidad de la embarcacion: ";
    cin >> v.capacidad;
    cout << "Ingrese la fecha del viaje (dia mes anio): ";
    cin >> v.dia >> v.mes >> v.anio;

    sprintf(v.identificador, "%c%c%d%02d%02d", v.matricula[0], v.matricula[1], v.anio, v.mes, v.dia);

    return v;
}

struct nodo* buscarViaje(struct nodo* nodo, char* identificador) {
    if (nodo == NULL || strcmp(nodo->viaje.identificador, identificador) == 0) {
        return nodo;
    }

    if (strcmp(nodo->viaje.identificador, identificador) < 0) {
        return buscarViaje(nodo->der, identificador);
    }

    return buscarViaje(nodo->izq, identificador);
}

struct nodo* nodoMinimo(struct nodo* nodo) {
    struct nodo* actual = nodo;

    while (actual->izq != NULL) {
        actual = actual->izq;
    }

    return actual;
}

struct nodo* eliminarViaje(struct nodo* raiz, char* identificador) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(identificador, raiz->viaje.identificador) < 0) {
        raiz->izq = eliminarViaje(raiz->izq, identificador);
    } else if (strcmp(identificador, raiz->viaje.identificador) > 0) {
        raiz->der = eliminarViaje(raiz->der, identificador);
    } else {
        if ((raiz->izq == NULL) || (raiz->der == NULL)) {
            struct nodo* temp = raiz->izq ? raiz->izq : raiz->der;

                        if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            struct nodo* temp = nodoMinimo(raiz->der);

            raiz->viaje = temp->viaje;

            raiz->der = eliminarViaje(raiz->der, temp->viaje.identificador);
        }
    }

    if (raiz == NULL) {
        return raiz;
    }

    raiz->altura = 1 + mayor(obtenerAltura(raiz->izq), obtenerAltura(raiz->der));

    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) >= 0) {
        return rotarDerecha(raiz);
    }

    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotarIzquierda(raiz->izq);
        return rotarDerecha(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) <= 0) {
        return rotarIzquierda(raiz);
    }

    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotarDerecha(raiz->der);
        return rotarIzquierda(raiz);
    }

    return raiz;
}

void registrarPasajero(struct nodo* raiz, char* identificador, Pasajero pasajero) {
    struct nodo* nodoViaje = buscarViaje(raiz, identificador);
    if (nodoViaje != NULL) {
        if (nodoViaje->viaje.cantidadPasajeros < nodoViaje->viaje.capacidad) {
            nodoViaje->viaje.pasajeros[nodoViaje->viaje.cantidadPasajeros++] = pasajero;
            cout << "Pasajero registrado correctamente en el viaje con ID: " << nodoViaje->viaje.identificador << endl;
        } else {
            cout << "La capacidad de la embarcacion ha sido alcanzada. No se puede registrar mas pasajeros." << endl;
        }
    } else {
        cout << "Viaje no encontrado." << endl;
    }
}

void listarPasajeros(struct nodo* raiz, char* identificador) {
    struct nodo* nodoViaje = buscarViaje(raiz, identificador);
    if (nodoViaje != NULL) {
        for (int i = 0; i < nodoViaje->viaje.cantidadPasajeros; i++) {
            cout << "ID: " << nodoViaje->viaje.pasajeros[i].id << ", Nombre: " << nodoViaje->viaje.pasajeros[i].nombre << ", Cedula: " << nodoViaje->viaje.pasajeros[i].Cedula << endl;
        }
    } else {
        cout << "Viaje no encontrado." << endl;
    }
}

int main() {
    int opcion;
    do {
        cout << "1. Registrar un nuevo viaje." << endl;
        cout << "2. Buscar un viaje por identificador." << endl;
        cout << "3. Listar todos los viajes." << endl;
        cout << "4. Eliminar un viaje por identificador." << endl;
        cout << "5. Registrar un pasajero en un viaje." << endl;
        cout << "6. Listar todos los pasajeros de un viaje." << endl;
        cout << "7. Salir." << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                Viaje nuevoViaje = crearViaje();
                raiz = insertar(raiz, nuevoViaje);
                break;
            }
             case 2: {
                char identificador[50];
                cout << "Ingrese el identificador del viaje a buscar: ";
                cin >> identificador;
                struct nodo* encontrado = buscarViaje(raiz, identificador);
                if (encontrado != NULL) {
                    Viaje v = encontrado->viaje;
                    cout << "Viaje encontrado:" << endl;
                    cout << "Matricula: " << v.matricula << endl;
                    cout << "Nombre Embarcacion: " << v.nombreEmbarcacion << endl;
                    cout << "Destino: " << v.destino << endl;
                    cout << "Precio: " << v.precio << endl;
                    cout << "Capacidad: " << v.capacidad << endl;
                    cout << "Fecha: " << v.dia << "/" << v.mes << "/" << v.anio << endl;
                } else {
                    cout << "Viaje no encontrado." << endl;
                }
                break;
            }
            case 3: {
                cout << "Lista de todos los viajes en orden ascendente de sus identificadores únicos:" << endl;
                inOrden(raiz);
                cout << endl;
                break;
            }
            case 4: {
                char identificador[50];
                cout << "Ingrese el identificador del viaje a eliminar: ";
                cin >> identificador;
                raiz = eliminarViaje(raiz, identificador);
                cout << "Viaje eliminado correctamente." << endl;
                break;
            }
            case 5: {
                char identificador[50];
                cout << "Ingrese el identificador del viaje: ";
                cin >> identificador;
                Pasajero nuevoPasajero;
                cout << "Ingrese el nombre del pasajero: ";
                cin >> nuevoPasajero.nombre;
                cout << "Ingrese la cédula del pasajero: ";
                cin >> nuevoPasajero.Cedula;
                registrarPasajero(raiz, identificador, nuevoPasajero);
                break;
            }
            case 6: {
                char identificador[50];
                cout << "Ingrese el identificador del viaje: ";
                cin >> identificador;
                listarPasajeros(raiz, identificador);
                break;
            }
            case 7: {
                cout << "Saliendo del programa." << endl;
                break;
            }
            default: {
                cout << "Opción inválida. Por favor, seleccione una opción válida." << endl;
                break;
            }
        }
    } while (opcion != 7);

    return 0;
}


