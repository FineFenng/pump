//
// Created by fenng on 18-8-8.
//

#include <pump/utility/log/Logger.h>


int main()
{
    using namespace pump::utility;
    LogLine::set_global_log_level(LogLevel::PUMP_INFO);
    LOG_INFO << "fefef";
    system("pause");
    return 0;

}

