#ifndef ASSIGNMENT4_SESSIONMANAGER_HPP
#define ASSIGNMENT4_SESSIONMANAGER_HPP

#include <map>
#include <string>

namespace Assignment4
{

class SessionManager
{
    public:
        // Value for user fetched given an unauthenticated session Id.
        const std::string _UNAUTHENTICATED = "";

    private: 
        // Track active sessions and the corresponding authenticated user.
        std::map<std::string, std::string> _session_to_user;
    
    public:
        /**
         * @brief Query for user matching provided session.
         * 
         * @param session (string): Session ID to query against.
         * 
         * @result (string) Matching user for Session Id (or SessionManager._UNAUTHENTICATED if no user).
        */
        std::string getUser(std::string session);

        /**
         * @brief Whether the provided session is authenticated as a user.
         * 
         * @param session (string): Session ID to verify.
         * 
         * @result (bool) True if Session ID matches has authenticated.
        */
        bool isAuthenticated(std::string session);

        /**
         * @brief Register the provided session as being authenticated for the specified user.
         * @note Overrides existing authentication if there is one currently for the session.
         * 
         * @param session (string): Session ID to track.
         * @param user (string): User that is authenticated.
         * 
         * @result void
        */
        void authenticateSession(std::string session, std::string user);

        /**
         * @brief Deregister the provided session so it is no longer authenticated to a user.
         * 
         * @param session (string): Session ID to track.
         * 
         * @result void
        */
        void deauthenticateSession(std::string session);
};

} // namespace Assignment4

#endif