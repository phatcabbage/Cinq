#ifndef CINQ_CXELEMENT_HXX__
#define CINQ_CXELEMENT_HXX__

#pragma once

#include "Cinq.hpp"
#include <set>
#include <list>



namespace Cinq
{
  class CXAttribute
  {
  public:
	typedef std::string name_type;
	typedef std::string value_type;

  private:
	name_type _name;
	value_type _value;

  public:
	CXAttribute(const name_type& name)
	  : _name(name)
	  , _value() {}
	
	CXAttribute(const name_type& name,
				const value_type& value)
	  : _name(name)
	  , _value(value) {}

	CXAttribute(const CXAttribute& a)
	  : _name(a._name)
	  , _value(a._value)
	{}

	CXAttribute(CXAttribute&& a)
	  : _name(a._name)
	  , _value(a._value)
	{}

	name_type Name() const
	{ return _name; }

	value_type Value() const
	{ return _value; }

	void Name(const name_type& value)
	{ _name = value; }

	void Value(const value_type& value)
	{ _value = value; }

	std::string ToString() const;

	bool
	operator<(const CXAttribute& o) const
	{ return _name < o._name; }
  };

  class CXAbstractBaseElement
  {
  public:
	typedef std::string name_type;

  protected:
	name_type _name;
	std::set<CXAttribute> _attributes;

  protected:
	CXAbstractBaseElement(const name_type& name)
	  : _name(name)
	  , _attributes()
	{}

	CXAbstractBaseElement(const CXAbstractBaseElement& e)
	  : _name(e._name)
	  , _attributes(e._attributes)
	{}
	
	CXAbstractBaseElement(CXAbstractBaseElement&& e)
	  : _name(e._name)
	  , _attributes(e._attributes)
	{
	  e._attributes.clear();
	}

  public:
	name_type Name() const
	{ return _name; }

	void Name(const name_type& name)
	{ _name = name; }

	void Add(CXAttribute attr)
	{
	  _attributes.insert(attr);
	}

	const std::set<CXAttribute>& Attributes() const
	{
	  return _attributes;
	}

	virtual std::string ToString() const = 0;
  };

  class CXElement;
  class CXProcessingInstruction;

  class CXElement : protected CXAbstractBaseElement
  {
  private:
	std::list<CXElement> _elements;
  public:
	CXElement(const name_type& name) 
	  : CXAbstractBaseElement(name)
	  , _elements()
	{}

	CXElement(const CXElement& e)
	  : CXAbstractBaseElement(e)
	  , _elements(e._elements)
	{}

	CXElement(CXElement&& e)
	  : CXAbstractBaseElement(e)
	  , _elements(e._elements)
	{
	  e._attributes.clear();
	}

	template<typename... Args>
	CXElement(const name_type& name, Args... args)
	  : CXAbstractBaseElement(name)
	  , _elements()
	{
	  Add(args...);
	}

	template<typename T, typename... Args>
	void Add(T&& t, Args... args)
	{
	  Add(t);
	  Add(args...);
	}
	
	const std::list<CXElement>& Elements() const
	{
	  return _elements;
	}

	void Add(CXElement);
	void Add(CXAttribute);

	virtual std::string ToString() const;
  };

  class CXProcessingInstruction : protected CXAbstractBaseElement
  {
  public:
	CXProcessingInstruction(const name_type& name)
	  : CXAbstractBaseElement(name)
	{}

	template<typename T,typename... Args>
	void Add(T&& t, Args... args)
	{
	  Add(t);
	  Add(args...);
	}

	virtual std::string ToString() const;
  };

  class CXDocument : public CXElement
  {
  private:
	std::list<CXProcessingInstruction> _processingInstructions;

  public:
	CXDocument(const name_type& name)
	  : CXElement(name)
	  , _processingInstructions()
	{}

	CXDocument(const CXDocument& d)
	  : CXElement(d)
	  , _processingInstructions(d._processingInstructions)
	{}

	CXDocument(CXDocument&& d)
	  : CXElement(d)
	  , _processingInstructions(d._processingInstructions)
	{
	  d._processingInstructions.clear();
	}

	template<typename... Args>
	CXDocument(const name_type& name, Args... args)
	  : CXElement(name)
	  , _processingInstructions()
	{
	  Add(args...);
	}

	const std::list<CXProcessingInstruction>& ProcessingInstructions() const
	{
	  return _processingInstructions;
	}

	void Add(CXProcessingInstruction);
	virtual std::string ToString() const;
  };
}

#endif // CINQ_CXELEMENT_HXX__
