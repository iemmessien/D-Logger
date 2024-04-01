/* Logger.hpp */

#ifndef LOGGER_HPP
#define LOGGER_HPP

// Files
#include "../../include/Drone_.hpp"
#include "Timer.cpp"

#define __FILENAME__ (strchr(__FILE__, '\\') ? strchr(__FILE__, '\\') + 1 : __FILE__)

namespace Logger {

    std::string logFile;
    std::string logFileName;
    std::string defaultFilePath = "C:/Users/E/Documents/Visual_Studio_2019/Projects/OnboardControl/OnboardControl/src/Threads/";
    
    static bool logFileCreated = false;

    //Timer timerObject;

    // Declare start and stop times
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> globalTime_;

    std::chrono::time_point<std::chrono::high_resolution_clock> timePoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> loggerInitializedTime;

    // Declare time point
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timepoint_t;

    Timepoint_t globalTime;
    //Timepoint_t globalTimeStamp;

    typedef enum {
        TRI_TRUE = 0,
        TRI_FALSE = 1,
        TRI_NEUTRAL = 2
    } TriState;

    // Declare timer start and end point
    enum Timer_t { START, END };

    // Output types
    enum Output { DEFAULT, JSON, BOTH, CSV, CSV_A };

    // Log levels
    enum Verbosity { DEBUG, INFO, NOTICE, WARNING, ERROR, CRITICAL, ALERT, EMERGENCY };

    // File extenstion
    enum FileExtensiOn { E_CSV };

    static std::map<std::string, Timepoint_t> timerMap;

    /***************************/
    /** Function declarations **/
    /***************************/
    bool InitializeLogger(const std::string &filePath, const std::string &fileName);
    void InitializeOutput();
    std::stringstream InitializeOutputFormat();
    std::string GetCurrentTimeStamp();
    double SystemUpTimeInMillis(const Timepoint_t &globalTimeStamp);
    double SystemUpTimeInMillis();
    bool Log(const std::string &message);
    bool Log(const std::string &message, const Logger::Verbosity &logLevel);
    bool Log(const std::string &message, const Logger::Verbosity &logLevel, const Logger::Output &outputType);
    void OutputLineSyntax();
    void OutputTxtSyntax(const Logger::Verbosity &logLevel, const std::string &message);
    void OutputJsonSyntax(const Logger::Verbosity &logLevel, const std::string &message);
    void OutputCsvSyntax(const Logger::Verbosity &logLevel, const std::string &message);
    void AppendCsvSyntax(const Logger::Verbosity &logLevel, const std::string &message);
    std::string GetVerbosityStringValue(const Logger::Verbosity &logLevel);
    TriState TimerStart(const std::string &customTimer);
    double TimerEnd(const std::string &customTimer);
    void TimerOutput(const double &timerDuration, const std::string &timerName, const TriState &timerCheck);
    bool Log_Timer(const std::string &customTimer, const Timer_t &timeParam);


    bool InitializeLogger(const std::string &filePath, const std::string &fileName) {
        
        bool loggerInitialized = false;

        // Get the current time
        Timepoint_t loggerInitializedTime = std::chrono::high_resolution_clock::now();

        std::time_t timePoint = std::chrono::high_resolution_clock::to_time_t(globalTime);

        std::string loggerInitializedTimeStamp = Logger::GetCurrentTimeStamp();

        // Get log file from file path and name extension
        logFile = filePath + fileName;
        defaultFilePath = filePath;
        logFileName = fileName;

        // Create input file stream object
        std::ifstream logFileIO(logFile);

        // Check if log file exists
        // Otherwise, create it
        if (!logFileIO) {

            // Create new file if it does not exist
            // Create output file stream object
            std::ofstream createFile;
            
            createFile.open(logFile);

            if (createFile.is_open()) {
                
                Logger::InitializeOutput();
            }
            
            // Close file stream object
            createFile.close();

        } else {
            Logger::InitializeOutput();
        }

        // Close file stream object if file exists
		logFileIO.close();

        loggerInitialized = true;

        return loggerInitialized;
    }

