#ifndef _IGENERATOR_HPP_
#define _IGENERATOR_HPP_

class IGenerator {
public:
    virtual void generate() = 0;
    virtual ~IGenerator() {}
};

#endif 
