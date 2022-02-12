#ifndef _TEXTURE_H

#define _TEXTURE_H

class Texture2d
{
public:
    inline Texture2d() : texture(0) {}
    inline ~Texture2d()
    {
        Delete();
    }
    inline void Delete()
    {
        if (texture)
        {
            glGenTextures(1, &texture);
            texture = 0;
        }
    }
    GLboolean load(const char *filename);

    inline void BindTexture() const
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

private:
    GLuint texture;
};

#endif