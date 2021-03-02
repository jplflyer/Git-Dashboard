# Git-Dashboard
This project provides a GUI dashboard for showing the status of your current Git repositories. It's the sort of thing you can leave running on a spare monitor, and with a glance, see the status of your repos. Each active repository has a rectangle with its name and current branch name. The background color represents whether this is the main/master branch, and a frame around the box indicates:

    * A blue frame indicates there are local commits that are not pushed
    * A red frame indicates there are upstream commits that are not pulled

That's it. I use this to keep me from doing new development on a release branch (for instance).

# Dependencies
This is a Qt project. You must have Qt installed to build.

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

# Supported Systems
This is a Qt project and should be buildable on any platform, but I am running this on Mac, so you might have to do a little work. For now, I open the GUI project in Qt Creator and build from there.

## To Do for Building

    * Build and test under Windows and Linux
    * Make a build system that doesn't require you to run it from Qt Creator

# Configuration
The first time you run Git-Dashboard, it looks for a configuration file. On Mac, it looks for:

    * ~/Library/Application Support/Git-Dashboard/config.json

If not found, one is created with default values. It should be self-explanatory, and it can be managed entirely from the GUI.

# License
Released under the MIT license, which is attached to this project.
