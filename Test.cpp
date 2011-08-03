#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <typeinfo>

#include "Filter.hpp"
#include "CXElement.hpp"

#ifdef TEST_COLLECTION
int 
main(int argc, char **argv)
{
  std::vector<int> ints;
  
  for(int i = 0; i < 100; ++i)
	ints.push_back(i);

  auto filter = Cinq::From(ints)
	.Where([](int n){ return (n & 1) == 0; })
	.Select([](int n){return n;});

  for(auto x: filter)
  {
	std::cout << x << std::endl;
  }

  return 0;
}
#else
using namespace Cinq;

int
main(int argc, char **argv)
{
  CXDocument emt("element",
				CXAttribute("hello", "world"),
				CXElement("first"),
				CXElement("second"));

  std::cout << emt.ToString() << std::endl;
  return 0;
}
#endif 
