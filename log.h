#ifndef LOG_H
#define LOG_H
#include <iostream>
namespace vincy {
static void log_print(const std::string& msg )
{
    std::cout<<msg<<std::endl;
}
}

#endif // LOG_H

