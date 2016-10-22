#ifndef ACK_H
#define ACK_H

using namespace std;

#include <utility>
#include <string>

namespace ACK {
    void notify_client_job_done(pair<int, string> const& job);
}

#endif
