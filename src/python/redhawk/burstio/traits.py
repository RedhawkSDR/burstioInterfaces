import numpy

from redhawk.burstioInterfaces import BURSTIO

class Traits(object):
    """
    Base traits class for describing relationship of CORBA and numeric types.
    """
    def __init__(self, port, burst, dtype, cxtype=None):
        self.PortType = port
        self.BurstType = burst
        self.NativeType = dtype
        self.ComplexType = cxtype

    def size(self):
        """
        Returns the byte size of a single native type element (e.g., 4 bytes
        for 'long').
        """
        return self.NativeType().itemsize

    def toArray(self, data):
        """
        Returns the CORBA-formatted input data as a native-typed numpy array.
        """
        return numpy.array(data, dtype=self.NativeType)

    def toComplexArray(self, data):
        """
        Returns the CORBA-formatted input data as a 2-D numpy array of size
        (N/2, 2). Each element in the array is a 2-tuple of real and imag
        values.

        Floating point types (float, double) return a 1-D array of native
        complex values.
        """
        return self.toArray(data).reshape(len(data)/2, 2)

class CharTraits(Traits):
    """
    Traits class for types that serialize as string data in CORBA.
    """
    def toArray(self, data):
        return numpy.fromstring(data, dtype=self.NativeType)

    # Clone the parent toArray docstring
    toArray.__doc__ = Traits.toArray.__doc__

class FPTraits(Traits):
    """
    Traits class for floating point types, which support native complex types.
    """
    def toComplexArray(self, data):
        """
        Returns the CORBA-formatted input data as a numpy array of native
        complex values.
        """
        return self.toArray(data).view(dtype=self.ComplexType)

ByteTraits = CharTraits(BURSTIO.burstByte, BURSTIO.ByteBurst, numpy.int8)
DoubleTraits = FPTraits(BURSTIO.burstDouble, BURSTIO.DoubleBurst, numpy.float64, numpy.complex128)
FloatTraits = FPTraits(BURSTIO.burstFloat, BURSTIO.FloatBurst, numpy.float32, numpy.complex64)
LongTraits = Traits(BURSTIO.burstLong, BURSTIO.LongBurst, numpy.int32)
LongLongTraits = Traits(BURSTIO.burstLongLong, BURSTIO.LongLongBurst, numpy.int64)
ShortTraits = Traits(BURSTIO.burstShort, BURSTIO.ShortBurst, numpy.int16)
UbyteTraits = CharTraits(BURSTIO.burstUbyte, BURSTIO.UbyteBurst, numpy.uint8)
UlongTraits = Traits(BURSTIO.burstUlong, BURSTIO.UlongBurst, numpy.uint32)
UlongLongTraits = Traits(BURSTIO.burstUlongLong, BURSTIO.UlongLongBurst, numpy.uint64)
UshortTraits = Traits(BURSTIO.burstUshort, BURSTIO.UshortBurst, numpy.uint16)
