#ifndef _VAO_H

#define _VAO_H

#define POSITION_ATTRIBUTE 0
#define NORMAL_ATTRIBUTE 2
#define DIFFUSE_ATTRIBUTE 3
#define SPECULAR_ATTRIBUTE 4
#define TEXCOORD0_ATTRIBUTE 8
#define TEXCOORD1_ATTRIBUTE 9
#define TEXCOORD2_ATTRIBUTE 10

#define BUFFER_OFFSET(offset) ((void *)(offset))

class VAO
{
public:
    inline VAO() : vao(0), vbos(NULL)
    {
    }
    inline ~VAO()
    {
        Delete();
    }
    inline void Delete()
    {
        if (vbos)
        {
            delete[] vbos;

            vbos = NULL;
        }
    }

    inline GLuint getVAO() const
    {
        return vao;
    }

    /*
    layout(location=0) in vec3 in_position;
    layout(location=2) in vec3 in_normal;
    layout(location=8) in vec2 in_texcoord;
    */
    GLboolean MakeSolidSphere(float radius, int slices, int stacks, GLsizei &numIndicies);
    GLboolean MakeSkybox();

protected:
    GLuint vao, *vbos;

private:
    static GLfloat sky_vertices[];
    static GLuint sky_indices[];
};

#endif