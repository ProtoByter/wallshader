#pragma once
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <SDL.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlobj.h>

using namespace gl;

namespace Utils {
    void GL_APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    void InitMessageCallback();

    void setKey(std::string& key);

    std::string getKey();
}