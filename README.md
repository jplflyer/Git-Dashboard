# Git-Dashboard
This project provides a GUI dashboard for showing the status of your current Git repositories. It's the sort of thing you can leave running on a spare monitor, and with a glance, see the status of your repos. Each active repository has a rectangle with its name and current branch name. The background color represents whether this is the main/master branch, and a frame around the box indicates:

  * A blue frame indicates there are local commits that are not pushed
  * A red frame indicates there are upstream commits that are not pulled

That's it. I use this to keep me from doing new development on a release branch (for instance).

# Status
Consider this version 0.9. It's a pretty simple app, after all. Missing:

  * Does not currently perform the fetch, so it won't report ahead/behind
  * It's REALLY ugly

# Where to Download
Once I have version 1.0.0 complete, I'll stand up a server for downloads.

# Dependencies
This is a Qt project. You must have Qt installed to build. I'm using version 5.15.2 as of March, 2021.

We require libgit2. As of March, 2021:

    cd /tmp
    wget https://github.com/libgit2/libgit2/releases/download/v1.1.0/libgit2-1.1.0.tar.gz
    tar xzf libgit2*
    cd libgit2-1.1.0
    mkdir build
    cd build
    export PKG_CONFIG_PATH="$OPENSSL_ROOT_DIR/lib/pkgconfig"
    export OPENSSL_ROOT_DIR="/usr/local/opt/openssl@1.1/"
    cmake .. -DBUILD_SHARED_LIBS=off
    cmake --build .
    cmake --build . --target install

This builds with static instead of dynamic libraries, which reduces link issues. The install will go into /usr/local/lib and include as appropriate. I have no clue what happens on Windows.

libgit2 requires openssl and ssh2. See the LIBS list in the Git-Dashboard.pro file.

You also need two of my libraries:

  * git@github.com:jplflyer/ShowLib.git
  * git@github.com:jplflyer/GitLib.git

Clone each of htem and then in the above order, cd in and:

    make
    sudo make install

# Supported Systems
This is a Qt project and should be buildable on any platform, but I am running this on Mac, so you might have to do a little work. For now, I open the GUI project in Qt Creator and build from there.

## To Do for Building

  * Build and test under Windows and Linux
  * Make a build system that doesn't require you to build it from Qt Creator

# Installation
Qt Creator will create a build directory with a name like `build-Git-Dashboard-Desktop_Qt_5_15_1_clang_64bit-Debug`. Inside there you'll find `Git-Dashboard.app`. Just copy that to your /Applications directory.

# Configuration
The first time you run Git-Dashboard, it looks for a configuration file. On Mac, it looks for:

    ~/Library/Application Support/Git-Dashboard/config.json

If not found, one is created with default values. It should be self-explanatory, and it can be managed entirely from the GUI.

You can use the configuration window to add repos to watch. If you're on the main window, you can get back to the configuration window from the menu.

# Contributing
Feel free to clone, update, but please issue a pull request. See the Conventions.md file.

# License
Released under the MIT license, which is attached to this project.
