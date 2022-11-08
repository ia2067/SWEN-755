#include <AuthApplication.hpp>

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

}

void AuthApplication::authEvent()
{
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();
      log("notice")
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in.";
    } else
      log("notice") << "User logged out.";
}