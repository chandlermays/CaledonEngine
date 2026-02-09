#include <iostream>
#include <string>
#include <cassert>
#include "CaledonEngine/DataStructures/Stack.h"
#include "CaledonEngine/DataStructures/Queue.h"
#include "CaledonEngine/DataStructures/DynamicStack.h"
#include "CaledonEngine/DataStructures/DynamicQueue.h"
#include "CaledonEngine/DataStructures/BinaryTree.h"

using namespace CE;

// Color codes for terminal output
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

// Test counters
int totalTests = 0;
int passedTests = 0;

// Helper function to print test results
void PrintTestResult(const std::string& testName, bool passed)
{
	totalTests++;
	if (passed)
	{
		passedTests++;
		std::cout << GREEN << "[PASS] " << RESET << testName << std::endl;
	}
	else
	{
		std::cout << RED << "[FAIL] " << RESET << testName << std::endl;
	}
}

// Helper function to print section headers
void PrintSection(const std::string& section)
{
	std::cout << "\n" << BLUE << "========== " << section << " ==========" << RESET << std::endl;
}

//=============================================================================
// STACK TESTS
//=============================================================================
void TestStackBasicOperations()
{
	PrintSection("Stack: Basic Operations");

	Stack<int> stack(5);

	// Test IsEmpty on new stack
	PrintTestResult("Stack IsEmpty (initially)", stack.IsEmpty());

	// Test Push
	stack.Push(10);
	stack.Push(20);
	stack.Push(30);
	PrintTestResult("Stack Push (3 items)", stack.Size() == 3);

	// Test Top
	PrintTestResult("Stack Top", stack.Top() == 30);

	// Test Pop
	stack.Pop();
	PrintTestResult("Stack Pop", stack.Top() == 20 && stack.Size() == 2);

	// Test IsFull
	stack.Push(30);
	stack.Push(40);
	stack.Push(50);
	PrintTestResult("Stack IsFull", stack.IsFull() && stack.Size() == 5);
}

void TestStackCopySemantics()
{
	PrintSection("Stack: Copy Semantics");

	Stack<int> stack1(5);
	stack1.Push(1);
	stack1.Push(2);
	stack1.Push(3);

	// Test Copy Constructor
	Stack<int> stack2(stack1);
	PrintTestResult("Stack Copy Constructor (size)", stack2.Size() == 3);
	PrintTestResult("Stack Copy Constructor (top)", stack2.Top() == 3);

	// Verify deep copy
	stack1.Pop();
	PrintTestResult("Stack Deep Copy (independence)", stack2.Top() == 3 && stack2.Size() == 3);

	// Test Copy Assignment
	Stack<int> stack3(5);
	stack3 = stack2;
	PrintTestResult("Stack Copy Assignment (size)", stack3.Size() == 3);
	PrintTestResult("Stack Copy Assignment (top)", stack3.Top() == 3);
}

void TestStackMoveSemantics()
{
	PrintSection("Stack: Move Semantics");

	Stack<int> stack1(5);
	stack1.Push(10);
	stack1.Push(20);
	stack1.Push(30);

	// Test Move Constructor
	Stack<int> stack2(std::move(stack1));
	PrintTestResult("Stack Move Constructor (size)", stack2.Size() == 3);
	PrintTestResult("Stack Move Constructor (top)", stack2.Top() == 30);

	// Test Move Assignment
	Stack<int> stack3(5);
	stack3 = std::move(stack2);
	PrintTestResult("Stack Move Assignment (size)", stack3.Size() == 3);
	PrintTestResult("Stack Move Assignment (top)", stack3.Top() == 30);
}

void TestStackExceptions()
{
	PrintSection("Stack: Exception Handling");

	Stack<int> stack(3);

	// Test Pop on empty stack
	bool popException = false;
	try
	{
		stack.Pop();
	}
	catch (const std::underflow_error&)
	{
		popException = true;
	}
	PrintTestResult("Stack Pop Exception (empty)", popException);

	// Test Top on empty stack
	bool topException = false;
	try
	{
		stack.Top();
	}
	catch (const std::underflow_error&)
	{
		topException = true;
	}
	PrintTestResult("Stack Top Exception (empty)", topException);
}

