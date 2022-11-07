#include <HelloApplication.hpp>

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPanel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

namespace Assignment4
{
HelloApplication::HelloApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Hello World!");
    setCssTheme("polished");

    auto pPanelContainer = std::make_unique<Wt::WContainerWidget>();
    pPanelContainer->addWidget(std::make_unique<Wt::WText>("Your name, please? "));

    _pNameEdit =  pPanelContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    Wt::WPushButton *button =  pPanelContainer->addWidget(std::make_unique<Wt::WPushButton>("Greet me."));
    pPanelContainer->addWidget(std::make_unique<Wt::WBreak>());
    _pGreeting =  pPanelContainer->addWidget(std::make_unique<Wt::WText>());

    auto greet = [this]{
        _pGreeting->setText("Hello There, " + _pNameEdit->text());
    };
    button->clicked().connect(greet);

    auto pPanel = std::make_unique<Wt::WPanel>();
    pPanel->setTitle("My Panel");
    pPanel->setCollapsible(true);
    pPanel->setCentralWidget(std::move(pPanelContainer));

    root()->addWidget(std::move(pPanel));
}
} // namespace Assignment4
