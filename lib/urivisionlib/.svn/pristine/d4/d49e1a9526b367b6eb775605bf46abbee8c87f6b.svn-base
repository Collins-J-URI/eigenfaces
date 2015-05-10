/*  NAME:
        ErrorReport.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ErrorReport class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstring>
#include <cstdlib>
//
#include "ErrorReport.h"

using namespace std;
using namespace uriVL;

#ifdef URIVL_VALID_RECT_SAFE
	bool UriVL::validDataRectIsSafe_ = URIVL_VALID_RECT_SAFE;
#else
	bool UriVL::validDataRectIsSafe_ = true;
#endif


ErrorReport::ErrorReport(ErrorCode theCode, const string& theMessage)
	:	code_(theCode),
        message_(theMessage)
{
}


ErrorReport::~ErrorReport(void)
{
}

ErrorCode ErrorReport::getCode(void) const
{
    return code_;
}

const char* ErrorReport::getMessage(void) const
{
    // Some day this should probably just return a "const string&" that refers 
    // to this->message_.  Until then though, keep a good "const char*" 
    // interface.
    char	*theMessage = new char[message_.length() + 1];
    strcpy(theMessage, message_.c_str());

    return theMessage;
}


void ErrorReport::appendToMessage(const string& addMsg) 
{
	message_ = message_ + "\r\t" + addMsg;
}