    void InitializeOutput() {

        // Declare input file stream object
        std::ifstream logFileIn(logFile);
        logFileIn.close();

        // Declare output file stream object
        std::ofstream logFileOut(logFile, std::ofstream::app);

        if (logFileOut.is_open()) {

            std::stringstream outputFormat = Logger::InitializeOutputFormat();
            
            logFileOut << outputFormat.rdbuf();
            logFileOut.close();
        }
    }

    std::stringstream InitializeOutputFormat() {

        std::stringstream output;

        output  << std::string(85, '-')
                << std::endl
                << std::string(85, ' ')
                << std::endl
                << "[Data logger started at: " <<  __TIMESTAMP__ "] "
                << "[" << __FILENAME__ <<"]"
                << std::endl
                << std::string(85, ' ')
                << std::endl
                << std::string(85, '-') 
                << std::endl;

        return output;
    }

    std::string GetCurrentTimeStamp() {

        time_t now;

        // Get current time
        time(&now);

        // Convert to local time
        struct tm* localTimeInfo = localtime(&now);
        
        // Convert to GMT time
        //struct tm* gmtTimeInfo = gmtime(&now);

        // Convert to calendar format
        char* timeStamp = asctime(localTimeInfo);
        timeStamp[strlen(timeStamp) - 1] = '\0';

        // Convert to both local time and calendar format
        char* localTimeStamp = ctime(&now);

        return localTimeStamp;

        //return timeStamp;
    }

    double SystemUpTimeInMillis(const Timepoint_t &globalTimeStamp) {

        Timepoint_t currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = currentTime - globalTime;

        return duration.count();
    }

    double SystemUpTimeInMillis() {

        Timepoint_t currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = currentTime - startTime;

        return duration.count();
    }

    bool Log(const std::string &message) {

        bool logged = false;

        // Check if log file has been created
        //if (logFileCreated == false) return false;

        if (message == "line" || message == "LINE") {
            OutputLineSyntax();
        } else {
            OutputTxtSyntax(Logger::Verbosity::DEBUG, message);
        }

        logged = true;

        return logged;
    }

    bool Log(const std::string &message, const Logger::Verbosity &logLevel) {
        
        bool logged = false;

        // Check if log file has been created
        //if (logFileCreated == false) return false;

        OutputTxtSyntax(logLevel, message);

        logged = true;

        return logged;
    }

    bool Log(const std::string &message, const Logger::Verbosity &logLevel, const Logger::Output &outputType) {
		
        std::cout << message << std::endl;

        bool logged = false;

        // Check if log file has been created
        //if (logFileCreated == false) return false;

		switch (outputType) {

            case Logger::Output::DEFAULT:
                OutputTxtSyntax(logLevel, message);
                break;
            
            case Logger::Output::JSON:
                OutputJsonSyntax(logLevel, message);
                break;
            
            case Logger::Output::BOTH:
                OutputTxtSyntax(logLevel, message);
                OutputJsonSyntax(logLevel, message);
                break;
            
            case Logger::Output::CSV:
                OutputCsvSyntax(logLevel, message);
                break;
            
            case Logger::Output::CSV_A:
                AppendCsvSyntax(logLevel, message);
                break;
            
            default:
                OutputTxtSyntax(logLevel, message);
                break;
		}

        logged = true;

        return logged;
	}

    void OutputLineSyntax() {

        std::ifstream logFileIn(logFile);
        logFileIn.close();

        std::ofstream logFileOut(logFile, std::ofstream::app);

        if (logFileOut.is_open()) {

            std::string line(85, '-');

            // Append line
            logFileOut << line << std::endl;

            // Close file
            logFileOut.close();
        }
    }

