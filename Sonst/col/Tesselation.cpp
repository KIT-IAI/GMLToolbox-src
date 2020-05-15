#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Tesselation.hpp"
#include "Context.hpp"

namespace col
{
    class ModeConverter
    {
    public:
        ModeConverter(TrianglesPtr& triangles) : m_triangles(triangles) {}

        void onBegin(GLenum mode)
        {
            m_mode = mode;
            m_state = 0;
        }

        void onIndex(size_t index)
        {
            if (m_mode == GL_TRIANGLES)
            {
                m_indices[m_state] = index;
                ++m_state;

                if (m_state == 3)
                {
                    m_state = 0;
                    m_triangles->addIndex(m_indices[0]);
                    m_triangles->addIndex(m_indices[1]);
                    m_triangles->addIndex(m_indices[2]);
                }
            }
            else if (m_mode == GL_TRIANGLE_STRIP)
            {
                if (m_state == 0)
                {
                    m_indices[0] = index;
                    ++m_state;
                }
                else if (m_state == 1)
                {
                    m_indices[1] = index;
                    ++m_state;
                }
                else if (m_state == 2)
                {
                    m_indices[2] = index;

                    m_triangles->addIndex(m_indices[0]);
                    m_triangles->addIndex(m_indices[1]);
                    m_triangles->addIndex(m_indices[2]);

                    m_indices[0] = m_indices[1];

                    m_state = 3;
                }
                else if (m_state == 3)
                {
                    m_indices[1] = index;

                    m_triangles->addIndex(m_indices[0]);
                    m_triangles->addIndex(m_indices[1]);
                    m_triangles->addIndex(m_indices[2]);

                    m_indices[0] = m_indices[2];
                    //m_indices[1] = m_indices[];

                    m_state = 2;
                }
            }
            else if (m_mode == GL_TRIANGLE_FAN)
            {
                if (m_state == 0)
                {
                    m_indices[0] = index;
                    ++m_state;
                }
                else if (m_state == 1)
                {
                    m_indices[1] = index;
                    ++m_state;
                }
                else
                {
                    m_indices[2] = index;
                    m_triangles->addIndex(m_indices[0]);
                    m_triangles->addIndex(m_indices[1]);
                    m_triangles->addIndex(m_indices[2]);
                    m_indices[1] = m_indices[2];
                }
            }
        }

    private:
        GLenum m_mode;
        size_t m_state;
        size_t m_indices[3];

        TrianglesPtr& m_triangles;
    };

    void convertIndices(GLUtesselator* tess, const Vertices& vertices, Indices indices)
    {
        //std::reverse(indices.begin(), indices.end());

        gluTessBeginContour(tess);

        for (size_t i(0); i < indices.size(); ++i)
        {
            size_t index = indices[i];

            const Vector3& position = vertices[index].getPosition();

            GLdouble coords[3];
            coords[0] = position.getX();
            coords[1] = position.getY();
            coords[2] = position.getZ();

            // bad bad bad bad bad bad bad bad
            gluTessVertex(tess, coords, reinterpret_cast<void*>(index));
        }

        gluTessEndContour(tess);
    }

    void CALLBACK onBegin(GLenum type, void* data)
    {
        ModeConverter* c = static_cast<ModeConverter*>(data);
        c->onBegin(type);
    }

    void CALLBACK onIndex(void* indexPtr, void* data)
    {
        ModeConverter* c = static_cast<ModeConverter*>(data);
        size_t index = reinterpret_cast<size_t>(indexPtr);

        c->onIndex(index);
    }

    void CALLBACK onError(GLenum errno, void* data)
    {
        assert(false);
    }

    typedef void(CALLBACK *NoParam)();

    TrianglesPtr tesselate(Context& context, const MeshPtr& mesh, const PolygonsPtr& polygons)
    {
        const Vertices& vertices = mesh->getVertices();

        TrianglesPtr triangles = context.createTriangles();
        ModeConverter m(triangles);

        GLUtesselator* tess = gluNewTess();

        gluTessCallback(tess, GLU_TESS_BEGIN_DATA, reinterpret_cast<NoParam>(&onBegin));
        gluTessCallback(tess, GLU_TESS_VERTEX_DATA, reinterpret_cast<NoParam>(&onIndex));
        gluTessCallback(tess, GLU_TESS_ERROR_DATA, reinterpret_cast<NoParam>(&onError));

        gluTessBeginPolygon(tess, &m);

        const Indices& indices = polygons->getIndices();
        convertIndices(tess, vertices, indices);

        const Holes& holes = polygons->getHoles();
        for (Holes::const_iterator it = holes.begin(); it != holes.end(); ++it)
        {
            const Indices& indices = *it;
            convertIndices(tess, vertices, indices);
        }

        gluTessEndPolygon(tess);

        gluDeleteTess(tess);

        triangles->setMaterial(polygons->getMaterial().lock());

        return triangles;
    }
}

