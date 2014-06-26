package burstio.stats;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Deque;
import java.util.LinkedList;
import java.util.List;

import org.ossie.properties.AnyUtils;

import burstio.Utils;

abstract class AbstractStatistics<E extends StatPoint> {

    private final String name_;
    private final int bitsPerElement_;
    private final int windowSize_ = 10;

    protected Deque<E> statistics_ = new LinkedList<E>();

    public AbstractStatistics (final String name, int bitsPerElement) {
        this.name_ = name;
        this.bitsPerElement_ = bitsPerElement;
    }

    public BULKIO.PortStatistics retrieve ()
    {
        Analysis<E> analysis = this.analyze(this.statistics_);

        float elements_per_second = analysis.getElementsPerSecond();
        float bits_per_second = elements_per_second * this.bitsPerElement_;

        List<CF.DataType> keywords = analysis.getKeywords();
        this.addKeywords(keywords);
        CF.DataType[] stat_keywords = keywords.toArray(new CF.DataType[keywords.size()]);

        return new BULKIO.PortStatistics(this.name_,
                                         elements_per_second,
                                         bits_per_second,
                                         analysis.getCallsPerSecond(),
                                         new String[0],
                                         analysis.getAverageQueueDepth(),
                                         analysis.getTimeSinceLastCall(),
                                         stat_keywords);
    }

    protected void addSample (E sample)
    {
        this.statistics_.add(sample);
        if (this.statistics_.size() > this.windowSize_) {
            this.statistics_.remove();
        }
    }

    protected void addKeywords (List<CF.DataType> keywords)
    {
    }

    protected abstract Analysis<E> analyze (Collection<E> samples);
}
