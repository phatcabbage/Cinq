#ifndef CINQ_FILTER_HPP__
#define CINQ_FILTER_HPP__

#pragma once
namespace Cinq
{
  namespace
  {

	template <typename T>
	T* 
	make_ptr()
	{
	  return (T*)0;
	}

	template <typename T>
	const T&
	make_ref()
	{
	  return *(make_ptr<T>());
	}
  }

  template <typename T>
  struct always_true
  {
	bool operator() (const T&) const
	{ return true; }
  };

  template <typename T>
  struct pass_thru
  {
	T operator() (const T& e) const
	{ return e; }
  };

  template <typename TElemFrom,
			typename TElemTo,
			typename TIterFrom,
			typename TSelector,
			typename TPredicate>
  class Filter
  {
	TIterFrom _from;
	TIterFrom _end;
	const TSelector& _selector;
	const TPredicate& _predicate;

  public:
	
	Filter(TIterFrom from,
		   TIterFrom end,
		   const TSelector& selector,
		   const TPredicate& predicate)
	  : _from(from)
	  , _end(end)
	  , _selector(selector)
	  , _predicate(predicate){}

	class const_iterator
	{
	  TIterFrom _from;
	  TIterFrom _end;
	  const TSelector& _selector;
	  const TPredicate& _predicate;

	  void
	  locate()
	  {
		while(!done())
		{
		  if(_predicate(*_from)) return;
		  ++_from;
		}
	  }

	  bool
	  done() const
	  {
		return (_from == _end);
	  }

	public:
	  const_iterator(TIterFrom from,
					 TIterFrom end,
					 const TSelector& selector,
					 const TPredicate& predicate)
		: _from(from)
		, _end(end)
		, _selector(selector)
		, _predicate(predicate)
	  { locate();}

	  TElemTo 
	  operator*() const
	  {
		return _selector(*_from);
	  }

	  const_iterator
	  operator++()
	  {
		++_from;
		locate();
		return *this;
	  }

	  bool
	  operator==(const const_iterator& other) const
	  {
		return done() && other.done();
	  }

	  bool
	  operator!=(const const_iterator& other) const
	  {
		return !done() || !other.done();
	  }
	};

	typedef TElemFrom value_type;

	const_iterator
	begin() const
	{
	  return const_iterator(_from, _end, _selector, _predicate);
	}

	const_iterator
	end() const
	{
	  return const_iterator(_end, _end, _selector, _predicate);
	}

	template <typename TSelector2>
	Filter<TElemTo,
		   decltype(make_ref<TSelector2>()(make_ref<TElemTo>())),
		   const_iterator,
		   TSelector2,
		   always_true<TElemTo> >
	Select(const TSelector2& selector)
		   {
	  return Filter<TElemTo,
					decltype(make_ref<TSelector2>()(make_ref<TElemTo>())),
					const_iterator,
					TSelector2,
					always_true<TElemTo> >
		(begin(), end(), selector, always_true<TElemTo>());
	}

	template<typename TPredicate2>
	Filter<TElemTo,
		   TElemTo,
		   const_iterator,
		   pass_thru<TElemTo>,
		   TPredicate2>
	Where(const TPredicate2& predicate)
	{
	  return Filter<TElemTo,
					TElemTo,
					const_iterator,
					pass_thru<TElemTo>,
					TPredicate2>
	  (begin(), end(), pass_thru<TElemTo>(), predicate);
	}
  };

  template <typename TCollFrom>
  Filter<typename TCollFrom::value_type,
		 typename TCollFrom::value_type,
		 typename TCollFrom::const_iterator,
		 pass_thru<typename TCollFrom::value_type>,
		 always_true<typename TCollFrom::value_type> >
  From(const TCollFrom& from)
  {
	return Filter<typename TCollFrom::value_type,
				  typename TCollFrom::value_type,
				  typename TCollFrom::const_iterator,
				  pass_thru<typename TCollFrom::value_type>,
				  always_true<typename TCollFrom::value_type> >
	  (from.begin(), from.end(),
	   pass_thru<typename TCollFrom::value_type>(),
	   always_true<typename TCollFrom::value_type>());
  }
}

#endif // CINQ_FILTER_HPP__
