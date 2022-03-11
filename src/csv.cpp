#include "csv.h"

Csv::Csv()
{
    valuesfieldsHeaders["type"] = TYPE;
    valuesfieldsHeaders["name"] = NAME,
    valuesfieldsHeaders["texture"] = TEXTURE;
    valuesfieldsHeaders["radius"] = RADIUS;
    valuesfieldsHeaders["a"] = SEMI_MAYOR_AXIS;
    valuesfieldsHeaders["e"] = ECCENTRICITY;
    tipos["galaxy"] = GALAXIA;
    tipos["star"] = ESTRELLA;
    tipos["planet"] = PLANETA;
    tipos["satellite"] = SATELITE;
}

bool Csv::load(const char *file)
{
    std::ifstream stream(file);

    std::cout << "read file - '" << file << "'" << std::endl;
    if (!stream.is_open())
    {
        std::cerr << "Could not open the file - '" << file << "'" << std::endl;

        return false;
    }

    return load(stream);
}

bool Csv::load(std::ifstream &stream)
{
    std::string line;

    // read headers
    if (!readHeaders(stream))
        return false;

    Galaxia *galaxia = NULL;
    Estrella *estrella = NULL;
    Planeta *planeta = NULL;

    for (int linea = 2; std::getline(stream, line); linea++)
    {
        std::istringstream svalue(line);
        std::string value;
        CosmoElemento *elem = NULL;

        for (int i = 0; std::getline(svalue, value, '\t'); i++)
        {
            switch (fieldsheaders[i])
            {
            case TYPE:
                switch (getTipo(value))
                {
                case GALAXIA:
                    elem = galaxia = new Galaxia();
                    break;
                case ESTRELLA:
                    elem = estrella = new Estrella();
                    if (galaxia == NULL)
                    {
                        // cout << "In line : " << linea << "You have defined the star without first creating the galaxy"
                    }
                    break;

                default:
                    elem = NULL;
                    break;
                }
                break;

            default:
                break;
            }
        }
    }

    return true;
}

bool Csv::readHeaders(std::ifstream &stream)
{
    std::string line;

    if (!std::getline(stream, line))
    {
        std::cerr << "Without headers - '" << std::endl;

        return false;
    }

    std::istringstream svalue(line);
    std::string value;

    while (std::getline(svalue, value, '\t'))
    {
        auto field = valuesfieldsHeaders[value.c_str()];

        fieldsheaders.push_back(field);
    }

    return true;
}