void TestStackClear()
{
	PrintSection("Stack: Clear Operation");

	Stack<int> stack(5);
	stack.Push(1);
	stack.Push(2);
	stack.Push(3);

	stack.Clear();
	PrintTestResult("Stack Clear", stack.IsEmpty() && stack.Size() == 0);
}

//=============================================================================
// QUEUE TESTS
//=============================================================================
void TestQueueBasicOperations()
{
	PrintSection("Queue: Basic Operations");

	Queue<int> queue(5);

	// Test IsEmpty on new queue
	PrintTestResult("Queue IsEmpty (initially)", queue.IsEmpty());

	// Test Enqueue
	queue.Enqueue(10);
	queue.Enqueue(20);
	queue.Enqueue(30);
	PrintTestResult("Queue Enqueue (3 items)", queue.GetSize() == 3);

	// Test Front and Back
	PrintTestResult("Queue Front", queue.Front() == 10);
	PrintTestResult("Queue Back", queue.Back() == 30);

	// Test Dequeue
	queue.Dequeue();
	PrintTestResult("Queue Dequeue", queue.Front() == 20 && queue.GetSize() == 2);

	// Test IsFull
	queue.Enqueue(40);
	queue.Enqueue(50);
	queue.Enqueue(60);
	PrintTestResult("Queue IsFull", queue.IsFull() && queue.GetSize() == 5);
}

void TestQueueCircularBuffer()
{
	PrintSection("Queue: Circular Buffer");

	Queue<int> queue(3);
	queue.Enqueue(1);
	queue.Enqueue(2);
	queue.Enqueue(3);

	queue.Dequeue();
	queue.Dequeue();

	queue.Enqueue(4);
	queue.Enqueue(5);

	PrintTestResult("Queue Circular (front)", queue.Front() == 3);
	PrintTestResult("Queue Circular (back)", queue.Back() == 5);
	PrintTestResult("Queue Circular (size)", queue.GetSize() == 3);
}

void TestQueueCopySemantics()
{
	PrintSection("Queue: Copy Semantics");

	Queue<int> queue1(5);
	queue1.Enqueue(1);
	queue1.Enqueue(2);
	queue1.Enqueue(3);

	// Test Copy Constructor
	Queue<int> queue2(queue1);
	PrintTestResult("Queue Copy Constructor (size)", queue2.GetSize() == 3);
	PrintTestResult("Queue Copy Constructor (front)", queue2.Front() == 1);

	// Verify deep copy
	queue1.Dequeue();
	PrintTestResult("Queue Deep Copy (independence)", queue2.Front() == 1 && queue2.GetSize() == 3);

	// Test Copy Assignment
	Queue<int> queue3(5);
	queue3 = queue2;
	PrintTestResult("Queue Copy Assignment (size)", queue3.GetSize() == 3);
	PrintTestResult("Queue Copy Assignment (front)", queue3.Front() == 1);
}

void TestQueueMoveSemantics()
{
	PrintSection("Queue: Move Semantics");

	Queue<int> queue1(5);
	queue1.Enqueue(10);
	queue1.Enqueue(20);
	queue1.Enqueue(30);

	// Test Move Constructor
	Queue<int> queue2(std::move(queue1));
	PrintTestResult("Queue Move Constructor (size)", queue2.GetSize() == 3);
	PrintTestResult("Queue Move Constructor (front)", queue2.Front() == 10);

	// Test Move Assignment
	Queue<int> queue3(5);
	queue3 = std::move(queue2);
	PrintTestResult("Queue Move Assignment (size)", queue3.GetSize() == 3);
	PrintTestResult("Queue Move Assignment (front)", queue3.Front() == 10);
}

