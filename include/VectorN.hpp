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
void destroy (ForwardIt begin, ForwardIt end) noexcept {
	while (begin++ != end) destroy (&*begin);
}
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
	~MyVectorNBuf () {
		destroy (coord_, coord_ + size_);
		::operator delete (coord_);
	}
};
//===================================================================================================
template <typename DataT, size_t Dimension = 3>
struct VectorN : private MyVectorNBuf<DataT> {
private:
	using MyVectorNBuf<DataT>::coord_;
	using MyVectorNBuf<DataT>::size_ ;
public:
	VectorN (const std::initializer_list<DataT> &data = {}) :
		MyVectorNBuf<DataT>((data.size() == Dimension) ? Dimension : 0 ) {
			if (data.size() == Dimension)
				for (size_t i = 0; i < Dimension ; ++i) coord_[i] = data[i];
			else std::cout << "Wrong initializer_list size" << std::endl ;
		}
	VectorN (const VectorN &other) :
		MyVectorNBuf<DataT>((other.size_() == Dimension) ? Dimension : 0 ) {
			for (size_t i = 0; i < Dimension; ++i) coord_[i] = other.coord_[i];
		}
	VectorN &operator=(const VectorN &other) {
		for (int i = 0; i < Dimension; ++i) coord_[i] = other.coord_[i];
	}
	virtual ~VectorN() {}
//===================================================================================================
	DataT get_coord(size_t num) const noexcept {
		//if (num > Dimension) throw std::invalid_argument("Wrong Vector coord number\n");
		return coord_[num];
	}
//===================================================================================================
	void  set_coord(size_t num, DataT val) noexcept {
		//if (num > Dimension) throw std::invalid_argument("Wrong Vector coord number\n");
		coord_[num] = val;
	}
//===================================================================================================
	VectorN &operator+=(const VectorN &other) noexcept {
		for (int i = 0 ; i < 3 ; ++i) coord_[i] += other.coord_[i];
		return *this;
	}
//===================================================================================================
	VectorN &operator*=(float lambda) noexcept {
		for (int i = 0; i < 3; ++i) coord_[i] *= lambda;
		return *this;
	}
//===================================================================================================
	VectorN operator-() const noexcept {
		VectorN<DataT> result;
		for (int i = 0; i < 3; ++i) result.coord_[i] = -coord_[i];
		return result;
	}
//===================================================================================================
	VectorN &operator-=(const VectorN &other) noexcept {
		for (int i = 0; i < 3; ++i) coord_[i] -= other.coord_[i];
		return *this;
	}
//===================================================================================================
	VectorN &operator^=(const VectorN &other) noexcept {
		DataT temp[3] = {coord_[1] * other.coord_[2] - other.coord_[1] * coord_[2], 
                         coord_[2] * other.coord_[0] - other.coord_[2] * coord_[0], 
                         coord_[0] * other.coord_[1] - other.coord_[0] * coord_[1]} ;
		for (int i = 0; i < 3; ++i) coord_[i] = temp[i] ;
		return *this;
	}
//===================================================================================================
	VectorN &operator/=(const float lambda) noexcept {
		for (int i = 0; i < 3; ++i) coord_[i] /= lambda;
		return *this;
	}
//===================================================================================================
	bool lessLengthThan(const VectorN &other) const noexcept {
		DataT length = {}, otherLength = {}; 
		for (int i = 0; i < 3; ++i) {
			length      += coord_[i]       * coord_[i]       ;
			otherLength += other.coord_[i] * other.coord_[i] ; 
		}
		return (length < otherLength);
	}
};
//===================================================================================================
template <typename DataT>
VectorN<DataT> operator+(const VectorN<DataT> &first, const VectorN<DataT> &second) noexcept {
	VectorN<DataT> result = first;
	return result += second;
}
//===================================================================================================
template <typename DataT>
VectorN<DataT> operator-(const VectorN<DataT> &first, const VectorN<DataT> &second) noexcept {
	VectorN<DataT> result = first;
	return result -= second;
}
//===================================================================================================
template <typename DataT>
VectorN<DataT> operator*(const VectorN<DataT> &vector, float lambda) noexcept {
	VectorN<DataT> result = vector;
	return result *= lambda;
}
//===================================================================================================
template <typename DataT>
VectorN<DataT> operator*(float lambda, const VectorN<DataT> &vector) noexcept {
	return vector * lambda;
}
//===================================================================================================
template <typename DataT>
DataT operator*(const VectorN<DataT> &first, const VectorN<DataT> &second) noexcept {
	DataT res = {};
	for (int i = 0; i < 3; ++i) 
		res += (first.get_coord(i) * second.get_coord(i));
	return res;
}
//===================================================================================================
template <typename DataT>
VectorN<DataT> operator^(const VectorN<DataT> &first, const VectorN<DataT> &second) noexcept {
	VectorN<DataT> result = first;
	return (result ^= second);
}
//===================================================================================================
template <typename DataT>
VectorN<DataT> operator/(const VectorN<DataT> &vector, float lambda) noexcept {
	VectorN<DataT> result = vector;
	return (result /= lambda);
}
//===================================================================================================
template <typename DataT>
bool operator==(const VectorN<DataT> &left, const VectorN<DataT> &right) noexcept {
	for (int i = 0 ; i < 3; ++i) 
		if (std::abs(left.get_coord(i) - right.get_coord(i)) > (__MY_EPSILON__) ) return false;
	return true;
}
//===================================================================================================
template <typename DataT>
bool operator!=(const VectorN<DataT> &left, const VectorN<DataT> &right) noexcept {
	return !(left == right);
}
//===================================================================================================
template <typename DataT>
std::ostream &operator<<(std::ostream &stream, const VectorN<DataT> &vector) {
	stream << "{ " << vector.get_coord(0) << " ; " 
	               << vector.get_coord(1) << " ; " 
				   << vector.get_coord(2) << " }" ;
	return stream;
}
//===================================================================================================
template <typename DataT>
std::istream &operator>>(std::istream &stream, VectorN<DataT> &vector) {
	DataT temp[3];
	stream >> temp[0] >> temp[1] >> temp[2];
	for (int i = 0; i < 3; ++i) vector.set_coord(i, temp[i]);
	return stream;
}
//===================================================================================================
template <typename DataT>
DataT square_distance(const VectorN<DataT> &fpoint, const VectorN<DataT> &spoint) noexcept {
	DataT res = {};
	for (int  i = 0; i < 3; ++i)
		res += ((fpoint.get_coord(i) - spoint.get_coord(i)) * (fpoint.get_coord(i) - spoint.get_coord(i)));
	return res;
}

}


#endif