#include "../include/VectorN.hpp"

int main() {
	std::cout << "Test VectorN<DataT>" << std::endl;
	float alpha = 5;
	CParaGL::VectorN<float, 3> vector_1 ({1, 2, 0});
	CParaGL::VectorN<float, 3> vector_2 ({4, 5, 0});
	std::cout << " Construct two Vector3<int>" << std::endl;
	std::cout << vector_1 << " and " << vector_2 << std::endl ;
	std::cout << "=================================" << std::endl;
	std::cout << "-vector_1" << std::endl;
	std::cout << -vector_1 << std::endl;
	std::cout << "=================================" << std::endl;
	std::cout << "vector_2 - vector_1" << std::endl;
	std::cout << vector_2 - vector_1 << std::endl;
	std::cout << "=================================" << std::endl;
	std::cout << "vector_1 * alpha (= 5)" << std::endl;
	std::cout << vector_1 * alpha  << std::endl;
	std::cout << "=================================" << std::endl;
	std::cout << "alpha * vector_1" << std::endl;
	std::cout << alpha * vector_1 << std::endl;
	std::cout << "=================================" << std::endl;
	std::cout << "vector_1 / alpha (= 5)" << std::endl;
	std::cout << vector_1 / alpha << std::endl;
	std::cout << "=================================" << std::endl;
	//std::cout << "vector_1 ^ vector_2" << std::endl;
	//std::cout << (vector_1 ^ vector_2) << std::endl; // we need brackets because of bit operation priority
	//std::cout << "=================================" << std::endl;

	return 0;
}