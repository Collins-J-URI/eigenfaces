/*  NAME:
        BackgroundRemover_dynamic.h
 
    DESCRIPTION:
        BackgroundRemover_dynamic public header.
        This class cannot be instantiated
        Inherited from: BackgroundRemover_dynamic
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BACKGROUND_REMOVER_DYNAMIC_H
#define URIVL_BACKGROUND_REMOVER_DYNAMIC_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "BackgroundRemover.h"

//
namespace uriVL
{
    /**	This class cannot be instantiated
     *	Inherited from: BackgroundRemover_dynamic
     *	Deal with changing background
     */
	class URIVL_EXPORT BackgroundRemover_dynamic : public BackgroundRemover
	{
		public:

			/**	Destructor
			 */
			virtual ~BackgroundRemover_dynamic(void);

			/**	Overidden virtual func
			 *
			 *	Stores a copy of img to use as firstBackground_(NOT background_) for future
			 *	image difference computations
			 *
			 *	@param	bkgd		raster image to store as firstBackground_
			 */			
			void setBackground(RasterImage* bkgd);
			
			/**	virtual func
			 *	Update background_
			 */
			virtual void updateBackground(RasterImage* img, ConstRectList rectList) = 0;
			
			
		protected:
        
            /** Returns a pointer to the first background image
             *  @return pointer to the first background image
             */
            RasterImage* getFirstBackground_(void);
            
			/**	Default constructor
			 */
			BackgroundRemover_dynamic(void);


			
		private:
			
			/**	First background image
			 */
			RasterImage* firstBackground_;
            
            
            /** Copy constructor disabled
             *  @param  obj     the object to copy
             */
            BackgroundRemover_dynamic(const BackgroundRemover_dynamic& obj);


            /** Copy operator disabled
             *  @param  obj     the object to copy
             */
            const BackgroundRemover_dynamic& operator = (const BackgroundRemover_dynamic& obj);
			
	};
}

#endif	//	BACKGROUND_REMOVER_DYNAMIC_H
