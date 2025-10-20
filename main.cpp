#include <iostream>

using namespace std;

// Clase Base Abstracta - MatrizBase
template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:

    // Constructor - inicializa filas y columnas
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}

    // Destructor virtual
    virtual ~MatrizBase() {
        // Destructor virtual
    }

    // Método para obtener valor
    // Usamos "const" porque no modifica la matriz
    virtual T obtenerValor(int f, int c) const = 0;
    
    // Método para establecer valor
    virtual void establecerValor(int f, int c, T val) = 0;

    // Método para cargar valores
    virtual void cargarValores() = 0;
 
    // Método para imprimir la matriz
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;

    // Método para imprimir la matriz
    virtual void imprimir() const = 0;

    // Getters para filas y columnas
    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};

// Clases Concretas (Implementaciones) - MatrizDinamica
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

    // Función auxiliar para reservar memoria
    void reservarMemoria(int f, int c) {
        // Usamos this->_filas y this->_columnas que vienen de MatrizBase
        this->_filas = f;
        this->_columnas = c;

        // Creamos el arreglo de punteros - filas
        _datos = new T*[f];
        // Para cada fila, creamos un arreglo de T - columnas
        for (int i = 0; i < f; ++i) {
            _datos[i] = new T[c];
            // Inicializamos a 0 
            for (int j = 0; j < c; ++j) {
                _datos[i][j] = T(); // T() es 0 para int/float
            }
        }
    }

    // Función auxiliar para liberar memoria
    void liberarMemoria() {
        // Solo liberamos si _datos no es nulo
        if (_datos != nullptr) {
            cout << "Liberando memoria de Matriz Dinámica (" 
                 << this->_filas << "x" << this->_columnas << ")..." << endl;

            // Borramos cada arreglo de columna (cada fila)
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            // Borramos el arreglo de punteros (filas)
            delete[] _datos;
            _datos = nullptr;
        }
    }


public:

    // Constructor Principal - Reserva memoria
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c), _datos(nullptr) {
        cout << "Creando Matriz Dinámica A (" << f << "x" << c << ")..." << endl;
        reservarMemoria(f, c);
    }

    // Destructor - Libera la memoria
    virtual ~MatrizDinamica() {
        cout << "Llamando al destructor de Matriz Dinámica..." << endl;
        liberarMemoria();
    }

    // Constructor de Copia
    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        cout << "Llamando al Constructor de Copia de Matriz Dinámica..." << endl;
        // Reservamos nuestra propia memoria
        reservarMemoria(otra._filas, otra._columnas);
        // Copiamos los valores
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                this->_datos[i][j] = otra._datos[i][j];
            }
        }
    }

    // Operador de Asignación por Copia
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        cout << "Llamando al Operador de Asignación por Copia de Matriz Dinámica..." << endl;
        // Evitamos autoasignación
        if (this != &otra) {
            return *this;;
        }

        // Liberamos la memoria que this ya tenía
        liberarMemoria();

        // Reservamos nueva memoria
        reservarMemoria(otra._filas, otra._columnas);

        // Copiamos los valores
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                this->_datos[i][j] = otra._datos[i][j];
            }
        }

        return *this;
    }

    // Constructor de Movimiento
    MatrizDinamica(MatrizDinamica<T>&& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        cout << "Llamando al Constructor de Movimiento de Matriz Dinámica..." << endl;
        // Robamos el puntero de 'otra'
        this->_datos = otra._datos;

        // Dejamos a 'otra' en un estado válido (vacío)
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }

    // Implementación de Métodos Virtuales
    T obtenerValor(int f, int c) const override {
        return _datos[f][c];
    }

    void establecerValor(int f, int c, T val) override {
        _datos[f][c] = val;
    }

    void cargarValores() override {
        // Cargamos valores fijos basados en el tipo
        if (typeid(T) == typeid(float)) {
             // Valores de la demo para A (float 3x2)
             if(this->_filas == 3 && this->_columnas == 2) {
                _datos[0][0] = 1.5; _datos[0][1] = 2.0;
                _datos[1][0] = 0.0; _datos[1][1] = 1.0;
                _datos[2][0] = 4.5; _datos[2][1] = 3.0;
             }
        } else if (typeid(T) == typeid(int)) {
            // Valores de la demo para E (int 2x2)
            if(this->_filas == 2 && this->_columnas == 2) {
                _datos[0][0] = 5; _datos[0][1] = 6;
                _datos[1][0] = 7; _datos[1][1] = 8;
            }
        }
    }

    void imprimir() const override {
        cout << "A =" << endl;
        for (int i = 0; i < this->_filas; ++i) {
            cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                cout << _datos[i][j] << " | ";
            }
            cout << endl;
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        cout << "(La suma es manejada por el método virtual de MatrizDinamica)" << endl;

        // Verificamos que las dimensiones coincidan
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Dimensiones incompatibles para la suma." << endl;
            return nullptr;
        }

        // Creamos una nueva matriz dinámica para el resultado
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

        // Realizamos la suma
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                // Obtieniendo el valor de "this"
                T val1 = this->_datos[i][j]; 
                // Obtenemos el valor de "otra" - usamos el método virtual
                T val2 = otra.obtenerValor(i, j); 
                
                resultado->establecerValor(i, j, val1 + val2);
            }
        }
        
        // Devolvemos el puntero
        return resultado;
    }

};