    void OutputTxtSyntax(const Logger::Verbosity &logLevel, const std::string &message) {

        std::string currentTime = Logger::GetCurrentTimeStamp();
        std::string logLvl = Logger::GetVerbosityStringValue(logLevel);
        const double systemUpTime = Logger::SystemUpTimeInMillis(globalTime);

        std::ifstream logFileIn(logFile);
        logFileIn.close();

        //std::ofstream logFileOutput(defaultFilePath);
        std::ofstream logFileOut(logFile, std::ofstream::app);

        if (logFileOut.is_open()) {

            logFileOut  << std::endl
                        << "[" << currentTime << "]"
                        << "[" << logLvl << "]" 
                        << " uptime: (" << systemUpTime << " ms) "
                        << message;

            logFileOut.close();

            std::cout << "Txt syntax!" << std::endl;
        }
    }

    void OutputJsonSyntax(const Logger::Verbosity &logLevel, const std::string &message) {

        std::string currentTime = Logger::GetCurrentTimeStamp();
        std::string logLvl = Logger::GetVerbosityStringValue(logLevel);
        const double systemUpTime = Logger::SystemUpTimeInMillis(globalTime);

        nlohmann::json jsonOutput = {
            {"timestamp", currentTime},
            {"verbosity", logLvl},
            {"uptime (ms)", systemUpTime},
            {"message", message}
        };

        std::string jsonDump = jsonOutput.dump();

        std::ifstream logFileIn(logFile);
        logFileIn.close();

        //std::ofstream logFileOutput(defaultFilePath);
        std::ofstream logFileOut(logFile, std::ofstream::app);

        if (logFileOut.is_open()) {

            logFileOut << std::endl << jsonDump;    // << "\n";
            
            logFileOut.close();

            std::cout << "Json syntax!" << std::endl;
        }
    }

    void OutputCsvSyntax(const Logger::Verbosity &logLevel, const std::string &message) {

        std::string currentTime = Logger::GetCurrentTimeStamp();
        std::string logLvl = Logger::GetVerbosityStringValue(logLevel);
        const double systemUpTime = Logger::SystemUpTimeInMillis(globalTime);

        std::ifstream logFileIn(logFile);
        logFileIn.close();

        //std::ofstream logFileOutput(defaultFilePath);
        std::ofstream logFileOut(logFile, std::ofstream::app);

        if (logFileOut.is_open()) {

            logFileOut  << std::endl
                        << "timestamp: " << currentTime << ", "
                        << "verbosity: " << logLvl << ", " 
                        << "uptime: " << systemUpTime << " ms, "
                        << "message: " << message;

            logFileOut.close();

            std::cout << "Csv syntax!" << std::endl;
        }
    }

    void AppendCsvSyntax(const Logger::Verbosity &logLevel, const std::string &message) {

        std::string currentTime = Logger::GetCurrentTimeStamp();
        std::string logLvl = Logger::GetVerbosityStringValue(logLevel);
        const double systemUpTime = Logger::SystemUpTimeInMillis(globalTime);

        std::ifstream logFileIn(logFile);
        logFileIn.close();

        //std::ofstream logFileOutput(defaultFilePath);
        std::ofstream logFileOut(logFile, std::ofstream::app);

        if (logFileOut.is_open()) {

            logFileOut  << std::endl
                        << ", "
                        << "message: " << message;

            logFileOut.close();

            std::cout << "Csv append!" << std::endl;
        }
    }

    std::string GetVerbosityStringValue(const Logger::Verbosity &logLevel) {

        switch(logLevel) {

            case DEBUG:
                return "DEBUG";
                break;
            case INFO:
                return "INFO";
                break;
            case NOTICE:
                return "NOTICE";
                break;
            case WARNING:
                return "WARNING";
                break;
            case ERROR:
                return "ERROR";
                break;
            case CRITICAL:
                return "CRITICAL";
                break;
            case ALERT:
                return "ALERT";
                break;
            case EMERGENCY:
                return "EMERGENCY";
                break;
            default:
                break;
        }
    }
    
    TriState TimerStart(const std::string &customTimer) {

        Timepoint_t startTime = std::chrono::high_resolution_clock::now();

        if (timerMap.empty()) {
            timerMap.emplace(std::pair<std::string, Timepoint_t>(customTimer, startTime));

        } else if (timerMap.find(customTimer) != timerMap.end()) {
            
            return TRI_TRUE;

        } else {
            timerMap.end();
            
            timerMap.emplace(std::pair<std::string, Timepoint_t>(customTimer, startTime));
        }

        return TRI_NEUTRAL;
    }

