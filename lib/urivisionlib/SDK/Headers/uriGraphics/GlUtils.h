/*  NAME:
        glUtils.h

    DESCRIPTION:
        GlUtils public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_GL_UTILS_H
#define URIVL_GL_UTILS_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif


namespace uriVL
{
	/** Contains various GL-related "utility" functions to allocate and delete
	 *  "shifted" 1D and 2D arrays of GLxxx elements
	 */
    class GlUtils
    {
        public:
        
            /** Allocates a 1D array of <code>GLfloat</code> elements that uses the
             *  index range specified by the parameters received
             *  @param  nl  low index value
             *  @param  nh  high index value
             *  @return new array
             *  @see    free_glfVector
             */
            static GLfloat* glfVector(int nl,int nh);

            /** Allocates a 1D array of <code>GLdloat</code> elements that uses the
             *  index range specified by the parameters received
             *  @param  nl  low index value
             *  @param  nh  high index value
             *  @return new array
             *  @see    free_gldVector
             */
            static GLdouble* gldVector(int nl,int nh);

            /** Allocates a 1D array of <code>GLuint</code> elements that uses the
             *  index range specified by the parameters received
             *  @param  nl  low index value
             *  @param  nh  high index value
             *  @return new array
             *  @see    free_gluiVector
             */
            static GLuint* gluiVector(int nl,int nh);

            /** Allocates a 1D array of <code>GLint</code> elements that uses the
             *  index range specified by the parameters received
             *  @param  nl  low index value
             *  @param  nh  high index value
             *  @return new array
             *  @see    free_gliVector
             */
            static GLint* gliVector(int nl,int nh);

            /** Allocates a 2D array of <code>GLfloat</code> elements that uses the
             *  index range specified by the parameters received
             *  @param  nrl  low row index value
             *  @param  nrh  high row index value
             *  @param  ncl  low column index value
             *  @param  nch  high column index value
             *  @return new array
             *  @see    free_glfMatrix
             */
            static GLfloat** glfMatrix(int nrl,int nrh,int ncl,int nch);

            /** Allocates a 2D array of <code>GLdouble</code> elements that uses the
             *  index range specified by the parameters received
             *  @param  nrl  low row index value
             *  @param  nrh  high row index value
             *  @param  ncl  low column index value
             *  @param  nch  high column index value
             *  @return new array
             *  @see    free_gldMatrix
             */
            static GLdouble** gldMatrix(int nrl,int nrh,int ncl,int nch);

            /** Frees the 1D array of <code>GLfloat</code> elements received as
             *  parameter.
             *  @param  nl  low index value
             *  @param  nh  high index value [not used by this function]
             *  @see    glfVector
             */
            static void free_glfVector(GLfloat* v,int nl,int nh);

            /** Frees the 1D array of <code>GLdouble</code> elements received as
             *  parameter.
             *  @param  nl  low index value
             *  @param  nh  high index value [not used by this function]
             *  @see    gldVector
             */
            static void free_gldVector(GLdouble* v,int nl,int nh);

            /** Frees the 1D array of <code>GLuint</code> elements received as
             *  parameter.
             *  @param  nl  low index value
             *  @param  nh  high index value [not used by this function]
             *  @see    gluiVector
             */
            static void free_gluiVector(GLuint* v,int nl,int nh);

            /** Frees the 1D array of <code>GLint</code> elements received as
             *  parameter.
             *  @param  nl  low index value
             *  @param  nh  high index value [not used by this function]
             *  @see    gliVector
             */
            static void free_gliVector(GLint* v,int nl,int nh);

            /** Frees the 2D array of <code>GLfloat</code> elements received as
             *  parameter.
             *  @param  nrl  low row index value
             *  @param  nrh  high row index value
             *  @param  ncl  low column index value
             *  @param  nch  high column index value
             *  @see    glfMatrix
             */
            static void free_glfMatrix(GLfloat** m,int nrl,int nrh,int ncl,int nch);

            /** Frees the 2D array of <code>GLdouble</code> elements received as
             *  parameter.
             *  @param  nrl  low row index value
             *  @param  nrh  high row index value
             *  @param  ncl  low column index value
             *  @param  nch  high column index value
             *  @see    gldMatrix
             */
            static void free_gldMatrix(GLdouble** m,int nrl,int nrh,int ncl,int nch);

        private:
        
            GlUtils(void);
            
            ~GlUtils(void);
        
    };
        
}

#endif  //  GL_UTILS_H
