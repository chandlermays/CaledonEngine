#pragma once
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <utility>

namespace CE
{
	/*-------------------------------------------------------------------------------------------------------------
	| --- A queue is a data structure that stores and retrieves items in a first-in, first-out (FIFO) manner. --- |
	-------------------------------------------------------------------------------------------------------------*/
	template <typename T>
	class Queue
	{
	private:
		T* m_pQueueArray;													// Dynamic array to hold queue elements
		size_t m_capacity;													// Maximum capacity of the queue
		int m_front;														// Index of the front element in the queue
		int m_rear;															// Index of the rear element in the queue
		size_t m_size;														// Number of elements currently in the queue

	public:
		explicit Queue(size_t capacity);									// Constructor
		~Queue();															// Destructor
		Queue(const Queue& other);											// Copy Constructor
		Queue& operator=(const Queue& other);								// Copy Assignment Operator
		Queue(Queue&& other) noexcept;										// Move Constructor
		Queue& operator=(Queue&& other) noexcept;							// Move Assignment Operator

		void Enqueue(const T& item);										// Adds an item to the rear of the queue
		void Enqueue(T&& item);												// Adds an item to the rear of the queue using move semantics
		void Dequeue();														// Removes the front item from the queue
		T& Front();															// Returns a reference to the front item in the queue
		const T& Front() const;												// Returns a const reference to the front item in the queue
		T& Back();															// Returns a reference to the back item in the queue
		const T& Back() const;												// Returns a const reference to the back item in the queue

		bool IsFull() const;												// Checks if the queue is full
		bool IsEmpty() const;												// Checks if the queue is empty
		size_t GetSize() const			{ return m_size; }					// Returns the number of elements in the queue
		size_t GetCapacity() const		{ return m_capacity; }				// Returns the maximum capacity of the queue

		void Clear();														// Clears all items from the queue
	};

	/*---------------------------------------------------------------
	| --- Constructor: Constructs the Queue with default values --- |
	---------------------------------------------------------------*/
	template<typename T>
	inline Queue<T>::Queue(size_t capacity)
		: m_pQueueArray(capacity > 0 ? new T[capacity] : nullptr)
		, m_capacity(capacity)
		, m_front(0)
		, m_rear(-1)
		, m_size(0)
	{
		if (capacity == 0)
		{
			throw std::invalid_argument("Queue capacity must be greater than 0");
		}
	}

	/*-------------------------------------------------------
	| --- Destructor: Cleans up any allocated resources --- |
	-------------------------------------------------------*/
	template<typename T>
	inline Queue<T>::~Queue()
	{
		delete[] m_pQueueArray;
	}

	/*------------------------------------------------------------
	| --- Copy Constructor: Creates a deep copy of the queue --- |
	------------------------------------------------------------*/
	template<typename T>
	inline Queue<T>::Queue(const Queue& other)
		: m_pQueueArray(other.m_capacity > 0 ? new T[other.m_capacity] : nullptr)
		, m_capacity(other.m_capacity)
		, m_front(other.m_front)
		, m_rear(other.m_rear)
		, m_size(other.m_size)
	{
		for (size_t i = 0; i < m_capacity; ++i)
		{
			m_pQueueArray[i] = other.m_pQueueArray[i];
		}
	}

	/*-------------------------------------------------------------------------------------------------
	| --- Copy Assignment Operator: Creates a deep copy of the queue and assigns it to this queue --- |
	-------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline Queue<T>& Queue<T>::operator=(const Queue& other)
	{
		if (this != &other)
		{
			T* newArray = other.m_capacity > 0 ? new T[other.m_capacity] : nullptr;

			for (size_t i = 0; i < other.m_capacity; ++i)
			{
				newArray[i] = other.m_pQueueArray[i];
			}

			delete[] m_pQueueArray;

			m_pQueueArray = newArray;
			m_capacity = other.m_capacity;
			m_front = other.m_front;
			m_rear = other.m_rear;
			m_size = other.m_size;
		}
		return *this;
	}

	/*----------------------------------------------------------------------------
	| --- Move Constructor: Moves resources from another queue to this queue --- |
	----------------------------------------------------------------------------*/
	template<typename T>
	inline Queue<T>::Queue(Queue&& other) noexcept
		: m_pQueueArray(other.m_pQueueArray)
		, m_capacity(other.m_capacity)
		, m_front(other.m_front)
		, m_rear(other.m_rear)
		, m_size(other.m_size)
	{
		other.m_pQueueArray = nullptr;
		other.m_capacity = 0;
		other.m_front = 0;
		other.m_rear = -1;
		other.m_size = 0;
	}

