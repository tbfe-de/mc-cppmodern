#ifndef PXT_H
#define PXT_H

#include <iostream>

#define PX(expr)\
    ((void)(std::cout << __FILE__  << ':' << __LINE__\
                      << "\t" #expr " --> "\
                      << (expr) << std::endl))

#endif
