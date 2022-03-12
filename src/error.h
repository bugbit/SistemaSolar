#ifndef _ERROR_H

#define _ERROR_H

class Error : public std::ostringstream
{
public:
    inline void print() const
    {
        std::cerr << str() << std::endl;
    }
};

extern Error aerr;

#endif