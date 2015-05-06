//
//  Matrix.cpp
//
//
//  Created by Christopher Glasz on 3/13/15.
//
//

#include "ColumnVector.h"
#include "RowVector.h"

using namespace csc450Lib_linalg_base;

Matrix::Matrix(int nbRows, int nbCols, float ** a) {
    this->nbRows = nbRows;
    this->nbCols = nbCols;
    this->a = a;
}

Matrix::Matrix(int nbRows, const int nbCols) {
    this->nbRows = nbRows;
    this->nbCols = nbCols;
    this->a = new float*[nbRows];
    for (int i = 0; i < nbRows; i++)
        a[i] = new float[nbCols];
}

Matrix::~Matrix() {
    for (int i = 0; i < nbRows; i++)
        delete [] a[i];
    delete[] a;
    
    this->nbRows = 0;
    this->nbCols = 0;
}

const Matrix* Matrix::transpose(const Matrix *matA) {
    Matrix* t = new Matrix(matA->cols(), matA->rows());
    
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matA->cols(); j++) {
            t->set(j, i, matA->get(i, j));
        }
    }
    
    return t;
}

Matrix* Matrix::add(const Matrix *matA,
                    const Matrix *matB) {
    if (matA->cols() != matB->cols() || matA->rows() != matB->rows())
        throw "Matrices do not match";
    
    Matrix * sum = new Matrix(matA->rows(), matA->cols());
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matA->cols(); j++) {
            sum->set(i, j, matA->get(i, j) + matB->get(i, j));
        }
    }
    
    return sum;
}

Matrix* Matrix::multiply(const Matrix *matA,
                         const Matrix *matB) {
    if (matA->cols() != matB->rows())
        throw "Matrices do not match";
    
    Matrix * prod = new Matrix(matA->rows(), matB->cols());
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matB->cols(); j++) {
            float element = 0;
            for (int k = 0; k < matA->cols(); k++) {
                element += matA->get(i, k) * matB->get(k, j);
            }
            prod->set(i, j, element);
        }
    }
    
    return prod;
    
}

Matrix* Matrix::multiply(float mult,
                         const Matrix *mat) {
    Matrix * prod = new Matrix(mat->rows(), mat->cols());
    for (int i = 0; i < mat->rows(); i++) {
        for (int j = 0; j < mat->cols(); j++) {
            prod->set(i, j, mult * mat->get(i,j));
        }
    }
    return prod;
}

const Matrix* Matrix::outerProduct(const ColumnVector *u,
                                   const ColumnVector *v) {
    Matrix * oProd = new Matrix(u->rows(),v->rows());
    
    for (int i = 0; i < u->rows(); i++) {
        for (int j = 0; j < v->rows(); j++) {
            oProd->set(i, j, u->get(i) * v->get(j));
        }
    }
    
    return oProd;
}

Matrix* Matrix::subtract(const Matrix *matA,
                         const Matrix *matB) {
    if (matA->cols() != matB->cols() || matA->rows() != matB->rows())
        throw "Matrices do not match";
    
    Matrix * diff = new Matrix(matA->rows(), matA->cols());
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matA->cols(); j++) {
            diff->set(i, j, matA->get(i, j) - matB->get(i, j));
        }
    }
    
    return diff;
}

ColumnVector* Matrix::subtract(const ColumnVector *matA,
                               const ColumnVector *matB) {
    if (matA->rows() != matB->rows())
        throw "Matrices do not match";
    
    ColumnVector * diff = new ColumnVector(matA->rows());
    for (int i = 0; i < matA->rows(); i++) {
        diff->set(i, matA->get(i) - matB->get(i));
        
    }
    
    return diff;
}


Matrix* Matrix::copyOf(const Matrix *matA) {
    Matrix *copy = new Matrix(matA->rows(), matA->cols());
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matA->cols(); j++) {
            copy->set(i, j, matA->get(i, j));
        }
    }
    return copy;
}

ColumnVector* Matrix::column(const Matrix *matA) {
    ColumnVector *column = new ColumnVector(matA->rows() * matA->cols());
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matA->cols(); j++) {
            column->set(i * matA->cols() + j, matA->get(i, j));
        }
    }
    return column;
}

