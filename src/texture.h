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
    /*
GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front
*/
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