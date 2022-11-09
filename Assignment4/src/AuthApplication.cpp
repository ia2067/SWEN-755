#include <AuthApplication.hpp>

#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WPanel.h>

AuthApplication::AuthApplication(const Wt::WEnvironment& env,
                                 std::shared_ptr<Assignment4::UserManager> pUserManager,
                                 std::shared_ptr<Assignment4::SessionManager> pSessionManager)
: Wt::WApplication(env),
  _pUserManager(pUserManager),
  _pSessionManager(pSessionManager),
  session_(appRoot() + "auth.db")
{
    session_.login().changed().connect(this, &AuthApplication::authEvent);

    root()->addStyleClass("container");
    setTheme(std::make_shared<Wt::WBootstrapTheme>());

    useStyleSheet("css/style.css");

    std::unique_ptr<Wt::Auth::AuthWidget> authWidget = 
        std::make_unique<Wt::Auth::AuthWidget>(Session::auth(), session_.users(), session_.login());

    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->model()->addOAuth(Session::oAuth());

    authWidget->setRegistrationEnabled(true);

    authWidget->processEnvironment();

    root()->addWidget(std::move(authWidget));

    instance()->session();
}

void AuthApplication::authEvent()
{
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();
      std::cout 
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in." << std::endl;
        std::string username = u.identity(Wt::Auth::Identity::LoginName).toUTF8();

        if(!_pUserManager->checkExist(username))
        {
            std::cout << "USER DOESN'T EXIST! ADDING THEM!" << std::endl;
            Assignment4::User newUser(u.id(), "", username, Assignment4::NORMAL);
            _pUserManager->addUser(newUser);
        }

        _pSessionManager->authenticateSession(sessionId(),
                                              u.id());

        std::unique_ptr<Wt::WPanel> authorized_panel;
        if(_pUserManager->checkExist(u.id()))
        {
            auto user = _pUserManager->fetchUser(u.id());
            switch (user.getPermLevel())
            {

            case Assignment4::ELEVATED:
                authorized_panel = CreateInstructorPanel();
                break;
            case Assignment4::ADMIN:
                authorized_panel = CreateAdminPanel();
                /* code */
                break;

            case Assignment4::NORMAL:
            default: // NOTE: PASSTHROUGH
                authorized_panel = CreateStudentPanel();
                break;
            }
            _pCurrentPanel = root()->addWidget(std::move(authorized_panel));
        }

    } else
    {
      std::cout << "User logged out." << std::endl;
      root()->removeWidget(_pCurrentPanel);
      _pSessionManager->deauthenticateSession(sessionId());
      
    }

}

std::unique_ptr<Wt::WPanel> AuthApplication::CreateStudentPanel()
{
    auto panelContainer = std::make_unique<Wt::WContainerWidget>();
    panelContainer->addWidget(std::make_unique<Wt::WText>("Enter your favorite color: "));

    _pNameEdit =  panelContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    Wt::WPushButton *button =  panelContainer->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    panelContainer->addWidget(std::make_unique<Wt::WBreak>());
    _pGreeting =  panelContainer->addWidget(std::make_unique<Wt::WText>());

    auto greet = [this]{
        _pGreeting->setText("Thank you, " + _pNameEdit->text() + " is a beautiful color!");
    };
    button->clicked().connect(greet);

    auto pPanel = std::make_unique<Wt::WPanel>();
    pPanel->setTitle("Student Panel");
    pPanel->setCollapsible(true);
    pPanel->setCentralWidget(std::move(panelContainer));

    return pPanel;
}

std::unique_ptr<Wt::WPanel> AuthApplication::CreateInstructorPanel()
{
    auto panelContainer = std::make_unique<Wt::WContainerWidget>();
    panelContainer->addWidget(std::make_unique<Wt::WText>("Enter your favorite fruit: "));

    _pNameEdit =  panelContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    Wt::WPushButton *button =  panelContainer->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    panelContainer->addWidget(std::make_unique<Wt::WBreak>());
    _pGreeting =  panelContainer->addWidget(std::make_unique<Wt::WText>());

    auto greet = [this]{
        _pGreeting->setText("Thank you, " + _pNameEdit->text() + " is a delicious fruit!");
    };
    button->clicked().connect(greet);

    auto pPanel = std::make_unique<Wt::WPanel>();
    pPanel->setTitle("Instructor Panel");
    pPanel->setCollapsible(true);
    pPanel->setCentralWidget(std::move(panelContainer));

    return pPanel;
}

std::unique_ptr<Wt::WPanel> AuthApplication::CreateAdminPanel()
{
    auto panelContainer = std::make_unique<Wt::WContainerWidget>();
    panelContainer->addWidget(std::make_unique<Wt::WText>("Enter your favorite animal: "));

    _pNameEdit =  panelContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    Wt::WPushButton *button =  panelContainer->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    panelContainer->addWidget(std::make_unique<Wt::WBreak>());
    _pGreeting =  panelContainer->addWidget(std::make_unique<Wt::WText>());

    auto greet = [this]{
        _pGreeting->setText("Thank you, " + _pNameEdit->text() + " is cute, I want one as a pet!");
    };
    button->clicked().connect(greet);

    auto pPanel = std::make_unique<Wt::WPanel>();
    pPanel->setTitle("Admin Panel");
    pPanel->setCollapsible(true);
    pPanel->setCentralWidget(std::move(panelContainer));

    return pPanel;
}