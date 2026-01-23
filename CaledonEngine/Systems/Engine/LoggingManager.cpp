#include "LoggingManager.h"
#include <iostream>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*------------------------------------------------------------------------
| --- Constructor: Constructs the LoggingManager with default values --- |
------------------------------------------------------------------------*/
CE::LoggingManager::LoggingManager()
{}

/*-------------------------------------------------------
| --- Destructor: Cleans up any allocated resources --- |
-------------------------------------------------------*/
CE::LoggingManager::~LoggingManager()
{
	CE_LOG("LoggingManager::~LoggingManager - Shutting down LoggingManager.");
	Shutdown();
}

/*----------------------------------------------------------
| --- GetInstance: Accessor for the singleton instance --- |
----------------------------------------------------------*/
CE::LoggingManager& CE::LoggingManager::GetInstance()
{
	static LoggingManager instance;
	return instance;
}

/*------------------------------------------------------------------------
| --- Initialize: Sets up the logging manager and opens the log file --- |
------------------------------------------------------------------------*/
bool CE::LoggingManager::Initialize()
{
#if _DEBUG
	m_logFile.open("CaledonEngine.txt");
	if (!m_logFile.is_open())
	{
		CE_LOG("LoggingManager::Initialize - Failed to open 'CaledonEngine.txt'");
		return false;
	}

	return true;

#else
	return true;

#endif
}

/*-----------------------------------------------------------------------
| --- Shutdown: Shutdown the logging manager and close the log file --- |
-----------------------------------------------------------------------*/
void CE::LoggingManager::Shutdown()
{
#if _DEBUG
	if (!m_logFile.is_open())
		return;

	if (m_logFile.is_open())
	{
		m_logFile.close();
	}
#endif
}

/*--------------------------------------------
| --- Log: Log a message to the log file --- |
--------------------------------------------*/
void CE::LoggingManager::Log(const std::string& message, bool newLine)
{
#if _DEBUG
	std::cout << message;
	if (newLine) std::cout << '\n';

	if (m_logFile.is_open())
	{
		m_logFile << message;
		if (newLine) m_logFile << '\n';
		m_logFile.flush();
	}

#else
	(void)message;
	(void)newLine;

#endif
}

/*--------------------------------------------------------------------
| --- LogMessage: Static method to log a message to the log file --- |
--------------------------------------------------------------------*/
void CE::LoggingManager::LogMessage(const std::string& message, bool newLine)
{
	GetInstance().Log(message, newLine);
}