/*  NAME:
        RigidTransformation3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RigidTransformation3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RigidTransformation3D.h"
#include "Translation3D.h"

using namespace uriVL;

RigidTransformation3D::RigidTransformation3D(void)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
}

RigidTransformation3D::RigidTransformation3D(const double* glMat)
        try :   Transformation3D(glMat),
                //
                rot_(NULL),
                trans_(NULL)        
{
    rot_ = NULL;
    trans_ = NULL;
}
catch(ErrorReport& e)
{
	e.appendToMessage("called by RigidTransformation3D constructor");
	throw e;
}

RigidTransformation3D::RigidTransformation3D(const double theMat[][4])
        try :   Transformation3D(theMat),
                //
                rot_(NULL),
                trans_(NULL)        
{
}
catch(ErrorReport& e)
{
	e.appendToMessage("called by RigidTransformation3D constructor");
	throw e;
}



RigidTransformation3D::RigidTransformation3D(const HomogeneousMatrix *theMat)
        try :   Transformation3D(theMat),
                //
                rot_(NULL),
                trans_(NULL)        
{
}
catch(ErrorReport& e)
{
	e.appendToMessage("called by RigidTransformation3D constructor");
	throw e;
}


RigidTransformation3D::RigidTransformation3D(const RigidTransformation3D *theTransf)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
    FAIL_CONDITION( theTransf == NULL,
                    kNullParameterError,
                    "Null pointer passed to RigidTransformation3D copy constructor"); 

    matrixIsReference_ = theTransf->matrixIsReference_;
    referenceHasChanged_ = true;

    if (matrixIsReference_)
    {
        double* const*const mat = homMat_->getMatrix(R_W_ACCESS);
        const double*const* theMat = theTransf->homMat_->getMatrix();
        for (int i=0; i<4; i++)
            for (int j=0; j<4; j++)
                mat[i][j] = theMat[i][j];
                
    }
    else
    {
        //  Copy the components
        if (theTransf->rot_ != NULL)
            rot_ = new Rotation3D(theTransf->rot_);
        else
            rot_ = new Rotation3D();;
            
        if (theTransf->trans_ != NULL)
            trans_ = new Translation3D(theTransf->trans_);
        else
            trans_ = new Translation3D();

    }        
    
}

RigidTransformation3D::RigidTransformation3D(const ReferenceFrame3D *startFrame, 
                                             const ReferenceFrame3D *endFrame)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "constructor not open for business yet"); 
}

RigidTransformation3D::RigidTransformation3D(const Rotation3D *theRot, const Translation3D *theTrans)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
    matrixIsReference_ = false;
    referenceHasChanged_ = true;
    
    //  Copy the components
    if (theRot != NULL)
        rot_ = new Rotation3D(theRot);
    else
        rot_ = new Rotation3D();;
        
    if (theTrans != NULL)
        trans_ = new Translation3D(theTrans);
    else
        trans_ = new Translation3D();

}

RigidTransformation3D::RigidTransformation3D(const Rotation3D *theRot)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
    matrixIsReference_ = false;
    referenceHasChanged_ = true;
    
    //  Copy the components
    if (theRot != NULL)
        rot_ = new Rotation3D(theRot);
    else
        rot_ = new Rotation3D();;
        
    trans_ = new Translation3D();

}

RigidTransformation3D::RigidTransformation3D(const Translation3D *theTrans)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
    matrixIsReference_ = false;
    referenceHasChanged_ = true;
    
    //  Copy the components
    rot_ = new Rotation3D();;
        
    if (theTrans != NULL)
        trans_ = new Translation3D(theTrans);
    else
        trans_ = new Translation3D();

}

RigidTransformation3D::RigidTransformation3D(RotationRef rotDataType, const double* rotData, const double* transData)
    :   rot_(new Rotation3D(rotDataType, rotData)),
        trans_(new Translation3D(transData))
{
}




RigidTransformation3D::RigidTransformation3D(const RigidTransformation3D& theField)
            :   Transformation3D( ),
                //
                rot_(NULL),
                trans_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Copy constructor not implemented.");
}

RigidTransformation3D::~RigidTransformation3D(void)
{
    if (rot_ != NULL)
        delete rot_;
        
    if (trans_ != NULL)
        delete trans_;
}

#if 0
#pragma mark -
#endif

const RigidTransformation3D& RigidTransformation3D::operator = (const RigidTransformation3D& theField)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Copy constructor not implemented.");

    return *this;
}



const Rotation3D *RigidTransformation3D::getRotationComponent(void)
{
    return rot_;
}

Rotation3D *RigidTransformation3D::getRotationComponent(ReadWriteStatus rwa)
{
    return rot_;
}

const Translation3D *RigidTransformation3D::getTranslationComponent(void)
{
    return trans_;
}

Translation3D *RigidTransformation3D::getTranslationComponent(ReadWriteStatus rwa)
{
    return trans_;
}

#if 0
#pragma mark -
#endif

void RigidTransformation3D::updateMatrix_(void) const
{
    if (referenceHasChanged_ && !matrixIsReference_)
    {
        double* const*const mat = homMat_->getMatrix(R_W_ACCESS);

        //  get the translation component
        const double* tXYZ  = trans_->getVector();
        mat[0][2] = tXYZ[0]; 
        mat[1][2] = tXYZ[1]; 
        mat[2][2] = tXYZ[2]; 
        
        //  compute the rotation 
        referenceHasChanged_ = false;
    }
    else if (matrixIsReference_ && referenceHasChanged_)
    {
        //  orthogonalize the matrix
        FAIL_CONDITION( true,
                        kFunctionNotImplemented,
                        "constructor not open for business yet"); 
        
    }

}        

void RigidTransformation3D::postComposeTransformation(const RigidTransformation3D* transf)
{
    (getMatrix(R_W_ACCESS))->postMultiplyBy(transf->getMatrix());
}

void RigidTransformation3D::postComposeTransformation(const Rotation3D*theRot)
{
    (getMatrix(R_W_ACCESS))->postMultiplyBy(theRot->getMatrix());
}

void RigidTransformation3D::postComposeTransformation(const Translation3D* theTrans)
{
    double* const *const A = getMatrix(R_W_ACCESS)->getMatrix(R_W_ACCESS);
    const double* T = theTrans->getVector();
    
    //	add to the 4th column of A the elements of the product Rot(A).T
    for (int i=0; i<3; i++)
    	A[i][3] += A[i][0]*T[0] + A[i][1]*T[1] + A[i][2]*T[2];

}
