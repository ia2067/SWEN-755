#include <AuthApplication.hpp>

#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WPanel.h>

AuthApplication::AuthApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env),
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

    _pInitialPanel = CreateInitialPanel();

}

void AuthApplication::authEvent()
{
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();
      log("notice")
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in.";

        root()->addWidget(std::move(_pInitialPanel));
    } else
    {
      log("notice") << "User logged out.";
      root()->removeWidget(_pInitialPanel.get());
    }

}

std::unique_ptr<Wt::WPanel> AuthApplication::CreateInitialPanel()
{
    auto panelContainer = std::make_unique<Wt::WContainerWidget>();
    panelContainer->addWidget(std::make_unique<Wt::WText>("Your name, please? "));

    _pNameEdit =  panelContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    Wt::WPushButton *button =  panelContainer->addWidget(std::make_unique<Wt::WPushButton>("Greet me."));
    panelContainer->addWidget(std::make_unique<Wt::WBreak>());
    _pGreeting =  panelContainer->addWidget(std::make_unique<Wt::WText>());

    auto greet = [this]{
        _pGreeting->setText("Hello There, " + _pNameEdit->text());
    };
    button->clicked().connect(greet);

    auto pPanel = std::make_unique<Wt::WPanel>();
    pPanel->setTitle("My Panel");
    pPanel->setCollapsible(true);
    pPanel->setCentralWidget(std::move(panelContainer));

    return pPanel;
}