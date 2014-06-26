package burstio.stats;

class ReceiverStatPoint extends StatPoint {
    public ReceiverStatPoint(int bursts, int elements, float queueDepth, double delay, int flushes, int dropped)
    {
        super(bursts, elements, queueDepth, delay);
        this.flushes = flushes;
        this.dropped = dropped;
    }

    public int flushes;
    public int dropped;
}