// Clase concreta - MatrizEstatica
template <typename T, int M, int N> // M filas, N columnas
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N]; // Arreglo estático

public:
    // Constructor - Inicializa filas y columnas
    MatrizEstatica() : MatrizBase<T>(M, N) {
        cout << "Creando Matriz Estática B (" << M << "x" << N << ")..." << endl;
        // Inicializamos a 0
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                _datos[i][j] = T(); // T() es 0 para int/float
            }
        }
    }

    // Destructor
    virtual ~MatrizEstatica() {
        cout << "Llamando al destructor de Matriz Estática..." << endl;
    }

    // Implementación de Métodos Virtuales
    T obtenerValor(int f, int c) const override {
        return _datos[f][c];
    }

    void establecerValor(int f, int c, T val) override {
        _datos[f][c] = val;
    }

    void cargarValores() override {
        // Cargamos valores fijos para la demo
        if (typeid(T) == typeid(float)) {
             // Valores de la demo para B (float 3x2)
             if(M == 3 && N == 2) {
                _datos[0][0] = 0.5; _datos[0][1] = 1.0;
                _datos[1][0] = 2.0; _datos[1][1] = 3.0;
                _datos[2][0] = 1.0; _datos[2][1] = 1.0;
             }
        } else if (typeid(T) == typeid(int)) {
            // Valores de la demo para D (int 2x2)
            if(M == 2 && N == 2) {
                _datos[0][0] = 1; _datos[0][1] = 2;
                _datos[1][0] = 3; _datos[1][1] = 4;
            }
        }
    }

    void imprimir() const override {
        cout << "B =" << endl;
        for (int i = 0; i < this->_filas; ++i) {
            cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                cout << _datos[i][j] << " | ";
            }
            cout << endl;
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        cout << "(La suma es manejada por el método virtual de MatrizEstatica)" << endl;

        // Verificamos que las dimensiones coincidan
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error." << endl;
            return nullptr;
        }

        // Creamos una nueva matriz estática para el resultado
        // El resultado de sumar una Estática es otra Estática.
        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();

        // Realizamos la suma
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                T val1 = this->_datos[i][j]; // Acceso directo
                T val2 = otra.obtenerValor(i, j); // Acceso polimórfico
                
                resultado->establecerValor(i, j, val1 + val2);
            }
        }
        
        return resultado;
    }
};



int main() {
    cout << "--- Sistema genérico de Álgebra Lineal ---" << endl;

    cout << "\n>> Demostración de Genericidad (Tipo FLOAT) <<" << endl;

    // 1. Creación de Matriz Dinámica (a través del puntero base)
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    A->cargarValores(); // Carga los valores float para 3x2
    A->imprimir();
    cout << endl;

    // 2. Creación de Matriz Estática (a través del puntero base)
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    B->cargarValores(); // Carga los valores float para 3x2
    B->imprimir();
    cout << endl;

    // 3. Operación Polimórfica (Suma)
    cout << "SUMANDO: Matriz C = A + B ..." << endl;

    // A es un puntero a MatrizBase pero apunta a una MatrizDinamica
    // la suma es manejada por el método virtual de MatrizDinamica
    MatrizBase<float>* C = A->sumar(*B);

    // Verificamos que la suma fue exitosa
    if (C != nullptr) {
        cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):" << endl;
        C->imprimir();
    }

    // Demostración de Limpieza de Memoria
    cout << "\n>> Demostración de Limpieza de Memoria <<" << endl;

    // Limpieza de Float
    cout << "Llamando al destructor de C..." << endl;
    delete C; // Destructor de MatrizDinamica
    cout << "Llamando al destructor de B..." << endl;
    delete B; // Destructor de MatrizEstatica
    cout << "Llamando al destructor de A..." << endl;
    delete A; // Destructor de MatrizDinamica

    cout << "\nSistema cerrado." << endl;

    return 0;
}