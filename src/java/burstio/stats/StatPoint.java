package burstio.stats;

import burstio.Utils;

class StatPoint {
    public StatPoint(int bursts, int elements, float queueDepth, double delay)
    {
        this.timestamp = Utils.now();
        this.bursts = bursts;
        this.elements = elements;
        this.queueDepth = queueDepth;
        this.delay = delay;
    }

    public BULKIO.PrecisionUTCTime timestamp;
    public int bursts;
    public int elements;
    public float queueDepth;
    public double delay;
}
