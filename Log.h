#pragma once
#include <iostream>

//打印日志
#if (defined DEBUG) || (defined _DEBUG)

#define myLog (std::cout << __FILE__ << "[" << __LINE__ << "] " << __FUNCTION__ << " ")

#else

struct nullstream : std::ostream {
	nullstream() : std::ios(0), std::ostream(0) {}
};

static nullstream logstream;
	
#define myLog logstream

#endif //  DEBUG