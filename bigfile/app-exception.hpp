#ifndef _APP_EXCEPTION_HPP_
#define _APP_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

class AppException : public std::logic_error
{
  public:
    explicit AppException(const std::string &message) : std::logic_error(message)
    {
    }

    AppException() : std::logic_error("") {
    }

    virtual ~AppException() throw()
    {
    }
    
};

#endif 