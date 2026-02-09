#pragma once
#include <iostream>
#include <stdexcept>
#include <utility>

namespace CE
{
	/*-------------------------------------------------------------------------------------------------------------------
	| --- A queue may be implemented as a linked list, and expand or shrink with each enqueue or dequeue operation. --- |
	-------------------------------------------------------------------------------------------------------------------*/
	template <typename T>
	class DynamicQueue
	{
	private:
		struct QueueNode
		{
			T value;														// The value stored in the node
			QueueNode* pNext;												// Pointer to the next node in the queue

			QueueNode(const T& val)											// Node Constructor (copy)
				: value(val)
				, pNext(nullptr)
			{ }

			QueueNode(T&& val)												// Node Constructor (move)
				: value(std::move(val))
				, pNext(nullptr)
			{ }
		};

		QueueNode* m_pFront;												// Pointer to the front node of the queue
		QueueNode* m_pRear;													// Pointer to the rear node of the queue
		size_t m_size;														// Number of elements in the queue

		void DestroyQueue();												// Helper function to destroy all nodes and free memory

	public:
		DynamicQueue();														// Constructor
		~DynamicQueue();													// Destructor
		DynamicQueue(const DynamicQueue& other);							// Copy Constructor
		DynamicQueue& operator=(const DynamicQueue& other);					// Copy Assignment Operator
		DynamicQueue(DynamicQueue&& other) noexcept;						// Move Constructor
		DynamicQueue& operator=(DynamicQueue&& other) noexcept;				// Move Assignment Operator

		void Enqueue(const T& item);										// Adds an item to the rear of the queue (copy)
		void Enqueue(T&& item);												// Adds an item to the rear of the queue (move)
		void Dequeue();														// Removes the front item from the queue
		T& Front();															// Returns a reference to the front item in the queue
		const T& Front() const;												// Returns a const reference to the front item in the queue
		T& Back();															// Returns a reference to the back item in the queue
		const T& Back() const;												// Returns a const reference to the back item in the queue

		bool IsEmpty() const;												// Checks if the queue is empty
		size_t GetSize() const		{ return m_size; }						// Returns the number of elements in the queue

		void Clear();														// Clears all items from the queue
	};

	/*----------------------------------------------------------------------
	| --- Constructor: Constructs the DynamicQueue with default values --- |
	----------------------------------------------------------------------*/
	template<typename T>
	inline DynamicQueue<T>::DynamicQueue()
		: m_pFront(nullptr)
		, m_pRear(nullptr)
		, m_size(0)
	{ }

	/*-------------------------------------------------------
	| --- Destructor: Cleans up any allocated resources --- |
	-------------------------------------------------------*/
	template<typename T>
	inline DynamicQueue<T>::~DynamicQueue()
	{
		DestroyQueue();
	}

	/*--------------------------------------------------------------------
	| --- Copy Constructor: Creates a deep copy of the dynamic queue --- |
	--------------------------------------------------------------------*/
	template<typename T>
	inline DynamicQueue<T>::DynamicQueue(const DynamicQueue& other)
		: m_pFront(nullptr)
		, m_pRear(nullptr)
		, m_size(0)
	{
		if (other.m_pFront == nullptr)
			return;

		QueueNode* pCurrent = other.m_pFront;

		while (pCurrent)
		{
			Enqueue(pCurrent->value);
			pCurrent = pCurrent->pNext;
		}
	}

	/*---------------------------------------------------------------------------------------------------------
	| --- Copy Assignment Operator: Creates a deep copy of the dynamic queue and assigns it to this queue --- |
	---------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline DynamicQueue<T>& DynamicQueue<T>::operator=(const DynamicQueue& other)
	{
		if (this != &other)
		{
			// Create temporary copy
			DynamicQueue temp(other);

			// Swap with temporary
			std::swap(m_pFront, temp.m_pFront);
			std::swap(m_pRear, temp.m_pRear);
			std::swap(m_size, temp.m_size);
		}
		return *this;
	}

	/*------------------------------------------------------------------------------------
	| --- Move Constructor: Moves resources from another dynamic queue to this queue --- |
	------------------------------------------------------------------------------------*/
	template<typename T>
	inline DynamicQueue<T>::DynamicQueue(DynamicQueue&& other) noexcept
		: m_pFront(other.m_pFront)
		, m_pRear(other.m_pRear)
		, m_size(other.m_size)
	{
		other.m_pFront = nullptr;
		other.m_pRear = nullptr;
		other.m_size = 0;
	}

