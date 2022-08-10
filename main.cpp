#include <iostream>
#include "HomegrownVector.h"

template <typename T>
std::string vec2string(const std::vector<T>& vec){
  std::string result;
  for(const auto& val : vec){
	result+= std::to_string(val) + ",";
  }
  // Remove the last comma
  result.pop_back();
  return result;
}

int main() {
  HomegrownVector<int> vc1;
  vc1.emplace_back(0);
  std::cout << vc1[0] << std::endl;
  vc1[0] = 5;
  std::cout << vc1[0] << std::endl;

  HomegrownVector<int> vc2(5ul);

  HomegrownVector<int> vc3{1, 2, 3, 4, 5};
  std::cout << vc3.to_string() << std::endl;
  for(auto i = 6; i < 10; ++i) {
	vc3.emplace_back(i);
  }

  // Sort
  std::cout << "Before sort:" << vc3.to_string() << std::endl;
  vc3.sort([](int v1, int v2){
	return v1>v2;
  });
  std::cout << "After sort:" << vc3.to_string() << std::endl;

  HomegrownVector<int> vc4(vc3);
  auto newvec4 = vc4.to_vec();

  return 0;
}
