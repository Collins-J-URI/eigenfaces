/*  NAME:
        ImageFeature.h
 
    DESCRIPTION:
        ImageFeature public header.
        ImageFeature is an abstract class (with pure virtual member function)
        Inherited by: Blob, Segment, Corner...
        Base class: DrawableObject2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_FEATURE_H
#define URIVL_IMAGE_FEATURE_H

#include <list>
#include <string>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "DrawableObject2D.h"
#include "ImageRect.h"
#include "ImageFeature.h"

namespace uriVL
{
    /** Abstract base class of Blob, Segment, Corner...
     */
    class URIVL_EXPORT ImageFeature : public DrawableObject2D
    {
        public:
        
           /**
			*  Default constructor.
            *
            */
            ImageFeature(void);

           /**
			*  Copy constructor.
            *
            *  @param      obj   reference to the object to copy
            */
            ImageFeature(const ImageFeature& obj);



            /** Destructor
             */
            virtual ~ImageFeature(void);

            
			
        private:
		
		
           /**
			*  Copy operator.
            *
            *  @param      obj   reference to the object to copy
            */
            const ImageFeature& operator = (const ImageFeature& obj);


    };

    typedef std::list<ImageFeature *> FeatureList;
	
}

#endif  //  IMAGE_FEATURE_H
