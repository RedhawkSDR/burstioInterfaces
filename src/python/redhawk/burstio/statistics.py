import time

from omniORB import any

from ossie.cf import CF
from bulkio.bulkioInterfaces import BULKIO

class BurstStatistics(object):
    def __init__(self, name, bytesPerElement, windowSize=10):
        self._name = name
        self._bitsPerElement = bytesPerElement * 8
        self._windowSize = windowSize
        self._statistics = []

    def _addKeywords(self, keywords, totals):
        pass

    def _sampleAdded(self, sample):
        pass

    def record (self, bursts, elements, queueDepth, delay):
        point = {
            'bursts':     bursts,
            'elements':   elements,
            'queueDepth': queueDepth,
            'delay':      delay,
            'timestamp':  time.time()
        }
        self._sampleAdded(point)
        
        self._statistics.append(point)
        if len(self._statistics) > self._windowSize:
            self._statistics.pop(0)

    def retrieve (self):
        totals = {}
        for stat in self._statistics:
            for key, value in stat.iteritems():
                totals[key] = totals.get(key, 0) + value

        if totals:
            first = self._statistics[0]
            current_time = time.time()
            totals['elapsed'] = current_time - first['timestamp']
            totals['count'] = len(self._statistics)

            elements_per_second = totals['elements'] / totals['elapsed']
            bits_per_second = elements_per_second * self._bitsPerElement
            avg_queue_depth = totals['queueDepth'] / totals['count']
            calls_per_sec = totals['count'] / totals['elapsed']
            time_since_last_call = current_time - self._statistics[-1]['timestamp']

            bursts_per_second =  totals['bursts'] / totals['elapsed']
            bursts_per_push = totals['bursts'] / float(totals['count'])
            elements_per_burst = totals['elements'] / float(totals['bursts'])
            average_latency = totals['delay'] / totals['count']
        else:
            elements_per_second = 0.0
            bits_per_second = 0.0
            calls_per_sec = 0.0
            avg_queue_depth = 0.0
            time_since_last_call = 0.0
            bursts_per_second = 0.0
            bursts_per_push = 0.0
            elements_per_burst = 0.0
            average_latency = 0.0

        # To be filled in by caller
        stream_ids = []

        # Add burst-specific stats to keywords
        burst_stats = [
            ('BURSTS_PER_SECOND',  bursts_per_second),
            ('BURSTS_PER_PUSH',    bursts_per_push),
            ('ELEMENTS_PER_BURST', elements_per_burst),
            ('AVERAGE_LATENCY',    average_latency)
        ]
        self._addKeywords(burst_stats, totals)
        keywords = [CF.DataType(k, any.to_any(v)) for k, v in burst_stats]

        return BULKIO.PortStatistics(self._name, elements_per_second, bits_per_second,
                                     calls_per_sec, stream_ids, avg_queue_depth,
                                     time_since_last_call, keywords)

class ReceiverStatistics(BurstStatistics):
    def __init__ (self, *args, **kwargs):
        BurstStatistics.__init__(self, *args, **kwargs)
        self._flushCount = 0
        self._lastFlushCount = 0
        self._burstsDropped = 0
        self._lastBurstsDropped = 0

    def _sampleAdded(self, sample):
        sample['flushes'] = self._flushCount - self._lastFlushCount
        sample['dropped'] = self._burstsDropped - self._lastBurstsDropped
        self._lastFlushCount = self._flushCount
        self._lastBurstsDropped = self._burstsDropped

    def flushOccurred (self, bursts):
        self._flushCount += 1
        self._burstsDropped += bursts

    def _addKeywords (self, keywords, totals):
        flushes = totals.get('flushes', 0)
        keywords.append(('QUEUE_FLUSHES', flushes))
        if 'bursts' in totals:
            drop_rate = totals['dropped'] / float(totals['bursts'])
        else:
            drop_rate = 0.0
        keywords.append(('DROPPED_RATIO', drop_rate))
        if self._flushCount > 0:
            keywords.append(('FLUSH_COUNT', self._flushCount))
            keywords.append(('BURSTS_DROPPED', self._burstsDropped))
