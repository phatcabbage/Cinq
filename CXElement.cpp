#include "CXElement.hpp"

#include <sstream>
// namespace
// {
//   template<typename T>
//   std::string
//   ToString(const T& t)

// }

namespace Cinq
{
  std::string 
  CXAttribute::ToString() const
  {
	return _name + "=\"" + _value + "\"";
  }
  
  std::string
  CXElement::ToString() const
  {
	std::ostringstream oss;
	oss << "<" + _name;
	for(auto& attr : _attributes)
	{
	  oss << " " + attr.ToString();
	}

	if(_elements.empty())
	{
	  oss << "/>";
	}
	else
	{
	  oss << ">";
	  for(auto& emt : _elements)
	  {
		oss << emt.ToString();
	  }
	  oss << "</" + _name + ">";
	}
	return oss.str();
  }

  std::string
  CXDocument::ToString() const
  {
	std::ostringstream oss;
	for(auto& x: _processingInstructions)
	  oss << x.ToString() + "\n";
	oss << (static_cast<CXElement>(*this)).ToString();
	return oss.str();
  }

  void
  CXElement::Add(CXElement emt)
  {
	_elements.push_back(emt);
  }

  void
  CXDocument::Add(CXProcessingInstruction pi)
  {
	_processingInstructions.push_back(pi);
  }

  void
  CXElement::Add(CXAttribute a)
  {
	CXAbstractBaseElement::Add(a);
  }

  std::string
  CXProcessingInstruction::ToString() const
  {
	std::ostringstream oss;
	oss << "<?" + _name;
	for(auto& attr : _attributes)
	  oss << " " << attr.ToString();
	oss << "?>";
	return oss.str();
  }
}
