#pragma once

#include "x3dGL.h"


template <typename T>
class ModeConverterListener
{
public:
  virtual void onTriangle(T i1, T i2, T i3) = 0;
  virtual void onError(GLenum err) = 0;
};

template <typename T>
class ModeConverter
{
public:
  ModeConverter() {}

  void onBegin(GLenum mode)
  {
    m_mode = mode;
    m_state = 0;
  }

  void onError(GLenum err)
  {
    m_listener->onError(err);
  }

  void onIndex(T index)
  {
    if (m_mode == GL_TRIANGLES)
    {
      onTriangleIndex(index);
    }
    else if (m_mode == GL_TRIANGLE_STRIP)
    {
      onTriangleStripIndex(index);
    }
    else if (m_mode == GL_TRIANGLE_FAN)
    {
      onTriangleFanIndex(index);
    }
  }

  void setListener(ModeConverterListener<T>* listener) { m_listener = listener; }
  ModeConverterListener<T>* getListener() { return m_listener; }

private:
  void onTriangleIndex(T index)
  {
    m_indices[m_state] = index;
    ++m_state;

    if (m_state == 3)
    {
      m_state = 0;
      m_listener->onTriangle(m_indices[0], m_indices[1], m_indices[2]);
    }
  }

  void onTriangleStripIndex(T index)
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

      m_listener->onTriangle(m_indices[0], m_indices[1], m_indices[2]);

      m_indices[0] = m_indices[1];

      m_state = 3;
    }
    else if (m_state == 3)
    {
      m_indices[1] = index;

      m_listener->onTriangle(m_indices[0], m_indices[1], m_indices[2]);

      m_indices[0] = m_indices[2];

      m_state = 2;
    }
  }

  void onTriangleFanIndex(T index)
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
      m_listener->onTriangle(m_indices[0], m_indices[1], m_indices[2]);
      m_indices[1] = m_indices[2];
    }
  }

  ModeConverterListener<T>* m_listener;

  GLenum m_mode;
  size_t m_state;
  T m_indices[3];
};
