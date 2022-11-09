#ifndef ASSIGNMENT4_AUTHAPP_HPP
#define ASSIGNMENT4_AUTHAPP_HPP

#include <Wt/WApplication.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WServer.h>
#include <Wt/WPanel.h>

#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/PasswordService.h>

#include <UserManager.hpp>
#include <SessionManager.hpp>

#include "model/Session.hpp"

class AuthApplication : public Wt::WApplication
{
public:
    AuthApplication(const Wt::WEnvironment& env,
                    std::shared_ptr<Assignment4::UserManager> pUserManager,
                    std::shared_ptr<Assignment4::SessionManager> pSessionManager);
    virtual ~AuthApplication() = default;

    void authEvent();

    std::unique_ptr<Wt::WPanel> CreateStudentPanel();
    std::unique_ptr<Wt::WPanel> CreateInstructorPanel();
    std::unique_ptr<Wt::WPanel> CreateAdminPanel();

private:
    std::shared_ptr<Assignment4::UserManager> _pUserManager;
    std::shared_ptr<Assignment4::SessionManager> _pSessionManager;

private:
    Session session_;

private:
    Wt::WLineEdit* _pNameEdit;
    Wt::WText* _pGreeting;

private:
    Wt::WPanel* _pCurrentPanel;
};


#endif
