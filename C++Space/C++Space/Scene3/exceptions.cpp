#include "exceptions.hpp"

const char* IMPOSSIBLE_ERR::what() const throw()
{
	return "IMPOSSIBLE ERROR: CHECK YOUR CODE!!!";
}

const char* NULL_INPUT_ERR::what() const throw()
{
	return "NULL POINTER ERROR: NULL POINTER GIVEN AS INPUT!!!";
}

const char* EMPTY_LIST_ERR::what() const throw()
{
	return "EMPTY LIST ERROR: UNABLE TO PERFORM OPERATION ON EMPTY LIST!!!";
}

const char* OVERFLOW_LIST_ERR::what() const throw()
{
	return "LIST FULL ERROR: UNABLE TO ADD OBJECT TO FULL LIST!!! OBJECT NUMBER EXCEEDED UINT_MAX VALUE!!!";
}

const char* MEMBER_DOES_NOT_EXIST_ERR::what() const throw()
{
	return "MEMBER DOES NOT EXIST ERROR: A REQUEST WAS MADE FOR A LIST MEMBER THAT DOES NOT EXIST!!!";
}