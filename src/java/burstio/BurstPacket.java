package burstio;

abstract class BurstPacket<A> {
    public String getStreamID() {
        return this.sri_.streamID;
    }

    public abstract int getSize ();
    public abstract A getData ();

    public boolean isComplex () {
        return (this.sri_.mode == 1);
    }

    public boolean getEOS () {
        return this.eos_;
    }

    public BULKIO.PrecisionUTCTime getTime () {
        return this.time_;
    }

    public BURSTIO.BurstSRI getSRI () {
        return this.sri_;
    }

    public boolean blockOccurred () {
        return this.blockOccurred_;
    }

    protected BurstPacket(boolean eos, BURSTIO.BurstSRI sri, BULKIO.PrecisionUTCTime time, boolean blockOccurred) {
        this.eos_ = eos;
        this.sri_ = sri;
        this.time_ = time;
        this.blockOccurred_ = blockOccurred;
    }

    private boolean eos_;
    private BURSTIO.BurstSRI sri_;
    private BULKIO.PrecisionUTCTime time_;
    private boolean blockOccurred_;
}