	/*---------------------------------------------------------------------------------------------------
	| --- Move Assignment Operator: Moves resources from another queue and assigns it to this queue --- |
	---------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline Queue<T>& Queue<T>::operator=(Queue&& other) noexcept
	{
		if (this != &other)
		{
			delete[] m_pQueueArray;

			m_pQueueArray = other.m_pQueueArray;
			m_capacity = other.m_capacity;
			m_front = other.m_front;
			m_rear = other.m_rear;
			m_size = other.m_size;

			other.m_pQueueArray = nullptr;
			other.m_capacity = 0;
			other.m_front = 0;
			other.m_rear = -1;
			other.m_size = 0;
		}
		return *this;
	}


	/*------------------------------------
	| --- Public Methods Definitions --- |
	------------------------------------*/
	/*--------------------------------------------------------
	| --- Enqueue: Adds an item to the rear of the queue --- |
	--------------------------------------------------------*/
	template<typename T>
	inline void Queue<T>::Enqueue(const T& item)
	{
		if (IsFull())
		{
			throw std::overflow_error("Queue overflow: cannot enqueue to full queue");
		}

		m_rear = (m_rear + 1) % static_cast<int>(m_capacity);
		m_pQueueArray[m_rear] = item;
		++m_size;
	}

	/*-----------------------------------------------------------------------------
	| --- Enqueue: Adds an item to the rear of the queue using move semantics --- |
	-----------------------------------------------------------------------------*/
	template<typename T>
	inline void Queue<T>::Enqueue(T&& item)
	{
		if (IsFull())
		{
			throw std::overflow_error("Queue overflow: cannot enqueue to full queue");
		}

		m_rear = (m_rear + 1) % static_cast<int>(m_capacity);
		m_pQueueArray[m_rear] = std::move(item);
		++m_size;
	}

	/*--------------------------------------------------------
	| --- Dequeue: Removes the front item from the queue --- |
	--------------------------------------------------------*/
	template<typename T>
	inline void Queue<T>::Dequeue()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue underflow: cannot dequeue from empty queue");
		}

		m_front = (m_front + 1) % static_cast<int>(m_capacity);
		--m_size;
	}

	/*-------------------------------------------------------------------
	| --- Front: Returns a reference to the front item in the queue --- |
	-------------------------------------------------------------------*/
	template<typename T>
	inline T& Queue<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no front element");
		}

		return m_pQueueArray[m_front];
	}

	/*-------------------------------------------------------------------------
	| --- Front: Returns a const reference to the front item in the queue --- |
	-------------------------------------------------------------------------*/
	template<typename T>
	inline const T& Queue<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no front element");
		}

		return m_pQueueArray[m_front];
	}

	/*-----------------------------------------------------------------
	| --- Back: Returns a reference to the back item in the queue --- |
	-----------------------------------------------------------------*/
	template<typename T>
	inline T& Queue<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no back element");
		}

		return m_pQueueArray[m_rear];
	}

	/*-----------------------------------------------------------------------
	| --- Back: Returns a const reference to the back item in the queue --- |
	-----------------------------------------------------------------------*/
	template<typename T>
	inline const T& Queue<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::underflow_error("Queue is empty: no back element");
		}

		return m_pQueueArray[m_rear];
	}

	/*---------------------------------------------
	| --- IsFull: Checks if the queue is full --- |
	---------------------------------------------*/
	template<typename T>
	inline bool Queue<T>::IsFull() const
	{
		return m_size == m_capacity;
	}

	/*-----------------------------------------------
	| --- IsEmpty: Checks if the queue is empty --- |
	-----------------------------------------------*/
	template<typename T>
	inline bool Queue<T>::IsEmpty() const
	{
		return m_size == 0;
	}

	/*------------------------------------------------
	| --- Clear: Clears all items from the queue --- |
	------------------------------------------------*/
	template<typename T>
	inline void Queue<T>::Clear()
	{
		m_front = 0;
		m_rear = -1;
		m_size = 0;
	}
}