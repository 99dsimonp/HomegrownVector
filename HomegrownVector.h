//
// Created by Simon on 06/01/2022.
//

#ifndef HOMEGROWNVECTOR_H_
#define HOMEGROWNVECTOR_H_

#include <stdexcept>
#include <memory>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cassert>

template <typename T>
class HomegrownVector{
 private:
  size_t _capacity; 	// Max size
  size_t _elements = 0; // Current size
  T* _start;			// Pointer to beginning

 public:
  /// Default empty vector with a single element
  HomegrownVector(){
	_start = (T*) calloc(1, sizeof(T));
	_capacity = 1;
  }

  /// Vector starting capacity given for ctor
  explicit HomegrownVector(size_t capacity){
	_start = (T*) calloc(capacity, sizeof(T));
	_capacity = capacity;
	std::cout << "Explicitly set capacity to " << capacity << " in ctor." << std::endl;
  }

  // Initializer list, calculate capacity n^2 such that the initializer list exactly fits
  HomegrownVector(std::initializer_list<T>&& inputs){
	auto input_size = inputs.size();
	_elements = input_size;
	// Find number of 0s before first set bit:
	auto msb = __builtin_clzl(input_size | 0)+1;
	// Bit-shift all set bits out except for msb. Multiply with 2 to get i^2 capacity
	_capacity = ((input_size >> (64-msb)) << 64-msb)*2;
	std::cout << "capacity is now: " << _capacity << std::endl;
	_start = (T*) calloc(_capacity, sizeof(T));
	memcpy(_start, std::data(inputs), sizeof(T)*inputs.size());
  }

  // Copy memory from other
  HomegrownVector(const HomegrownVector<T>& other){
	_elements = other._elements;
	_capacity = other._capacity;
	_start = (T*)calloc(_capacity, sizeof(T));
	memcpy(_start, other._start, sizeof(T)*_elements);
  }

  // Steal memory from other
  HomegrownVector(HomegrownVector<T>&& other) noexcept {
	_elements = other._elements;
	_capacity = other._capacity;
	_start = other._start;
	other._start = nullptr;
  }

  ~HomegrownVector(){
	delete _start;
  }

  T& operator[](size_t ix){
	if(ix>= _elements){
	  throw std::out_of_range("Index was out of range.");
	}
	return _start[ix];
  }

  [[nodiscard]] std::vector<T> to_vec() const{
	std::vector<T> result(_start, _start+_elements);
	assert(result.size() == _elements);
	return result;
  }

  [[nodiscard]] std::string to_string() const{
	std::string result;
	for(auto i=0ul; i< _elements-1; ++i){
	  result+= std::to_string(_start[i]) + ", ";
	}
	// Add the last element without comma
	result+= std::to_string(_start[_elements-1]);
	return result;
  }

  void emplace_back(const T& element){
	if(_elements == _capacity){
	  auto* new_start = (T*)calloc(_capacity*2, sizeof(T));
	  memcpy(new_start, _start, sizeof(T)*_elements);
	  delete _start;
	  _start = new_start;
	  _capacity = _capacity*2;
	}
	_start[_elements++] = element;
  }

  void emplace_front(const T& element){
	// If at capacity, allocate more space:
	if(_elements == _capacity){
	  auto* new_start = (T*)calloc(_capacity*2, sizeof(T));
	  memcpy(new_start+1, _start, sizeof(T)*_elements);
	  delete _start;
	  _start = new_start;
	  _capacity = _capacity*2;
	}
	// Expensive operation of copying all data one space to the right
	// Alternatively place values in the middle and grow both ways?
	else{
	  memcpy(_start+1, _start, sizeof(T)*_elements);
	}
	// Finally, assign to the first element
	_start[0] = element;
	_elements+=1;
  }


  [[nodiscard]] inline size_t size() const noexcept{
	return _elements;
  }

  // Accept a lambda to sort the vector with
  template<typename L>
  void sort(const L& lambda){
	std::sort(_start, _start+_elements, lambda);
  }

};
#endif //HOMEGROWNVECTOR_H_
