//
//  Subject.h
//
//
//  Created by James Collins on 5/11/15.
//
//

//=================================
// include guard
#ifndef ____Subject_included__
#define ____Subject_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#pragma once
namespace csc450Lib_linalg_base {

	class Subject
	{
	public:
		Subject(Matrix *images);
		~Subject();
		int getID();
	private:
		static int ID;
		int id;

		Matrix *images
	};

}
#endif /* defined(____Subject_included__) */

