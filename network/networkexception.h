#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

/**
 * \file NetworkException.h
 * \brief This exception is throw when a network error occurs
 * \author RIT Grégoire & VAILLANT Rémi
 */

#include <iostream>
#include <sstream>
#include <exception>

class NetworkException : public std::exception
{
public:
	NetworkException(const char * msg, int code) : _code(code)
	{
		std::ostringstream oss;
		oss << "Exception : " << msg << " . Code :"
				<< code;
		_msg = oss.str();
	}

	virtual ~NetworkException() throw(){}

	virtual const char * what() const throw() { return _msg.c_str(); }

	int code() const throw() { return _code; }

private:
	int _code;
	std::string _msg;
};

#endif // NETWORKEXCEPTION_H
