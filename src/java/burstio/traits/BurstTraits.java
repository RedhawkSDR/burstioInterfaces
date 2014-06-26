package burstio.traits;

import java.util.Collection;

public interface BurstTraits<B,A>
{
    public int byteSize();
    public int burstLength(B burst);
    public B[] toArray(Collection<B> bursts);
    public B createBurst(A data, BURSTIO.BurstSRI sri, BULKIO.PrecisionUTCTime timestamp, boolean eos);
    public BURSTIO.BurstSRI sri(B burst);
    public boolean eos(B burst);
}
