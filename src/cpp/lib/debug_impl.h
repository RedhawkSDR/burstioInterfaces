#ifndef BURSTIO_DEBUG_IMPL_H
#define BURSTIO_DEBUG_IMPL_H

#include <burstio/debug.h>


#if NO_RH_LOGGER
#if HAVE_LOG4CXX
#define GET_LOGGER(classname) log4cxx::Logger::getLogger(#classname)
#define _LOG_TRACE(logger, expression) LOG4CXX_TRACE(logger, expression)
#define _LOG_DEBUG(logger, expression) LOG4CXX_DEBUG(logger, expression)
#define _LOG_INFO(logger, expression)  LOG4CXX_INFO(logger, expression)
#define _LOG_WARN(logger, expression)  LOG4CXX_WARN(logger, expression)
#define _LOG_ERROR(logger, expression) LOG4CXX_ERROR(logger, expression)
#define _LOG_FATAL(logger, expression) LOG4CXX_FATAL(logger, expression)
#else
#define GET_LOGGER(classname) #classname
#define _LOG_TRACE(logger, expression) _LOG(5, TRACE, logger, expression)
#define _LOG_DEBUG(logger, expression) _LOG(4, DEBUG, logger, expression)
#define _LOG_INFO(logger, expression)  _LOG(3, INFO,  logger, expression)
#define _LOG_WARN(logger, expression)  _LOG(2, WARN,  logger, expression)
#define _LOG_ERROR(logger, expression) _LOG(1, ERROR, logger, expression)
#define _LOG_FATAL(logger, expression) _LOG(0, FATAL, logger, expression)
#endif
#else
#define GET_LOGGER(classname) rh_logger::Logger::getLogger(#classname)
#define _LOG_TRACE(logger, expression) RH_TRACE(logger, expression)
#define _LOG_DEBUG(logger, expression) RH_DEBUG(logger, expression)
#define _LOG_INFO(logger, expression)  RH_INFO(logger, expression)
#define _LOG_WARN(logger, expression)  RH_WARN(logger, expression)
#define _LOG_ERROR(logger, expression) RH_ERROR(logger, expression)
#define _LOG_FATAL(logger, expression) RH_FATAL(logger, expression)
#endif

#define _LOG_INSTANCE(level, expression) _LOG_##level(this->__logger, expression)
#define LOG_INSTANCE_TRACE(expression) _LOG_INSTANCE(TRACE, expression)
#define LOG_INSTANCE_DEBUG(expression) _LOG_INSTANCE(DEBUG, expression)
#define LOG_INSTANCE_INFO(expression)  _LOG_INSTANCE(INFO, expression)
#define LOG_INSTANCE_WARN(expression)  _LOG_INSTANCE(WARN, expression)
#define LOG_INSTANCE_ERROR(expression) _LOG_INSTANCE(ERROR, expression)
#define LOG_INSTANCE_FATAL(expression) _LOG_INSTANCE(FATAL, expression)

#define PREPARE_CLASS_LOGGING(classname)                        \
    template <>                                                 \
    LoggerPtr classname::__classlogger(GET_LOGGER(classname));


#endif // BURSTIO_DEBUG_IMPL_H
