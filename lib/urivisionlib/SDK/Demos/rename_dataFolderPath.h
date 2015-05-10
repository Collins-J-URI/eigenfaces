/*  NAME:
        dataFolderPath.h

    DESCRIPTION:
        
		This file is a bit of a hack to simplify access to the data sample folder
		for all the demos.  Initially, I had a list of cases
			#if URIVL_OS_MACOS
			
			#elif URIVL_OS_WIN32
			
			#elif URIVL_OS_UNIX
			
			#else
				//problem
				
		that defined the relative location of the data sample folder for each OS.  Not pretty, but did the job.
		
		This all change with the relase of the plague upon Mac developers, that is, with
		when Xcode 4.x came out.  Now I had to deal with different relative paths depending on
		whether I was actually working (with Xcode 3) or just trying to make a final build (Xcode 4).
		
		So, now, all you have to do is set the proper paths for your development platform, and
		the demos should run fine.
			

    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef DATA_FOLDER_PATH_H
#define DATA_FOLDER_PATH_H

//	NOTE:	The paths I put here are the ones that work for *my* personal working
//			environments for these different system.  Yours may be (slightly) different

#if URIVL_OS_MACOS

//  Xcode 4
//    #define COLOR_DIR_PATH		"../../../../../../../../../../Data_Samples/Color/"
//    #define GRAY_DIR_PATH		"../../../../../../../../../../Data_Samples/Gray/"
//    #define MOVIE_DIR_PATH		"../../../../../../../../../../Data_Samples/Movies/"
//    #define STEREO_DIR_PATH		"../../../../../../../../../../Data_Samples/StereoPairs/"
//    #define OUTPUT_DIR_PATH		"../../../../../../Output/"


//  Xcode 3.x
    #define COLOR_DIR_PATH		"../../../../../../../../Data_Samples/Color/"
    #define GRAY_DIR_PATH		"../../../../../../../../Data_Samples/Gray/"
    #define MOVIE_DIR_PATH		"../../../../../../../../Data_Samples/Movies/"
    #define STEREO_DIR_PATH		"../../../../../../../../Data_Samples/StereoPairs/"
    #define OUTPUT_DIR_PATH		"../../../../Output/"

#elif URIVL_OS_WIN32

    #define COLOR_DIR_PATH		"../../../../../../Data_Samples/Color/"
    #define GRAY_DIR_PATH		"../../../../../../Data_Samples/Gray/"
    #define MOVIE_DIR_PATH		"../../../../../../Data_Samples/Movies/"
    #define STEREO_DIR_PATH		"../../../../../../Data_Samples/StereoPairs/"
    #define OUTPUT_DIR_PATH		"../../../../Output/"

#elif URIVL_OS_UNIX

    #define COLOR_DIR_PATH		"../../../../../../Data_Samples/Color/"
    #define GRAY_DIR_PATH		"../../../../../../Data_Samples/Gray/"
    #define MOVIE_DIR_PATH		"../../../../../../Data_Samples/Movies/"
    #define STEREO_DIR_PATH		"../../../../../../Data_Samples/StereoPairs/"
    #define OUTPUT_DIR_PATH		"../../../../Output/"

#else
    #error "OS not supported"
#endif




#endif  //  DATA_FOLDER_PATH_H
