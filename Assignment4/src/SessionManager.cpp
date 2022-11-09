#include "SessionManager.hpp"

namespace Assignment4
{
    std::string SessionManager::getUser(std::string session)
    {
        // Query map to get value for provided session.
        std::map<std::string, std::string>::iterator i = _session_to_user.find(session);
        
        // Return found user if there was one, otherwise default unauthenticated user.
        if ( i != _session_to_user.end())
        {
            return i->second;
        }
        else
        {
            return _UNAUTHENTICATED;
        }
    }

    bool SessionManager::isAuthenticated(std::string session)
    {
        std::string user = getUser(session);
        return user == _UNAUTHENTICATED;
    }

    void SessionManager::authenticateSession(std::string session, std::string user)
    {
        _session_to_user[session] = user;
    }

    void SessionManager::deauthenticateSession(std::string session)
    {
        _session_to_user.erase(session);
    }
}