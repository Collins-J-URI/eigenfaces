/*  NAME:
        StereoDisparity.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib StereoDisparity class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "StereoDisparity.h"

using namespace uriVL;

StereoDisparity::StereoDisparity(StereoDisparityType type, bool isRect) 
		:	Disparity(),
			//
			type_(type),
			isRectifiedStereoDisparity_(isRect)
{
}

StereoDisparity::StereoDisparity(const StereoDisparity* disp)
	try	:	Disparity(disp),
			//
			type_(disp->type_),
			isRectifiedStereoDisparity_(disp->isRectifiedStereoDisparity_)

{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by StereoDisparity constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( disp == NULL,
                    kNullImagePointError,
                    "NULL pointer passed as parameter to StereoDisparity constructor");

	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid pointer passed as parameter to StereoDisparity constructor");
}


StereoDisparity::StereoDisparity(const StereoDisparity& disp)
	try	:	Disparity(disp),
			//
			type_(disp.type_),
			isRectifiedStereoDisparity_(disp.isRectifiedStereoDisparity_)

{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by StereoDisparity constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid reference passed as parameter to StereoDisparity constructor");
}


StereoDisparity::StereoDisparity(const Vector2D* disp, float conf, StereoDisparityType type,
								 bool isRect) 
	try	:	Disparity(disp, conf),
			//
			type_(type),
			isRectifiedStereoDisparity_(isRect)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by Disparity constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( disp == NULL,
                    kNullImagePointError,
                    "NULL ImagePoint passed as parameter to StereoDisparity constructor");

	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid ImagePoint passed as parameter to StereoDisparity constructor");
}


StereoDisparity::StereoDisparity(int dx, int dy, float conf, StereoDisparityType type, bool isRect) 
		:	Disparity(dx, dy, conf),
			//
			type_(type),
			isRectifiedStereoDisparity_(isRect)
{
}


StereoDisparity::~StereoDisparity(void)
{
}

StereoDisparityType StereoDisparity::getType(void) const
{
	return type_;
}

bool StereoDisparity::isRectifiedStereoDisparity(void) const
{
	return isRectifiedStereoDisparity_;
}

