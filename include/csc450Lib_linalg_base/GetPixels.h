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
        static float** getPixelSquare(string filename,
                                      int width = 320, int height = 243);
        static float** getPixels(string filename,
                                      int width = 320, int height = 243);
        
        
        static int getdir (string dir, vector<string> &files);

        
        float** getPixels(void);
        float** getPixelSquare(int size);
        void loadImage(string filename, int width = 320, int height = 243);
        
    };
}
#endif /* defined(____GetPixels_included__) */