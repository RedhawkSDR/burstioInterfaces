package burstio.stats;

import java.util.List;

import burstio.Utils;

class ReceiverAnalysis extends Analysis<ReceiverStatPoint> {
    protected void addKeywords (List<CF.DataType> keywords)
    {
        super.addKeywords(keywords);
        Utils.addKeyword(keywords, "QUEUE_FLUSHES", this.totalFlushes);
        Utils.addKeyword(keywords, "DROPPED_RATIO", this.totalDropped / (double)this.totalBursts);
    }

    public void add (ReceiverStatPoint sample) {
        super.add(sample);
        this.totalFlushes += sample.flushes;
        this.totalDropped += sample.dropped;
    }

    private int totalFlushes = 0;
    private int totalDropped = 0;
}
