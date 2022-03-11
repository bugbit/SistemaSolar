#ifndef _CSV_H

#define _CSV_H

#include "astro.h"

enum EFieldHeader
{
    None,
    TYPE,
    NAME,
    TEXTURE,
    RADIUS,
    SEMI_MAYOR_AXIS,
    ECCENTRICITY
};

class Csv
{
public:
    Csv();
    bool load(const char *file);
    bool load(std::ifstream &stream);

private:
    std::map<std::string, EFieldHeader> valuesfieldsHeaders;
    std::map<std::string, COSMOTIPOELEMENTO> tipos;
    std::vector<EFieldHeader> fieldsheaders;

    bool readHeaders(std::ifstream &stream);
    inline COSMOTIPOELEMENTO getTipo(std::string &value)
    {
        return tipos[value];
    }
};

#endif