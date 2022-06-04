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
    GLboolean loadCubeMap(
        const char *PosXFilename,
        const char *NegXFilename,
        const char *PosYFilename,
        const char *NegYFilename,
        const char *PosZFilename,
        const char *NegZFilename);

    inline void BindTexture() const
    {
        glBindTexture(target, texture);
    }

private:
    GLenum target = GL_TEXTURE_2D;
    GLuint texture;

    GLboolean loadCubeMapSide(GLenum side, const char *filename);
};

#endif