void TestQueueExceptions()
{
	PrintSection("Queue: Exception Handling");

	Queue<int> queue(3);

	// Test Dequeue on empty queue
	bool dequeueException = false;
	try
	{
		queue.Dequeue();
	}
	catch (const std::underflow_error&)
	{
		dequeueException = true;
	}
	PrintTestResult("Queue Dequeue Exception (empty)", dequeueException);

	// Test Front on empty queue
	bool frontException = false;
	try
	{
		queue.Front();
	}
	catch (const std::underflow_error&)
	{
		frontException = true;
	}
	PrintTestResult("Queue Front Exception (empty)", frontException);

	// Test Enqueue on full queue
	queue.Enqueue(1);
	queue.Enqueue(2);
	queue.Enqueue(3);

	bool enqueueException = false;
	try
	{
		queue.Enqueue(4);
	}
	catch (const std::overflow_error&)
	{
		enqueueException = true;
	}
	PrintTestResult("Queue Enqueue Exception (full)", enqueueException);
}

//=============================================================================
// DYNAMIC STACK TESTS
//=============================================================================
void TestDynamicStackBasicOperations()
{
	PrintSection("DynamicStack: Basic Operations");

	DynamicStack<int> stack;

	// Test IsEmpty on new stack
	PrintTestResult("DynamicStack IsEmpty (initially)", stack.IsEmpty());

	// Test Push
	stack.Push(10);
	stack.Push(20);
	stack.Push(30);
	PrintTestResult("DynamicStack Push (3 items)", stack.Size() == 3);

	// Test Top
	PrintTestResult("DynamicStack Top", stack.Top() == 30);

	// Test Pop
	stack.Pop();
	PrintTestResult("DynamicStack Pop", stack.Top() == 20 && stack.Size() == 2);
}

void TestDynamicStackCopySemantics()
{
	PrintSection("DynamicStack: Copy Semantics");

	DynamicStack<int> stack1;
	stack1.Push(1);
	stack1.Push(2);
	stack1.Push(3);

	// Test Copy Constructor
	DynamicStack<int> stack2(stack1);
	PrintTestResult("DynamicStack Copy Constructor (size)", stack2.Size() == 3);
	PrintTestResult("DynamicStack Copy Constructor (top)", stack2.Top() == 3);

	// Verify order is preserved
	stack2.Pop();
	PrintTestResult("DynamicStack Copy Order (1)", stack2.Top() == 2);
	stack2.Pop();
	PrintTestResult("DynamicStack Copy Order (2)", stack2.Top() == 1);

	// Verify deep copy
	PrintTestResult("DynamicStack Deep Copy (independence)", stack1.Size() == 3);

	// Test Copy Assignment
	DynamicStack<int> stack3;
	stack3 = stack1;
	PrintTestResult("DynamicStack Copy Assignment (size)", stack3.Size() == 3);
	PrintTestResult("DynamicStack Copy Assignment (top)", stack3.Top() == 3);
}

void TestDynamicStackMoveSemantics()
{
	PrintSection("DynamicStack: Move Semantics");

	DynamicStack<int> stack1;
	stack1.Push(10);
	stack1.Push(20);
	stack1.Push(30);

	// Test Move Constructor
	DynamicStack<int> stack2(std::move(stack1));
	PrintTestResult("DynamicStack Move Constructor (size)", stack2.Size() == 3);
	PrintTestResult("DynamicStack Move Constructor (top)", stack2.Top() == 30);

	// Test Move Assignment
	DynamicStack<int> stack3;
	stack3 = std::move(stack2);
	PrintTestResult("DynamicStack Move Assignment (size)", stack3.Size() == 3);
	PrintTestResult("DynamicStack Move Assignment (top)", stack3.Top() == 30);
}

void TestDynamicStackClear()
{
	PrintSection("DynamicStack: Clear Operation");

	DynamicStack<int> stack;
	stack.Push(1);
	stack.Push(2);
	stack.Push(3);

	stack.Clear();
	PrintTestResult("DynamicStack Clear", stack.IsEmpty() && stack.Size() == 0);
}

