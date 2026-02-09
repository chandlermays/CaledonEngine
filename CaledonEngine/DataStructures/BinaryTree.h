#pragma once
#include <iostream>
#include <functional>

namespace CE
{
	/*------------------------------------------------------------------------------------------------------------------------
	| --- A binary tree is a nonlinear linked structure in which each node may point to two other nodes, and every node  --- |
	| --- but the root node has a single predecessor. Binary trees expedite the process of searching large sets of data. --- |
	------------------------------------------------------------------------------------------------------------------------*/
	template <typename T>
	class BinaryTree
	{
	private:
		struct TreeNode
		{
			T value;													// The value stored in the node
			TreeNode* pLeft;											// Pointer to the left child node
			TreeNode* pRight;											// Pointer to the right child node

			TreeNode(const T& val)										// Node Constructor
				: value(val)
				, pLeft(nullptr)
				, pRight(nullptr)
			{}
		};

		TreeNode* m_pRoot;												// Pointer to the root node of the binary tree

		void Insert(TreeNode*& pNode, TreeNode* pNewNode);				// Helper function to insert a new node into the tree
		void DestroySubTree(TreeNode* pNode);							// Helper function to recursively destroy the tree and free memory
		void DeleteNode(const T& item, TreeNode*& pNode);				// Helper function to delete a node with the specified value from the tree
		void MakeDeletion(TreeNode*& pNode);							// Helper function to handle the deletion of a node and reattach the tree properly
		void DisplayInOrder(TreeNode* pNode) const;						// Helper function to display the tree in order recursively
		void DisplayPreOrder(TreeNode* pNode) const;					// Helper function to display the tree in pre-order recursively
		void DisplayPostOrder(TreeNode* pNode) const;					// Helper function to display the tree in post-order recursively

		TreeNode* CopyTree(TreeNode* pNode);							// Helper function to create a deep copy of the tree structure

	public:
		BinaryTree();													// Constructor
		~BinaryTree();													// Destructor
		BinaryTree(const BinaryTree& other);							// Copy Constructor
		BinaryTree& operator=(const BinaryTree& other);					// Copy Assignment Operator
		BinaryTree(BinaryTree&& other) noexcept;						// Move Constructor
		BinaryTree& operator=(BinaryTree&& other) noexcept;				// Move Assignment Operator

		void InsertNode(const T& item);									// Public function to insert a new value into the binary tree
		bool SearchNode(const T& item) const;							// Public function to search for a value in the binary tree, returns true if found, false otherwise
		void Remove(const T& item);										// Public function to remove a value from the binary tree if it exists
		bool IsEmpty() const { return m_pRoot == nullptr; }				// Public function to check if the binary tree is empty

		void DisplayInOrder() const { DisplayInOrder(m_pRoot); }		// Public function to display the tree in order
		void DisplayPreOrder() const { DisplayPreOrder(m_pRoot); }		// Public function to display the tree in pre-order
		void DisplayPostOrder() const { DisplayPostOrder(m_pRoot); }	// Public function to display the tree in post-order

		template<typename Func>
		void TraverseInOrder(Func func) const;							// Public function to perform a generic in-order traversal with a custom callback function
	};

	/*---------------------------------------------------------------------
	| --- Constructor: Constructs the Binary Tree with default values --- |
	---------------------------------------------------------------------*/
	template<typename T>
	inline BinaryTree<T>::BinaryTree()
		: m_pRoot{ nullptr }
	{ }

	/*-------------------------------------------------------
	| --- Destructor: Cleans up any allocated resources --- |
	-------------------------------------------------------*/
	template<typename T>
	inline BinaryTree<T>::~BinaryTree()
	{
		DestroySubTree(m_pRoot);
	}

	/*------------------------------------------------------------------
	| --- Copy Constructor: Creates a deep copy of the binary tree --- |
	------------------------------------------------------------------*/
	template<typename T>
	inline BinaryTree<T>::BinaryTree(const BinaryTree& other)
		: m_pRoot{ CopyTree(other.m_pRoot) }
	{ }

	/*------------------------------------------------------------------------------------------------------
	| --- Copy Assignment Operator: Creates a deep copy of the binary tree and assigns it to this tree --- |
	------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other)
	{
		if (this != &other)
		{
			DestroySubTree(m_pRoot);
			m_pRoot = CopyTree(other.m_pRoot);
		}
		return *this;
	}

	/*---------------------------------------------------------------------------------
	| --- Move Constructor: Moves resources from another binary tree to this tree --- |
	---------------------------------------------------------------------------------*/
	template<typename T>
	inline BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept
		: m_pRoot{ other.m_pRoot }
	{
		other.m_pRoot = nullptr;
	}

