#pragma once
#include "Manager.h"
#include <string>
#include <fstream>

namespace CE
{
#define CE_LOG(message) CE::LoggingManager::LogMessage(message)

    class LoggingManager : public Manager
    {
    private:
        std::ofstream m_logFile;                                                            // The output log for debugging

        LoggingManager();                                                                   // Constructor

    public:
        ~LoggingManager();                                                                  // Destructor
        LoggingManager(const LoggingManager&) = delete;                                     // Prevent copy-construction
        LoggingManager& operator=(const LoggingManager&) = delete;                          // Prevent copy-assignment
        LoggingManager(LoggingManager&&) = delete;                                          // Prevent move-construction
        LoggingManager& operator=(LoggingManager&&) = delete;                               // Prevent move-assignment

		static LoggingManager& GetInstance();                                               // Returns the singleton instance of the LoggingManager

        bool Initialize() override;                                                         // Initialize the logging manager
        void Shutdown() override;                                                           // Shutdown the logging manager

		void Log(const std::string& message, bool newLine = true);                          // Log a message to the log file

		static void LogMessage(const std::string& message, bool newLine = true);            // Static method to log a message
    };
}