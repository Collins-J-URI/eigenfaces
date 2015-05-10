/*  NAME:
        Matrix.h
 
    DESCRIPTION:
        Matrix public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MATRIX_H
#define URIVL_MATRIX_H

#include <string>
#include <cmath>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

using std::string;

namespace uriVL
{    
    /** Struct to hold all the TriState indicators of a matrix to create
     */
    typedef struct MatrixIndicators {
                                        TriState  symmetryStatus;
                                        TriState  antisymmetryStatus;
                                        TriState  diagonalityStatus;
                                        TriState  identityStatus;
                                        TriState  orthogonalityStatus;
                                        TriState  upperTriangularityStatus;
                                        TriState  lowerTriangularityStatus;
                                        TriState  fullRankStatus;
										//
                                        TriState  normalized1Status;
                                        TriState  normalized2Status;
                                        TriState  normalizedInfStatus;
										                                        
    } MatrixIndicators;
    
     
    /**
     * Implements the Matrix class of Linear Algebra matrices.
     * <b>Note:</b> Being somewhat inconsistant, I added flags to store information about the
     * matrix such as whether it is symmetric, orthogonal, upper-triangular, etc.  On the one
     * hand, it might be nice to keep track of that because it allows to be "smarter" when we
     * need to solve an SLE (e.g. it's good to know if the matrix is orthogonal or triangular)
     * or compute the determinant (e.g. if the matrix is triangular).  On the other hand, it
     * is a bit of a hassle to keep track of this, and this could be claimed to be the user's
     * job to do so.  At this point, the flags are declared but not accessible, and their
     * computation/update is not fully implemented.
     *
     *  @author     Joshua Sawyer and jean-yves herve',
     *                      3D Group for Interactive Visualization,
     *                      Department of Computer Science and Statistics,
     *                      University of Rhode Island
     */
    class URIVL_EXPORT Matrix
    {
        friend class ColumnVector;
        friend class RowVector;
        friend class HomogeneousMatrix;

//        /**
//         * This operator creates a new Matrix equal to the sum of the two terms.  Throws an
//         * exception if the matrices' dimensions do not match.
//         *
//         * @param a  left term in the addition
//         * @param b  right term in the addition
//         * @return a reference to <code>a</code> + <code>b</code>
//         */
//        friend const Matrix operator +(const Matrix& a, const Matrix& b);
//
//
//        /**
//         * This operator creates a new Matrix equal to the difference of the two terms.  Throws an
//         * exception if the matrices' dimensions do not match.
//         *
//         * @param a  left term in the difference
//         * @param b  right term in the difference
//         * @return a reference to <code>a</code> - <code>b</code>
//         */
//        friend const Matrix operator -(const Matrix& a, const Matrix& b);
//

		/**
         * This operator creates a new Matrix equal to the product of the two terms.  Throws an
         * exception if the matrices' dimensions do not match (number of columns of the left term
         * should be equal to the number of rows of the right term)
         *
         * @param a  left term in the product
         * @param b  right term in the product
         * @return a reference to <code>a</code> * <code>b</code>
         */
        friend const Matrix operator *(const Matrix& a, const Matrix& b);


       /**
         * This operator creates a new Matrix equal to the multiplication of a matrix by
         *  a scalar term
         *
         * @param a  matrix to apply the scalar multiplication to
         * @param s  scalar term to apply to the matrix
         * @return a reference to <code>s</code> * <code>a</code>
         */
        friend const Matrix operator *(const double s, const Matrix& a);

        /**
         * This operator creates a new Matrix equal to the multiplication of a matrix by
         *  a scalar term
         *
         * @param a  matrix to apply the scalar multiplication to
         * @param s  scalar term to apply to the matrix
         * @return a reference to <code>s</code> * <code>a</code>
         */
        friend const Matrix operator *(const Matrix& a, const double s);



        public:

            /** Creates a nbRows x nbCols matrix whose elements have not been initialized.  This is 
             *  the fastest way to get a new matrix if you are going to assign values directly by 
             *  yourself.
             *
             * @param nbRows the number of rows in the matrix
             * @param nbCols the number of columns in the matrix
             */
            Matrix(int nbRows, int nbCols);

            /** "v-valued" constructor of Matrix class.  Creates a nbRows x nbCols matrix
             *  where all elements are v.
             *
             *  @param nbRows the number of rows in the matrix
             *  @param nbCols the number of columns in the matrix
             *  @param v the value with which to fill the matrix
             */
            Matrix(int nbRows, int nbCols, double v);

            /** Copy constructor
             *
             *  @param b a pointer to the Matrix to copy
             */
            Matrix(const Matrix* b);

            /** Copy constructor
             *
             *  @param b a reference to the Matrix to copy
             */
            Matrix(const Matrix& b);

            /** "array" constructor of Matrix class.  Creates a nbRows x nbCols matrix
             * where the elements are the elements of the 2D array referenced by m.  This function
             *  does not allocate a new area; instead it uses m as its own data array.
             *
             * @param nbRows the number of rows in the matrix
             * @param nbCols the number of columns in the matrix
             * @param m a pointer to a 2D array
			 * @param	safe	when true, a copy of the matrix array m will be created and stored
             * @param matrixIndic  struct describing all that is known about the new matrix
             */
            Matrix(int nbRows, int nbCols, double** m, bool safe=true, 
					const MatrixIndicators* matrixIndic=NULL);


            /**	Destructor
             */
            virtual ~Matrix(void);


            /** Creates a new nbRows x nbCols matrix and copies into it the elements of the 
             *  2D array referenced by m.
             *
             *  @param nbRows the number of rows in the matrix
             *  @param nbCols the number of columns in the matrix
             *  @param m a pointer to a 2D array
             */
            static Matrix* copyMatrix(int nbRows, int nbCols, const double*const* m);


            /** Produces a new diagonal matrix at the dimensions specified and initializes
             *  all diagonal elements to zero
             *
             *  @param  nbRows   number of rows of the new matrix
             *  @param  nbCols   number of columnss of the new matrix
             *  @return a new nbRows x nbCols null matrix
             */
            static Matrix* nullMatrix(int nbRows, int nbCols);
            
            /** Produces a new diagonal matrix at the dimensions specified and initializes
             *  all diagonal elements with the value specified.
             *
             *  @param  nbRows   number of rows of the new matrix
             *  @param  nbCols   number of columnss of the new matrix
             *  @param  v        value to assign to all diagonal elements
             *  @return a new nbRows x nbCols diagonal matrix
             */
            static Matrix* diagonalMatrix(int nbRows, int nbCols, double v);
            
            /** Produces a new diagonal matrix at the dimensions specified and initializes
             *  all diagonal elements with the value specified.
             *
             *  @param  nbRows   number of rows of the new matrix
             *  @param  nbCols   number of columnss of the new matrix
             *  @param  v        value to assign to all diagonal elements
             *  @return a new nbRows x nbCols diagonal matrix
             */
            static Matrix* diagonalMatrix(int nbRows, int nbCols, const double* v);
            

            /** Produces a new identity matrix at the dimensions specified.  To produce a 
             *  non-square matrix with a unit diagonal, use diagonalMatrix instead.
             *
             *  @param  n   size of the new matrix
             *  @return a new n x n identity matrix
             */
            static Matrix* identityMatrix(int n);
          
            /** produces a new matrix filled with random values between lowerBound 
             *  (inclusive) and upperBound (exclusive)
             *  @param nbRows the number of rows in the matrix
             *  @param nbCols the number of columns in the matrix
             *  @param lowerBound   lower bound for the range of random values
             *  @param upperBound   upper bound for the range of random values
             *  @return a new nbRows x nbCols random matrix
             */
            static Matrix* randomMatrix(int nbRows, int nbCols, double lowerBound, 
                                        double upperBound);

            /**
             * This function returns a int corresponding to the number of rows in <code>this</code>
             *
             * @return the number of rows in this matrix
             */
            int getNbRows(void) const;

            /**
             * This function returns a int corresponding to the number of columns in <code>this</code>
             *
             * @return the number of columns in this matrix
             */
            int getNbCols(void) const;

            /** Returns a read-only pointer to the matrix data.  If you want to have full read-write
             *  access to the data, call instead getMatrixRW.
             *  @return the matrice's private read-only 2D array of data
             */
            const double *const* getMatrix(void) const;

            /** Returns a pointer to the matrix data that allows to set new values to the matrix's 
             *  elements.
             *  As a side effect, this function sets to IS_UNKNOWN all the matrix's indicators.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return the matrix's private read-write 2D array of data
             */
            double *const* getMatrix(ReadWriteStatus rwa);


            /** Check whether the matrix is symmetric.
             *  A "real-world" matrix is never exactly symmetric unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost symmetric" status can call checkSymmetryStatus() and
			 *	then decide whether or not to "enforce" symmetry by calling enforceSymmetry().
             *  This function updates the isSymmmetric_ indicator.
			 *
             *  @see isSymmmetric_
			 *	@see checkSymmetryStatus
             *  @see enforceSymmetry
			 *
             *  @return symmetry test result as IS_TRUE or IS_FALSE
             */
            TriState isSymmetric(void);
            
            /** Check whether the matrix is antisymmetric.
             *  A "real-world" matrix is never exactly antisymmetric unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost antisymmetric" status can call checkAntisymmetryStatus() and
			 *	then decide whether or not to "enforce" symmetry by calling enforceAntisymmetry().
             *  This function updates the isAntisymmmetric_ indicator.
			 *
             *  @see isAntisymmmetric_;
			 *	@see checkAntisymmetryStatus
             *  @see enforceAntisymmetry
			 *
             *  @return antisymmetry test result as IS_TRUE or IS_FALSE
             */
            TriState isAntisymmetric(void);
            
            /** Check whether the matrix is orthogonal.
             *  A "real-world" matrix is never exactly orthogonal unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost orthogonal" status can call checkOrthogonalityStatus() and
			 *	then decide whether or not to "enforce" symmetry by calling enforceAntisymmetry().
             *  The user can decide to "enforce" orthogonality by calling enforceOthogonality().
             *  This function updates the isOrthogonal_ indicator.
			 *
             *  @see isOrthogonal_;
			 *	@see checkOrthogonalityStatus
             *  @see enforceOthogonality
			 *
             *  @return orthogonality test result as IS_TRUE or IS_FALSE
             */
            TriState isOrthogonal(void);
            
            /** Check whether the matrix is diagonal.
             *  A "real-world" matrix is never exactly diagonal unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost diagonal" status can call checkDiagonalityStatus() and
			 *	then decide whether or not to "enforce" diagonality by calling enforceDiagonality().
             *  This function updates the isDiagonal_ indicator.
			 *
             *  @see isDiagonal_;
             *  @see checkDiagonalityStatus
             *  @see enforceDiagonality
			 *
             *  @return diagonality test result as IS_TRUE or IS_FALSE
             */
            TriState isDiagonal(void);
            
            /** Check whether the matrix is an identity matrix.
             *  A "real-world" matrix is never exactly identity unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost identity" status can call checkIdentityStatus() and
			 *	then decide whether or not to "enforce" symmetry by calling enforceIdentity().
             *  This function updates the isIdentity_ indicator.
			 *
             *  @see isIdentity_;
             *  @see checkIdentityStatus
             *  @see enforceIdentity
			 *
             *  @return identity test result as IS_TRUE or IS_FALSE
             */
            TriState isIdentity(void);
            
            /** Check whether the matrix is upper-triangular.
             *  A "real-world" matrix is never exactly upper-triangular unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost upper-triangular" status can call checkUpperTriangularityStatus() and
			 *	then decide whether or not to "enforce" symmetry by calling enforceUpperTriangularity_().
             *  This function updates isUpperTriangular_ indicator.
			 *
             *  @see isUpperTriangular_;
             *  @see checkUpperTriangularityStatus
             *  @see enforceUpperTriangularity
			 *
             *  @return upper-triangularity test result as IS_TRUE or IS_FALSE
             */
            TriState isUpperTriangular(void);
            
            /** Check whether the matrix is lower-triangular.
             *  A "real-world" matrix is never exactly lower-triangular unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance.  
			 *	The user wishing a finer control of "almost lower-triangular" status can call checkLowerTriangularityStatus() and
			 *	then decide whether or not to "enforce" symmetry by calling enforceLowerTriangularity_().
             *  This function updates isLowerTriangular_ indicator.
			 *
             *  @see isLowerTriangular_;
             *  @see checkLowerTriangularityStatus
             *  @see enforceLowerTriangularity
			 *
             *  @return lower-triangularity test result as IS_TRUE or IS_FALSE
             */
            TriState isLowerTriangular(void);
            
            /** Check whether the matrix is symmetric.
             *  A "real-world" matrix is never exactly symmetric unless it was explicitly created that way.  This
             *  function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  
			 *	The user can decide to "enforce" symmetry by calling enforceSymmetry_().
             *  This function updates the isSymmmetric_ indicator.
             *  @see isSymmmetric_
             *  @see enforceSymmetry
             *  @return maximum deviation from symmetry in the matrix
             */
            double checkSymmetryStatus(void);
            
            /** Check whether the matrix is antisymmetric.
             *  A "real-world" matrix is never exactly antisymmetric unless it was explicitly created that way.  This
             *  function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" antisymmetry by calling enforceAntiSymmetry_().
             *  This function updates isAntisymmmetric_ indicator.
             *  @see isAntisymmmetric_;
             *  @see enforceAntiSymmetry
             *  @return maximum deviation from antisymmetry in the matrix
             */
            double checkAntisymmetryStatus(void);
            
            /** Check whether the matrix is orthogonal.
             *  A "real-world" matrix is never exactly orthogonal unless it was explicitly created that way.  This
             *  function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" orthogonality by calling enforceOthogonality_().
             *  This function updates isOrthogonal_ indicator.
             *  @see isOrthogonal_;
             *  @see enforceOthogonality
             *  @return maximum deviation from orthogonality in the matrix
             */
            double checkOrthogonalityStatus(void);
            
            /** Check whether the matrix is diagonal.
             *  A "real-world" matrix is never exactly diagonal unless it was explicitly created that way.  This
             *  function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" diagonality by calling enforceDiagonality_().
             *  This function updates isDiagonal_ indicator.
             *  @see isDiagonal_;
             *  @see enforceDiagonality
             *  @return maximum deviation from diagonality in the matrix
             */
            double checkDiagonalityStatus(void);
            
            /** Check whether the matrix is an identity matrix.
             *  A "real-world" matrix is never exactly identity unless it was explicitly created that way.  This
             *  function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" indentity by calling enforceIdentitity_().
             *  This function updates isUnity_ indicator.
             *  @see isDiagonal_;
             *  @see enforceIdentity_
             *  @return maximum deviation from identity in the matrix
             */
            double checkIdentityStatus(void);
            
            /** Check whether the matrix is upper-triangular.
             *  A "real-world" matrix is never exactly upper-triangular unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" upper-triangularity by calling 
             *  enforceUpperTriangularity_().
             *  This function updates isUpperTriangular_ indicator.
             *  @see isUpperTriangular_;
             *  @see enforceUpperTriangularity_
             *  @return maximum deviation from upper-triangularity in the matrix
             */
            double checkUpperTriangularityStatus(void);
            
            /** Check whether the matrix is lower-triangular.
             *  A "real-world" matrix is never exactly lower-triangular unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" lower-triangularity by calling 
             *  enforceUpperTriangularity_().
             *  This function updates isLowerTriangular_ indicator.
             *  @see isLowerTriangular_;
             *  @see enforceSymmetry_
             *  @return maximum deviation from lower-triangularity in the matrix
             */
            double checkLowerTriangularityStatus(void);
            
			
            /** Check whether the matrix is normalized according to Norm 1.
             *  A "real-world" matrix is never exactly normalized unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" normalization by calling 
             *  normalize1().
             *  This function updates isNormalized1_ indicator.
             *  @see isNormalized1_;
             *  @see normalize1
             *  @return maximum deviation from norm1 normality in the matrix
             */
            double checkNormalization1Status(void);
            
			
            /** Check whether the matrix is normalized according to Norm 2.
             *  A "real-world" matrix is never exactly normalized unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" normalization by calling 
             *  normalize2().
             *  This function updates isNormalized2_ indicator.
             *  @see isNormalized2_;
             *  @see normalize2
             *  @return maximum deviation from norm2 normality in the matrix
             */
            double checkNormalization2Status(void);

            /** Check whether the matrix is normalized according to Norm 1.
             *  A "real-world" matrix is never exactly normalized unless it was explicitly created that way.  
             *  This function checks the property within a fixed relative tolerance and returns the largest deviation
             *  encountered.  The user can decide to "enforce" normalization by calling 
             *  normalize1().
             *  This function updates isNormalizedInf_ indicator.
             *  @see isNormalizedInf_;
             *  @see normalizeInf
             *  @return maximum deviation from norm Inf normality in the matrix
             */
            double checkNormalizationInfStatus(void);
            
			
            /** Forces a matrix that passes the test for symmetry of checkIsSymmetric_ to be exactly
             *  symmetric.  Does so by replacing all element mat_[i][j] and mat_[i][j] by
             *  0.5*(mat_[i][j] + mat_[i][j]).
             *  @see isSymmmetric_
             *  @see checkIsSymmetric
             */
            void enforceSymmetry(void);
            
            /** Forces a matrix that passes the test for antisymmetry of checkIsAntisymmetric_ to be exactly
             *  antisymmetric.  Does so by replacing all element mat_[i][j] and mat_[i][j] by
             *  +/- 0.5*(mat_[i][j] - mat_[i][j]).
             *  @see isAntisymmmetric_;
             *  @see checkIsAntisymmetric
             */
            void enforcesAntisymmetry(void);
            
            /** Forces a matrix that passes the test for orthogonality of checkIsSymmetric_ to be exactly
             *  orthogonality.  Does so by applying an orthogonalization algorithm.
             *  @see isOrthogonal_;
             *  @see checkIsOrthogonal
             */
            void enforceOthogonality(void);
            
            /** Forces a matrix that passes the test for diagonality of checkIsDiagonal_ to be exactly
             *  diagonal.  Does so by setting to 0 all non-diagonal elements.
             *  @see isDiagonal_;
             *  @see checkIsDiagonal
             */
            void enforceDiagonality(void);
            
            /** Forces a matrix that passes the test for identity of checkIsIdentity_ to be exactly
             *  identity.  Does so by setting to 0 all non-diagonal elements and to 1 all diagonal elements.
             *  @see isIdentity_;
             *  @see checkIsIddentity
             */
            void enforceIdentity(void);
            
            /** Forces a matrix that passes the test for upper-triangularity of checkIsUpperTriangular_ to be exactly
             *  upper-triangular.  Does so by setting to 0 all superdiagonal elements.
             *  @see isUpperTriangular_;
             *  @see checkIsUpperTriangular
             */
            void enforceUpperTriangularity(void);
            
            /** Forces a matrix that passes the test for lower-triangularity of checkIsLowerTriangular_ to be exactly
             *  lower-triangular.  Does so by setting to 0 all subdiagonal elements.
             *  @see isLowerTriangular_;
             *  @see checkIsLowerTriangular
             */
            void enforceLowerTriangularity(void);
            
            /**
             * This function adds the parameter <code>Matrix b</code> to <code>this</code>
             * thereby modifying <code>this</code>.
             *
             * @param b the Matrix to add to <code>this</code>
             */
            void operator +=(const Matrix& b);

            /**  
             * This function subtracts the parameter <code>Matrix </code> from <code>this</code>
             * thereby modifying <code>this</code>.
             *
             * @param b the Matrix to subtract from <code>this</code>
             */
            virtual void operator -=(const Matrix& b);

            /**
             * This function multiplies every element of <code>this</code> by the scalar parameter
             * thereby modifying <code>this</code>.
             * 
             * @param s the scalar by which to multiply each element of <code>this</code>
             */
            virtual void operator*=(const double s);

            /**
             * This function premultiplies the parameter <code>Matrix b</code> by <code>this</code>
             * thereby modifying <code>this</code>.         
             *
             * @param b the matrix that is premultiplied by <code>this</code>
             */
            virtual void operator *=(const Matrix& b);

            /**
             * This function premultiplies <code>this</code> by the parameter <code>Matrix b</code>
             * thereby modifying <code>this</code>
             *
             * @param b the matrix that is premultiplying <code>this</code>
             */
            virtual void preMultiplyBy(const Matrix* b);

            /**
             * This function postmultiplies <code>this</code> by the parameter <code>Matrix b</code>
             * thereby modifying <code>this</code>
             *
             * @param b the matrix that is postmultiplying <code>this</code>
             */
            virtual void postMultiplyBy(const Matrix* b);

            /** Assigns to <code>this</code> matrix the matrix received as parameter (resizes it if necessary
             * @param b the matrix to set this object equal to
             */
            const Matrix& operator =(const Matrix& b);

            /**
             * This function does an element-wise comparison to the parameter <code>Matrix b</code>
             * with <code>this</code>.  <code>Matrix b</code> == <code>this</code> iff:
             *
             *      i.  the dimensions are same
             *  AND ii. all elements are the same
             * @param b the matrix to compare this one to
             *  @return a quadstate indicate if the matrics are equal, nearly equal, or different
             * 
             */
            TriState operator ==(const Matrix& b) const;

            /**
             * This function returns the element of the Matrix at row theRow and column theCol.
             * When accessing an element of the matrix, the indices are directly related to the
             * structure of the 2D-array storing them, <i>not</i> the row and column containing it.
             * 
             * @param theRow the row of the element to return
             * @param theCol the column of the element to return
             * @return the element at theRow,theCol
             */
            double operator ()(int theRow, int theCol) const;

            /**
             * This function swaps rows of the Matrix (rather, of mat_)
             *
             * @param r1 the first row to swap
             * @param r2 the second row to swap
             */
            void rowSwap(int r1, int r2);

            /**
             * This function swaps columns of the Matrix (rather, of mat_)
             *
             * @param c1 the first column to swap
             * @param c2 the second column to swap
             */
            void colSwap(int c1, int c2);

            /**
             * This function returns a double corresponding to the "1" norm of the Matrix <code>this</code>.
             *
             * @return the "1" norm of <code>this</code>
             */
            double norm1(void) const;

            /**
             * This function returns a double corresponding to the "Infinity" norm of the Matrix <code>this</code>.
             *
             * @return the "Infinity" norm of <code>this</code>
             */
            double normInf(void) const;

            /**
             * This function returns a double corresponding to the determinant of the Matrix <code>this</code>.
             *
             * @return the determinant of <code>this</code>
             */
            double determinant(void) const;

            /**
             * This function returns a reference to a new Matrix object, the transposition of 
             *    <code>this</code> matrix.
             *
             * @return a reference to the transposition of this matrix
             */
            virtual Matrix* transposition(void) const;

            /**
             * This function transposes <code>this</code>, thereby altering <code>this</code>.
             */
            virtual void transpose(void);

            /**
             * This function creates a new Matrix corresponding to the inverse of <code>this</code> (if nonsingular).
             * It then returns a reference to that inverse.
             * 
             * @return a reference to the inverse of <code>this</code>
             */
            virtual Matrix* inverse(void) const;

            /**
             * This function inverts <code>this</code> (if nonsingular), thereby modifying <code>this</code>.
             */
            virtual void invert(void);

            //========================================================================
            //  "to String" output functions
            //========================================================================

            /**
             * This function returns a string corresponding to the matrix using the static delimiting
             * strings for the Matrix class.
             */
            string toString(void) const;

            /**
             * This function returns a string corresponding to the matrix using the parameters
             * as delimiting strings.
             * 
             * @param theBeginArrayStr the string used to denote the beginning of the array
             * @param theEndArrayStr the string used to denote the end of the array
             * @param theElmtSepStr the string used to denote the seperation of elements
             * @param theEolAtEor a bool describing whether per-row end of lines should be used
             */
            string toString(string theBeginArrayStr, string theEndArrayStr,
                            string theElmtSepStr, bool theEolAtEor) const;

            /**
             * This function resets the delimiting strings used when generating a string representation
             * for a given matrix.
             *
             * @param theBeginArrayStr the string used to denote the beginning of the array
             * @param theEndArrayStr the string used to denote the end of the array
             * @param theElmtSepStr the string used to denote the seperation of elements
             * @param theEolAtEor a bool describing whether per-row end of lines should be used
             */
            static void setToStringFormat(string theBeginArrayStr,
                                          string theEndArrayStr,string theElmtSepStr, bool theEolAtEor);

        private:

            /** Number of rows of the matrix
             */
            int  nbRows_;
            
            
            /*  Number of columns of the matrix
             */
            int  nbCols_;

            /** Number of storage rows of the matrix.  To facilitate the reuse of a matrix object
             *  in video processing applications, the matrix's data array may be allocated at a 
             *  larger size (the "storage" dimensions) than the matrix's nominal dimensions
             */
            int nbStorageRows_;
            
            /** Number of storage columns of the matrix.  To facilitate the reuse of a matrix object
             *  in video processing applications, the matrix's data array may be allocated at a 
             *  larger size (the "storage" dimensions) than the matrix's nominal dimensions
             */
            int nbStorageCols_;
            
            /**
             *  The data for the Matrix.  The data is stored in the following fashion:  mat_ is a 1D-array
             *  of double pointers.  Each of these points to a double array.
             */
            double** mat_;

            /** Indicates whether the array mat_ was allocated by the matrix or is simply a reference
             *  to an array created outside of the class
             */
            bool storageIsLocal_;
            
            /** Indicates whether the matrix is known to be diagonal
             */
            TriState  isDiagonal_;

            /** Indicates whether the matrix is known to be an identity matrix
             */
            TriState  isIdentity_;

            /** Indicates whether the matrix is known to be upper-triangular
             */
            TriState  isUpperTriangular_;

            /** Indicates whether the matrix is known to be lower-triangular
             */
            TriState  isLowerTriangular_;

            /** Indicates whether the matrix is known to be a full rank matrix (which includes nonsingular for
             *  a square matrix)
             */
            TriState  isFullRank_;

            /** Indicates whether the matrix is known to be symmetric
             */
            TriState  isSymmetric_;

            /** Indicates whether the matrix is known to be antisymmetric
             */
            TriState  isAntisymmetric_;

            /** Indicates whether the matrix is known to be orthogonal
             */
            TriState  isOrthogonal_;

            /** Is this matrix normalized with respect to N1 norm?
             */
            TriState    isNormalized1_;
            
            /** Is this matrix normalized with respect to N2 (Euclidean) norm?
             */
            TriState    isNormalized2_;
            
            /** Is this matrix normalized with respect to N_infinity norm?
             */
            TriState    isNormalizedInf_;

            /** Beginning of row or col array for output as a string
             */
            static string  beginArrayStr_;

            /** End of row or col array for output as a string
             */
            static string  endArrayStr_;    //  end of a column or row

            /** Separation between elements for output as a string
             */
            static string  elmtSepStr_;

            /** Does the output format require an end of line character at the end of the string?
             */
            static bool  eolAtEor_;

            /** Absolute tolerance for all properties of the matrix class
             */
            const static double MATRIX_PROP_ABS_TOL;
            
            /** Relative tolerance for all properties of the matrix class
             */
            const static double MATRIX_PROP_REL_TOL;
            

            /**
             * Deletes and nullifies the 2D array, mat_.
             */
            void killMat_();
            
            /**
             * Allocates mat_ at the storage dimensions.
             */
            void allocateMatrix_(void);

			/**	Computes the sum of two matrices.  All dimension and allocation checkup is assumed
			 *	to have been made prior to calling this function
			 *	@param	a	left term of the matrix sum, dimensions m x n
			 *	@param	b	right term of the matrix sum, dimensions m x n
			 *	@param	c	result of the matrix sum, dimensions m x n
			 *	@param	m	number of rows of a, b, and of the sum matrix c
			 *	@param	n	number of columns of a, b, and of the sum matrix c
			 */
			static void sum_(const double *const* a, const double *const* b, double *const* c, 
							 int m, int n);

			/**	Computes the difference of two matrices.  All dimension and allocation checkup is assumed
			 *	to have been made prior to calling this function
			 *	@param	a	left term of the matrix difference, dimensions m x n
			 *	@param	b	right term of the matrix difference, dimensions m x n
			 *	@param	c	result of the matrix sum, dimensions m x n
			 *	@param	m	number of rows of a, b, and of the difference matrix c
			 *	@param	n	number of columns of a, b, and of the difference matrix c
			 */
			static void difference_(const double *const* a, const double *const* b, double *const* c, 
									int m, int n);

			/**	Computes the product of two matrices.  All dimension and allocation checkup is assumed
			 *	to have been made prior to calling this function
			 *	@param	a	left term of the matrix product, dimensions m x n
			 *	@param	b	right term of the matrix product, dimensions n x p
			 *	@param	c	result of the matrix product, dimensions m x p
			 *	@param	m	number of rows of a and of the product matrix c
			 *	@param	n	number of columns of a and of rows of b
			 *	@param	m	number of columns of b and of the product matrix c
			 */
			static void product_(const double *const* a, const double *const* b, double *const* c, 
								 int m, int n, int p);


    };  //  Matrix

}

#endif  //  MATRIX_H
