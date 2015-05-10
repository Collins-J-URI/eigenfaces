/*  NAME:
        SLE_Solvers_main.cpp

    DESCRIPTION:
        demonstration application for solution of Systems of Linear Equations

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
#include <string>
#include <cmath>

#include "Matrix.h"
#include "ColumnVector.h"
#include "LinearSolver_LU.h"
#include "LinearSolver_Jacobi.h"
#include "LinearSolver_GaussSeidel.h"
#include "LinearSolver_QR.h"
#include "LinearSolver_SVD.h"

using namespace std;
using namespace uriVL;


int main(int argc, char** argv)
{
    try {
			//-----------------------------------------------------
			//	LU Solver
			//-----------------------------------------------------
            {
				cout << "LU solver" << endl << "--------------" << endl;
				double **a = new double*[6];
				double  a0[] = {2, -3, 5, 1, 8, 2}, 
						a1[] = {-4, 7, -14, 0, 4, 3}, 
						a2[] = {6, -10, 15, 2, 1, 1},
						a3[] = {2, 1, 1, 4, 0, 4},
						a4[] = {1, -7, 4, 0, 4, 2},
						a5[] = {12, -1, -1, 4, 0, 4};
				a[0]=a0; a[1]=a1; a[2]=a2; a[3]=a3; a[4]=a4; a[5]=a5;
				Matrix* A = Matrix::copyMatrix(6, 6, a);

				double **b = new double*[6];
				double  b0[] = {22, -22, 36, -17, 18}, 
						b1[] = {-18, 60, -40, 83, 14}, 
						b2[] = {40, -81, 64, -79, -4}, 
						b3[] = {7, -3, 41, 34, 43},
						b4[] = {28, -30, -1, -48, -30},
						b5[] = {23, -113, 103, 76, 47};
				b[0] = b0; b[1] = b1; b[2] = b2; b[3] = b3; b[4]=b4; b[5]=b5;
				Matrix* B = Matrix::copyMatrix(6, 6, b);
						
				LinearSolver_LU* solverLU = new LinearSolver_LU(A, true);            
				LinearSolverRecord *recordLU = solverLU->solve(B, true);
				
				double const*const*const x = recordLU->sol->getMatrix();
				for (int i=0; i<6; i++)
				{
					for (int j=0; j<5; j++)
					{
						cout << "\t" << x[i][j];
					}
					cout << endl;
				}
				cout << endl;
				delete recordLU;
				delete solverLU;
							
				recordLU = LinearSolver_LU::solve(A, B);
				
				double const*const*const y = recordLU->sol->getMatrix();
				for (int i=0; i<6; i++)
				{
					for (int j=0; j<5; j++)
					{
						cout << "\t" << y[i][j];
					}
					cout << endl;
				}
				cout << endl;
				delete recordLU;
			}
			
			//-----------------------------------------------------
			//	Jacobi Solver
			//-----------------------------------------------------
            {
				cout << "Jacobi solver" << endl << "--------------" << endl;
				double **a = new double*[4];
//v1 converges
				double  a0[] = {10, -3, 5, 1}, 
//v2 diverges
//				double  a0[] = {1, -3, 5, 10}, 
						a1[] = {-4, -14, 7, 0}, 
						a2[] = {6, -1, 15, 2},
						a3[] = {2, 1, 1, 7};
				a[0]=a0; a[1]=a1; a[2]=a2; a[3]=a3;
				Matrix* A = Matrix::copyMatrix(4, 4, a);

				double **b = new double*[4];
				double  b0[] = {10, -3, 5, 1, 13, 23}, 
						b1[] = {-4, -14, 7, 0, -11, -11}, 
						b2[] = {6, -1, 15, 2, 22, 57}, 
						b3[] = {2, 1, 1, 7, 11, 35};
				b[0] = b0; b[1] = b1; b[2] = b2; b[3] = b3;
				Matrix* B = Matrix::copyMatrix(4, 6, b);
						
				LinearSolver_Jacobi* solverJacobi = new LinearSolver_Jacobi(A, true);
				LinearSolverRecord *recordJacobi = solverJacobi->solve(B, true);
				
				if (recordJacobi->status == LINEAR_SOLVER_SUCCEDED)
				{
					cout << "iteration succeded" << endl;
					cout << "x = ";
					double const*const*const x = recordJacobi->sol->getMatrix();
					for (int i=0; i<4; i++)
					{
						for (int j=0; j<6; j++)
						{
							cout << "\t" << x[i][j];
						}
						cout << endl;
					}
					cout << endl;
				}
				else 
					cout << "iteration failed" << endl;
				
				delete recordJacobi;
				delete solverJacobi;
				

				recordJacobi = LinearSolver_Jacobi::solve(A, B);
				
				if (recordJacobi->status == LINEAR_SOLVER_SUCCEDED)
				{
					cout << "iteration succeded" << endl;
					cout << "x = ";
					double const*const*const x = recordJacobi->sol->getMatrix();
					for (int i=0; i<4; i++)
					{
						for (int j=0; j<6; j++)
						{
							cout << "\t" << x[i][j];
						}
						cout << endl;
					}
					cout << endl;
				}
				else 
					cout << "iteration failed" << endl;
				
				delete recordJacobi;
			}

			//-----------------------------------------------------
			//	Gauss-Seidel Solver
			//-----------------------------------------------------
            {
				cout << "Gauss-Seidel solver" << endl << "--------------" << endl;
				double **a = new double*[4];
//v1 converges
				double  a0[] = {10, -3, 5, 1}, 
//v2 diverges
//				double  a0[] = {1, -3, 5, 10}, 
						a1[] = {-4, -14, 7, 0}, 
						a2[] = {6, -1, 15, 2},
						a3[] = {2, 1, 1, 7};
				a[0]=a0; a[1]=a1; a[2]=a2; a[3]=a3;
				Matrix* A = Matrix::copyMatrix(4, 4, a);

				double **b = new double*[4];
				double  b0[] = {10, -3, 5, 1, 13, 23}, 
						b1[] = {-4, -14, 7, 0, -11, -11}, 
						b2[] = {6, -1, 15, 2, 22, 57}, 
						b3[] = {2, 1, 1, 7, 11, 35};
				b[0] = b0; b[1] = b1; b[2] = b2; b[3] = b3;
				Matrix* B = Matrix::copyMatrix(4, 6, b);
						
				LinearSolver_GaussSeidel* solverGaussSeidel = new LinearSolver_GaussSeidel(A, true);
				LinearSolverRecord *recordGaussSeidel = solverGaussSeidel->solve(B, true);
				
				if (recordGaussSeidel->status == LINEAR_SOLVER_SUCCEDED)
				{
					cout << "iteration succeded" << endl;
					cout << "x = ";
					double const*const*const x = recordGaussSeidel->sol->getMatrix();
					for (int i=0; i<4; i++)
					{
						for (int j=0; j<6; j++)
						{
							cout << "\t" << x[i][j];
						}
						cout << endl;
					}
					cout << endl;
				}
				else 
					cout << "iteration failed" << endl;
				
				delete recordGaussSeidel;
				delete solverGaussSeidel;
				

				recordGaussSeidel = LinearSolver_GaussSeidel::solve(A, B);
				
				if (recordGaussSeidel->status == LINEAR_SOLVER_SUCCEDED)
				{
					cout << "iteration succeded" << endl;
					cout << "x = ";
					double const*const*const x = recordGaussSeidel->sol->getMatrix();
					for (int i=0; i<4; i++)
					{
						for (int j=0; j<6; j++)
						{
							cout << "\t" << x[i][j];
						}
						cout << endl;
					}
					cout << endl;
				}
				else 
					cout << "iteration failed" << endl;
				
				delete recordGaussSeidel;
			}

			

			//-----------------------------------------------------
			//	QR Solver
			//-----------------------------------------------------
            {
				cout << "QR solver" << endl << "--------------" << endl;
				double **r = new double*[6];
				double  r0[] = {1, 0, 0},
						r1[] = {0, 1, 0},
						r2[] = {0, 0, 1},
						r3[] = {-1, 1, 0},
						r4[] = {-1, 0, 1},
						r5[] = {0, -1, 1};
				r[0] = r0; r[1] = r1; r[2] = r2; r[3] = r3, r[4] = r4, r[5] = r5;
				Matrix *R = Matrix::copyMatrix(6, 3, r);
				LinearSolver_QR *solverQR = new LinearSolver_QR(R, true);
				
				double rb[] = {10, 5, -7, -5, -17, -12};
				Matrix *RB = new ColumnVector(6, rb);
				LinearSolverRecord *recordQR = new LinearSolverRecord();
				solverQR->solve(RB, recordQR, 0);

				double const*const*const xR = recordQR->sol->getMatrix();
				for (int i=0; i<3; i++)
				{
						cout << "\t" << xR[i][0] << endl;
				}
				cout << endl << "--------------" << endl;

				rb[0] = 10.2; rb[1] = 4.9;
				Matrix *RB2 = new ColumnVector(6, rb);
				solverQR->solve(RB2, recordQR, 0);
				double const*const*const xR2 = recordQR->sol->getMatrix();
				for (int i=0; i<3; i++)
				{
						cout << "\t" << xR2[i][0] << endl;
				}
				cout << endl;

				delete RB2;
				delete RB;
				delete solverQR;
				delete recordQR;
			}
			

			//-----------------------------------------------------
			//	SVD Solver
			//-----------------------------------------------------
			{
				cout << "SVD solver" << endl << "--------------" << endl;
				double **a = new double*[4];
				double  a0[] = {2, -3, 5, 1}, 
						a1[] = {-4, 7, -14, 0}, 
						a2[] = {6, -10, 15, 2},
						a3[] = {2, 1, 1, 4};
				a[0]=a0; a[1]=a1; a[2]=a2; a[3]=a3;
				Matrix* A = Matrix::copyMatrix(4, 4, a);

				double** b = new double*[4];
				double  b0[] = {2, -3, 5, 1, 5, 15}, 
						b1[] = {-4, 7, -14, 0, -11, -32}, 
						b2[] = {6, -10, 15, 2, 13, 39}, 
						b3[] = {2, 1, 1, 4, 8, 23};
				b[0] = b0; b[1] = b1; b[2] = b2; b[3] = b3;
				Matrix* B = Matrix::copyMatrix(4, 6, b);

				LinearSolver_SVD* solverSVD = new LinearSolver_SVD(A, true);
				LinearSolverRecord* recordSVD = solverSVD->solve(B, true);
				
				double const*const*const x = recordSVD->sol->getMatrix();
				for (int i=0; i<4; i++)
				{
					for (int j=0; j<6; j++)
					{
						cout << "\t" << x[i][j];
					}
					cout << endl;
				}
				cout << endl;
				delete recordSVD;
				delete solverSVD;
							
				recordSVD = LinearSolver_SVD::solve(A, B);
				
				double const*const*const y = recordSVD->sol->getMatrix();
				for (int i=0; i<4; i++)
				{
					for (int j=0; j<6; j++)
					{
						cout << "\t" << y[i][j];
					}
					cout << endl;
				}
				cout << endl;
				delete recordSVD;
				
				double** s = new double*[6];
				double  s0[] = {1, 0, 0},
						s1[] = {0, 1, 0},
						s2[] = {0, 0, 1},
						s3[] = {-1, 1, 0},
						s4[] = {-1, 0, 1},
						s5[] = {0, -1, 1};
				s[0] = s0; s[1] = s1; s[2] = s2; s[3] = s3, s[4] = s4, s[5] = s5;
				Matrix* S = Matrix::copyMatrix(6, 3, s);
				solverSVD = new LinearSolver_SVD(S, true);
				
				double sb[] = {10, 5, -7, -5, -17, -12};
				Matrix* SB = new ColumnVector(6, sb);
				recordSVD = new LinearSolverRecord();
				solverSVD->solve(SB, recordSVD, 0);

				double const*const*const x2 = recordSVD->sol->getMatrix();
				for (int i=0; i<3; i++)
				{
						cout << "\t" << x2[i][0] << endl;
				}
				cout << endl << "--------------" << endl;

				sb[0] = 10.2; sb[1] = 4.9;
				Matrix* SB2 = new ColumnVector(6, sb);
				solverSVD->solve(SB2, recordSVD, 0);
				double const*const*const x3 = recordSVD->sol->getMatrix();
				for (int i=0; i<3; i++)
				{
						cout << "\t" << x3[i][0] << endl;
				}
				cout << endl;
				
				delete []s;
				delete SB;
				delete SB2;
//				delete solverSVD;
				delete recordSVD;
			}	
			
            return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
	
}
