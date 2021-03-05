#pragma once

/**
 * This implements a thread that periodically fetches repositories.
 */
class FetchWorker
{
public:
    FetchWorker();

    void run();
};

