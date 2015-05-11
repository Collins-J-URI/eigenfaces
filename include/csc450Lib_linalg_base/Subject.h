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