//=============================================================================
// DYNAMIC QUEUE TESTS
//=============================================================================
void TestDynamicQueueBasicOperations()
{
	PrintSection("DynamicQueue: Basic Operations");

	DynamicQueue<int> queue;

	// Test IsEmpty on new queue
	PrintTestResult("DynamicQueue IsEmpty (initially)", queue.IsEmpty());

	// Test Enqueue
	queue.Enqueue(10);
	queue.Enqueue(20);
	queue.Enqueue(30);
	PrintTestResult("DynamicQueue Enqueue (3 items)", queue.GetSize() == 3);

	// Test Front and Back
	PrintTestResult("DynamicQueue Front", queue.Front() == 10);
	PrintTestResult("DynamicQueue Back", queue.Back() == 30);

	// Test Dequeue
	queue.Dequeue();
	PrintTestResult("DynamicQueue Dequeue", queue.Front() == 20 && queue.GetSize() == 2);
}

void TestDynamicQueueCopySemantics()
{
	PrintSection("DynamicQueue: Copy Semantics");

	DynamicQueue<int> queue1;
	queue1.Enqueue(1);
	queue1.Enqueue(2);
	queue1.Enqueue(3);

	// Test Copy Constructor
	DynamicQueue<int> queue2(queue1);
	PrintTestResult("DynamicQueue Copy Constructor (size)", queue2.GetSize() == 3);
	PrintTestResult("DynamicQueue Copy Constructor (front)", queue2.Front() == 1);

	// Verify order is preserved
	queue2.Dequeue();
	PrintTestResult("DynamicQueue Copy Order (1)", queue2.Front() == 2);
	queue2.Dequeue();
	PrintTestResult("DynamicQueue Copy Order (2)", queue2.Front() == 3);

	// Verify deep copy
	PrintTestResult("DynamicQueue Deep Copy (independence)", queue1.GetSize() == 3);

	// Test Copy Assignment
	DynamicQueue<int> queue3;
	queue3 = queue1;
	PrintTestResult("DynamicQueue Copy Assignment (size)", queue3.GetSize() == 3);
	PrintTestResult("DynamicQueue Copy Assignment (front)", queue3.Front() == 1);
}

void TestDynamicQueueMoveSemantics()
{
	PrintSection("DynamicQueue: Move Semantics");

	DynamicQueue<int> queue1;
	queue1.Enqueue(10);
	queue1.Enqueue(20);
	queue1.Enqueue(30);

	// Test Move Constructor
	DynamicQueue<int> queue2(std::move(queue1));
	PrintTestResult("DynamicQueue Move Constructor (size)", queue2.GetSize() == 3);
	PrintTestResult("DynamicQueue Move Constructor (front)", queue2.Front() == 10);

	// Test Move Assignment
	DynamicQueue<int> queue3;
	queue3 = std::move(queue2);
	PrintTestResult("DynamicQueue Move Assignment (size)", queue3.GetSize() == 3);
	PrintTestResult("DynamicQueue Move Assignment (front)", queue3.Front() == 10);
}

void TestDynamicQueueClear()
{
	PrintSection("DynamicQueue: Clear Operation");

	DynamicQueue<int> queue;
	queue.Enqueue(1);
	queue.Enqueue(2);
	queue.Enqueue(3);

	queue.Clear();
	PrintTestResult("DynamicQueue Clear", queue.IsEmpty() && queue.GetSize() == 0);
}

//=============================================================================
// BINARY TREE TESTS
//=============================================================================
void TestBinaryTreeBasicOperations()
{
	PrintSection("BinaryTree: Basic Operations");

	BinaryTree<int> tree;

	// Test IsEmpty on new tree
	PrintTestResult("BinaryTree IsEmpty (initially)", tree.IsEmpty());

	// Test InsertNode
	tree.InsertNode(50);
	tree.InsertNode(30);
	tree.InsertNode(70);
	tree.InsertNode(20);
	tree.InsertNode(40);
	tree.InsertNode(60);
	tree.InsertNode(80);

	PrintTestResult("BinaryTree InsertNode", !tree.IsEmpty());

	// Test SearchNode
	PrintTestResult("BinaryTree SearchNode (found)", tree.SearchNode(40));
	PrintTestResult("BinaryTree SearchNode (not found)", !tree.SearchNode(100));
}

