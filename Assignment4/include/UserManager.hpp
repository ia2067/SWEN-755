#ifndef ASSIGNMENT4_USERMANAGER_HPP
#define ASSIGNMENT4_USERMANAGER_HPP

#include "User.hpp"
#include "PermissionLevel.hpp"
#include <map>
#include <vector>
#include <string>

namespace Assignment4
{

class UserManager
{
    private: 
        // Track known Users
        std::map<std::string, User> _users;
    
    public:
        /**
         * @brief Add a new user to be managed.
         * 
         * @param user (User): User to be added.
         * 
         * @result void
        */
        void addUser(User user);

        /**
         * @brief Remove specified user.
         * 
         * @param user (User): User to be removed.
         * 
         * @result void
        */
        void removeUser(User user);

        /**
         * @brief Check whether the provided username password matches any authentication.
         * 
         * @param uname (string): Username to check for authentication.
         * @param pass (string): Password to check against username.
         * 
         * @result (bool) True if authentication passed (uname exists and has pass), false otherwise.
        */
        bool checkAuthenticate(std::string uname, std::string pass);

        /**
         * @brief Check if the provided user exists (is being managed).
         * 
         * @param uname (string): Username to check for existance.
         * 
         * @result (bool) True if user exists, false otherwise.
        */
        bool checkExist(std::string uname);


        /**
         * @brief Get all users registered to be managed.
         * 
         * @param none
         * 
         * @result (List<User>) List of managed users.
        */
        std::vector<User> getAllUsers();
};

} // namespace Assignment4

#endif