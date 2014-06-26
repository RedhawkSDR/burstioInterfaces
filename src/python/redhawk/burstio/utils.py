import math
import time

from bulkio.bulkioInterfaces import BULKIO
from redhawk.burstioInterfaces import BURSTIO

def now():
    fsec, wsec = math.modf(time.time())
    return createTimestamp(wsec, fsec)

def createTimestamp(wsec, fsec=0.0):
    return BULKIO.PrecisionUTCTime(BULKIO.TCM_CPU, BULKIO.TCS_VALID, 0.0, wsec, fsec)

def createSRI(streamID, id_='', xdelta=1.0, mode=0, flags=0, tau=0.0, theta=0.0, gain=0.0,
              uwlength=0, bursttype=0, burstLength=0, CHAN_RF=0.0, baudestimate=0.0,
              carrieroffset=0.0, SNR=0.0, modulation='', baudrate=0.0, fec='', fecrate='',
              randomizer='', overhead='', expectedStartOfBurstTime=None):
    if expectedStartOfBurstTime is None:
        expectedStartOfBurstTime = createTimestamp(0.0)
    hversion = 1
    return BURSTIO.BurstSRI(hversion, streamID, id_, xdelta, mode, flags, tau, theta, gain, uwlength,
                            bursttype, burstLength, CHAN_RF, baudestimate, carrieroffset,
                            SNR, modulation, baudrate, fec, fecrate, randomizer, overhead,
                            expectedStartOfBurstTime, [])

def usec_to_sec(usec):
    return usec * 1e-6

def sec_to_usec(sec):
    return sec * 1e6
