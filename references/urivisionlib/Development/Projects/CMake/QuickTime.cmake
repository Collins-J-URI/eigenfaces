#    File:
#        QuickTime.cmake
#
#    Description:
#        A CMake module for the uriVL CMake project.  This module attempts to
#        scan the current system for the location of the QuickTime header
#        files and associated binaries.
#
#    COPYRIGHT:
#        (c) 2003-2008, 3D Group for Interactive Visualization
#                       University of Rhode Island.
#
#        Licensed under the Academic Free License - v. 3.0
#        For more information see http://opensource.org/licenses/academic.php
#
#    Author:
#        Christopher T.S. Allen

IF(WIN32)
    IF(NOT CYGWIN)
        FIND_PATH(QuickTime_cinclude_dir QuickTime.h     