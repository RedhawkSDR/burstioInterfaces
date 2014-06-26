package burstio.stats;

import java.util.Collection;

public class SenderStatistics extends AbstractStatistics<StatPoint> {

    public SenderStatistics (final String name, int bitsPerElement)
    {
        super(name, bitsPerElement);
    }

    public void record (int bursts, int elements, float queueDepth, double delay)
    {
        this.addSample(new StatPoint(bursts, elements, queueDepth, delay));
    }

    protected Analysis<StatPoint> analyze (Collection<StatPoint> samples)
    {
        Analysis<StatPoint> analysis = new Analysis<StatPoint>();
        analysis.add(samples);
        return analysis;
    }
}
