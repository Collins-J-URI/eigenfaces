/*------------------------------------------------------------------+
|	testRandomCpp.cpp												|
|																	|
|	Just a little test program for the random generation function.	|
+------------------------------------------------------------------*/

#include <iostream>
#include <ctime>
#include <cmath>
//
#include "RandomGenerator_C.h"
#include "RandomGenerator_R250_521.h"

using namespace std;
using namespace uriVL;


#define	kNbElements		10
#define kNbSeq			4
#ifndef	true
#define	true	1
#define	false	0
#endif

int main(void)
{
    cout << "========================================" << endl << "\tC generator" << endl;
    cout << "========================================" << endl;
    
    cout << endl << "----------------------------------------" << endl;
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextDouble_st() << "\t ";
	cout << endl<< "----------------------------------------" << endl;
	cout << "integers in range 10-45" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextInt_st(10, 45) << "\t ";
	cout << endl<< "----------------------------------------" << endl;
	cout << "integers in range 10-45" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextInt_st(10, 45) << "\t ";

    cout << endl <<"----------------------------------------" << endl;
    RandomGenerator_C::setSeed_st(1234);
    cout << endl << "...reseed 1234" << endl;
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << endl << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextDouble_st() << "\t ";

    cout << endl<< "----------------------------------------" << endl;
    RandomGenerator_C::setSeed_st(4567);
    cout << "...reseed 4567" << endl;
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << endl << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextDouble_st() << "\t ";

    cout << endl<< "----------------------------------------" << endl;
    RandomGenerator_C::setSeed_st(1234);
    cout << "...reseed 1234" << endl;
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextDouble_st() << "\t ";

	
	cout << endl<< "----------------------------------------" << endl;
	cout << "integers in range 10-45" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_C::nextInt_st(10, 45) << "\t ";
   

    cout << endl << endl <<
         "========================================" << endl << "\tR250-R521 generator" << endl;
    cout << "========================================" << endl;
    
    cout << endl << "----------------------------------------" << endl;
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_R250_R521::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_R250_R521::nextDouble_st() << "\t ";

    cout << endl <<"----------------------------------------" << endl;
    RandomGenerator_R250_R521::setSeed_st(1234);
    cout << "...reseed 1234" << endl;
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_R250_R521::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_R250_R521::nextDouble_st() << "\t ";

    cout << endl<< "----------------------------------------" << endl;
    RandomGenerator_R250_R521::setSeed_st(4567);
    cout << "Long numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_R250_R521::nextULong_st() << "\t ";
    cout << endl << "-----" << endl;
    cout << "Double numbers" << endl;
    for (int i=0; i<10; i++)
        cout << RandomGenerator_R250_R521::nextDouble_st() << "\t ";

static const double INV_MAX_ULONG = 1.L / pow(2.L, 8.L*sizeof(long)); 
cout << endl << endl << "INV_MAX_ULONG = " << INV_MAX_ULONG << endl;

    cout << endl<< "----------------------------------------" << endl;
//    RandomGenerator_R250_R521::setSeed_st(12);
    for (int i=0; i<50; i++)
    {
        unsigned long val = RandomGenerator_R250_R521::nextULong_st();
        cout << val << "\t" << (INV_MAX_ULONG * val) << endl;
    }
    cout << endl << "-----" << endl;
    cout << endl << "Double numbers" << endl;
//    RandomGenerator_R250_R521::setSeed_st(12);
    for (int i=0; i<50; i++)
        cout << RandomGenerator_R250_R521::nextDouble_st(5.L, 12.L) << endl;

/*
    unsigned long maxLong = 0;
    double maxDouble = 0.L;
    for (int i=0; i<10000; i++)
    {
        unsigned long val = RandomGenerator_R250_R521::nextULong_st();
        if (val > maxLong)
            maxLong = val;
      
        double dVal = RandomGenerator_R250_R521::nextDouble_st();
        cout << dVal << "\t";
        if (dVal > maxDouble)
            maxDouble = dVal;
    }
    cout <<endl << endl << "max long = " << maxLong << endl;
    cout << "max double = " << maxDouble << endl;
*/
   
/*
	int		theSeed, keepTime,
			i, iSeq,
			*seedVal;
	GLfloat	**xVal;
	
			
	//==================================================================
	//	Part 1:   I allocate my arrays
	//==================================================================
	xVal = glfMatrix(kNbSeq, kNbElements);
	seedVal = (int *) calloc(kNbSeq, sizeof(int));
	randGen = (RandomGenerator **) calloc(kNbSeq, sizeof(RandomGenerator *));
		

	//==================================================================
	//	Part 1:   I create my random generators.
	//	Note that unlike with the C version, I can create and use multiple
	//	pseudo-random sequences concurrently.
	//==================================================================
	for (iSeq=0; iSeq<kNbSeq; iSeq++)
		randGen[iSeq] = new RandomGenerator();
		

	//==================================================================
	//	Part 2:   Use the generator to produce pseudo-random sequences
	//	I only use here 3 of the generators, I will use the fourth later
	//	to verify that I get the same sequence again
	//==================================================================
	//	2.1 Using System time as seed
	//-------------------------------
	//	If you are developing a game, you want to make sure that you
	//	generate a different sequence each time a player runs your 
	//	application: use the system timer for that.  Since we want
	//	to reset the random sequence, we need a negative seed.
	keepTime = (int) time(NULL);	//	time() returns a long int
	seedVal[0] = -keepTime;
	//
	//	2.2 Predefined seed
	//-------------------------------
	//	Sometimes it is good, while you test your program, to get the same
	//	"random" sequence over and over as you iron out the kinks in yor code
	seedVal[1] = -406;
	//
	//	2.3 Predefined seed
	//-------------------------------
	//	We use the same seed for seq. 3 as for seq. 2
	//		--> we will get the same numbers
	seedVal[2] = -406;
	//
	//------------------------------------------
	//	We seed the sequences and generate their first element
	//------------------------------------------
	//	Just a little show of pointer arithmetics.  Normally, you would
	//	use a loop on iSeq
	iSeq = 0;
	theSeed = seedVal[0];
	xVal[iSeq][0] = randGen[0]->ran0(&theSeed);
	seedVal[iSeq] = theSeed;
	//
	iSeq++;
	xVal[iSeq][0] = randGen[iSeq]->ran0(&(seedVal[iSeq]));
	//
	iSeq++;
	xVal[iSeq][0] = randGen[iSeq]->ran0(seedVal+iSeq);
	
	//------------------------------------------
	//	Since our sequences have been seeded, we don't need to pass a pointer
	//	to the seed anymore.  Still, just to show a bit of pointer arithmetics,
	//	I will pass the seed as an argument each and every time for one of the
	//	sequence.  For the other two sequences, I will call the default (no argument)
	//	form of the function.  It makes no difference.
	//------------------------------------------
	for (i=1; i<kNbElements;i++)
	{
		xVal[0][i] = randGen[0]->ran0(seedVal);
		xVal[1][i] = randGen[1]->ran0();
		xVal[2][i] = randGen[2]->ran0();
	}
		
	
	//==================================================================
	//	Part 3:   We print out the sequences we just generated
	//==================================================================
	iSeq = 0;
	cout << "1. Using system time as seed\n----------------------------" << endl;
	for (i=0; i<kNbElements;i++)
		cout << xVal[iSeq][i] << "\t";
	//
	iSeq++;
	cout << "\n\n2. Using 406 as seed\n--------------------" << endl;
	for (i=0; i<kNbElements;i++)
		cout << xVal[iSeq][i] << "\t";
	//
	iSeq++;
	cout << "\n\n3. Using 406 as seed again\n--------------------------" << endl;
	for (i=0; i<kNbElements;i++)
		cout << xVal[iSeq][i] << "\t";

	//==================================================================
	//	Part 4:   Reusing System time from run 1 as seed
	//==================================================================
	//	It does not matter when you generate the RandomGenerator object.
	//	What counts is the seed you use and the order in the sequence.
	//	For example here, if you stored system time in your first run, then
	//	it is as good a constant as our 406.  You can therefore get
	//	back the same sequence (without having to store it if it is very long).
	iSeq = kNbSeq-1;
	seedVal[iSeq] = -keepTime;
	xVal[iSeq][0] = randGen[iSeq]->ran0(seedVal+iSeq);
	//
	//	It should be obvious by now that you can always re-use any of your
	//	RandomGenerator objects, and also re-seed your sequence in the middle
	//	of your program to go back to the begining of the sequence
	iSeq = 1;
	seedVal[iSeq] = -keepTime;
	xVal[iSeq][0] = randGen[iSeq]->ran0(seedVal+iSeq);
	
	//	We generate the values for this sequence
	iSeq = kNbSeq-1;
	for (i=1; i<kNbElements; i++)
		xVal[iSeq][i] = randGen[iSeq]->ran0();
	
	//	I start the time sequence then after a while swich to the 406 sequence
	iSeq = 1;
	for (i=1; i<kNbElements/2; i++)
		xVal[1][i] = randGen[1]->ran0();
	seedVal[1] = -406;
	for (; i<kNbElements; i++)
		xVal[1][i] = randGen[1]->ran0(seedVal+iSeq);
		
		
	//==================================================================
	//	Part 5:   We print out the sequences we just generated
	//==================================================================
	cout << "\n\n4. Using system time from seq. 1 as seed again" << endl;
	cout << "----------------------------------------------" << endl;
	iSeq = kNbSeq-1;
	for (i=0; i<kNbElements;i++)
		cout << xVal[iSeq][i] << "\t";


	cout << "\n\n5. Start with the \"time\" sequence, ";
	cout << "then switch to the \"406\" sequence after a while\n";
	cout << "-----------------------------------------------------------"
		<< "-----------------------\n";
	iSeq = 1;
	for (i=0; i<kNbElements;i++)
		cout << xVal[iSeq][i] << "\t";


	//==================================================================
	//	Part 6:   Just to be nice I do some cleanup (not really needed)
	//==================================================================
	free_glfMatrix(xVal, kNbSeq);
	free((char *) seedVal);
	for (iSeq = 0; iSeq < kNbElements; iSeq++)
		delete randGen[i];
	free((char *) randGen);
	
*/	
	return 0;
}
