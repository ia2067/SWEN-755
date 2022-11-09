#include <AuthApplication.hpp>

#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WPanel.h>

std::string idleJavascript();

AuthApplication::AuthApplication(const Wt::WEnvironment& env,
                                 std::shared_ptr<Assignment4::UserManager> pUserManager,
                                 std::shared_ptr<Assignment4::SessionManager> pSessionManager)
: Wt::WApplication(env),
  _pUserManager(pUserManager),
  _pSessionManager(pSessionManager),
  session_(appRoot() + "auth.db")
{
    session_.login().changed().connect(this, &AuthApplication::authEvent);

    root()->addStyleClass("container");
    setTheme(std::make_shared<Wt::WBootstrapTheme>());

    useStyleSheet("css/style.css");

    std::unique_ptr<Wt::Auth::AuthWidget> authWidget = 
        std::make_unique<Wt::Auth::AuthWidget>(Session::auth(), session_.users(), session_.login());

    // authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    authWidget->model()->addOAuth(Session::oAuth());

    authWidget->setRegistrationEnabled(true);

    authWidget->processEnvironment();

    root()->addWidget(std::move(authWidget));

    instance()->session();
}

void AuthApplication::authEvent()
{
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();
      std::cout 
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in." << std::endl;
        std::string username = u.identity(Wt::Auth::Identity::LoginName).toUTF8();

        if(!_pUserManager->checkExist(username))
        {

            std::cout << "USER DOESN'T EXIST! ADDING THEM!" << std::endl;

            Assignment4::PermissionLevel auth_level;
            int uid= std::stoi(u.id());
            if(uid == 1)
                auth_level = Assignment4::ADMIN;
            else if (uid == 2)
                auth_level = Assignment4::ELEVATED;
            else
                auth_level = Assignment4::NORMAL;

            Assignment4::User newUser(u.id(), "", username, auth_level);
            _pUserManager->addUser(newUser);
        }

        _pSessionManager->authenticateSession(sessionId(),
                                              u.id());

        std::unique_ptr<Wt::WPanel> authorized_panel;
        if(_pUserManager->checkExist(u.id()))
        {
            auto user = _pUserManager->fetchUser(u.id());
            switch (user.getPermLevel())
            {

            case Assignment4::ELEVATED:
                authorized_panel = CreateInstructorPanel();
                break;
            case Assignment4::ADMIN:
                authorized_panel = CreateAdminPanel();
                /* code */
                break;

            case Assignment4::NORMAL:
            default: // NOTE: PASSTHROUGH
                authorized_panel = CreateStudentPanel();
                break;
            }
            _pCurrentPanel = root()->addWidget(std::move(authorized_panel));
            root()->doJavaScript(idleJavascript());
        }

    } else
    {
      std::cout << "User logged out." << std::endl;
      root()->removeWidget(_pCurrentPanel);
      
      _pSessionManager->deauthenticateSession(sessionId());
    }

}

std::unique_ptr<Wt::WPanel> AuthApplication::CreateStudentPanel()
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

std::unique_ptr<Wt::WPanel> AuthApplication::CreateInstructorPanel()
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

std::unique_ptr<Wt::WPanel> AuthApplication::CreateAdminPanel()
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

std::string idleJavascript()
{
    return R"(
    var script = document.createElement('script');
    script.src = 'https://code.jquery.com/jquery-3.6.1.min.js';
    script.type = 'text/javascript';
    document.getElementsByTagName('head')[0].appendChild(script);

    function addOverlay() {
	var elemDiv = document.createElement('div');
  
  elemDiv.style.cssText = "display: none; z-index: 2; background: #000; position: fixed; width: 100%; height: 100%; top: 0px;  left: 0px; text-align: center;"; 
  elemDiv.id = "overlay";
  document.body.appendChild(elemDiv);
    
  var textbox = document.createElement('h1')
  textbox.style.cssText = "background-color: black; color: white;";
  textbox.id = "overlay_text";
  //elemDiv.innerHTML = "<h1 style=\"background-color: black; color: white;\" id=overlay_text> </h1>"
  elemDiv.appendChild(textbox);
}

function toggleOverlay(){
	var overlay = document.getElementById('overlay');
	overlay.style.opacity = .3;
	if(overlay.style.display == "block"){
		overlay.style.display = "none";
	} else {
		overlay.style.display = "block";
	}
}

function displayOverlay(en) {
	if (en) {
  	overlay.style.display = "block";
  }
  else {
  	overlay.style.display = "none";
  }
}

function setOverlayText(text) {
	document.getElementById('overlay_text').innerHTML=text;
}

var idleTime = 0;
addOverlay();
$(document).ready(function () {
  // Increment the idle time counter every minute.
  var idleInterval = setInterval(timerIncrement, 1000); // 1 sec

  // Zero the idle timer on mouse movement.
  $(this).mousemove(function (e) {
    idleTime = 0;
  });
  $(this).keypress(function (e) {
    idleTime = 0;
  });
});

function timerIncrement() {
  idleTime++;

	warn_time = 7;
  kill_time = 15;

  
  if (idleTime < warn_time) {
    displayOverlay(false);
    console.log(idleTime)
  }
  else if (idleTime < kill_time) { // 10 sec
    setOverlayText("WARNING: Inactive " + idleTime + " sec. Ending Session at " + kill_time +" sec idle. Move cursor to extend session.")
    displayOverlay(true);
    console.log(idleTime)
  } else {
    alert("Session Expired due to inactivity.");
    idleTime = 0;
    $(".Wt-auth-logged-in > button").click();
    setOverlayText("");
    displayOverlay(false);
    console.log("Session Ended.")
    clearInterval(idleInterval);
  }
})";
}