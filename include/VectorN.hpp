#ifndef __VectorN_HPP__
#define __VectorN_HPP__

#ifndef __MY_EPSILON__
#define __MY_EPSILON__ 1e-7
#endif

#include <iostream>

namespace CParaGL {
//===================================================================================================
template <typename DataT>
void construct (DataT *pointer, const DataT &other) { new (pointer) DataT (other); }
//===================================================================================================
template <typename DataT>
void destroy (DataT *pointer) noexcept { pointer->~DataT(); }
//===================================================================================================
template <typename ForwardIt>
void destroy (ForwardIt begin, ForwardIt end) noexcept { while (begin++ != end) destroy (&*begin); }
//===================================================================================================
template <typename DataT>
struct MyVectorNBuf {
protected:
	DataT *coord_ ; 
	size_t size_  ;

	MyVectorNBuf (const size_t size = 0) : 
		coord_ ((size == 0) ? nullptr : static_cast<DataT *>(::operator new (sizeof(DataT) * size))),
		size_(size) {}
	MyVectorNBuf (const MyVectorNBuf &other)            = delete;
	MyVectorNBuf &operator= (const MyVectorNBuf &other) = delete;
	MyVectorNBuf (MyVectorNBuf &&other) noexcept : 
		coord_ (other.coord_),
		size_  (other.size_) {
            other.coord_ = nullptr;
            other.size_  = 0;
        }
	MyVectorNBuf &operator= (MyVectorNBuf &&other) noexcept {
		if (this == &other) return *this;

		std::swap (coord_, other.coord_);
		std::swap (size_ , other.size_ );

		return *this;
	}
	virtual ~MyVectorNBuf () {
		destroy (coord_, coord_ + size_);
		::operator delete (coord_);
	}
};
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
struct VectorN : private MyVectorNBuf<DataT> {
public:
	using MyVectorNBuf<DataT>::coord_;
	using MyVectorNBuf<DataT>::size_ ;
	VectorN (const std::initializer_list<DataT> &data = {}) :
		MyVectorNBuf<DataT>(Dimension) {
			size_ = 0;
			auto element = data.begin();
			while (size_ < data.size() && size_ < Dimension) {
				construct<DataT> (coord_ + size_, *element);
				++size_;
				++element;
			}
			while (size_ < Dimension) {
				construct<DataT> (coord_ + size_, DataT{});
				++size_;
			}
		}
	VectorN (const VectorN &other) :
		MyVectorNBuf<DataT>(other.size_) {
			size_ = 0;
			while (size_ < other.size_) {
				construct<DataT> (coord_ + size_, other.coord_[size_]);
				++size_;
			}
		}
	VectorN &operator=(const VectorN &other) {
		for (size_t i = 0; i < other.size_; ++i) coord_[i] = other.coord_[i];
	}
	virtual ~VectorN() {}
//===================================================================================================
	DataT &operator[] (const size_t num) {
		if (num >= Dimension) throw std::invalid_argument{"Out of range VectorN!\n"};
		return coord_[num];
	}
//===================================================================================================
	const DataT &operator[] (const size_t num) const{
		if (num >= Dimension) throw std::invalid_argument{"Out of range VectorN!\n"};
		return coord_[num];
	}
//===================================================================================================
	VectorN &operator+=(const VectorN &other) noexcept {
		for (size_t i = 0 ; i < size_ ; ++i) coord_[i] += other.coord_[i];
		return *this;
	}
//===================================================================================================
	VectorN &operator*=(float lambda) noexcept {
		for (size_t i = 0; i < size_; ++i) coord_[i] *= lambda;
		return *this;
	}
//===================================================================================================
	DataT operator*=(const VectorN &other) noexcept {
		DataT result = {};
		for (size_t i = 0; i < size_; ++i) result += (coord_[i] * other.coord_[i]);
		return result;
	}
//===================================================================================================
	VectorN operator-() const noexcept {
		VectorN<DataT, Dimension> result;
		for (size_t i = 0; i < size_; ++i) result.coord_[i] = -coord_[i];
		return result;
	}
//===================================================================================================
	VectorN &operator-=(const VectorN &other) noexcept {
		for (size_t i = 0; i < size_; ++i) coord_[i] -= other.coord_[i];
		return *this;
	}
//===================================================================================================
	VectorN &operator^=(const VectorN &other) noexcept {
		if (size_       != 3) { std::cout << "Wrong Dimension!" << std::endl; return *this;}
		if (other.size_ != 3) { std::cout << "Wrong Dimension!" << std::endl; return *this;}
		DataT temp[3] = {coord_[1] * other.coord_[2] - other.coord_[1] * coord_[2], 
                         coord_[2] * other.coord_[0] - other.coord_[2] * coord_[0], 
                         coord_[0] * other.coord_[1] - other.coord_[0] * coord_[1]} ;
		for (int i = 0; i < 3; ++i) coord_[i] = temp[i] ;
		return *this;
	}
//===================================================================================================
	VectorN &operator/=(const float lambda) noexcept {
		for (size_t i = 0; i < size_; ++i) coord_[i] /= lambda;
		return *this;
	}
//===================================================================================================
	bool lessLengthThan(const VectorN &other) const noexcept {
		DataT length = {}, otherLength = {}; 
		for (size_t i = 0; i < size_; ++i) {
			length      += coord_[i]       * coord_[i]       ;
			otherLength += other.coord_[i] * other.coord_[i] ; 
		}
		return (length < otherLength);
	}
};
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
VectorN<DataT, Dimension> operator+(const VectorN<DataT, Dimension> &first, 
	const VectorN<DataT, Dimension> &second) noexcept {
		VectorN<DataT> result = first;
		return result += second;
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
VectorN<DataT, Dimension> operator-(const VectorN<DataT, Dimension> &first, 
	const VectorN<DataT, Dimension> &second) noexcept {
		VectorN<DataT, Dimension> result = first;
		return result -= second;
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
VectorN<DataT, Dimension> operator*(const VectorN<DataT, Dimension> &vector, 
	float lambda) noexcept {
		VectorN<DataT, Dimension> result = vector;
		return result *= lambda;
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
VectorN<DataT, Dimension> operator*(float lambda, const VectorN<DataT, Dimension> &vector) noexcept {
	return vector * lambda;
}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
DataT operator*(const VectorN<DataT, Dimension> &first, 
	const VectorN<DataT, Dimension> &second) noexcept {
		return first * second;
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
VectorN<DataT, Dimension> operator^(const VectorN<DataT, Dimension> &first, 
	const VectorN<DataT, Dimension> &second) noexcept {
		VectorN<DataT, Dimension> result = first;
		return (result ^= second);
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
VectorN<DataT, Dimension> operator/(const VectorN<DataT, Dimension> &vector, float lambda) noexcept {
	VectorN<DataT, Dimension> result = vector;
	return (result /= lambda);
}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
bool operator==(const VectorN<DataT, Dimension> &left, 
	const VectorN<DataT, Dimension> &right) noexcept {
		for (size_t i = 0 ; i < Dimension; ++i) 
			if (std::abs(left[i] - right[i]) > (__MY_EPSILON__) ) return false;
		return true;
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
bool operator!=(const VectorN<DataT, Dimension> &left, 
	const VectorN<DataT, Dimension> &right) noexcept {
		return !(left == right);
	}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
std::ostream &operator<<(std::ostream &stream, const VectorN<DataT, Dimension> &vector) {
	stream << "{" ;
	for (size_t i = 0; i < Dimension - 1; ++i) stream << vector[i] << " ; " ;
	if (Dimension - 1 >= 0) stream << vector[Dimension - 1];
	stream << "}" ; 
	return stream;
}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
std::istream &operator>>(std::istream &stream, VectorN<DataT, Dimension> &vector) {
	DataT temporary;
	for (size_t i = 0; i < Dimension; ++i) { stream >> temporary; vector[i] = temporary; }
	return stream;
}
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
DataT square_distance(const VectorN<DataT, Dimension> &fpoint, 
	const VectorN<DataT, Dimension> &spoint) noexcept {
		DataT res = {};
		for (size_t i = 0; i < Dimension; ++i)
			res += ((fpoint[i] - spoint[i]) * (fpoint[i] - spoint[i])) ;
		return res;
	}
//===================================================================================================


}


#endif