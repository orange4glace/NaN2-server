// logger.h
#ifndef LOGGER_H_
#define LOGGER_H_
#define BOOST_LOG_DYN_LINK 1

#include <boost/log/trivial.hpp>

#define L_DEBUG BOOST_LOG_TRIVIAL(debug)

#endif