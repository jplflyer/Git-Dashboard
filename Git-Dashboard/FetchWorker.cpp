#include <chrono>
#include <iostream>
#include <thread>

#include <git/Git.h>

#include "Configuration.h"
#include "FetchWorker.h"

using std::chrono::seconds;
using std::cout;
using std::endl;

/**
 * The constructor will start our thread.
 */
FetchWorker::FetchWorker() {
    std::thread myThread( [=](){ run(); } );
    myThread.detach();
}

/**
 * Run forever.
 */
void
FetchWorker::run() {
    Configuration & config = Configuration::singleton();

    while(true) {
        Repository::Vector repos = config.getRepositories();
        for (const Repository::Pointer &repo: repos) {
            repo->fetch();
        }
        std::this_thread::sleep_for(seconds(60));
    }
}
