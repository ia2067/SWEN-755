#ifndef ASSIGNMENT4_AUTHAPP_HPP
#define ASSIGNMENT4_AUTHAPP_HPP

#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WServer.h>

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>


#include "model/Session.hpp"

class AuthApplication : public Wt::WApplication
{
public:
    AuthApplication(const Wt::WEnvironment& env);
    virtual ~AuthApplication() = default;

    void authEvent();

private:
    Session session_;
};


#endif
