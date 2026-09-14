/*------------------------------
| File: Project.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <string>

class Project
{
private:
	std::string m_name;																			// Name of the project
	std::string m_rootDirectory;																// Root directory of the project
	std::string m_modulePath;																	// Path to the game module of the project
	std::string m_masterAssetsPath;																// Path to the master assets of the project

public:
	Project() = default;																		// Constructor
	~Project() = default;																		// Destructor
	Project(const Project&) = delete;															// Prevent copy-construction
//	Project& operator=(const Project&) = delete;												// Prevent copy-assignment
	Project(Project&&) = delete;																// Prevent move-construction
	Project& operator=(Project&&) = delete;														// Prevent move-assignment
	
	bool Load(const std::string& projectFilePath);												// Loads the project from the specified file path

	const std::string& GetName() const					{ return m_name; }						// Returns the name of the project
	const std::string& GetRootDirectory() const			{ return m_rootDirectory; }				// Returns the root directory of the project
	const std::string& GetModulePath() const			{ return m_modulePath; }				// Returns the path to the game module of the project
	const std::string& GetMasterAssetsPath() const		{ return m_masterAssetsPath; }			// Returns the path to the master assets of the project
};