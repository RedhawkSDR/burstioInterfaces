#ifndef BURSTIO_DEBUG_H
#define BURSTIO_DEBUG_H

#include <ossie/debug.h>

namespace burstio {

#ifdef NO_RH_LOGGER

#if HAVE_LOG4CXX
    typedef log4cxx::LoggerPtr LoggerPtr;
#else
    typedef std::string LoggerPtr;
#endif

    inline LoggerPtr getChildLogger (LoggerPtr parent, const std::string& name)
    {
#if HAVE_LOG4CXX
        std::string log_name;
        parent->getName(log_name);
        log_name += "." + name;
        return log4cxx::Logger::getLogger(log_name);
#else
        return parent + "." + name;
#endif
    }

#else 
    typedef rh_logger::LoggerPtr LoggerPtr;

    inline LoggerPtr getChildLogger (LoggerPtr parent, const std::string& name)
    {
        std::string log_name;
        parent->getName(log_name);
        log_name += "." + name;
        return rh_logger::Logger::getLogger(log_name);
    }

#endif

}


#define ENABLE_INSTANCE_LOGGING                 \
    private:                                    \
        static LoggerPtr __classlogger;         \
        LoggerPtr __logger;

#endif // BURSTIO_DEBUG_H
