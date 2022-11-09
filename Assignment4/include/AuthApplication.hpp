#ifndef ASSIGNMENT4_AUTHAPP_HPP
#define ASSIGNMENT4_AUTHAPP_HPP

#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WServer.h>
#include <Wt/WPanel.h>

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>


#include "model/Session.hpp"

class AuthApplication : public Wt::WApplication
{
public:
    AuthApplication(const Wt::WEnvironment& env);
    virtual ~AuthApplication() = default;

    void authEvent();

    std::unique_ptr<Wt::WPanel> CreateInitialPanel();

private:
    Session session_;

private:
    Wt::WLineEdit* _pNameEdit;
    Wt::WText* _pGreeting;

private:
    std::unique_ptr<Wt::WPanel> _pInitialPanel;
};


#endif
