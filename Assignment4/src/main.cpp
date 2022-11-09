#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <AuthApplication.hpp>
#include <HelloApplication.hpp>

#include "SessionManager.hpp"
#include "UserManager.hpp"

std::shared_ptr<Assignment4::UserManager> createUserManager() 
{
    std::shared_ptr<Assignment4::UserManager> user_man = std::make_shared<Assignment4::UserManager>();

    user_man->addUser(Assignment4::User("user1", "user1", "User One", Assignment4::PermissionLevel::NORMAL));
    user_man->addUser(Assignment4::User("user2", "user2", "User Two", Assignment4::PermissionLevel::ELEVATED));
    user_man->addUser(Assignment4::User("user3", "user3", "User Three", Assignment4::PermissionLevel::ADMIN));
    
    return user_man;
}

std::shared_ptr<Assignment4::SessionManager> createSessionManager()
{
    std::shared_ptr<Assignment4::SessionManager> session_man = std::make_shared<Assignment4::SessionManager>();

    return session_man;
}

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env)
{

    std::shared_ptr<Assignment4::UserManager> user_man = createUserManager();
    std::shared_ptr<Assignment4::SessionManager> session_man = createSessionManager();


    return std::make_unique<AuthApplication>(env,
                                             user_man,
                                             session_man);
}

int main(int argc, char *argv[])
{
    std::cout << "Assignment 4" << std::endl;

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
