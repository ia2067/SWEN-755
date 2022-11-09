#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <AuthApplication.hpp>
#include <HelloApplication.hpp>

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env)
{
    return std::make_unique<AuthApplication>(env);
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
