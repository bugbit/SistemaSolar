#include "texture.h"

GLboolean Texture2d::load(const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);

    if (surface == NULL)
    {
        std::cerr << "IMG_Load:  " << IMG_GetError() << std::endl;

        return GL_FALSE;
    }

    target = GL_TEXTURE_2D;

    /* Create The Texture */
    glGenTextures(1, &texture);

    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture(GL_TEXTURE_2D, texture);

    std::cout << "Loaded texture " << filename << " " << surface->w << "x" << surface->h << std::endl;

    // Enforce RGB/RGBA
    int format;
    SDL_Surface *formattedSurf;
    if (surface->format->BytesPerPixel == 3)
    {

        formattedSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
        format = GL_RGB;
    }
    else
    {
        formattedSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        format = GL_RGBA;
    }

    /* Generate The Texture */
    glTexImage2D(GL_TEXTURE_2D, 0, format, formattedSurf->w,
                 formattedSurf->h, 0, format,
                 GL_UNSIGNED_BYTE, formattedSurf->pixels);

    /* Linear Filtering */
    // Reducción/Ampliación de Textura
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Interpolación Mapeado de texturas
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_FreeSurface(formattedSurf);
    SDL_FreeSurface(surface);

    return GL_TRUE;
}

/*
GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front
*/
GLboolean Texture2d::loadCubeMap(
    const char *PosXFilename,
    const char *NegXFilename,
    const char *PosYFilename,
    const char *NegYFilename,
    const char *PosZFilename,
    const char *NegZFilename)
{
    target = GL_TEXTURE_CUBE_MAP;

    /* Create The Texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    if (!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, PosXFilename))
        return GL_FALSE;

    if (!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, NegXFilename))
        return GL_FALSE;
    if (!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, PosYFilename))
        return GL_FALSE;
    if (!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, NegYFilename))
        return GL_FALSE;
    if (!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, PosZFilename))
        return GL_FALSE;
    if (!loadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, NegZFilename))
        return GL_FALSE;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return GL_TRUE;
}

GLboolean Texture2d::loadCubeMapSide(GLenum side, const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);

    if (surface == NULL)
    {
        std::cerr << "IMG_Load:  " << IMG_GetError() << std::endl;

        return GL_FALSE;
    }

    std::cout << "Loaded texture " << filename << " " << surface->w << "x" << surface->h << std::endl;

    // Enforce RGB/RGBA
    int format;
    SDL_Surface *formattedSurf;
    if (surface->format->BytesPerPixel == 3)
    {

        formattedSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
        format = GL_RGB;
    }
    else
    {
        formattedSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        format = GL_RGBA;
    }

    /* Generate The Texture */
    glTexImage2D(side, 0, format, formattedSurf->w, formattedSurf->h, 0, format, GL_UNSIGNED_BYTE, formattedSurf->pixels);

    SDL_FreeSurface(formattedSurf);
    SDL_FreeSurface(surface);

    return GL_TRUE;
}