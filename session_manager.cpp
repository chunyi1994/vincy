#include "session_manager.h"
#include "utils.h"
namespace vincy {


Session &SessionManager::getSessions(const HttpRequest &request)
{
    int sessionid = sessionId(request.cookie("sessionid"));
    if(sessionid == -1)
    {
        sessionid = maxSessionID_;
        maxSessionID_++;
    }

    return sessions_[sessionid];
}

Session &SessionManager::getSessions(int sessionID)
{
    return sessions_[sessionID];
}

bool SessionManager::find(int sessionId)
{
    auto iter = sessions_.find(sessionId);
    if(iter == sessions_.end())
    {
        return false;
    }
    return true;
}

//vincyvincy:1
int SessionManager::sessionId(std::string str)
{
    for(size_t i = 0; i < str.length(); i++)
    {
        str[i] += 3;
    }
    size_t pos = str.find(":");
    if(pos == std::string::npos || pos == str.length() - 1)
    {
        return -1;
    }
    else{
        std::string numStr =  str.substr(pos + 1, str.length() - pos - 1);
        if(!isDigit(numStr))
        {
            return -1;
        }
        int ret = string2int(numStr);
        if(sessions_.find(ret) == sessions_.end())
        {
            return -1;
        }
        return ret;
    }
}

SessionManager::SessionManager() :
    maxSessionID_(0),
    sessions_()
{

}

std::string SessionManager::newSession()
{
    sessions_[maxSessionID_];
    std::string ret = "vincyvincy:" + int2string(maxSessionID_);
    for(size_t i = 0; i < ret.length(); i++)
    {
        ret[i] -= 3;
    }
    maxSessionID_++;
    return ret;
}

}