Matrix* Matrix::deflate(const Matrix *matA,
                       const ColumnVector *eigenvector,
                       float eigenvalue) {
    float norm = eigenvector->norm2();
    ColumnVector *u = (ColumnVector*)Matrix::multiply(eigenvalue/(norm * norm),
                                                      eigenvector);
    
    Matrix *deflated = Matrix::copyOf(matA);
    deflated = Matrix::subtract(matA,
                                Matrix::multiply(eigenvector,
                                                 Matrix::transpose(u)));
    
    return deflated;
}

float Matrix::dotProduct(const ColumnVector *u,
                         const ColumnVector *v) {
    
    if (u->rows() != v->rows())
        throw "Vectors do not match";
    
    float dot = 0;
    for (int i = 0; i < u->rows(); i++) {
        dot += u->get(i) * v->get(i);
    }
    
    return dot;
    
}

const Matrix* Matrix::mask(const Matrix *matA,
                           const Matrix *mask) {
    if (matA->cols() != mask->cols() || matA->rows() != mask->rows())
        throw "Matrices do not match";
    
    Matrix * masked = new Matrix(matA->rows(), matA->cols());
    for (int i = 0; i < matA->rows(); i++) {
        for (int j = 0; j < matA->cols(); j++) {
            masked->set(i, j, matA->get(i, j) * mask->get(i, j));
        }
    }
    
    return masked;
}

int Matrix::cols() const {
    return nbCols;
}

int Matrix::rows() const {
    return nbRows;
}

float Matrix::get(int theRow, int theCol) const {
    return a[theRow][theCol];
}

ColumnVector* Matrix::getColumn(int theCol) const {
    ColumnVector* col = new ColumnVector(nbCols);
    for (int i = 0; i < nbCols; i++)
        col->set(i, a[i][theCol]);
    return col;
}

float** Matrix::getArray() const {
    return a;
}

float Matrix::norm1() const {
    float norm = 0;
    float current;
    
    for (int i = 0; i < nbRows; i++) {
        current = 0;
        for (int j = 0; j < nbCols; j++) {
            current += std::abs(a[i][j]);
        }
        if (current > norm)
            norm = current;
    }
    
    return norm;
}

float Matrix::norm2() const {
    float norm = 0;
    float current;
    
    for (int i = 0; i < nbRows; i++) {
        current = 0;
        for (int j = 0; j < nbCols; j++) {
            current += std::abs(a[i][j]);
        }
        if (current > norm)
            norm = current;
    }
    
    return norm;
}

float Matrix::normInf() const {
    float norm = 0;
    float current;
    
    for (int j = 0; j < nbCols; j++) {
        current = 0;
        for (int i = 0; i < nbRows; i++) {
            current += std::abs(a[i][j]);
        }
        if (current > norm)
            norm = current;
    }
    
    return norm;
}

void Matrix::set(int theRow, int theCol, float theVal) {
    a[theRow][theCol] = theVal;
}

void Matrix::addRow(const RowVector *row) {
    if (row->cols() != nbCols)
        throw "Matrices do not match";
    
    float **newa = new float*[nbRows+1];
    for (int i = 0; i < nbRows; i++) {
        newa[i] = new float[nbCols];
        for (int j = 0; j < nbCols; j++) {
            newa[i][j] = a[i][j];
        }
    }
    newa[nbRows] = new float[nbCols];
    for (int j = 0; j < nbCols; j++) {
        newa[nbRows][j] = row->get(j);
    }
    
    nbRows += 1;
    this->a = newa;
    
}

void Matrix::addColumn(const ColumnVector *col) {
    if (col->rows() != nbRows)
        throw "Matrices do not match";
    
    float **newa = new float*[nbRows];
    for (int i = 0; i < nbRows; i++) {
        newa[i] = new float[nbCols+1];
        for (int j = 0; j < nbCols; j++) {
            newa[i][j] = a[i][j];
        }
        newa[i][nbCols] = col->get(i);
    }
    
    nbCols += 1;
    this->a = newa;
    
}

const RowVector* Matrix::averageRow(void) const {
    RowVector *ave = new RowVector(nbCols);
    for (int j = 0; j < nbCols; j++) {
        float e = 0;
        for (int i = 0; i < nbRows; i++) {
            e += a[i][j];
        }
        e /= nbRows;
        ave->set(j, e);
    }
    return ave;
}

