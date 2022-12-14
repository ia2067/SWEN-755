#ifndef ASSIGNMENT4_HELLOAPP_HPP
#define ASSIGNMENT4_HELLOAPP_HPP

#include <Wt/WApplication.h>
#include <memory.h> //shared_ptr

namespace Wt
{
    class WLineEdit;
    class WText;
} // namespace Wt

namespace Assignment4
{

class HelloApplication : public Wt::WApplication
{
private:
    Wt::WLineEdit* _pNameEdit;
    Wt::WText* _pGreeting;
public:
    HelloApplication(const Wt::WEnvironment& env);
    virtual ~HelloApplication() = default;
private:
    std::unique_ptr<Wt::WPanel> CreateInitialPanel();
    std::unique_ptr<Wt::WPanel> CreateStudentPanel();
    std::unique_ptr<Wt::WPanel> CreateInstructorPanel();
    std::unique_ptr<Wt::WPanel> CreateAdminPanel();
}; 

} // namespace Assignment4




#endif