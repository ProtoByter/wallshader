#include "Project.hpp"
#include <glbinding/gl/gl.h>

Project::Project(const char *path) {
    FILE* f = fopen(path, "r");
    fclose(f);
}

Project::~Project() {
    gl::glDeleteProgram(this->program);
}

void Project::Save(const char *path) {
    FILE* f = fopen(path, "w");
    fclose(f);
}
