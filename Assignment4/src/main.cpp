#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <AuthApplication.hpp>
#include <HelloApplication.hpp>

#include "SessionManager.hpp"
#include "UserManager.hpp"

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env)
{
    return std::make_unique<AuthApplication>(env);
}

Assignment4::UserManager createUserManager() 
{
    Assignment4::UserManager user_man;

    user_man.addUser(Assignment4::User("user1", "user1", "User One", Assignment4::PermissionLevel::NORMAL));
    user_man.addUser(Assignment4::User("user2", "user2", "User Two", Assignment4::PermissionLevel::ELEVATED));
    user_man.addUser(Assignment4::User("user3", "user3", "User Three", Assignment4::PermissionLevel::ADMIN));
    
    return user_man;
}

Assignment4::SessionManager createSessionManager()
{
    Assignment4::SessionManager session_man;

    return session_man;
}

int main(int argc, char *argv[])
{
    std::cout << "Assignment 4" << std::endl;

    Assignment4::UserManager user_man = createUserManager();
    Assignment4::SessionManager session_man = createSessionManager();

    try {
        Wt::WServer server{argc, argv, WTHTTP_CONFIGURATION};

        server.addEntryPoint(Wt::EntryPointType::Application, createApplication);

        Session::configureAuth();

        server.run();
    } catch (Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (Wt::Dbo::Exception &e) {
        std::cerr << "Dbo exception: " << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}
