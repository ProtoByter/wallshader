#include "Utils.hpp"

namespace Utils {
    void GL_APIENTRY MessageCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar* message, const void*) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenGL Error", message, nullptr);
    }

    void InitMessageCallback() {
    #ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
    #endif
        glDebugMessageCallback(Utils::MessageCallback, 0);
    }

    std::wstring getConfigPath() {
        PWSTR cfgPath = nullptr;
        SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &cfgPath);
        return {cfgPath};
    }

    void setKey(std::string& key) {
        FILE* keyfile = _wfopen((getConfigPath()+L"/wallshader.key").c_str(), L"w");
        fwrite(key.c_str(), sizeof(key[0]), key.size(), keyfile);
        fclose(keyfile);
    }

    std::string getKey() {
        FILE* keyfile = _wfopen((getConfigPath()+L"/wallshader.key").c_str(), L"r");

        if (!keyfile) {
            std::string key = "XXXXX-XXXXX-XXXXX-XXXXX";
            setKey(key);
            return key;
        }

        std::string key;
        key.resize(24, '\0');
        fread(&key[0], sizeof(key[0]), key.size(), keyfile);
        fclose(keyfile);
        return key;
    }
}