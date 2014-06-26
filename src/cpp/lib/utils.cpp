#include <time.h>

#include <burstio/utils.h>

namespace burstio {
    namespace utils { 
        BULKIO::PrecisionUTCTime now ()
        {
            struct timespec tod;
            clock_gettime(CLOCK_REALTIME, &tod);
            BULKIO::PrecisionUTCTime tstamp;
            tstamp.tcmode = BULKIO::TCM_CPU;
            tstamp.tcstatus = 1;
            tstamp.toff = 0.0;
            tstamp.twsec = tod.tv_sec;
            tstamp.tfsec = tod.tv_nsec * 1e-9;
            return tstamp;
        }

        double elapsed (const BULKIO::PrecisionUTCTime& begin, const BULKIO::PrecisionUTCTime& end)
        {
            return (end.twsec - begin.twsec) + (end.tfsec - begin.tfsec);
        }
    }
}
