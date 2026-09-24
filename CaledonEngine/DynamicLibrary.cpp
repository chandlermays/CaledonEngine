/*------------------------------
| File: DynamicLibrary.cpp
| Author: Chandler Mays
------------------------------*/
#include "DynamicLibrary.h"

#include <Windows.h>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------
| --- Constructor: Constructs the DynamicLibrary with default values --- |
------------------------------------------------------------------------*/
CE::DynamicLibrary::DynamicLibrary()
	: m_pHModule{ nullptr }
{ }

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::DynamicLibrary::~DynamicLibrary()
{
	Unload();
}

/*-----------------------------------------------------------------
| --- Load: Loads the dynamic library from the specified path --- |
-----------------------------------------------------------------*/
bool CE::DynamicLibrary::Load(const std::string& path)
{
	Unload();

	m_pHModule = static_cast<void*>(LoadLibraryA(path.c_str()));
	return m_pHModule != nullptr;
}

/*--------------------------------------------
| --- Unload: Unload the dynamic library --- |
--------------------------------------------*/
void CE::DynamicLibrary::Unload()
{
	if (m_pHModule)
	{
		FreeLibrary(static_cast<HMODULE>(m_pHModule));
		m_pHModule = nullptr;
	}
}

/*---------------------------------------------------------------------
| --- IsLoaded: Checks if the dynamic library is currently loaded --- |
---------------------------------------------------------------------*/
bool CE::DynamicLibrary::IsLoaded() const
{
	return m_pHModule != nullptr;
}

/*---------------------------------------------------------------------------------------------
| --- GetFunctionAddress: Returns the address of a function in the loaded dynamic library --- |
---------------------------------------------------------------------------------------------*/
void* CE::DynamicLibrary::GetFunctionAddress(const std::string& functionName) const
{
	if (!m_pHModule)
		return nullptr;

	return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_pHModule), functionName.c_str()));
}