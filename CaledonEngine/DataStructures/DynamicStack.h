#pragma once
#include <iostream>
#include <stdexcept>

namespace CE
{
	/*------------------------------------------------------------------------------------------------------------
	| --- A stack may be implemented as a linked list, and expand or shrink with each push or pop operation. --- |
	------------------------------------------------------------------------------------------------------------*/
	template <typename T>
	class DynamicStack
	{
	private:
		struct StackNode
		{
			T value;														// The value stored in the node
			StackNode* pNext;												// Pointer to the next node in the stack

			StackNode(const T& val)											// Node Constructor (copy)
				: value(val)
				, pNext(nullptr)
			{ }

			StackNode(T&& val)												// Node Constructor (move)
				: value(std::move(val))
				, pNext(nullptr)
			{ }
		};

		StackNode* m_pTop;													// Pointer to the top node of the stack
		size_t m_size;														// Number of elements in the stack

		void DestroyStack();												// Helper function to destroy all nodes and free memory

	public:
		DynamicStack();														// Constructor
		~DynamicStack();													// Destructor
		DynamicStack(const DynamicStack& other);							// Copy Constructor
		DynamicStack& operator=(const DynamicStack& other);					// Copy Assignment Operator
		DynamicStack(DynamicStack&& other) noexcept;						// Move Constructor
		DynamicStack& operator=(DynamicStack&& other) noexcept;				// Move Assignment Operator

		void Push(const T& item);											// Pushes an item onto the stack (copy)
		void Push(T&& item);												// Pushes an item onto the stack (move)
		void Pop();															// Pops the top item from the stack
		T& Top();															// Returns a reference to the top item on the stack
		const T& Top() const;												// Returns a const reference to the top item on the stack

		bool IsEmpty() const;												// Checks if the stack is empty
		size_t Size() const		{ return m_size; }							// Returns the number of elements in the stack

		void Clear();														// Clears all items from the stack
	};

	/*----------------------------------------------------------------------
	| --- Constructor: Constructs the DynamicStack with default values --- |
	----------------------------------------------------------------------*/
	template<typename T>
	inline DynamicStack<T>::DynamicStack()
		: m_pTop{ nullptr }
		, m_size{ 0 }
	{ }

	/*-------------------------------------------------------
	| --- Destructor: Cleans up any allocated resources --- |
	-------------------------------------------------------*/
	template<typename T>
	inline DynamicStack<T>::~DynamicStack()
	{
		DestroyStack();
	}

	/*--------------------------------------------------------------------
	| --- Copy Constructor: Creates a deep copy of the dynamic stack --- |
	--------------------------------------------------------------------*/
	template<typename T>
	inline DynamicStack<T>::DynamicStack(const DynamicStack& other)
		: m_pTop{ nullptr }
		, m_size{ 0 }
	{
		if (other.m_pTop == nullptr)
			return;

		// Create a temporary stack to reverse the order
		StackNode* pCurrent = other.m_pTop;
		StackNode* pPrevious = nullptr;
		StackNode* pTemp = nullptr;

		// First pass: create nodes in reverse order
		while (pCurrent)
		{
			StackNode* pNewNode = new StackNode(pCurrent->value);
			pNewNode->pNext = pPrevious;
			pPrevious = pNewNode;
			pCurrent = pCurrent->pNext;
		}

		// Second pass: reverse back to original order
		pCurrent = pPrevious;
		pPrevious = nullptr;

		while (pCurrent)
		{
			pTemp = pCurrent->pNext;
			pCurrent->pNext = pPrevious;
			pPrevious = pCurrent;
			pCurrent = pTemp;
		}

		m_pTop = pPrevious;
		m_size = other.m_size;
	}

