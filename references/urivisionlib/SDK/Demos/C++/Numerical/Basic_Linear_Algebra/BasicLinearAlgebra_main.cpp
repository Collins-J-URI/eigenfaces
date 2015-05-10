/*  NAME:
        BasicLinearAlgebra_main.cpp

    DESCRIPTION:
        demonstration application for basic linear algebra computations

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
+--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "Matrix.h"
#include "ColumnVector.h"
#include "RowVector.h"

using namespace std;
using namespace uriVL;


int main(int argc, char** argv)
{
    try {


        //------------------------------------------------------------------------------
        //  Test basic creation, addition, multiplication operations on small matrices 
        //  and write result to the console
        //------------------------------------------------------------------------------
        double **a = new double*[6];
        double  a0[] = {1, 2, 3, 4, 5, 6}, 
                a1[] = {0, -1, 2, 2, -3, 0}, 
                a2[] = {1, 0, 1, 0, 6, 2},
                a3[] = {5, 2, -1, 1, -3, 1};
                a[0]=a0; a[1]=a1; a[2]=a2; a[3]=a3;
        
        Matrix *A = Matrix::copyMatrix(4, 6, a);
        Matrix *AT = A->transposition();
        Matrix B = *A * *AT;
        Matrix C = B * B;
//        Matrix D = B + (B + Matrix::identityMatrix(4)) * B - 2*B;
        string strA = A->toString("\t", "\n", "\t", false);
        string strAT = AT->toString("\t", "\n", "\t", false);
        string strB = B.toString("\t", "\n", "\t", false);
        string strC = C.toString("\t", "\n", "\t", false);
//        string strD = D.toString("\t", "\n", "\t", false);
        cout << strA << endl << endl;
        cout << strAT << endl << endl;
        cout << strB << endl << endl;
        cout << strC << endl << endl;
//        cout << strD << endl << endl;

        const int VDIM = 10;
        double v1[VDIM] = {-5, 6, 7, 0, -1, 10, 3, 10, 5, 5},
               v2[VDIM] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
        ColumnVector    *colVect1 = new ColumnVector(VDIM, v1);
        RowVector       *rowVect1 = new RowVector (VDIM, v2),
                        *rowVect2;
        rowVect2 = static_cast<RowVector*>(colVect1->transposition());
        string strColV1 = colVect1->toString("\t", "\n", "\t", false);                          
        string strRowV1 = rowVect1->toString("\t", "\n", "\t", false);                          
        string strRowV2 = rowVect2->toString("\t", "\n", "\t", false);                          
        cout << strColV1 << endl << endl;
        cout << strRowV1 << endl << endl;
        cout << strRowV2 << endl << endl;

        //------------------------------------------------------------------------------
        //  Larger-scale tests on a bunch of semi-large-ish random matrices, with
        /// output to a file to be tested by Mathematica
        //------------------------------------------------------------------------------
        ofstream mmaOut("mmaOut.nb");
        //
        //  define matrix norm in MMA
        mmaOut << "MatrixNormInf[A_] := Max[Apply[Plus, Map[Abs, Transpose[A]]]];" << endl;
        mmaOut << "MatrixNorm1[A_] := Max[Apply[Plus, Map[Abs, A]]];" << endl << endl;
        
        Matrix *A1 = Matrix::randomMatrix(3, 5, 0.L, 1.L);
        Matrix *A2 = Matrix::randomMatrix(5, 4, 0.L, 1.L);
        Matrix B1 = (*A1) * (*A2);
        mmaOut << "A1=" << A1->toString("{", "}", ",", false) << ";" << endl;
        mmaOut << "A2=" << A2->toString("{", "}", ",", false) << ";" << endl;
        mmaOut << "B1=" << B1.toString("{", "}", ",", false) << ";" << endl;
        mmaOut << "resB1=MatrixNorm1[B1-A1.A2]" << endl;
        delete A1;
        delete A2;

        A1 = Matrix::randomMatrix(30, 10, 0.L, 1.L);
        A2 = Matrix::randomMatrix(10, 20, 0.L, 1.L);
        Matrix B2 = (*A1) * (*A2);
        mmaOut << "A1=" << A1->toString("{", "}", ",", false) << ";" << endl;
        mmaOut << "A2=" << A2->toString("{", "}", ",", false) << ";" << endl;
        mmaOut << "B2=" << B2.toString("{", "}", ",", false) << ";" << endl;
        mmaOut << "resB2=MatrixNorm1[B2-A1.A2]" << endl;
        delete A1;
        delete A2;
        
        
        mmaOut << flush;
        mmaOut.close();
        
        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
