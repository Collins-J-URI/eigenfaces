#include <stdlib.h>        
#include <fstream>
#include <iostream>

#include "fileIO_SGI.h"

using namespace std;

static char getbyte(FILE *inf);
static void putbyte(FILE *outf,unsigned char val);
static unsigned short getshort(FILE *inf);
static void putshort(FILE *outf,unsigned short val);
static long getlong(FILE *inf);
static int putlong(FILE *outf,unsigned long val);							



////////////////////////////////////////////////////////////////////////////////
// Utilities used by writeSgiRgbImage and readSgiRgbImage.
////////////////////////////////////////////////////////////////////////////////
static char getbyte(FILE *inf)
{
   unsigned char buf;
   fread(&buf,1,1,inf);
   return (buf);
}

static void putbyte(FILE *outf,unsigned char val)
{
   unsigned char buf[1]; 
   buf[0] = val;
   fwrite(buf,1,1,outf);
}

static unsigned short getshort(FILE *inf)
{
   unsigned char buf[2];
   fread(buf,2,1,inf);
   return (buf[0]<<8)+(buf[1]<<0);
}

static void putshort(FILE *outf,unsigned short val)
{
   unsigned char buf[2]; 
   buf[0] = (val>>8);
   buf[1] = (val>>0);
   fwrite(buf,2,1,outf);
}

static long getlong(FILE *inf)
{
   unsigned char buf[4];
   fread(buf,4,1,inf);
   return (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+(buf[3]<<0);
}

static int putlong(FILE *outf,unsigned long val)
{
   unsigned char buf[4];
   
   buf[0] = (val>>24);
   buf[1] = (val>>16);
   buf[2] = (val>>8);
   buf[3] = (val>>0);
   return fwrite(buf,4,1,outf);
}


////////////////////////////////////////////////////////////////////////////////
// Read an image in the .SGI format. 
////////////////////////////////////////////////////////////////////////////////
unsigned char* readSgiRgbImage(char* infn, int* m, int* n)
{
	unsigned char	*img;
	unsigned short	imgCode;
	unsigned char	bytesPerChannel;
	unsigned short	dimension, xsize, ysize, zsize;
	unsigned long	pixmin,pixmax;
	unsigned long	colormap;
	//
	char	imageName[80];
	char	storage;
	FILE	*of;
	int		i, y;

	if (!(of = fopen(infn,"r"))) 
		return 0;

	imgCode=getshort(of);  
	storage=getbyte(of);    
	bytesPerChannel=getbyte(of);   
	dimension=getshort(of);  
	*n=xsize=getshort(of);  
	*m=ysize=getshort(of);   
	zsize=getshort(of);    
	pixmin=getlong(of);   
	pixmax=getlong(of);   

	for(i=0; i<4; i++) 
		getbyte(of);

	fread(imageName,80,1,of);  
	colormap=getlong(of);         

	for(i=0; i<404; i++)
		getbyte(of);

	if(imgCode != 474)
		return 0;

	if(storage)
		return 0;

	img=new unsigned char[ysize*xsize];

	//   for(y=ysize-1; y>=0; y--) fread(img+y*xsize,xsize,1,of);
	for(y=0; y<ysize; y++)
		fread(img+y*xsize,xsize,1,of);

	fclose(of);

	cout << "Read the image: " << infn << endl;

	return img;
}


////////////////////////////////////////////////////////////////////////////////
// Write an image in the .SGI format. 
////////////////////////////////////////////////////////////////////////////////
int writeSgiRgbImage(unsigned char* img, int m, int n, char* outfn)
{
   FILE *of;
   char iname[80];
   int i, y;
   if(!(of = fopen(outfn,"w"))) return 0;
   putshort(of,474);        // imgCode                       
   putbyte(of,0);           // STORAGE is VERBATIM         
   putbyte(of,1);           // BPC is 1                    
   putshort(of,2);          // DIMENSION is 2              
   putshort(of,n);          // XSIZE                       
   putshort(of,m);          // YSIZE                       
   putshort(of,1);          // ZSIZE                       
   putlong(of,0);           // PIXMIN is 0                 
   putlong(of,255);         // PIXMAX is 255               
   for(i=0; i<4; i++) putbyte(of,0);   // DUMMY 4 bytes       
   strcpy(iname,"Claude Duchesne!");
   fwrite(iname,80,1,of);   // IMAGENAME           
   putlong(of,0);           // COLORMAP is 0       
   for(i=0; i<404; i++) putbyte(of,0); // DUMMY 404 bytes     
   //for(y=m-1; y>=0; y--) fwrite(img+y*n,n,1,of);
   for(y=0; y<m; y++) fwrite(img+y*n,n,1,of);
   fclose(of);

   cout << "Wrote the image: " << outfn << endl;
	  
   return(1);
}
