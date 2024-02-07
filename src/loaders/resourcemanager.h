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
    static std::string _path;

    static std::map<std::string, Shader*> _shaders;
    static std::map<std::string, VoxelModel*> _rowmodels;
    static std::map<std::string, Texture*> _textures;
    static std::map<std::string, Particles*> _particles;
public:
    static void init(std::string str);
    static void loadShader(std::string str);
    static void loadTexture(std::string str);
    static void loadModel(std::string str, std::string type);
    static void loadVoxelParticles(std::string str);

    static void addShader(Shader* shader, std::string name);
    static void addTexture(Texture* texture, std::string name);
	static void addModel(VoxelModel* row, std::string name);
	static void addMesh(Mesh* mesh);
    static void addParticles(Particles* particles, std::string name);

    static Shader* getShader(std::string name);
    static Texture* getTexture(std::string name);
    static VoxelModel* getModel(std::string name);
    static Particles* getParticles(std::string name);
};