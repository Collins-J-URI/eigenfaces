//
//  GetPixels.h
//
//
//  Created by James Collins on 5/11/15.
//
//

//=================================
// include guard
#ifndef ____GetPixels_included__
#define ____GetPixels_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>

using namespace std;


namespace csc450Lib_linalg_base {
    
    //=================================
    // forward declared dependencies
    
    /**
     * Defines a matrix of floating point values, along with a decent number of
     *  mathematical functions that can be performed on it
     */
    class GetPixels {
    private:
        /**
         * Two dimensional array to define the matrix
         */
        float ** pixels;
        
        /**
         * Number of rows
         */
        int nbRows;
        
        /**
         * Number of columns
         */
        int nbCols;
        
    public:

		/*
		*	Since not all photos are square, receive a square image from the data of the given file
		*	@param filename the path to the file to alter
		*	@param width the width of the image
		*	@param height the height of the image
		*	return a 2D float array of the newly comprised square image
		*/
        static float** getPixelSquare(string filename,
                                      int width = 320, int height = 243);

		/*
		*	Store the Pixel Data from a file into a 2D array of floats
		*	@param filename the path to the file to get pixel data from
		*	@param width the width of the image
		*	@param height the height of the image
		*	return The Matrix containing all the images and data for this subject
		*/
        static float** getPixels(string filename,
                                      int width = 320, int height = 243);
        
		/*
		*	Gets all the files in the given directory and stores the names and paths to a vector
		*	@param dir the directory to search for files
		*	@param files the vector to store all filenames into
		*	return 0 if success, ANY if failure
		*/
        static int getdir (string dir, vector<string> &files);

		/*
		*	Store the Pixel Data from a file into a 2D array of floats
		*	return The Matrix containing all the images and data for this subject
		*/
        float** getPixels(void);

		/*
		*	Since not all photos are square, receive a square image from the data of the given file
		*	return a 2D float array of the newly comprised square image
		*/
        float** getPixelSquare(int size);

		/*
		*	Load data image from a given file
		*	@param filename the path to the file to get pixel data from
		*	@param width the width of the image
		*	@param height the height of the image
		*/
        void loadImage(string filename, int width = 320, int height = 243);
        
    };
}
#endif /* defined(____GetPixels_included__) */