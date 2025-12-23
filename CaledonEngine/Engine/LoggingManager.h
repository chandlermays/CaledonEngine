#pragma once
#include "Manager.h"
#include <string>
#include <fstream>
#include <mutex>

namespace CE
{
#define CE_LOG(message) CE::LoggingManager::LogMessage(message)

    class LoggingManager : public Manager
    {
    private:
        std::ofstream m_logFile;                                                            // The output log for debugging
        std::mutex m_mutex;                                                                 // A mutex lock for thread-safety

        LoggingManager();                                                                   // Constructor

    public:
        ~LoggingManager();                                                                  // Destructor
        LoggingManager(const LoggingManager&) = delete;                                     // Prevent copy-construction
        LoggingManager& operator=(const LoggingManager&) = delete;                          // Prevent copy-assignment
        LoggingManager(LoggingManager&&) = delete;                                          // Prevent move-construction
        LoggingManager& operator=(LoggingManager&&) = delete;                               // Prevent move-assignment

        static LoggingManager& GetInstance();                                               // Singleton accessor

        bool Initialize() override;                                                         // Initialize the logging manager
        void Shutdown() override;                                                           // Shutdown the logging manager

		void Log(const std::string& message, bool newLine = true);                          // Log a message to the log file

		static void LogMessage(const std::string& message, bool newLine = true);            // Static method to log a message
    };
}