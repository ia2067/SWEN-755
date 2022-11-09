#ifndef ASSIGNMENT4_SESSION_HPP
#define ASSIGNMENT4_SESSION_HPP

#include <Wt/Auth/Login.h>
#include <Wt/Auth/RegistrationModel.h>
#include <Wt/Auth/Dbo/UserDatabase.h>

#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>

#include "model/User.hpp"

namespace dbo = Wt::Dbo;

using UserDatabase = Wt::Auth::Dbo::UserDatabase<AuthInfo>;

class Session : public dbo::Session
{
public:
    Session(const std::string& sqliteDb);

    static void configureAuth();


    dbo::ptr<User> user() const;

    Wt::Auth::AbstractUserDatabase& users();
    Wt::Auth::Login& login() { return _login; }

    static const Wt::Auth::AuthService& auth();
    static const Wt::Auth::PasswordService& passwordAuth();
    static const std::vector<const Wt::Auth::OAuthService *> oAuth();

private:
    std::unique_ptr<UserDatabase> _pUsers;
    Wt::Auth::Login _login;

};

#endif 
