#include <HelloApplication.hpp>

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

namespace Assignment4
{
HelloApplication::HelloApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Hello World!");

    root()->addWidget(std::make_unique<Wt::WText>("Your name, please? "));
    _pNameEdit = root()->addWidget(std::make_unique<Wt::WLineEdit>());

    Wt::WPushButton *button = root()->addWidget(std::make_unique<Wt::WPushButton>("Greet me."));
    root()->addWidget(std::make_unique<Wt::WBreak>());
    _pGreeting = root()->addWidget(std::make_unique<Wt::WText>());

    auto greet = [this]{
        _pGreeting->setText("Hello There, " + _pNameEdit->text());
    };
    button->clicked().connect(greet);
}
} // namespace Assignment4
