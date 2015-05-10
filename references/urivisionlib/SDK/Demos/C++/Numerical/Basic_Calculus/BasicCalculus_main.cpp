/*  NAME:
        BasicCalculus_main.cpp

    DESCRIPTION:
        demonstration application for basic calculus computations

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
//
#include "PolynomialFunction1D.h"
#include "PolynomialFunction1D_F.h"
//
#include "MyFunction1D.h"
#include "OtherFunction1D.h"
#include "YetAnotherFunction1D.h"

using namespace std;
using namespace uriVL;


int main(int argc, char** argv)
{
    try
	{
		//--------------------------------------------------------------------------
		//	Part 1: Use a custom Function1D subclass
		//--------------------------------------------------------------------------
		{
			cout << endl << "Part 1: Use a custom Function1D subclass that implements" << 
					" its first and second derivatives." << endl <<
					"--------------------------------------------------------------" << endl;
				
			MyFunction1D f;
			double x = 1.5;
			cout << "p(" << x << ") = " << f.func(x) << endl;
			//	first and second derivatives of the MyFunction1D class override
			//	that of the Function1D class
			cout << "derivative functions defined in Function1D class, implemented in MyFunction1D" << endl;
			cout << "p'(" << x << ") = " << f.dfunc(x) << endl;
			cout << "p''(" << x << ") = " << f.d2func(x) << endl;
		}

		//--------------------------------------------------------------------------
		//	Part 2: Use another custom Function1D subclass that does not implement
		//			its second derivative
		//--------------------------------------------------------------------------
		{
			cout << endl << "Part 2: Use a custom Function1D subclass that does not" << 
					" implement its second derivatives." << endl <<
					"--------------------------------------------------------------" << endl;
			Function1D* g = new OtherFunction1D();
			double x = 1.5;
			cout << "p(" << x << ") = " << g->func(x) << endl;
			//	first derivative of the OtherFunction1D class overrides
			//	that of the Function1D class
			cout << "derivative functions defined in Function1D class, implemented in OtherFunction1D" << endl;
			cout << "p'(" << x << ") = " << g->dfunc(x) << endl;
			//	second derivative approximation of the Function1D class
			cout << "second derivative approximation implemented in Function1D" << endl;
			cout << "p''(" << x << ") = " << g->d2func(x) << endl;
			
			delete g;
		}

		//--------------------------------------------------------------------------
		//	Part h: Use another custom Function1D subclass that does not implement
		//			its derivatives
		//--------------------------------------------------------------------------
		{
			cout << endl << "Part 2: Use a custom Function1D subclass that does not" << 
					" implement its first and second derivatives." << endl <<
					"--------------------------------------------------------------" << endl;
			YetAnotherFunction1D h;
			double x = 2.0;
			cout << "p(" << x << ") = " << h.func(x) << endl;
			//	first and second derivative approximations of the Function1D class
			cout << "first and second derivative approximations implemented in Function1D" << endl;
			cout << "p'(" << x << ") = " << h.dfunc(x) << endl;
			cout << "p''(" << x << ") = " << h.d2func(x) << endl;
		}

		//--------------------------------------------------------------------------
		//	Part 4: Use the PolynomialFunction1D class to implement the
		//			same function as h above
		//--------------------------------------------------------------------------
        
		{
			cout << endl << "Part 4: Use the PolynomialFunction1D class" << endl <<
					"--------------------------------------------------------------" << endl;
			double a[5] = {5, 4, 3, 2, 1};  //   x^4 + 2 x^3 + 3 x^2 + 4 x + 5
			
			PolynomialFunction1D* poly = new PolynomialFunction1D(4, a);
			Function1D* p = poly;
			
			//	use x = 0 to verify that I get back the coefficients as expected
			double x = 0.0;
			cout << "p(" << x << ") = " << p->func(x) << endl;
			//	first and second derivatives of the PolynomialFunction1D class overrides
			//	that of the Function1D class
			cout << "derivative functions defined in Function1D class, implemented in PolynomialFunction1D" << endl;
			cout << "p'(" << x << ") = " << p->dfunc(x) << endl;
			cout << "p''(" << x << ") = " << p->d2func(x) << endl;
			//	Higher derivatives are only defined for the PolynomialFunction1D class
			cout << "derivative functions specific to PolynomialFunction1D class" << endl;
			cout << "dnfunc(x, 1):  p'(" << x << ") = " << poly->dnfunc(x, 1) << endl;
			cout << "dnfunc(x, 2):  p''(" << x << ") = " << poly->dnfunc(x, 2) << endl;
			cout << "dnfunc(x, 3):  p'''(" << x << ") = " << poly->dnfunc(x, 3) << endl;
			cout << "dnfunc(x, 4):  p^(4)(" << x << ") = " << poly->dnfunc(x, 4) << endl;
			cout << "dnfunc(x, 5):  p^(5)(" << x << ") = " << poly->dnfunc(x, 5) << endl << endl;

			x = 2.0;
			cout << "p(" << x << ") = " << p->func(x) << endl;
			//	first and second derivatives of the PolynomialFunction1D class overrides
			//	that of the Function1D class
			cout << "derivative functions defined in Function1D class, implemented in PolynomialFunction1D" << endl;
			cout << "p'(" << x << ") = " << p->dfunc(x) << endl;
			cout << "p''(" << x << ") = " << p->d2func(x) << endl;
			//	Higher derivatives are only defined for the PolynomialFunction1D class
			cout << "derivative functions specific to PolynomialFunction1D class" << endl;
			cout << "dnfunc(x, 1):  p'(" << x << ") = " << poly->dnfunc(x, 1) << endl;
			cout << "dnfunc(x, 2):  p''(" << x << ") = " << poly->dnfunc(x, 2) << endl;
			cout << "dnfunc(x, 3):  p'''(" << x << ") = " << poly->dnfunc(x, 3) << endl;
			cout << "dnfunc(x, 4):  p^(4)(" << x << ") = " << poly->dnfunc(x, 4) << endl;
			cout << "dnfunc(x, 5):  p^(5)(" << x << ") = " << poly->dnfunc(x, 5) << endl << endl;
			
			delete poly;
		}

		//--------------------------------------------------------------------------
		//	Part 5: Everything above could be reproduced with "float" versions
		//			of the classes
		//--------------------------------------------------------------------------
        
		{
			cout << endl << "Part 5: Use the PolynomialFunction1D_F class" << endl <<
					"--------------------------------------------------------------" << endl;
			float a[5] = {5, 4, 3, 2, 1};  //   x^4 + 2 x^3 + 3 x^2 + 4 x + 5
			
			PolynomialFunction1D_F* poly = new PolynomialFunction1D_F(4, a);
			Function1D_F* p = poly;
			
			//	use x = 0 to verify that I get back the coefficients as expected
			float x = 0.f;
			cout << "p(" << x << ") = " << p->func(x) << endl;
			//	first and second derivatives of the PolynomialFunction1D class overrides
			//	that of the Function1D class
			cout << "derivative functions defined in Function1D_F class, implemented in PolynomialFunction1D_F" << endl;
			cout << "p'(" << x << ") = " << p->dfunc(x) << endl;
			cout << "p''(" << x << ") = " << p->d2func(x) << endl;
			//	Higher derivatives are only defined for the PolynomialFunction1D class
			cout << "derivative functions specific to PolynomialFunction1D_F class" << endl;
			cout << "dnfunc(x, 1):  p'(" << x << ") = " << poly->dnfunc(x, 1) << endl;
			cout << "dnfunc(x, 2):  p''(" << x << ") = " << poly->dnfunc(x, 2) << endl;
			cout << "dnfunc(x, 3):  p'''(" << x << ") = " << poly->dnfunc(x, 3) << endl;
			cout << "dnfunc(x, 4):  p^(4)(" << x << ") = " << poly->dnfunc(x, 4) << endl;
			cout << "dnfunc(x, 5):  p^(5)(" << x << ") = " << poly->dnfunc(x, 5) << endl << endl;

			x = 2.f;
			cout << "p(" << x << ") = " << p->func(x) << endl;
			//	first and second derivatives of the PolynomialFunction1D class overrides
			//	that of the Function1D class
			cout << "derivative functions defined in Function1D_F class, implemented in PolynomialFunction1D_F" << endl;
			cout << "p'(" << x << ") = " << p->dfunc(x) << endl;
			cout << "p''(" << x << ") = " << p->d2func(x) << endl;
			//	Higher derivatives are only defined for the PolynomialFunction1D class
			cout << "derivative functions specific to PolynomialFunction1D_F class" << endl;
			cout << "dnfunc(x, 1):  p'(" << x << ") = " << poly->dnfunc(x, 1) << endl;
			cout << "dnfunc(x, 2):  p''(" << x << ") = " << poly->dnfunc(x, 2) << endl;
			cout << "dnfunc(x, 3):  p'''(" << x << ") = " << poly->dnfunc(x, 3) << endl;
			cout << "dnfunc(x, 4):  p^(4)(" << x << ") = " << poly->dnfunc(x, 4) << endl;
			cout << "dnfunc(x, 5):  p^(5)(" << x << ") = " << poly->dnfunc(x, 5) << endl << endl;
			
			delete poly;
		}
        return 0;
    }
    catch (uriVL::ErrorReport & e) 
	{
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
