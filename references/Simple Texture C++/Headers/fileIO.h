#ifndef	FILE_IO_H
#define	FILE_IO_H

typedef enum eimageFileType
{
		kUnknownType = -1,
		kTGA_COLOR,				//	24-bit color image
		kTGA_GRAY,
		kPPM,					//	24-bit color image
		kPGM					//	8-bit gray-level image
} eImageFileType;

typedef enum efileErrorCode
{
		kNoIOerror = 0,
		kFileNameNull,
		kFileNotFound,
		kCannotOpenWrite,
		kCannotOpenRead,
		kWrongFileType,
		kUnknownFileType,
		kErrorReading,
		kErrorWriting,
		kEndOfFileError,
		kMemAllocError
} eFileErrorCode;

#endif