#ifndef MATRIX_FUNCTIONS_H_INCLUDED
#define MATRIX_FUNCTIONS_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

/**
 * Estructura base para una matriz de enteros.
 * La matriz se debe guardar en "row-major order".
 **/
template <typename T>
struct matrix {
    unsigned int rows;
    unsigned int cols;
    T* elements;
};

/**
 * Una implementación naive de transposición de matrices cuadradas.
 **/
template <typename T>
void matrix_transpose_naive(matrix<T>* inMatrix, matrix<T>* outMatrix,
                            unsigned int rows, unsigned int cols,
                            unsigned int rowOffset, unsigned int colOffset,
                            const unsigned int minSubMatrixSize = 0) {
    const unsigned int rowLimit = rowOffset + rows;
    const unsigned int colLimit = colOffset + cols;
    for(unsigned int i = rowOffset; i < rowLimit; i++) {
        for(unsigned int j = colOffset; j < colLimit; j++) {
            outMatrix->elements[j * rows + i] = inMatrix->elements[i * rows + j];
        }
    }
}

/**
 * Imprime una matriz en la salida estándar. Esta función utiliza std::cout.
 **/
template <typename T>
void print_matrix(matrix<T>* matrix) {
    for(unsigned int i = 0; i < matrix->rows; i++) {
        for(unsigned int j = 0; j < matrix->cols; j++) {
            std::cout << matrix->elements[i * matrix->rows + j];
            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

/**
 *  Reserva espacio para un objeto T_matrix, mediante malloc.
 **/
template <typename T>
void alloc_matrix(matrix<T>* matrix, unsigned int rows, unsigned int cols) {
    matrix->elements = (T*) malloc(sizeof(T) * rows * cols);
    matrix->rows = rows;
    matrix->cols = cols;
}

/**
 *  Libera memoria asociada a los elementos de la matriz, mediante free.
 **/
template <typename T>
void dealloc_matrix(matrix<T>* matrix) {
    free(matrix->elements);
}

/**
 *  Inicializa la matriz con valores enteros aleatorios entre cero y upper_limit (excluyendo este
 *  último).
 *  TODO: Cambiar el rng por uno que use la STL.
 **/
template <typename T>
void randInt_init_matrix(matrix<T>* matrix, unsigned int upper_limit) {
    srand(time(NULL));
    for(unsigned int i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->elements[i] = rand() % upper_limit;
    }
}

/**
 *  Inicializa la matriz con valores flotantes aleatorios entre cero y upper_limit (excluyendo este
 *  último).
 *  TODO: Cambiar el rng por uno que use la STL.
 **/
template <typename T>
void randFloat_init_matrix(matrix<T>* matrix, float upper_limit) {
    srand(time(NULL));
    for(unsigned int i = 0; i < matrix->rows * matrix->cols; i++) {
        matrix->elements[i] = static_cast<float>(rand())/static_cast<float>(RAND_MAX / upper_limit);
    }
}

/**
 *  Algoritmo recursivo que divide el problema hasta un límite (arbitrario), que luego invoca a
 *  matrix_transpose_naive.
**/
//const int minSubMatrixSize = 1; //Tamaño mínimo de la sub-matriz.
template <typename T>
void matrix_transpose_co(matrix<T>* inMatrix, matrix<T>* outMatrix,
                         unsigned int rows, unsigned int cols,
                         unsigned int rowOffset, unsigned int colOffset,
                         const unsigned int minSubMatrixSize = 1) {
    if( rows > minSubMatrixSize || cols > minSubMatrixSize) {
        if( cols >= rows) {
            const int halfCols = cols / 2;
            matrix_transpose_co(inMatrix, outMatrix, rows, halfCols, rowOffset, colOffset,
                                minSubMatrixSize);
            matrix_transpose_co(inMatrix, outMatrix, rows, halfCols, rowOffset,
                                colOffset + halfCols, minSubMatrixSize);
        } else {
            const int halfRows = rows / 2;
            matrix_transpose_co(inMatrix, outMatrix, halfRows, cols, rowOffset, colOffset,
                                minSubMatrixSize);
            matrix_transpose_co(inMatrix, outMatrix, halfRows, cols, rowOffset + halfRows,
                                colOffset, minSubMatrixSize);
        }
    } else {
        matrix_transpose_naive(inMatrix, outMatrix, rows, cols, rowOffset, colOffset);
    }
}

/**
 *  Algoritmo naive que multiplica dos matrices (A (n x m) y B (m x p)) y guarda el resultado en outMatrix.
 *  Este algoritmo incurre en O(nmp) operaciones, y (en el peor caso) O(nmp) fallas de cache.
 **/
template <typename T>
void matrix_multiply_naive(matrix<T>* outMatrix, matrix<T>* matrixA, matrix<T>* matrixB) {
    for(unsigned int i = 0; i < matrixA->rows; i++) {
        for(unsigned int j = 0; j < matrixB->cols; j++) {
            T sum = 0;
            for(unsigned int k = 0; k < matrixA->cols; k++) {
                sum += matrixA->elements[i * matrixA->rows + k] * matrixB->elements[k * matrixB->cols + j];
            }
            outMatrix->elements[i * matrixA->rows + j] = sum;
        }
    }
}

#endif // MATRIX_FUNCTIONS_H_INCLUDED
