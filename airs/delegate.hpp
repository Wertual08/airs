#pragma once
#include <iostream>
#include <cstddef>



namespace airs
{
	template <typename T> class delegate;

	template<typename RTYPE, typename ...ATYPE>
	class delegate<RTYPE(ATYPE...)>
	{
	private:
		using stub_type = RTYPE(*)(void*, ATYPE...);

		void* Object;
		stub_type Stub;
		std::size_t ObjectSize;

		template <class T, RTYPE(T:: * TMethod)(ATYPE...)>
		static RTYPE method_stub(void* this_ptr, ATYPE... params)
		{
			return (static_cast<T*>(this_ptr)->*TMethod)(params...);
		}
		template <class T, RTYPE(T:: * TMethod)(ATYPE...) const>
		static RTYPE const_method_stub(void* this_ptr, ATYPE... params)
		{
			return (static_cast<T*>(this_ptr)->*TMethod)(params...);
		}
		template <RTYPE(*TMethod)(ATYPE...)>
		static RTYPE function_stub(void* this_ptr, ATYPE... params)
		{
			return (TMethod)(params...);
		}
		template <typename LAMBDA>
		static RTYPE lambda_stub(void* this_ptr, ATYPE... arg)
		{
			return (reinterpret_cast<LAMBDA*>(this_ptr)->operator())(arg...);
		}

		void assign(void* object, stub_type stub, std::size_t size)
		{
			if (ObjectSize) delete[] Object;

			if ((ObjectSize = size) > 0)
			{
				Object = new char[size];
				for (std::size_t i = 0; i < size; i++)
				{
					reinterpret_cast<char*>(Object)[i] = reinterpret_cast<char*>(object)[i];
				}
			}
			else Object = object;
			Stub = stub;
		}

	public:
		delegate() { Object = nullptr; Stub = nullptr; ObjectSize = 0; }
		delegate(const delegate& d) 
		{ 
			assign(d.Object, d.Stub, d.ObjectSize);
		}
		template <typename LAMBDA>
		delegate(const LAMBDA& instance)
		{
			assign(const_cast<LAMBDA*>(&instance), &lambda_stub<LAMBDA>, 0);
		}
		template <typename LAMBDA>
		delegate(LAMBDA&& instance)
		{
			assign(&instance, &lambda_stub<LAMBDA>, sizeof(LAMBDA));
		}
		~delegate()
		{
			if (ObjectSize > 0)
			{
				delete[] Object;
				ObjectSize = 0;
				Object = nullptr;
			}
		}
		
		delegate& operator=(const delegate& d) 
		{
			assign(d.Object, d.Stub, d.ObjectSize);
			return *this;
		} 
		template <typename LAMBDA>
		delegate& operator=(const LAMBDA& instance)
		{
			assign(const_cast<LAMBDA*>(&instance), &lambda_stub<LAMBDA>, 0);
			return *this;
		}
		template <typename LAMBDA>
		delegate& operator=(LAMBDA&& instance)
		{
			throw std::runtime_error("go fuck yourself");
			//assign(&instance, &lambda_stub<LAMBDA>, sizeof(LAMBDA));
			return *this;
		}

		inline bool empty() const { return !Stub; }
		inline operator bool() const { return Stub; }

		RTYPE operator()(ATYPE... arg) const
		{
			return (*Stub)(Object, arg...);
		}

		bool operator == (const delegate& d) const { return d.Stub == Stub && d.Object == Object && d.ObjectSize == ObjectSize; }
		bool operator != (const delegate& d) const { return d.Stub != Stub || d.Object != Object || d.ObjectSize == ObjectSize; }

		template <class T, RTYPE(T::* TMethod)(ATYPE...)>
		void bind(T& instance)
		{
			assign(&instance, &method_stub<T, TMethod>, 0);
		}
		template <class T, RTYPE(T::* TMethod)(ATYPE...)>
		void bind(T* instance)
		{
			assign(instance, &method_stub<T, TMethod>, 0);
		}
		template <class T, RTYPE(T:: * TMethod)(ATYPE...) const>
		void bind(T const& instance)
		{
			assign(const_cast<T*>(&instance), &const_method_stub<T, TMethod>, 0);
		}
		template <class T, RTYPE(T::* TMethod)(ATYPE...) const>
		void bind(T const* instance)
		{
			assign(const_cast<T*>(instance), &const_method_stub<T, TMethod>, 0);
		}
		template <RTYPE(*TMethod)(ATYPE...)>
		void bind()
		{
			assign(nullptr, &function_stub<TMethod>, 0);
		}
		template <typename LAMBDA>
		void bind(const LAMBDA& instance)
		{
			assign(const_cast<LAMBDA*>(&instance), &lambda_stub<LAMBDA>, 0);
		}
		template <typename LAMBDA>
		void bind(LAMBDA&& instance)
		{
			assign(&instance, &lambda_stub<LAMBDA>, sizeof(LAMBDA));
		}
		void clear() { assign(nullptr, nullptr, 0); };
	};
}