	/*---------------------------------------------------------------------------------------------------------
	| --- Copy Assignment Operator: Creates a deep copy of the dynamic stack and assigns it to this stack --- |
	---------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline DynamicStack<T>& DynamicStack<T>::operator=(const DynamicStack& other)
	{
		if (this != &other)
		{
			DynamicStack temp(other);

			std::swap(m_pTop, temp.m_pTop);
			std::swap(m_size, temp.m_size);
		}
		return *this;
	}

	/*------------------------------------------------------------------------------------
	| --- Move Constructor: Moves resources from another dynamic stack to this stack --- |
	------------------------------------------------------------------------------------*/
	template<typename T>
	inline DynamicStack<T>::DynamicStack(DynamicStack&& other) noexcept
		: m_pTop{ other.m_pTop }
		, m_size{ other.m_size }
	{
		other.m_pTop = nullptr;
		other.m_size = 0;
	}

	/*-----------------------------------------------------------------------------------------------------------
	| --- Move Assignment Operator: Moves resources from another dynamic stack and assigns it to this stack --- |
	-----------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline DynamicStack<T>& DynamicStack<T>::operator=(DynamicStack&& other) noexcept
	{
		if (this != &other)
		{
			DestroyStack();

			m_pTop = other.m_pTop;
			m_size = other.m_size;

			other.m_pTop = nullptr;
			other.m_size = 0;
		}
		return *this;
	}


	/*-------------------------------------
	| --- Private Methods Definitions --- |
	-------------------------------------*/
	/*-----------------------------------------------------------------
	| --- DestroyStack: Destroys all nodes and frees their memory --- |
	-----------------------------------------------------------------*/
	template<typename T>
	inline void DynamicStack<T>::DestroyStack()
	{
		while (m_pTop)
		{
			StackNode* pNode = m_pTop;
			m_pTop = m_pTop->pNext;
			delete pNode;
		}

		m_size = 0;
	}


	/*------------------------------------
	| --- Public Methods Definitions --- |
	------------------------------------*/
	/*----------------------------------------------------
	| --- Push: Pushes an item onto the stack (copy) --- |
	----------------------------------------------------*/
	template<typename T>
	inline void DynamicStack<T>::Push(const T& item)
	{
		StackNode* pNewNode = new StackNode(item);
		pNewNode->pNext = m_pTop;
		m_pTop = pNewNode;
		++m_size;
	}

	/*----------------------------------------------------
	| --- Push: Pushes an item onto the stack (move) --- |
	----------------------------------------------------*/
	template<typename T>
	inline void DynamicStack<T>::Push(T&& item)
	{
		StackNode* pNewNode = new StackNode(std::move(item));
		pNewNode->pNext = m_pTop;
		m_pTop = pNewNode;
		++m_size;
	}

	/*-----------------------------------------------
	| --- Pop: Pops the top item from the stack --- |
	-----------------------------------------------*/
	template<typename T>
	inline void DynamicStack<T>::Pop()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Stack underflow: cannot pop from empty stack");
		}

		StackNode* pTemp = m_pTop;
		m_pTop = m_pTop->pNext;
		delete pTemp;
		--m_size;
	}

	/*---------------------------------------------------------------
	| --- Top: Returns a reference to the top item on the stack --- |
	---------------------------------------------------------------*/
	template<typename T>
	inline T& DynamicStack<T>::Top()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Stack underflow: cannot retrieve top from empty stack");
		}

		return m_pTop->value;
	}

	/*---------------------------------------------------------------------
	| --- Top: Returns a const reference to the top item on the stack --- |
	---------------------------------------------------------------------*/
	template<typename T>
	inline const T& DynamicStack<T>::Top() const
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Stack underflow: cannot retrieve top from empty stack");
		}

		return m_pTop->value;
	}

	/*-----------------------------------------------
	| --- IsEmpty: Checks if the stack is empty --- |
	-----------------------------------------------*/
	template<typename T>
	inline bool DynamicStack<T>::IsEmpty() const
	{
		return m_pTop == nullptr;
	}

	/*------------------------------------------------
	| --- Clear: Clears all items from the stack --- |
	------------------------------------------------*/
	template<typename T>
	inline void DynamicStack<T>::Clear()
	{
		DestroyStack();
	}
}