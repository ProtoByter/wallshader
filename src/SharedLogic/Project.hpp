#pragma once

#include <vector>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

struct ShaderSource {
    const char* source;
};

class Project {
public:
    Project() = delete;
    explicit Project(const char* path);
    void Save(const char* path);
    ~Project();
private:
    gl::GLuint program;
    std::vector<ShaderSource> sources;
};


