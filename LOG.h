/**================================================================================================
*
Basic logger utility.
Has three levels. Use DEBUG_LOG(T), INFO_LOG(T) or ERROR_LOG(T) to log.

Doesn't log if LOG_LEVEL is undefined.
Priorities are as follows: [LOG_LEVEL_ERROR] > [LOG_LEVEL_INFO] > [LOG_LEVEL_DEBUG]

================================================================================================**/

#pragma once
#include <string>
#include <iostream>

#if defined(LOG_LEVEL_DEBUG)
#define DEBUG_LOG(msg) std::cout << "DEBUG: "; std::cout << msg << std::endl;
#endif
#if not defined(LOG_LEVEL_DEBUG)
#define DEBUG_LOG(msg);
#endif

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO)
#define INFO_LOG(msg) std::cout << "INFO: "; std::cout << msg << std::endl;
#endif
#if not defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO)
#define INFO_LOG(msg);
#endif

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || defined(LOG_LEVEL_ERROR)
#define ERROR_LOG(msg) std::cout << "ERROR: "; std::cout << msg << std::endl;
#endif
#if not defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || defined(LOG_LEVEL_ERROR))
#define ERROR_LOG(msg);
#endif