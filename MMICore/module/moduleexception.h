#ifndef MODULEEXCEPTION_H
#define MODULEEXCEPTION_H

#ifndef ModuleException_H
#define ModuleException_H

/**
 * \file ModuleException.h
 * \brief This exception is throw when a network error occurs
 * \author RIT Grégoire & VAILLANT Rémi
 */

#include <iostream>
#include <sstream>
#include <exception>

class ModuleException : public std::exception
{
public:
	ModuleException(const char * msg, int code) : _code(code)
	{
		std::ostringstream oss;
		oss << "Exception : " << msg << " . Code :"
				<< code;
		_msg = oss.str();
	}

	virtual ~ModuleException() throw(){}

	virtual const char * what() const throw() { return _msg.c_str(); }

	int code() const throw() { return _code; }

private:
	int _code;
	std::string _msg;
};

#endif // ModuleException_H


#endif // MODULEEXCEPTION_H
