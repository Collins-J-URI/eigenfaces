#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>

using namespace std;

const int width = 320;
const int height = 243;


//array of floats to store all the values
float pixels[height][width];

float** getPixelSquare(const int size){

	//create a dynamic 2D array
	float** square = new float*[size];
	int i = 0;
	for (i; i < size; i++){
		square[i] = new float[size];
	}

	//Center the pixels in the image using margins on width
	int minW = (width - size) / 2;
	int maxW = (width - minW);

	//grab the pixels and store them in our pointer
	i = 0;
	for (i; i < size; i++){
		int j = minW;
		for (j; j < maxW; j++){
			square[i][j] = pixels[i][j];
		}
	}

	return square;
}

void loadImage(string filename){
	ifstream input(filename);
	

	if (!input.good()){
		cout << "ERROR IN OPENING FILE" << endl;
		cout << "Press Any Key to Continue..." << endl;
		cin.get();
	}

	//loop and input all the values into the array
	int i = 0;
	for (i; i < height; i++){
		int j = 0;
		for (j; j < width; j++){
			input >> pixels[i][j];
		}
	}
	//done with input, close file;
	input.close();
	ofstream output("output.txt");

	//loop and input all the values into the array
	i = 0;
	for (i; i < height; i++){

		int j = 0;
		for (j; j < width; j++){
			if (j == 240){
				cout << fixed << setprecision(4) << setw(10) << pixels[i][j] << " ";
			}

			output << pixels[i][j] << "\t";
		}

		cout << endl;
	}

	//done with output file, close it.
	output.close();
}

int main(){
	loadImage("subject01.txt");
	return 0;
}