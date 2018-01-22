#ifndef PXT_H
#define PXT_H

#include <iostream>
#include <sstream>
#include <boost/type_index.hpp>

#define PX(...)\
	((void)(std::cout << "::: " << __FUNCTION__\
			  << '[' << __LINE__ << ']'\
                          << "\t" #__VA_ARGS__ " --> " << (__VA_ARGS__)\
			  << std::endl))

#define PT(...)\
	((void)(std::cout << "::: " << __FUNCTION__\
			  << '[' << __LINE__ << ']'\
                          << "\t" #__VA_ARGS__ " --> "\
			  << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
			  << std::endl))

#define PX_(expect, ...)\
	do {\
		std::ostringstream result;\
		result.flags(std::cout.flags());\
		result.precision(std::cout.precision());\
		result << (__VA_ARGS__);\
		++PN_::done;\
		if (result.str() == expect) break;\
		std::cout << "::: " << __FUNCTION__ << '[' << __LINE__ << ']'\
                          << "\t" #__VA_ARGS__ " --> " << result.str()\
			  << "\t!= " << expect << std::endl;\
		++PN_::fail;\
	} while (false)

#define PT_(expect, ...)\
	do {\
		std::ostringstream result;\
		result.flags(std::cout.flags());\
		result.precision(std::cout.precision());\
		result << boost::typeindex::type_id_with_cvr<__VA_ARGS__>();\
		++PN_::done;\
		if (result.str() == expect) break;\
		std::cout << "::: " << __FUNCTION__ << '[' << __LINE__ << ']'\
                          << "\t" #__VA_ARGS__ " --> " << result.str()\
			  << "\t!= " << expect << std::endl;\
		++PN_::fail;\
	} while (false)

#define PN_(testname)\
	struct PN_ {\
		PN_() {\
			std::cout << "... " #testname << std::endl;\
		}\
		~PN_() {\
			std::cout << "=== " << __FILE__ << ": "\
				  << done << " tests, "\
				  << fail << " failed."\
				  << std::endl;\
		}\
		static int done, fail;\
	} PN_test;\
	int PN_::done, PN_::fail;

#endif
