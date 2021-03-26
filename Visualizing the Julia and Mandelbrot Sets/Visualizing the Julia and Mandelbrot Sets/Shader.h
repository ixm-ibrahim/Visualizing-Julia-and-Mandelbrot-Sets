#ifndef SHADER
#define SHADER

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
    private:
        void CheckCompileErrors(unsigned int, std::string);

    public:
        unsigned int ID;

        Shader();
        Shader(const char*, const char*);
        Shader(const char*, const char*, const char*, const char*);
        ~Shader();

        void Use();

        void SetBool(const std::string&, bool) const;
        void SetInt(const std::string&, int) const;
        void SetFloat(const std::string&, float) const;

        void SetVec2(const std::string&, const glm::vec2&) const;
        void SetVec2(const std::string&, float, float) const;
        void SetVec3(const std::string&, const glm::vec3&) const;
        void SetVec3(const std::string&, float, float, float) const;
        void SetVec4(const std::string&, const glm::vec4&) const;
        void SetVec4(const std::string&, float, float, float, float);

        void SetMat2(const std::string&, const glm::mat2&) const;
        void SetMat3(const std::string&, const glm::mat3&) const;
        void SetMat4(const std::string&, const glm::mat4&) const;
};

#endif