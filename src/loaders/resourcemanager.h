#pragma once

#include "../graphics/shader.h"
#include "../graphics/mesh.h"
#include "../graphics/texture.h"
#include "../graphics/voxelparticles.h"

#include <string>
#include <map>

class Shader;
class Mesh;
class VoxelParticles;

class ResourceManager {
public:    
    static std::string path;

    static std::map<std::string, Shader*> shaders;
    static std::map<std::string, _voxels*> rowmodels;
    static std::map<std::string, Texture*> textures;
    static std::map<std::string, Particles*> particles;

    static void init(std::string str);
    static void loadShader(std::string str);
    static void loadTexture(std::string str);
    static void loadModel(std::string str, std::string type);
    static void loadVoxelParticles(std::string str);

    // static void addShader(Shader* shader, std::string name);
    static void addShader(Shader* shader, std::string name);
    static void addTexture(Texture* texture, std::string name);
	static void addModel(_voxels* row, std::string name);
	static void addMesh(Mesh* mesh);

    static Shader* getShader(std::string name);
    static Texture* getTexture(std::string name);
    static _voxels* getModel(std::string model);
};