	/*--------------------------------------------------------------------------------------------------------
	| --- Move Assignment Operator: Moves resources from another binary tree and assigns it to this tree --- |
	--------------------------------------------------------------------------------------------------------*/
	template<typename T>
	inline BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& other) noexcept
	{
		if (this != &other)
		{
			DestroySubTree(m_pRoot);
			m_pRoot = other.m_pRoot;
			other.m_pRoot = nullptr;
		}
		return *this;
	}


	/*-------------------------------------
	| --- Private Methods Definitions --- |
	-------------------------------------*/
	/*-------------------------------------------------------
	| --- Insert: Inserts the node into the binary tree --- |
	-------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::Insert(TreeNode*& pNode, TreeNode* pNewNode)
	{
		if (pNode == nullptr)
		{
			pNode = pNewNode;
		}
		else if (pNewNode->value < pNode->value)
		{
			Insert(pNode->pLeft, pNewNode);
		}
		else
		{
			Insert(pNode->pRight, pNewNode);
		}
	}

	/*--------------------------------------------------------------
	| --- DestroySubTree: Destroys the binary tree recursively --- |
	--------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::DestroySubTree(TreeNode* pNode)
	{
		if (pNode)
		{
			DestroySubTree(pNode->pLeft);
			DestroySubTree(pNode->pRight);
			delete pNode;
		}
	}

	/*-----------------------------------------------------------
	| --- DeleteNode: Deletes the node from the binary tree --- |
	-----------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::DeleteNode(const T& item, TreeNode*& pNode)
	{
		if (!pNode)
			return;

		if (item < pNode->value)
		{
			DeleteNode(item, pNode->pLeft);
		}
		else if (item > pNode->value)
		{
			DeleteNode(item, pNode->pRight);
		}
		else
		{
			MakeDeletion(pNode);
		}
	}

	/*----------------------------------------------------------------
	| --- MakeDeletion: Deletes the node and reattaches the tree --- |
	----------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::MakeDeletion(TreeNode*& pNode)
	{
		if (!pNode)
			return;

		TreeNode* pTempNode = nullptr;

		if (pNode->pRight == nullptr)
		{
			pTempNode = pNode;
			pNode = pNode->pLeft;
			delete pTempNode;
		}
		else if (pNode->pLeft == nullptr)
		{
			pTempNode = pNode;
			pNode = pNode->pRight;
			delete pTempNode;
		}
		else
		{
			// Node has two children - find inorder successor (leftmost node in right subtree)
			pTempNode = pNode->pRight;
			while (pTempNode->pLeft)
			{
				pTempNode = pTempNode->pLeft;
			}

			// Attach left subtree to inorder successor
			pTempNode->pLeft = pNode->pLeft;
			pTempNode = pNode;
			pNode = pNode->pRight;
			delete pTempNode;
		}
	}

	/*-------------------------------------------------------------
	| --- CopyTree: Creates a deep copy of the tree structure --- |
	-------------------------------------------------------------*/
	template<typename T>
	inline typename BinaryTree<T>::TreeNode* BinaryTree<T>::CopyTree(TreeNode* pNode)
	{
		if (!pNode)
			return nullptr;

		TreeNode* pNewNode = new TreeNode(pNode->value);
		pNewNode->pLeft = CopyTree(pNode->pLeft);
		pNewNode->pRight = CopyTree(pNode->pRight);
		return pNewNode;
	}

	/*----------------------------------------------------------------
	| --- DisplayInOrder: Displays the tree in order recursively --- |
	----------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::DisplayInOrder(TreeNode* pNode) const
	{
		if (pNode)
		{
			DisplayInOrder(pNode->pLeft);
			std::cout << pNode->value << std::endl;
			DisplayInOrder(pNode->pRight);
		}
	}

	/*---------------------------------------------------------------------
	| --- DisplayPreOrder: Displays the tree in pre-order recursively --- |
	---------------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::DisplayPreOrder(TreeNode* pNode) const
	{
		if (pNode)
		{
			std::cout << pNode->value << std::endl;
			DisplayPreOrder(pNode->pLeft);
			DisplayPreOrder(pNode->pRight);
		}
	}

	/*-----------------------------------------------------------------------
	| --- DisplayPostOrder: Displays the tree in post-order recursively --- |
	-----------------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::DisplayPostOrder(TreeNode* pNode) const
	{
		if (pNode)
		{
			DisplayPostOrder(pNode->pLeft);
			DisplayPostOrder(pNode->pRight);
			std::cout << pNode->value << std::endl;
		}
	}


	/*------------------------------------
	| --- Public Methods Definitions --- |
	------------------------------------*/
	/*----------------------------------------------------------------------------
	| --- InsertNode: Creates a new node and inserts it into the binary tree --- |
	----------------------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::InsertNode(const T& item)
	{
		TreeNode* pNewNode = new TreeNode(item);
		Insert(m_pRoot, pNewNode);
	}

	/*----------------------------------------------------------------------
	| --- SearchNode: Searches the binary tree for the specified value --- |
	----------------------------------------------------------------------*/
	template<typename T>
	inline bool BinaryTree<T>::SearchNode(const T& item) const
	{
		TreeNode* pNode = m_pRoot;

		while (pNode)
		{
			if (pNode->value == item)
			{
				return true;
			}
			else if (item < pNode->value)
			{
				pNode = pNode->pLeft;
			}
			else
			{
				pNode = pNode->pRight;
			}
		}

		return false;
	}

	/*-------------------------------------------------------------------------------
	| --- Remove: Deletes the specified value from the binary tree if it exists --- |
	-------------------------------------------------------------------------------*/
	template<typename T>
	inline void BinaryTree<T>::Remove(const T& item)
	{
		DeleteNode(item, m_pRoot);
	}

	/*--------------------------------------------------------------------------
	| --- TraverseInOrder: Generic traversal with custom callback function --- |
	--------------------------------------------------------------------------*/
	template<typename T>
	template<typename Func>
	inline void BinaryTree<T>::TraverseInOrder(Func func) const
	{
		std::function<void(TreeNode*)> traverse = [&](TreeNode* pNode)
			{
				if (pNode)
				{
					traverse(pNode->pLeft);
					func(pNode->value);
					traverse(pNode->pRight);
				}
			};
		traverse(m_pRoot);
	}
}