#ifndef ASSIGNMENT4_USER_HPP
#define ASSIGNMENT4_USER_HPP

#include "PermissionLevel.hpp"
#include <string>

namespace Assignment4
{
class User
{
    public:
        const PermissionLevel DEFAULT_PERMISSION_LEVEL = PermissionLevel::NORMAL;

    public: User(std::string uname, std::string pass, std::string name, PermissionLevel perm) 
    {
        _uname = uname;
        setPass(pass);
        setName(name);
        setPermLevel(perm);
    }

    public: User(std::string uname, std::string pass, std::string name) 
    {
        User(uname, pass, name, DEFAULT_PERMISSION_LEVEL);
    }

    /**
     * @brief Check if the provided password matches the pass for user.
     * 
     * @param check (string): Password to check for auth.
     * 
     * @result (bool) True if provided password is match for user.
    */
    bool checkAuth(std::string check) { return _pass == check;}


    private:
        std::string _uname;
        void setUname(std::string uname) { _uname = uname; }
    public: 
        std::string getUname() { return _uname; }


    private:
        std::string _pass;
    public:
        std::string getPass() { return _pass; }
        void setPass(std::string pass) { _pass = pass; }


    private:
        std::string _name;
    public: 
        std::string getName() { return _name; }
        void setName(std::string name) { _name = name; }


    private:
        PermissionLevel _perm;
    public:
        PermissionLevel getPermLevel() { return _perm; }
        void setPermLevel(PermissionLevel perm) { _perm = perm; }
};
}
#endif