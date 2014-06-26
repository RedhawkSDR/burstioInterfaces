package burstio.stats;

import java.util.Collection;
import java.util.List;

import burstio.Utils;

public class ReceiverStatistics extends AbstractStatistics<ReceiverStatPoint> {
    
    public ReceiverStatistics (final String name, int bitsPerElement)
    {
        super(name, bitsPerElement);
    }

    public void record (int bursts, int elements, float queueDepth, double delay)
    {
        this.addSample(new ReceiverStatPoint(bursts, elements, queueDepth, delay, 0, 0));
    }

    public void flushOccurred (int bursts)
    {
        ReceiverStatPoint last = this.statistics_.getLast();
        last.flushes++;
        last.dropped += bursts;

        this.flushCount_++;
        this.burstsDropped_ += bursts;
    }

    protected Analysis<ReceiverStatPoint> analyze (Collection<ReceiverStatPoint> samples)
    {
        Analysis<ReceiverStatPoint> analysis = new ReceiverAnalysis();
        analysis.add(samples);
        return analysis;
    }

    protected void addKeywords (List<CF.DataType> keywords)
    {
        if (this.flushCount_ > 0) {
            Utils.addKeyword(keywords, "FLUSH_COUNT", this.flushCount_);
            Utils.addKeyword(keywords, "BURSTS_DROPPED", this.burstsDropped_);
        }
    }

    private int flushCount_ = 0;
    private int burstsDropped_ = 0;
}
