- [SWEN-755 Softrware Architecture](#swen-755-softrware-architecture)
  - [How to use this repo](#how-to-use-this-repo)
    - [Environment](#environment)
    - [Setup](#setup)
      - [VSCode Setup](#vscode-setup)
      - [CLI setup](#cli-setup)
  - [Projects](#projects)
    - [firstCmakeProject](#firstcmakeproject)


# SWEN-755 Softrware Architecture
This repo is for Ian, Tyler and Dave to complete the SWEN-755 Software Architecture Projects. Most of the code will be C++ and compiled with CMake
## How to use this repo
Each folder represents a CMake 'project', for the time being all project is it's own unique cmake project, where the toplevel simply pulls them all in. My prefered method of editing / building is with VSCode, however it should be possible to also use whatever IDE you want, but I will just outline how to setup VSCode and then also how to use a CLI, through linux or wsl or otherwise.

### Environment
I personally am using a Ubuntu Server 22.04 VM on my desktop as an environment for both building and running. Probably would be best to all use this, however not exactly necessary. I might experiment with [WSL](https://docs.microsoft.com/en-us/windows/wsl/about) later if time allows. There is CMake for Windows, however I have never used it, but it should all be cross platform.

### Setup
1. (optional) Setup Github SSH keys
   
   This is very nice to not have to enter your password in all the time, a 'how to' on this can be found [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh).
2.  Clone the code
3.  Ensure the build tools are installed, we need `gcc`, `cmake` and `gdb`. 
   
    They *should* be included with build-essentials (`sudo apt install build-essentials`) but you can check the versions of each to be sure they are there, and manually install each if necessary.
#### VSCode Setup
Using VScode probably requires the most amount of initial setup but imo is very worth it. There are a lot of tools built in for source code navigation and debugging.
1. Point a window of VSCode at the checked out code.
   1. To do this, I used the Remote-SSH extention to SSH into my environment.
2. Once in, you should be prompted with a toast to install some recommened extensions, install them. They should be the following.
   1. C/C++
   2. C/C++ Extension Pack
   3. CMake
   4. CMake Tools
   5. GitLens - Git supercharged
   6. Markdown All in One
   7. C/C++ Themes
3. Select a CMake kit
4. Configure the project (should be done automatically when it realizes it is a cmake project)
5. Navigate to the CMake extention tab, here you can see all the 'targets' for every project. 
6. Build away!

#### CLI setup
Using the CLI is actually very simple from here. 
1. `cd` into the directory of the project you want to build.
2. `mkdir build` to create a build directory
3. `cd build` to enter the build directory
4. Use `cmake ..` to configure the project
5. run `make` to build. If all goes well, you should see something along the lines of `[100%] Built Target So and so`
6. Any executables built will then be in this build folder. 

## Projects
### firstCmakeProject
This project is mostly a test-bed for playing with CMake for me. It can be used as an example for creating new projects, as time progresses I will add more and more to this 'hello world' project.

Source can be found here: [Source](firstCmakeProject)
