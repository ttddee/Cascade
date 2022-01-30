#pragma once

#include <string>
#include <vector>
#include <memory>

class SpvCompiler {
public:
    SpvCompiler();
    ~SpvCompiler();

    //bool compileGLSLFromFile(const std::string& path);
    bool compileGLSLFromCode(const std::string& code, const std::string& shaderType);

    std::vector<unsigned int> getSpirV();
    std::string getError();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

};

