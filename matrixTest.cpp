#include <iostream>
#include <fstream>
#include <unistd.h>
#include <tgmath.h>
#include "Matrix.h"
#include "ColumnVector.h"
#include "LinearSolver.h"
#include "LinearSolver_LU.h"
#include "PolyFunction1D.h"
#include "MatrixGenerator.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_sle;

/**
 * Defines a trigonometric function
 */
class Trig : public PolyFunction1D {
private:
    /**
     * The number of points is 2 * n + 1
     */
    int n;
    
    /**
     * The upper bound of x coordinates
     */
    float L;
    
public:
    /**
     * Constructor initializes variables
     *
     * @param n
     *          The number of points is 2 * n + 1
     *
     * @param L
     *          The upper bound of x coordinates
     *
     * @param coeffs
     *          The coefficients of the form [K, a[1..k], b[1..k]]
     */
    Trig(int n, float L, float *coeffs) : PolyFunction1D(2 * n + 1, coeffs) {
        this->n = n;
        this->L = L;
    }
    
    float func(float x) const {
        float y = coefficients[0];
        for (int k = 1; k <= n; k++) {
            y += coefficients[k] * cos(2 * k * M_PI * x / L);
            y += coefficients[n + k] * sin(2 * k * M_PI * x / L);
        }
        return y;
    }
    
    bool isExactDerivativeDefined() {
        return false;
    }
};

/**
 * Defines a Lagrange polynomial interpolation between given points
 */
class Lagrange : public Function1D {
private:
    /**
     * Number of points to interpolate between
     */
    int nbPoints;
    
    /**
     * X coordinate of each point
     */
    ColumnVector *xs;
    
    /**
     * Y coordinate of each point
     */
    ColumnVector *ys;
    
public:
    /**
     * Constructor initializes variables
     *
     * @param nbPoints
     *          Number of points to interpolate between
     *
     * @param xs
     *          X coordinate of each point
     *
     * @param ys
     *          Y coordinate of each point
     */
    Lagrange(int nbPoints, ColumnVector *xs, ColumnVector *ys) {
        this->nbPoints = nbPoints;
        this->xs = xs;
        this->ys = ys;
    }
    
    ~Lagrange(void) {
        nbPoints = 0;
    }
    
    float func(float x) const {
        float y = 0;
        
        for (int i = 0; i < nbPoints; i++) {
            float l = 1;
            for (int j = 0; j < nbPoints; j++) {
                if (j != i) {
                    l *= ((x - xs->get(j)) / (xs->get(i) - xs->get(j)));
                }
            }
            y += l * ys->get(i);
        }
        return y;
    }
    
    bool isExactDerivativeDefined() {
        return false;
    }
};

