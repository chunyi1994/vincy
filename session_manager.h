#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H
#include <map>
#include "http_request.h"
#include "singleton.h"

namespace vincy {

typedef std::map<std::string, std::string> Session;

class SessionManager
{
public:
    SessionManager();

    std::string newSession();
    Session& getSessions(const HttpRequest& request);
    Session& getSessions(int sessionID);
    bool find(int sessionId);

    int sessionId(std::string str);

private:
    int maxSessionID_;
    std::map<int, Session> sessions_;
};

static SessionManager gSessionManager;
}
#endif // SESSION_MANAGER_H
