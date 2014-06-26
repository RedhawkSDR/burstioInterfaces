#ifndef BURSTIO_BURSTSTATISTICS_H
#define BURSTIO_BURSTSTATISTICS_H

#include <string>
#include <list>

#include <BULKIO/bio_runtimeStats.h>

namespace burstio {

    struct StatPoint
    {
        StatPoint();

        BULKIO::PrecisionUTCTime timestamp;

        size_t bursts;
        size_t elements;
        float queueDepth;
        float delay;

        StatPoint& operator+= (const StatPoint&);
    };

    template <class StatType>
    class BurstStatistics
    {
    public:
        BurstStatistics(const std::string& name, size_t bitsPerElement);
        ~BurstStatistics();

        void record (size_t bursts, size_t elements, float queueDepth, float delay);

        BULKIO::PortStatistics* retrieve() const;

    protected:
        typedef StatType stat_type;

        virtual void addKeywords_ (const stat_type& totals, size_t count,
                                   _CORBA_Sequence<CF::DataType>& keywords) const;

        const std::string name_;
        std::list<stat_type> statistics_;
        size_t bitsPerElement_;
        size_t windowSize_;
    };

    class SenderStatistics : public BurstStatistics<StatPoint>
    {
    public:
        SenderStatistics(const std::string& name, size_t bitsPerElement);

    private:
        typedef BurstStatistics<StatPoint> super;
    };

    struct ReceiverStatPoint : public StatPoint
    {
        ReceiverStatPoint();

        size_t flushes;
        size_t dropped;

        ReceiverStatPoint& operator+= (const ReceiverStatPoint&);
    };

    class ReceiverStatistics : public BurstStatistics<ReceiverStatPoint>
    {
    public:
        ReceiverStatistics(const std::string& name, size_t bitsPerElement);

        void flushOccurred(size_t bursts);

    private:
        typedef BurstStatistics<ReceiverStatPoint> super;

        virtual void addKeywords_ (const ReceiverStatPoint& totals, size_t count,
                                   _CORBA_Sequence<CF::DataType>& keywords) const;

        CORBA::ULong flushCount_;
        CORBA::ULong burstsDropped_;
    };
}

#endif // BURSTIO_BURSTSTATISTICS_H
