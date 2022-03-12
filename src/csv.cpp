#include "csv.h"
#include "error.h"

Csv::Csv() : galaxia(NULL)
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
        aerr << "Could not open the file - '" << file << "'" << std::endl;

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

    Estrella *estrella = NULL;
    Planeta *planeta = NULL;

    galaxia = NULL;
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
                    if (galaxia == NULL)
                    {
                        aerr << "In line : " << linea << "You have defined the star without first creating the galaxy" << std::endl;

                        return false;
                    }
                    elem = estrella = new Estrella();
                case PLANETA:
                    if (estrella == NULL)
                    {
                        aerr << "In line : " << linea << "You have defined the planet without first creating the star" << std::endl;

                        return false;
                    }
                    elem = planeta = new Planeta();
                    estrella->add(planeta);
                    break;

                default:
                    elem = NULL;
                    break;
                }
                break;
            case NAME:
                if (elem == NULL)
                {
                    aerr << "In line : " << linea << "The first column must be 'type'" << std::endl;

                    return false;
                }
                elem->setName(value);
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
        aerr << "Without headers - '" << std::endl;

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