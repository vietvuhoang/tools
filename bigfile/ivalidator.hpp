#ifndef _IVALIDATOR_HPP_
#define _IVALIDATOR_HPP_

class IValidator {
public:
    virtual void validate() = 0;
    virtual ~IValidator() {}
};

#endif 
