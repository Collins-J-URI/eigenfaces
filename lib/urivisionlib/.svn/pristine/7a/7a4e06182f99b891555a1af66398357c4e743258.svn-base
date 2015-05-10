/*  NAME:
        MoviePlayer.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib MoviePlayer class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#if URIVL_CAN_PLAY_MOVIES

#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
#include "MoviePlayer.h"
//
#if URIVL_USE_QUICKTIME
    #include "_MoviePlayer_QT.h"
#elif URIVL_USE_LIBQUICKTIME
   #include "_MoviePlayer_libquicktime.h"
#endif

using namespace std;
using namespace uriVL;



//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

MoviePlayer::MoviePlayer(void)
		try	:	VideoSource( ),
				//
				#if URIVL_USE_QUICKTIME
					player_(new _MoviePlayer_QT( ))
				#else
					player_(new _MoviePlayer_libquicktime( ))
				#endif				
{
    #if URIVL_USE_QUICKTIME
        player_ = new _MoviePlayer_QT( );
    #else
        player_ = new _MoviePlayer_libquicktime( );
    #endif
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MoviePlayer constructor");
	throw e;
}


MoviePlayer::MoviePlayer(const MoviePlayer& theObj)
		try	:	VideoSource( ),
				//
				#if URIVL_USE_QUICKTIME
					player_(new _MoviePlayer_QT( ))
				#else
					player_(new _MoviePlayer_libquicktime( ))
				#endif				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "MoviePlayer copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MoviePlayer constructor");
	throw e;
}



MoviePlayer::~MoviePlayer(void)
{
    delete player_;
}


const MoviePlayer& MoviePlayer::operator = (const MoviePlayer& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "MoviePlayer copy operator not implemented.");

	return *this;
}
#if 0
#pragma mark -
#endif


void MoviePlayer::setSource(void)
{
	try
	{
		player_->setSource_();
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::setSource");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::setSource");
	}
}

void MoviePlayer::setSource(const char* thePath)
{
    FAIL_CONDITION( thePath==NULL,
                    kFileNullPathError,
                    "Null path passed as parameter to MoviePlayer::setSource");
	try
	{
		player_->setSource_(thePath);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::setSource");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::setSource");
	}
}

void MoviePlayer::setDestination(RasterImage* destination)
{
	try
	{
		player_->setDestination_(destination);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::setDestination");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::setDestination");
	}
}


#if 0
#pragma mark -
#endif


RasterImage* MoviePlayer::getFrame(void)
{
	RasterImage* imgOut = NULL;
	
	try
	{
		imgOut = player_->getFrame_(1);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::getFrame");
	}
	
	return imgOut;
}


RasterImage* MoviePlayer::getFrame(int n)
{
	RasterImage* imgOut = NULL;
	
	try
	{
		imgOut = player_->getFrame_(n);
		updateImageRasterStatus_(imgOut);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::getFrame");
	}
	
	return imgOut;
}


void MoviePlayer::getFrame(RasterImage* imgOut)
{
    FAIL_CONDITION( imgOut == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as destination to a movie player");
	
	try
	{
		player_->getFrame_(1, imgOut);
		updateImageRasterStatus_(imgOut);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::getFrame");
	}
}

void MoviePlayer::getFrame(int n, RasterImage* imgOut)
{
    FAIL_CONDITION( imgOut == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as destination to a movie player");
    FAIL_CONDITION( n < 1, 
                    kInvalidParameterError, 
                    "movie player asked to skip invalid number of frames");

	try
	{
		player_->getFrame_(n, imgOut);	
		updateImageRasterStatus_(imgOut);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by MoviePlayer::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in MoviePlayer::getFrame");
	}
}


#if 0
//---------------------------------------------------------
#pragma mark -
#pragma mark misc access functions
//---------------------------------------------------------
#endif


bool MoviePlayer::isFinished(void) const
{
    return player_->isFinished_();
}

void MoviePlayer::goToBeginning(void)
{
    player_->goToBeginning_();
}


int MoviePlayer::getWidth(void) const
{
    return player_->getWidth_();
}

int MoviePlayer::getHeight(void) const
{
    return player_->getHeight_();
}

const ImageRect* MoviePlayer::getRect(void) const
{
    return player_->getRect_();
}

long MoviePlayer::getCurrentTime(void) const
{
    return player_->getCurrentTime_();
}

long MoviePlayer::getNextTime(void) const
{
    return player_->getNextTime_();
}

long MoviePlayer::getFrameIndex(void) const
{
    return player_->getFrameIndex_();
}

#endif // URIVL_CAN_PLAY_MOVIES
