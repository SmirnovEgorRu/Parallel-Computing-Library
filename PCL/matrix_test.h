#ifndef _PCL_MATRIX_TEST_HEADER_
#define _PCL_MATRIX_TEST_HEADER_

#include<vector>
#include <iostream>
#include"matrix.h"

class matrix_test {

	void pcl_matrix_test(matrix& a, matrix& b) {
		size_t start_time = clock();

		matrix c = a*b;
		
		std::cout << "pcl multiplication time: " << (clock() - start_time) / 1000.0 << std::endl;

		pcl::scheduler::delete_scheduler();
	}
	void scalar_matrix_test(matrix& a, const matrix& b) {
		size_t start_time = clock();

		matrix c = a.mult_scalar(b);

		std::cout << "scalar multiplication time: " << (clock() - start_time) / 1000.0 << std::endl;
	}

public:
	void execute() {
		const size_t n = 2000;

		//const size_t m = 1000;
		//const size_t k = 1000;
		//matrix a(n, m);
		//matrix b(m, k);

		matrix a(n, n);
		matrix b(n, n);

		a.rnd_input();
		b.rnd_input();

		pcl_matrix_test(a, b);
		scalar_matrix_test(a, b);
	}
};




#endif
