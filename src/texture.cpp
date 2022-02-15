#include "texture.h"

GLboolean Texture2d::load(const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);

    if (surface == NULL)
    {
        std::cerr << "IMG_Load:  " << IMG_GetError() << std::endl;

        return GL_FALSE;
    }

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(formattedSurf);
    SDL_FreeSurface(surface);

    return GL_TRUE;
}