const ColumnVector* Matrix::averageColumn(void) const {
    ColumnVector *ave = new ColumnVector(nbRows);
    for (int i = 0; i < nbRows; i++) {
        float e = 0;
        for (int j = 0; j < nbCols; j++) {
            e += a[i][j];
        }
        e /= nbCols;
        ave->set(i, e);
    }
    return ave;
    
}

ColumnVector* Matrix::eigenvector(const ColumnVector *init, int kmax, float tol) const {
    // make a copy of the initial vector
    ColumnVector *x = (ColumnVector*)Matrix::copyOf(init);
    
    int k = 1;
    int imax = 0;
    float s = 1;
    bool converged = false;
    
    // get an initial l and lastl
    const ColumnVector *y = (ColumnVector*)Matrix::multiply(this, x);
    float l = y->normInf();
    float lastl = l + 100;
    
    // loop until converged
    for (k = 1; k < kmax && !converged; k++) {
        y = (ColumnVector*)Matrix::multiply(this, x);
        l = y->normInf();
        if (abs(l - lastl) < tol)
            converged = true;
        lastl = l;
        imax = y->maxInd();
        
        // sign of eigenalue
        s = (y->get(imax) * x->get(imax)) < 0 ? -1 : 1;
        x = (ColumnVector*)Matrix::multiply(1.0 / l, y);
    }
    if (k == kmax)
        cout << "No convergence\n";
    return x;
}

float Matrix::eigenvalue(const ColumnVector *init, int kmax, float tol) const {
    // make a copy of the initial vector
    ColumnVector *x = (ColumnVector*)Matrix::copyOf(init);
    
    int k = 1;
    int imax = 0;
    float s = 1;
    bool converged = false;
    
    // get an initial l and lastl
    const ColumnVector *y = (ColumnVector*)Matrix::multiply(this, x);
    float l = y->normInf();
    float lastl = l + 100;
    
    // loop until converged
    for (k = 1; k < kmax && !converged; k++) {
        y = (ColumnVector*)Matrix::multiply(this, x);
        l = y->normInf();
        if (abs(l - lastl) < tol)
            converged = true;
        lastl = l;
        imax = y->maxInd();
        
        // sign of eigenalue
        s = (y->get(imax) * x->get(imax)) < 0 ? -1 : 1;
        x = (ColumnVector*)Matrix::multiply(1.0 / l, y);
    }
    if (k == kmax)
        cout << "No convergence\n";
    float val = s * l;
    return val;
}

void Matrix::swapRows(int r1, int r2) {
    for (int i = 0; i < nbCols; i++){
        float temp;
        temp = a[r2][i];
        a[r2][i] = a[r1][i];
        a[r1][i] = temp;
    }
}

void Matrix::setMatrix(float ** a) {
    this->a = a;
}

void Matrix::transpose() {
    float ** b = new float*[nbCols];
    for (int i = 0; i < nbCols; i++)
        b[i] = new float[nbRows];
    
    for (int i = 0; i < nbRows; i++) {
        for (int j = 0; j < nbCols; j++) {
            b[j][i] = a[i][j];
        }
    }
    int temp = nbRows;
    nbRows = nbCols;
    nbCols = temp;
    
    a = b;
    
}

char* Matrix::toString(const char* theBeginArrayStr,
                       const char* theEndArrayStr,
                       const char* theElmtSepStr,
                       bool theEolAtEor) const {
    char *str = (char *)malloc(9*nbRows*nbCols*sizeof(char)+
                               nbCols*nbRows*sizeof(theElmtSepStr)+
                               (nbCols+1)*sizeof(theBeginArrayStr)+
                               (nbCols+1)*sizeof(theEndArrayStr));
    strcpy(str, theBeginArrayStr);
    char current[50] = "initialize";
    for (int i = 0; i < nbRows; i++) {
        strcat(str, theBeginArrayStr);
        for (int j = 0; j < nbCols; j++) {
            sprintf(current, "%f", a[i][j]);
            strcat(str, current);
            if(j < nbCols-1)
                strcat(str, theElmtSepStr);
        }
        strcat(str, theEndArrayStr);
        if (i < nbRows-1) {
            if (theEolAtEor)
                strcat(str, "\n");
            else
                strcat(str, theElmtSepStr);
        }
        
    }
    strcat(str, theEndArrayStr);
    
    return str;
}