	/*-----------------------------------------------------------------------------------------------------------
	| --- Move Assignment Operator: Moves resources from another dynamic queue and assigns it to this queue --- |
	-----------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline DynamicQueue<T>& DynamicQueue<T>::operator=(DynamicQueue&& other) noexcept
	{
		if (this != &other)
		{
			DestroyQueue();

			m_pFront = other.m_pFront;
			m_pRear = other.m_pRear;
			m_size = other.m_size;

			other.m_pFront = nullptr;
			other.m_pRear = nullptr;
			other.m_size = 0;
		}
		return *this;
	}


	/*-------------------------------------
	| --- Private Methods Definitions --- |
	-------------------------------------*/
	/*-----------------------------------------------------------------
	| --- DestroyQueue: Destroys all nodes and frees their memory --- |
	-----------------------------------------------------------------*/
	template<typename T>
	inline void DynamicQueue<T>::DestroyQueue()
	{
		QueueNode* pCurrent = m_pFront;
		QueueNode* pNext = nullptr;

		while (pCurrent)
		{
			pNext = pCurrent->pNext;
			delete pCurrent;
			pCurrent = pNext;
		}

		m_pFront = nullptr;
		m_pRear = nullptr;
		m_size = 0;
	}


	/*------------------------------------
	| --- Public Methods Definitions --- |
	------------------------------------*/
	/*--------------------------------------------------------
	| --- Enqueue: Adds an item to the rear of the queue --- |
	--------------------------------------------------------*/
	template<typename T>
	inline void DynamicQueue<T>::Enqueue(const T& item)
	{
		QueueNode* pNewNode = new QueueNode(item);

		if (IsEmpty())
		{
			m_pFront = pNewNode;
			m_pRear = pNewNode;
		}
		else
		{
			m_pRear->pNext = pNewNode;
			m_pRear = pNewNode;
		}

		++m_size;
	}

	/*-----------------------------------------------------------------------------
	| --- Enqueue: Adds an item to the rear of the queue using move semantics --- |
	-----------------------------------------------------------------------------*/
	template<typename T>
	inline void DynamicQueue<T>::Enqueue(T&& item)
	{
		QueueNode* pNewNode = new QueueNode(std::move(item));

		if (IsEmpty())
		{
			m_pFront = pNewNode;
			m_pRear = pNewNode;
		}
		else
		{
			m_pRear->pNext = pNewNode;
			m_pRear = pNewNode;
		}

		++m_size;
	}

	/*--------------------------------------------------------
	| --- Dequeue: Removes the front item from the queue --- |
	--------------------------------------------------------*/
	template<typename T>
	inline void DynamicQueue<T>::Dequeue()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue underflow: cannot dequeue from empty queue");
		}

		QueueNode* pTemp = m_pFront;
		m_pFront = m_pFront->pNext;

		// If queue is now empty, reset rear pointer
		if (m_pFront == nullptr)
		{
			m_pRear = nullptr;
		}

		delete pTemp;
		--m_size;
	}

	/*-------------------------------------------------------------------
	| --- Front: Returns a reference to the front item in the queue --- |
	-------------------------------------------------------------------*/
	template<typename T>
	inline T& DynamicQueue<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no front element");
		}

		return m_pFront->value;
	}

	/*-------------------------------------------------------------------------
	| --- Front: Returns a const reference to the front item in the queue --- |
	-------------------------------------------------------------------------*/
	template<typename T>
	inline const T& DynamicQueue<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no front element");
		}

		return m_pFront->value;
	}

	/*-----------------------------------------------------------------
	| --- Back: Returns a reference to the back item in the queue --- |
	-----------------------------------------------------------------*/
	template<typename T>
	inline T& DynamicQueue<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no back element");
		}

		return m_pRear->value;
	}

	/*-----------------------------------------------------------------------
	| --- Back: Returns a const reference to the back item in the queue --- |
	-----------------------------------------------------------------------*/
	template<typename T>
	inline const T& DynamicQueue<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no back element");
		}

		return m_pRear->value;
	}

	/*-----------------------------------------------
	| --- IsEmpty: Checks if the queue is empty --- |
	-----------------------------------------------*/
	template<typename T>
	inline bool DynamicQueue<T>::IsEmpty() const
	{
		return m_pFront == nullptr;
	}

	/*------------------------------------------------
	| --- Clear: Clears all items from the queue --- |
	------------------------------------------------*/
	template<typename T>
	inline void DynamicQueue<T>::Clear()
	{
		DestroyQueue();
	}
}