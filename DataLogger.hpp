/* DataLogger.hpp */

#include "../../include/Drone.hpp"

#include "../../libs/clogging/Logger.h"

#include "stdafx.h"
#include "Logger.h"

namespace DataLogger {


    Initialize() {

        time_t now = time(0);
        char* date_time_string = ctime(&now);
        cout << date_time_string << endl;

        INIT_CLOGGING;
        CLOGGING_TIMER;
        ADD_FILE("clogging_now_time.log", "C:/Users/E/Documents/Visual Studio 2019/Projects/OnboardControl/OnboardControl/logs/");

        //ADD_FILE("something.log");
        //ADD_FILE("clogging.log");
        ADD_FILE("clogging.csv");

        string startup_message = "Starting up...";
        CLOG(startup_message, Verbosity::INFO, Output::JSON);
        //CLOG(startup_message, Verbosity::WARN, Output::BOTH);
        //CLOG_T("startup_timer", Timer_t::START);
        //CLOG_T("startup_timer", Timer_t::END);
        //CLOG_T("network_timer", Timer_t::START);
        //CLOG_T("network_timer", Timer_t::END);

        std::string msg = "hello world!";
        //CLOG(msg, Verbosity::WARN, Output::BOTH);
        std::string msg2 = "appended";

        CLOG_T("timer1", Timer_t::START);
        CLOG_T("timer2", Timer_t::START);

        CLOG(msg, Verbosity::WARN, Output::CSV);
        CLOG(msg2, Verbosity::WARN, Output::CSV_A);

        CLOG_T("timer1", Timer_t::END);
        for (size_t i = 0; i < 2; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        }
        CLOG_T("timer2", Timer_t::END);

        //CLOG_VS(msg, Verbosity::EMERG, Output_vs::TO_FILE_BOTH);

    }

    Example() {

        INIT_CLOGGING;
	CLOGGING_TIMER;

	//ADD_FILE("something.log");
	//ADD_FILE("clogging.log");
	ADD_FILE("clogging.csv");

	std::string msg = "hello world!";
	//CLOG(msg, Verbosity::WARN, Output::BOTH);
	std::string msg2 = "appended";

	CLOG_T("timer1", Timer_t::START);
	CLOG_T("timer2", Timer_t::START);

	CLOG(msg, Verbosity::WARN, Output::CSV);
	CLOG(msg2, Verbosity::WARN, Output::CSV_A);

	CLOG_T("timer1", Timer_t::END);
	for (size_t i = 0; i < 2; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	CLOG_T("timer2", Timer_t::END);

	//CLOG_VS(msg, Verbosity::EMERG, Output_vs::TO_FILE_BOTH);
    }

}   // namespace DataLogger