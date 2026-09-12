#pragma once
#include <string>

namespace CE
{
	class DynamicLibrary
	{
	private:
		void* m_pHModule;														// Handle to the loaded dynamic library (HINSTANCE on Windows, void* on POSIX)

	public:
		DynamicLibrary();														// Constructor
		~DynamicLibrary();														// Destructor
		DynamicLibrary(const DynamicLibrary&) = delete;							// Prevent copy-construction
		DynamicLibrary& operator=(const DynamicLibrary&) = delete;				// Prevent copy-assignment
		DynamicLibrary(DynamicLibrary&&) = delete;								// Prevent move-construction
		DynamicLibrary& operator=(DynamicLibrary&&) = delete;					// Prevent move-assignment

		bool Load(const std::string& path);										// Load the dynamic library from the specified path
		void Unload();															// Unload the dynamic library
		bool IsLoaded() const;													// Check if the dynamic library is currently loaded

		void* GetFunctionAddress(const std::string& functionName) const;		// Returns the address of a function in the loaded dynamic library
	};
}