    double TimerEnd(const std::string &customTimer) {

        Timepoint_t stopTime = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> timerDuration;

        // Declare time duration
        //std::chrono::duration<float> elapsed;

        if (timerMap.find(customTimer) != timerMap.end()) {

            Timepoint_t startTime = timerMap.find(customTimer)->second;
            
            timerDuration = stopTime - startTime;

            std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);

            const float time_sec = elapsed.count() * 1000.0f;

            std::cout << "Time taken: " << time_sec << " ms" << std::endl;

            // Remove map of timer to time
            timerMap.erase(customTimer);

            return timerDuration.count();
        }
        else {
            return 0.0;
        }
    }

    void TimerOutput(const double &timerDuration, const std::string &timerName, const TriState &timerCheck) {

        bool timerExist = true;
        std::string timerExistMessage = "";

        if (timerDuration == 0 && timerCheck == TRI_FALSE) {
            timerExistMessage = "The timer does not exist.";
            timerExist = false;
        } else if (timerDuration != 0 && timerCheck == TRI_FALSE) {
            timerExistMessage = "The same named timer is already running.";
        }

        std::string currentTime = Logger::GetCurrentTimeStamp();

        std::ifstream logFileIn(defaultFilePath + "timeLogger.log");
        logFileIn.close();

        std::ofstream logFileOut(defaultFilePath + "timeLogger.log", std::ofstream::app);

        if (logFileOut.is_open() && timerExist == true && timerExistMessage != "" && timerCheck != TRI_NEUTRAL) {

            logFileOut  << std::endl
                        << "[" << currentTime << "]"
                        << "[" << timerName << "] -> "
                        << "Duration: "
                        << timerDuration << std::endl;
            
            logFileOut.close();

        } else if (logFileOut.is_open() && timerExist == false) {

            logFileOut  << std::endl
                        << "[" << currentTime << "]"
                        << "[" << timerName << "] -> "
                        << timerExistMessage << std::endl;
            
            logFileOut.close();

        } else if (logFileOut.is_open() && timerCheck == TRI_TRUE) {

            logFileOut  << std::endl
                        << "[" << currentTime << "]"
                        << "[" << timerName << "] -> "
                        << timerExistMessage << std::endl;
            
            logFileOut.close();
        }
    }

    bool Log_Timer(const std::string &customTimer, const Timer_t &timeParam) {	
	
        TriState timerCheck = TRI_FALSE;

        double timerDuration;

        bool logged = false;

        switch (timeParam) {
        
            case START:
                timerCheck = Logger::TimerStart(customTimer);
                Logger::TimerOutput(0, customTimer, timerCheck);
                timerCheck = TRI_FALSE;
                break;
            
            case END:
                timerDuration = TimerEnd(customTimer);
                TimerOutput(timerDuration, customTimer, timerCheck);
                break;
        }

        logged = true;

        return logged;
    }

}   // namespace Logger

#endif

int main() {
    std::cout << "Logger" << std::endl << std::endl;

    // Initialize logger
    bool initializedLogger = Logger::InitializeLogger(Logger::defaultFilePath, "Logger.log");
    
    // 1
    bool log = Logger::Log("Hello!");

    // 2
    //bool log_ = Logger::Log("Hello...", Logger::Verbosity::INFO);

    // 3
    //bool logged = Logger::Log("Hello, there!", Logger::Verbosity::INFO, Logger::Output::CSV_A);

    // 4
    bool loggedTimer1 = Logger::Log_Timer("my_first_timer", Logger::Timer_t::START);

    bool loggedTimer2 = Logger::Log_Timer("my_second_timer", Logger::Timer_t::START);

    bool loggedTimer3 = Logger::Log_Timer("my_first_timer", Logger::Timer_t::END);

    bool loggedTimer4 = Logger::Log_Timer("my_second_timer", Logger::Timer_t::END);


    //std::cout << "Logged: " << logged << std::endl << std::endl;

    

}

