#pragma once
#include <algorithm>
#include <vector>



namespace airs
{
	template<typename T>
	class ring
	{
	private:
		T* data = nullptr;
		std::size_t position = 0;
		std::size_t length = 0;
		std::size_t data_size = 0;

	public:
		ring() noexcept { }
		ring(std::size_t cap) :
			data(new T[cap]),
			data_size(cap)
		{
		}
		~ring()
		{
			delete[] data;
			data = nullptr;
			position = 0;
			length = 0;
			data_size = 0;
		}

		T& at(std::size_t i) { return data[(position + i) % data_size]; }
		const T& at(std::size_t i) const { return data[(position + i) % data_size]; }
		T& operator[](std::size_t i) { return at(i); }
		const T& operator[](std::size_t i) const { return at(i); }

		void clear() noexcept { length = 0; }
		void reserve(std::size_t cap)
		{
			T* tmp = new T[cap];
			length = std::min(length, cap);

			for (std::size_t i = 0; i < length; i++)
				tmp[i] = std::move(at(i));

			delete[] data;
			data_size = cap;
			position = 0;
			data = tmp;
		}
		void resize(std::size_t s)
		{
			if (s > data_size) reserve(s);
			
			length = s;
		}

		void push_back(const T& value) 
		{  
			if (length >= data_size) reserve(1 + data_size * 2);
			at(length++) = value;
		}
		void push_back(T&& value)
		{
			if (length >= data_size) reserve(1 + data_size * 2);
			at(length++) = std::move(value);
		}
		void pop_back() { at(--length) = T(); }
		T& back() { return at(length - 1); }
		const T& back() const { return at(length - 1); }

		void push_front(const T& value) 
		{
			if (length >= data_size) reserve(1 + data_size * 2);
			if (position > 0) position--;
			else position = data_size - 1;
			length++;
			at(0) = value;
		}
		void push_front(T&& value)
		{
			if (length >= data_size) reserve(1 + data_size * 2);
			if (position > 0) position--;
			else position = data_size - 1;
			length++;
			at(0) = std::move(value);
		}
		void pop_front() { at(0) = T(); position = (position + 1) % data_size; length--; }
		T& front() { return at(0); }
		const T& front() const { return at(0); }

		std::size_t capacity() const noexcept { return data_size; }
		std::size_t size() const noexcept { return length; }
		bool empty() const noexcept { return length > 0; }
	};
}