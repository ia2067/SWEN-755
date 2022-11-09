#include <HelloApplication.hpp>

#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPanel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>

namespace Assignment4
{
HelloApplication::HelloApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Hello World!");
    setCssTheme("polished");
    srand(time(NULL));

    int rVal = rand() % 4;
    std::cout << "R Value: " << rVal << std::endl;

    std::unique_ptr<Wt::WPanel> pPanel;

    if (rVal == 0)
        pPanel = CreateInitialPanel();
    else if (rVal == 1)
        pPanel = CreateStudentPanel();
    else if (rVal == 2)
        pPanel = CreateInstructorPanel();
    else 
        pPanel = CreateAdminPanel();

    root()->addWidget(std::move(pPanel));
}

std::unique_ptr<Wt::WPanel> HelloApplication::CreateInitialPanel()
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

std::unique_ptr<Wt::WPanel> HelloApplication::CreateStudentPanel()
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

std::unique_ptr<Wt::WPanel> HelloApplication::CreateInstructorPanel()
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

std::unique_ptr<Wt::WPanel> HelloApplication::CreateAdminPanel()
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

} // namespace Assignment4
