//Some custom exceptions.
#ifndef __IceS2exceptions__
#define __IceS2exceptions__
#include <exception>

class IMPOSSIBLE_ERR:public std::exception
{
public:
	const char* what() const throw();
};
class NULL_INPUT_ERR:public std::exception
{
public:
	const char* what() const throw();
};
class EMPTY_LIST_ERR:public std::exception
{
public:
	const char* what() const throw();
};
class OVERFLOW_LIST_ERR:public std::exception
{
public:
	const char* what() const throw();
};
class MEMBER_DOES_NOT_EXIST_ERR:public std::exception
{
public:
	const char* what() const throw();
};

#endif