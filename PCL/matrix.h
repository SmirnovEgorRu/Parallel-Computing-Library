#ifndef _PCL_MATRIX_HEADER_
#define _PCL_MATRIX_HEADER_

#include<vector>
#include"scheduler.h"


class matrix {
public:
	size_t n;
	size_t m;
	std::vector<std::vector<int>> elements;

	void rnd_input() {
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				elements[i][j] = rand();
			}
		}
	}

	void multiplication(const matrix& b, matrix& c, size_t i, size_t j) {
		size_t n = this->m;
		c.elements[i][j] = 0;

		for (size_t k = 0; k < n; ++k)
			c.elements[i][j] += this->elements[i][k] * b.elements[k][j];
	}


	void mult(const matrix& b, matrix& c, size_t i) {
		size_t n_other = this->m;

		for (size_t j = 0; j < m; ++j) {
			c.elements[i][j] = 0;
			for (size_t k = 0; k < n_other; ++k) {
				c.elements[i][j] += this->elements[i][k] * b.elements[k][j];
			}
		}
	}


	const matrix& operator*(const matrix& other){
		pcl::scheduler tasks;

		matrix result(this->n, other.m);
		for (size_t i = 0; i < n; ++i) {
				tasks.add_task(&matrix::mult, this, std::ref(other), std::ref(result), i);
		}

		pcl::scheduler::wait();
		return result;
	}

	const matrix& mult_scalar(const matrix& other) {
		matrix result(this->n, other.m);

		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				multiplication(other, result, i, j);
			}
		}
		return result;
	}

	matrix(size_t new_n, size_t new_m) : n(new_n), m(new_m), elements(n) {
		for (size_t i = 0; i < n; ++i)
			elements[i].resize(m);
	};
};



#endif