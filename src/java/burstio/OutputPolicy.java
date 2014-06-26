package burstio;

public interface OutputPolicy {
    public int getMaxBursts ();
    public void setMaxBursts (int bursts);
    public int getLatencyThreshold ();
    public void setLatencyThreshold (int usec);
    public int getByteThreshold ();
    public void setByteThreshold (int bytes);
    public void flush ();
}
