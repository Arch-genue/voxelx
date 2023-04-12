#include "resourceloader.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string ResourceLoader::respath = "";

void ResourceLoader::setPath(std::string str) {
    respath = str;
}

void ResourceLoader::loadShaders() {
    std::string path = respath + "shaders/";
    std::string* paths;
    paths = new std::string[100];
    for (const auto & entry : fs::directory_iterator(path)) {
        //Shader* shader = load_shader(entry.path().string(), entry.path().string());
        //paths = entry.path();
        //std::cout << entry.path() << std::endl;
    }
    delete[] paths;
}
void ResourceLoader::loadTextures() {
    std::string path = respath + "textures/";
    for (const auto & entry : fs::directory_iterator(path)) {
        //std::cout << entry.path() << std::endl;
    }
}
void ResourceLoader::loadModels() {
    std::string path = respath + "models/";
    for (const auto & entry : fs::directory_iterator(path)) {
        //std::cout << entry.path() << std::endl;
    }
}