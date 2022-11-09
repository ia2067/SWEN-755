#include <model/Session.hpp>
#include <model/User.hpp>

#include <vector>

#include <Wt/Auth/Dbo/AuthInfo.h>

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Auth/GoogleService.h>
#include <Wt/Dbo/backend/Sqlite3.h>

using namespace Wt;

namespace {

    Wt::Auth::AuthService myAuthService;
    Wt::Auth::PasswordService myPasswordService{myAuthService};
    std::vector<std::unique_ptr<Wt::Auth::OAuthService>> myOAuthServices;

}


Session::Session(const std::string& sqliteDb)
{
    auto connection = std::make_unique<Dbo::backend::Sqlite3>(sqliteDb);
    // connection->setProperty("show-queries", "true");
    setConnection(std::move(connection));

    mapClass<User>("user");
    mapClass<AuthInfo>("auth_info");
    mapClass<AuthInfo::AuthIdentityType>("auth_identity");
    mapClass<AuthInfo::AuthTokenType>("auth_token");

    try {
        createTables();
        std::cerr << "Created database." << std::endl;
    } catch (Wt::Dbo::Exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing Database";
    }

    _pUsers = std::make_unique<UserDatabase>(*this);
    
}

void Session::configureAuth()
{
    myAuthService.setAuthTokensEnabled(true, "logincookie");
    myAuthService.setEmailVerificationEnabled(true);
    myAuthService.setEmailVerificationRequired(true);

    std::unique_ptr<Wt::Auth::PasswordVerifier> verifier =
        std::make_unique<Wt::Auth::PasswordVerifier>();
    verifier->addHashFunction(std::make_unique<Wt::Auth::BCryptHashFunction>(7));
    myPasswordService.setVerifier(std::move(verifier));
    myPasswordService.setAttemptThrottlingEnabled(true);
    myPasswordService.setStrengthValidator(std::make_unique<Wt::Auth::PasswordStrengthValidator>());

    if (Wt::Auth::GoogleService::configured())
        myOAuthServices.push_back(std::make_unique<Wt::Auth::GoogleService>(myAuthService));

    for (unsigned i = 0; i < myOAuthServices.size(); ++i)
        myOAuthServices[i]->generateRedirectEndpoint();
}

Auth::AbstractUserDatabase& Session::users()
{
    return *_pUsers;
}
const Auth::AuthService& Session::auth()
{
    return myAuthService;
}
const Auth::PasswordService& Session::passwordAuth()
{
    return myPasswordService;
}
const std::vector<const Auth::OAuthService *> Session::oAuth()
{
    std::vector<const Auth::OAuthService *> result;
    for (auto &auth : myOAuthServices) {
        result.push_back(auth.get());
    }
    return result;
}