int main() {
    
    /********************************************
     *              DECLARATIONS                *
     ********************************************/
    const int n = 4;
    
    // Upper bound of the points (lower is zero
    const float L = 10;
    
    // Number of points
    const int len = 2 * n + 1;
    
    // Seed the random matrix generator
    MatrixGenerator::seed();
    
    // X values to interpolate between
    ColumnVector *xvals = MatrixGenerator::getUniformSample(n, L);
    
    Matrix *mat;
    ColumnVector *cols[5];
    for (int i = 0; i < 5; i++) {
        mat = MatrixGenerator::getRandom(4,4);
        cols[i] = Matrix::column(mat);
    }
    Matrix *A = cols[0];
    for (int i = 1; i < 5; i++) {
        A->addColumn(cols[i]);
    }
    
    // Matrix to use for polynomial interpolation
    Matrix *a1 = MatrixGenerator::getPolynomial(len,xvals);
    
    // Matrix to use for trigonometric interpolation
    Matrix *a2 = MatrixGenerator::getTrigonometric(n,L);
    
    // Vector of y values
    ColumnVector *b = MatrixGenerator::getRandomColumn(len);
    
    
    /********************************************
     *              COMPUTATION                 *
     ********************************************/
    
    // Solve
    LinearSolver_LU *solver = new LinearSolver_LU();
    const ColumnVector *x1 = (ColumnVector*)solver->solve(a1, b)->getSolution();
    const ColumnVector *x2 = (ColumnVector*)solver->solve(a2, b)->getSolution();
    
    // Get the coefficients
    float *coeffs1 = new float[len];
    float *coeffs2 = new float[len];
    for (int i = 0; i < len; i++) {
        coeffs1[i] = x1->get(i);
        coeffs2[i] = x2->get(i);
    }
    
    // Polynomial interpolation
    Function1D *f1 = new PolyFunction1D(len, coeffs1);
    
    // Trigonometric interpolation
    Function1D *f2 = new Trig(n, L, coeffs2);
    
    // Lagrange interpolation
    Function1D *f3 = new Lagrange(len, xvals, b);
    
    // Bounds of the function
    float min = xvals->get(0);
    float max = xvals->get(len-1);
    
    // Give some buffer on the ends
    float dist = max - min;
    float buffer = dist / 6.0f;
    min -= buffer;
    max += buffer;
    
    /********************************************
     *          COMMAND LINE OUTPUT             *
     ********************************************/
    
    cout << "A: \n" << A->toString("",""," ",true) << "\n\n";
    cout << "ave col: \n" << A->averageColumn()->toString("",""," ",true) << "\n\n";
    cout << "mat: \n" << mat->toString("",""," ",true) << "\n\n";
    
    const Matrix *mattmat = Matrix::multiply(Matrix::transpose(mat), mat);
    cout << "mattmat: \n" << mattmat->toString("",""," ",true) << "\n\n";
    const ColumnVector* init = MatrixGenerator::getRandomColumn(4);
    cout << "eigenvalue: \n" << mattmat->eigenvalue(init, 10000,
                                                    0.1) << "\n\n";
    cout << "eigenvector: \n" << mattmat->eigenvector(init, 10000,
                                                  0.1)->toString("{","}",",",false) << "\n\n";
    
    cout << "Eigensystem[";
    cout << mattmat->toString("{","}",",",false) << "]//N\n";
    
    /*
    cout << "A: \n" << a1->toString("",""," ",true) << "\n\n";
    cout << "b: \n" << b->toString("",""," ",true) << "\n\n";
    cout << "SOLUTION: \n" << x1->toString("",""," ",true) << "\n\n";
    
    cout << "LinearSolve[";
    cout << a1->toString("{","}",",",false) << ",{";
    cout << b->toString("","",",",false) << "}]";
    
    cout << "\nf[x_]:={";
    cout << x1->toString ("", "", ",", false) << "}.{{1}";
    for (int i = 1; i < x1 -> rows (); i++) {
        cout << ",{x^" << i << "}";
    }
    cout << "};\nPlot[f[x],{x," << min << "," << max << "}]\n";
    */
    /********************************************
     *              WRITE TO FILE               *
     ********************************************/
    /*
    ofstream myfile;
    myfile.open ("output/output.txt");
    
    myfile << len << '\n';
    
    for (int i = 0; i < len; i++) {
        myfile << xvals->get(i) << ' ' << b->get(i) << '\n';
    }
    
    // Display a little more than the lefmost and rightmost values
    int numpts = (int)((max - min) / 0.001);
    
    // Write number of points
    myfile << numpts << '\n';
    float xval = min;
    
    // Write each coordinate
    for (int i = 0; i < numpts; i++) {
        myfile << xval << ' ' << f1->func(xval) << '\n';
        xval+=0.001;
    }
    
    // Write number of points
    myfile << numpts << '\n';
    xval = min;
    
    // Write each coordinate
    for (int i = 0; i < numpts; i++) {
        myfile << xval << ' ' << f2->func(xval) << '\n';
        xval+=0.001;
    }
    
    // Write number of points
    myfile << numpts << '\n';
    xval = min;
    
    // Write each coordinate
    for (int i = 0; i < numpts; i++) {
        myfile << xval << ' ' << f3->func(xval) << '\n';
        xval+=0.001;
    }
    
    // Close file
    myfile.close();
    */
}