void TestBinaryTreeRemove()
{
	PrintSection("BinaryTree: Remove Operations");

	BinaryTree<int> tree;
	tree.InsertNode(50);
	tree.InsertNode(30);
	tree.InsertNode(70);
	tree.InsertNode(20);
	tree.InsertNode(40);
	tree.InsertNode(60);
	tree.InsertNode(80);

	// Remove leaf node
	tree.Remove(20);
	PrintTestResult("BinaryTree Remove (leaf)", !tree.SearchNode(20));

	// Remove node with one child
	tree.InsertNode(75);
	tree.Remove(80);
	PrintTestResult("BinaryTree Remove (one child)", !tree.SearchNode(80) && tree.SearchNode(75));

	// Remove node with two children
	tree.Remove(70);
	PrintTestResult("BinaryTree Remove (two children)", !tree.SearchNode(70) && tree.SearchNode(60) && tree.SearchNode(75));
}

void TestBinaryTreeCopySemantics()
{
	PrintSection("BinaryTree: Copy Semantics");

	BinaryTree<int> tree1;
	tree1.InsertNode(50);
	tree1.InsertNode(30);
	tree1.InsertNode(70);

	// Test Copy Constructor
	BinaryTree<int> tree2(tree1);
	PrintTestResult("BinaryTree Copy Constructor (search 50)", tree2.SearchNode(50));
	PrintTestResult("BinaryTree Copy Constructor (search 30)", tree2.SearchNode(30));
	PrintTestResult("BinaryTree Copy Constructor (search 70)", tree2.SearchNode(70));

	// Verify deep copy
	tree1.Remove(30);
	PrintTestResult("BinaryTree Deep Copy (independence)", tree2.SearchNode(30));

	// Test Copy Assignment
	BinaryTree<int> tree3;
	tree3 = tree2;
	PrintTestResult("BinaryTree Copy Assignment (search 50)", tree3.SearchNode(50));
	PrintTestResult("BinaryTree Copy Assignment (search 30)", tree3.SearchNode(30));
}

void TestBinaryTreeMoveSemantics()
{
	PrintSection("BinaryTree: Move Semantics");

	BinaryTree<int> tree1;
	tree1.InsertNode(50);
	tree1.InsertNode(30);
	tree1.InsertNode(70);

	// Test Move Constructor
	BinaryTree<int> tree2(std::move(tree1));
	PrintTestResult("BinaryTree Move Constructor (search 50)", tree2.SearchNode(50));
	PrintTestResult("BinaryTree Move Constructor (search 30)", tree2.SearchNode(30));

	// Test Move Assignment
	BinaryTree<int> tree3;
	tree3 = std::move(tree2);
	PrintTestResult("BinaryTree Move Assignment (search 50)", tree3.SearchNode(50));
	PrintTestResult("BinaryTree Move Assignment (search 30)", tree3.SearchNode(30));
}

void TestBinaryTreeTraversal()
{
	PrintSection("BinaryTree: Traversal");

	BinaryTree<int> tree;
	tree.InsertNode(50);
	tree.InsertNode(30);
	tree.InsertNode(70);
	tree.InsertNode(20);
	tree.InsertNode(40);

	// Test TraverseInOrder
	int sum = 0;
	tree.TraverseInOrder([&sum](const int& value) {
		sum += value;
		});
	PrintTestResult("BinaryTree TraverseInOrder (sum)", sum == 210); // 50+30+70+20+40 = 210

	// Test that traversal is in sorted order
	int previous = -1;
	bool inOrder = true;
	tree.TraverseInOrder([&previous, &inOrder](const int& value) {
		if (value <= previous)
			inOrder = false;
		previous = value;
		});
	PrintTestResult("BinaryTree TraverseInOrder (sorted)", inOrder);
}

