#include "UserManager.hpp"

namespace Assignment4
{
    void UserManager::addUser(User user)
    {
        _users.emplace(user.getUname(), user);
    }

    void UserManager::removeUser(User user)
    {
        _users.erase(user.getUname());
    }

    bool UserManager::checkAuthenticate(std::string uname, std::string pass)
    {
        // Check for user existance. Automatic auth fail for no user.
        std::map<std::string, User>::iterator i = _users.find(uname);
        if (i == _users.end())
        {
            return false;
        }

        // Check found username authentication with provided pass.
        return i->second.checkAuth(pass);
    }

    bool UserManager::checkExist(std::string uname)
    {
        // Username was found (iterator is not past the end of Map).
        return _users.find(uname) != _users.end();
    }

    User UserManager::fetchUser(std::string uname)
    {
        return _users.find(uname)->second;
    }

    std::vector<User> UserManager::getAllUsers()
    {
        std::vector<User> res;

        // Iterate over all pairs in the User Management map and pick off user.
        for (std::map<std::string, User>::iterator i=_users.begin(); i != _users.end(); i++)
        {
            res.push_back(i->second);
        }

        return res;
    }
}