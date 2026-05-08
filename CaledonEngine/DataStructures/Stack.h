/*------------------------------
| File: Stack.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <iostream>
#include <stdexcept>

namespace CE
{
	/*------------------------------------------------------------------------------------------------------------
	| --- A stack is a data structure that stores and retrieves items in a last-in, first-out (LIFO) manner. --- |
	------------------------------------------------------------------------------------------------------------*/
	template <typename T>
	class Stack
	{
	private:
		T* m_pStackArray;														// Dynamic array to hold stack elements
		size_t m_capacity;														// Maximum capacity of the stack
		int m_top;																// Index of the top element in the stack (-1 when empty)

	public:
		explicit Stack(size_t capacity);										// Constructor
		~Stack();																// Destructor
		Stack(const Stack& other);												// Copy Constructor
		Stack& operator=(const Stack& other);									// Copy Assignment Operator
		Stack(Stack&& other) noexcept;											// Move Constructor
		Stack& operator=(Stack&& other) noexcept;								// Move Assignment Operator

		void Push(const T& item);												// Pushes an item onto the stack
		void Push(T&& item);													// Pushes an item onto the stack using move semantics
		void Pop();																// Pops the top item from the stack
		T& Top();																// Returns a reference to the top item on the stack
		const T& Top() const;													// Returns a const reference to the top item on the stack

		bool IsFull() const;													// Checks if the stack is full
		bool IsEmpty() const;													// Checks if the stack is empty
		size_t Size() const		{ return m_top + 1; }							// Returns the number of elements in the stack
		size_t Capacity() const { return m_capacity; }							// Returns the maximum capacity of the stack

		void Clear();															// Clears all items from the stack by resetting the top index
	};

	/*---------------------------------------------------------------
	| --- Constructor: Constructs the Stack with default values --- |
	---------------------------------------------------------------*/
	template<typename T>
	inline Stack<T>::Stack(size_t capacity)
		: m_pStackArray(capacity > 0 ? new T[capacity] : nullptr)
		, m_capacity(capacity)
		, m_top(-1)
	{
		if (capacity == 0)
		{
			throw std::invalid_argument("Stack capacity must be greater than 0");
		}
	}

	/*-------------------------------------------------------
	| --- Destructor: Cleans up any allocated resources --- |
	-------------------------------------------------------*/
	template<typename T>
	inline Stack<T>::~Stack()
	{
		delete[] m_pStackArray;
	}

	/*------------------------------------------------------------
	| --- Copy Constructor: Creates a deep copy of the stack --- |
	------------------------------------------------------------*/
	template<typename T>
	inline Stack<T>::Stack(const Stack& other)
		: m_pStackArray(other.m_capacity > 0 ? new T[other.m_capacity] : nullptr)
		, m_capacity(other.m_capacity)
		, m_top(other.m_top)
	{
		for (int i = 0; i <= m_top; ++i)
		{
			m_pStackArray[i] = other.m_pStackArray[i];
		}
	}

	/*-------------------------------------------------------------------------------------------------
	| --- Copy Assignment Operator: Creates a deep copy of the stack and assigns it to this stack --- |
	-------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline Stack<T>& Stack<T>::operator=(const Stack& other)
	{
		if (this != &other)
		{
			T* newArray = other.m_capacity > 0 ? new T[other.m_capacity] : nullptr;

			for (int i = 0; i <= other.m_top; ++i)
			{
				newArray[i] = other.m_pStackArray[i];
			}

			delete[] m_pStackArray;

			m_pStackArray = newArray;
			m_capacity = other.m_capacity;
			m_top = other.m_top;
		}
		return *this;

	}

	/*----------------------------------------------------------------------------
	| --- Move Constructor: Moves resources from another stack to this stack --- |
	----------------------------------------------------------------------------*/
	template<typename T>
	inline Stack<T>::Stack(Stack&& other) noexcept
		: m_pStackArray(other.m_pStackArray)
		, m_capacity(other.m_capacity)
		, m_top(other.m_top)
	{
		other.m_pStackArray = nullptr;
		other.m_capacity = 0;
		other.m_top = -1;
	}

	/*---------------------------------------------------------------------------------------------------
	| --- Move Assignment Operator: Moves resources from another stack and assigns it to this stack --- |
	---------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline Stack<T>& Stack<T>::operator=(Stack&& other) noexcept
	{
		if (this != &other)
		{
			delete[] m_pStackArray;

			m_pStackArray = other.m_pStackArray;
			m_capacity = other.m_capacity;
			m_top = other.m_top;

			other.m_pStackArray = nullptr;
			other.m_capacity = 0;
			other.m_top = -1;
		}
		return *this;

	}

	/*---------------------------------------------
	| --- Push: Pushes an item onto the stack --- |
	---------------------------------------------*/
	template<typename T>
	inline void Stack<T>::Push(const T& item)
	{
		if (IsFull())
		{
			throw std::overflow_error("Stack overflow: cannot push onto a full stack");
		}

		m_pStackArray[++m_top] = item;
	}

	/*------------------------------------------------------------------
	| --- Push: Pushes an item onto the stack using move semantics --- |
	------------------------------------------------------------------*/
	template<typename T>
	inline void Stack<T>::Push(T&& item)
	{
		if (IsFull())
		{
			throw std::overflow_error("Stack overflow: cannot push onto a full stack");
		}

		m_pStackArray[++m_top] = std::move(item);
	}

	/*-----------------------------------------------
	| --- Pop: Pops the top item from the stack --- |
	-----------------------------------------------*/
	template<typename T>
	inline void Stack<T>::Pop()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Stack underflow: cannot pop from empty stack");
		}

		--m_top;
	}

	/*---------------------------------------------------------------
	| --- Top: Returns a reference to the top item on the stack --- |
	---------------------------------------------------------------*/
	template<typename T>
	inline T& Stack<T>::Top()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Stack is empty: no top element");
		}

		return m_pStackArray[m_top];
	}

	/*---------------------------------------------------------------------
	| --- Top: Returns a const reference to the top item on the stack --- |
	---------------------------------------------------------------------*/
	template<typename T>
	inline const T& Stack<T>::Top() const
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Stack is empty: no top element");
		}

		return m_pStackArray[m_top];
	}

	/*---------------------------------------------
	| --- IsFull: Checks if the stack is full --- |
	---------------------------------------------*/
	template<typename T>
	inline bool Stack<T>::IsFull() const
	{
		return m_top == static_cast<int>(m_capacity) - 1;
	}

	/*-----------------------------------------------
	| --- IsEmpty: Checks if the stack is empty --- |
	-----------------------------------------------*/
	template<typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return m_top == -1;
	}

	/*---------------------------------------------------------------------------
	| --- Clear: Clears all items from the stack by resetting the top index --- |
	---------------------------------------------------------------------------*/
	template<typename T>
	inline void Stack<T>::Clear()
	{
		m_top = -1;
	}
}