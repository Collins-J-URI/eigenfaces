/*  NAME:
        GraphWorld.cpp

    DESCRIPTION:
        implementation of the uriVisionLib GraphWorld class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "GraphWorld.h"
#if URIVL_USE_QUICKTIME
    #include "_GraphWorld_QT.h"
#else
    #include "_GraphWorld_gen.h"
#endif


/* Implements the GraphWorld class.
 *
 *	Wrapper class for the GWorld structure (or rather, pointer to a GWorld
 *	structure) of Apple's QuickDraw and QuickTime libraries.
 *
 *  Important: The PixMap stored by the GWorld pads the rows of the raster to
 *  round their length in bytes to a multiple of 32.  Make sure not to confuse
 *  the nominal bounding rectangle of the GraphWorld (
 */

using namespace uriVL;

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

GraphWorld::GraphWorld(int pixelDepth, const ImageRect* boundsRect)
	try	:	graphWorld_(
						#if URIVL_USE_QUICKTIME
							new _GraphWorld_QT(pixelDepth, boundsRect)
						#else
							new _GraphWorld_gen(pixelDepth, boundsRect)
						#endif
						)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GraphWorld constructor");
	throw e;
}


GraphWorld::GraphWorld(int pixelDepth, int nbRows, int nbCols)
	try	:	graphWorld_(
						#if URIVL_USE_QUICKTIME
							new _GraphWorld_QT(pixelDepth, nbRows, nbCols)
						#else
							new _GraphWorld_gen(pixelDepth, nbRows, nbCols)
						#endif
						)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GraphWorld constructor");
	throw e;
}


GraphWorld::GraphWorld(const GraphWorld* theGWorld)
	:	graphWorld_(NULL)
{
//    PixMapHandle    thePixmap = GetGWorldPixMap(theGWorld->gworld_);

    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GraphWorld clone constructor is not operative yet");
}


GraphWorld::GraphWorld(const GraphWorld& theGWorld)
	:	graphWorld_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GraphWorld clone constructor is not operative yet");
}

GraphWorld::~GraphWorld()
{
    delete graphWorld_;
}

const GraphWorld& GraphWorld::operator = (const GraphWorld& theGWorld)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GraphWorld = operator is not operative yet");
	return *this;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif


int GraphWorld::getBaseRasterType(void) const
{
    return graphWorld_->getBaseRasterType_();
}

int GraphWorld::getBytesPerRow(void) const
{
    return graphWorld_->getBytesPerRow_();
}

int GraphWorld::getPixelDepth(void) const
{
    return graphWorld_->getPixelDepth_();
}

int GraphWorld::getEffectiveNbCols(void) const
{
    return graphWorld_->getEffectiveNbCols_();
}

int GraphWorld::getNbRows(void) const
{
    return graphWorld_->getNbRows_();
}

long GraphWorld::getTotalByteCount(void) const
{
    return (long) graphWorld_->getNbRows_() * getBytesPerRow();
}

unsigned char* GraphWorld::getRaster(void)
{
    return graphWorld_->getRaster_();
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark copy and raster access
//----------------------------------------------------------
#endif

void GraphWorld::copyInto(GraphWorld* destination, const ImageRect* sourceRect, 
                          const ImageRect* destRect, bool inverseGray) const
{
    graphWorld_->copyInto_(destination->graphWorld_, sourceRect, destRect, inverseGray);
}

void GraphWorld::draw(void)
{
    graphWorld_->draw_();
}

#if 0
#pragma mark -
#endif

void GraphWorld::setAsMovieGraphWorld(Movie theMovie)
{
    graphWorld_->setAsMovieGraphWorld_(theMovie);
}

void GraphWorld::setAsImporterGraphWorld(GraphicsImportComponent importer)
{
    return graphWorld_->setAsImporterGraphWorld_(importer);
}


/*
void GraphWorld::setAsGrabberGraphWorld(FrameGrabber *grabber)
{
    graphWorld_->setAsGrabberGraphWorld_(grabber);
}

#if URIVL_USE_QUICKTIME
    void GraphWorld::addToBufferRing(VdigBufferRec *bufferRec)
    {
        graphWorld_->addToBufferRing_(bufferRec);
    }
#endif
*/

void GraphWorld::setGraphWorld(const GraphWorld* theGWorld)
{
    graphWorld_->setGraphWorld_(theGWorld->graphWorld_);
}


void GraphWorld::restoreGraphWorld(void)
{
    graphWorld_->restoreGraphWorld_();
}



