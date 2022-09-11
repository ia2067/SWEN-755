# firstCmakeProject

This project I am mostly using to test. 
## File Hierarchy

```bash
.
├── CMakeLists.txt
├── include
│   ├── HelloWorld.hpp
│   └── HelloWorldPrinter.hpp
├── README.md
└── src
    ├── HelloWorld.cpp
    ├── HelloWorldPrinter.cpp
    └── main.cpp

2 directories, 7 files
```
## Whats happening
In it's current state there is a lot going on.

The program ```HelloWorld``` is the main entry point. It will spin off a child process which calls ```HelloWorld_helper```. This child process (which is a different executable), will create a ```HelloWorldPrinter``` object, which has it's own thread. This thread will count down from 10 before saying 'Hello World!'. ```HelloWorld_helper``` will then wait for the thread to join, and the exit. At which point, ```HelloWorld``` has been waiting for this process to finish, and will finish itself.