//
//  GetPixels.cpp
//
//
//  Created by James Collins on 5/11/15.
//
//

#include "GetPixels.h"
using namespace csc450Lib_linalg_base;


float** GetPixels::getPixels(string filename, int width, int height){
    
    int size = height;
    
    ifstream input(filename);
    
    if (!input.good()){
        cout << "ERROR IN OPENING FILE" << endl;
        cout << "Press Any Key to Continue..." << endl;
        cin.get();
    }
    
    //loop and input all the values into the array
    float **pixels = new float*[height];
    for (int i = 0; i < height; i++){
        pixels[i] = new float[width];
        for (int j = 0; j < width; j++){
            input >> pixels[i][j];
        }
    }
    //done with input, close file;
    input.close();
    
    return pixels;
}

float** GetPixels::getPixelSquare(string filename, int width, int height){
    
    int size = height;
    
    ifstream input(filename);
    
    if (!input.good()){
        cout << "ERROR IN OPENING FILE" << endl;
        cout << "Press Any Key to Continue..." << endl;
        cin.get();
    }
    
    //loop and input all the values into the array
    float **pixels = new float*[height];
    for (int i = 0; i < height; i++){
        pixels[i] = new float[width];
        for (int j = 0; j < width; j++){
            input >> pixels[i][j];
        }
    }
    //done with input, close file;
    input.close();
    
    //Center the pixels in the image using margins on width
    int minW = (width - size) / 2;
    int maxW = (width - minW);
    
    //grab the pixels and store them in our pointer
    float** square = new float*[size];
    for (int i = 0; i < size; i++){
        square[i] = new float[size];
        for (int j = minW; j < maxW; j++){
            square[i][j-minW] = pixels[i][j];
        }
    }
    
    return square;
}

int GetPixels::getdir(string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


float** GetPixels::getPixels(){
    return pixels;
}

float** GetPixels::getPixelSquare(int size){
    
    //Center the pixels in the image using margins on width
    int minW = (nbCols - size) / 2;
    int maxW = (nbCols - minW);
    
    //grab the pixels and store them in our pointer
    float** square = new float*[size];
    for (int i = 0; i < size; i++){
        square[i] = new float[size];
        for (int j = minW; j < maxW; j++){
            square[i][j-minW] = pixels[i][j];
        }
    }
    
    return square;
}

void GetPixels::loadImage(string filename, int width, int height){
    nbRows = height;
    nbCols = width;
    
	ifstream input(filename);

	if (!input.good()){
		cout << "ERROR IN OPENING FILE" << endl;
		cout << "Press Any Key to Continue..." << endl;
		cin.get();
	}

	//loop and input all the values into the array
    this->pixels = new float*[nbRows];
	for (int i = 0; i < nbRows; i++){
        pixels[i] = new float[nbCols];
		for (int j = 0; j < nbCols; j++){
			input >> pixels[i][j];
		}
	}
	//done with input, close file;
	input.close();
}