//=============================================================================
// STRING TYPE TESTS
//=============================================================================
void TestWithStrings()
{
	PrintSection("String Type Tests");

	// Test Stack with strings
	Stack<std::string> stringStack(3);
	stringStack.Push("Hello");
	stringStack.Push("World");
	PrintTestResult("Stack with strings", stringStack.Top() == "World");

	// Test Queue with strings
	Queue<std::string> stringQueue(3);
	stringQueue.Enqueue("First");
	stringQueue.Enqueue("Second");
	PrintTestResult("Queue with strings", stringQueue.Front() == "First");

	// Test DynamicStack with strings
	DynamicStack<std::string> dynStringStack;
	dynStringStack.Push("Dynamic");
	dynStringStack.Push("Stack");
	PrintTestResult("DynamicStack with strings", dynStringStack.Top() == "Stack");

	// Test DynamicQueue with strings
	DynamicQueue<std::string> dynStringQueue;
	dynStringQueue.Enqueue("Dynamic");
	dynStringQueue.Enqueue("Queue");
	PrintTestResult("DynamicQueue with strings", dynStringQueue.Front() == "Dynamic");

	// Test BinaryTree with strings
	BinaryTree<std::string> stringTree;
	stringTree.InsertNode("banana");
	stringTree.InsertNode("apple");
	stringTree.InsertNode("cherry");
	PrintTestResult("BinaryTree with strings", stringTree.SearchNode("apple"));
}

//=============================================================================
// MAIN FUNCTION
//=============================================================================
int main()
{
	std::cout << YELLOW << "\n╔════════════════════════════════════════╗" << RESET << std::endl;
	std::cout << YELLOW << "║  Data Structures Test Suite           ║" << RESET << std::endl;
	std::cout << YELLOW << "╚════════════════════════════════════════╝" << RESET << std::endl;

	// Stack Tests
	TestStackBasicOperations();
	TestStackCopySemantics();
	TestStackMoveSemantics();
	TestStackExceptions();
	TestStackClear();

	// Queue Tests
	TestQueueBasicOperations();
	TestQueueCircularBuffer();
	TestQueueCopySemantics();
	TestQueueMoveSemantics();
	TestQueueExceptions();

	// DynamicStack Tests
	TestDynamicStackBasicOperations();
	TestDynamicStackCopySemantics();
	TestDynamicStackMoveSemantics();
	TestDynamicStackClear();

	// DynamicQueue Tests
	TestDynamicQueueBasicOperations();
	TestDynamicQueueCopySemantics();
	TestDynamicQueueMoveSemantics();
	TestDynamicQueueClear();

	// BinaryTree Tests
	TestBinaryTreeBasicOperations();
	TestBinaryTreeRemove();
	TestBinaryTreeCopySemantics();
	TestBinaryTreeMoveSemantics();
	TestBinaryTreeTraversal();

	// String Tests
	TestWithStrings();

	// Print Summary
	std::cout << "\n" << YELLOW << "╔════════════════════════════════════════╗" << RESET << std::endl;
	std::cout << YELLOW << "║  Test Summary                          ║" << RESET << std::endl;
	std::cout << YELLOW << "╚════════════════════════════════════════╝" << RESET << std::endl;
	std::cout << "Total Tests: " << totalTests << std::endl;
	std::cout << GREEN << "Passed: " << passedTests << RESET << std::endl;
	std::cout << RED << "Failed: " << (totalTests - passedTests) << RESET << std::endl;

	double percentage = (static_cast<double>(passedTests) / totalTests) * 100.0;
	std::cout << "Success Rate: " << percentage << "%" << std::endl;

	if (passedTests == totalTests)
	{
		std::cout << GREEN << "\n✓ All tests passed!" << RESET << std::endl;
	}
	else
	{
		std::cout << RED << "\n✗ Some tests failed." << RESET << std::endl;
	}

	return (passedTests == totalTests) ? 0 : 1;
}