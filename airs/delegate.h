#pragma once



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

		delegate(void* anObject, stub_type aStub)
		{
			Object = anObject;
			Stub = aStub;
		}
		void assign(void* anObject, stub_type aStub)
		{
			Object = anObject;
			Stub = aStub;
		}

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
			return (static_cast<LAMBDA*>(this_ptr)->operator())(arg...);
		}

	public:
		delegate() { Object = nullptr; Stub = nullptr; }
		delegate(const delegate& d) 
		{ 
			Object = d.Object; 
			Stub = d.Stub; 
		}
		template <typename LAMBDA>
		delegate(const LAMBDA& lambda) { assign((void*)(&lambda), &lambda_stub<LAMBDA>); } 
		
		delegate& operator=(const delegate& d) 
		{
			Object = d.Object; 
			Stub = d.Stub;
			return *this;
		} 
		template <typename LAMBDA>
		delegate& operator=(const LAMBDA& instance) 
		{
			assign((void*)(&instance), lambda_stub<LAMBDA>);
			return *this;
		}

		inline bool empty() const { return !Stub; }
		inline operator bool() const
		{
			return Stub;
		}

		RTYPE operator()(ATYPE... arg) const
		{
			return (*Stub)(Object, arg...);
		}

		bool operator == (const delegate& d) const { return d.Stub == Stub && d.Object == Object; }
		bool operator != (const delegate& d) const { return d.Stub != Stub || d.Object != Object; }

		template <class T, RTYPE(T:: * TMethod)(ATYPE...)>
		void bind(T& instance)
		{
			assign(&instance, method_stub<T, TMethod>);
		}
		template <class T, RTYPE(T::* TMethod)(ATYPE...)>
		void bind(T* instance)
		{
			assign(instance, method_stub<T, TMethod>);
		}
		template <class T, RTYPE(T:: * TMethod)(ATYPE...) const>
		void bind(T const& instance)
		{
			assign(const_cast<T*>(&instance), const_method_stub<T, TMethod>);
		}
		template <class T, RTYPE(T::* TMethod)(ATYPE...) const>
		void bind(T const* instance)
		{
			assign(const_cast<T*>(instance), const_method_stub<T, TMethod>);
		}
		template <RTYPE(*TMethod)(ATYPE...)>
		void bind()
		{
			assign(nullptr, function_stub<TMethod>);
		}
		template <typename LAMBDA>
		void bind(const LAMBDA& instance)
		{
			assign((void*)(&instance), lambda_stub<LAMBDA>);
		}
		void clear() { Stub = nullptr; };
	};
}
