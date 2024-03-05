#pragma once

#include "../graphics/renderer.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/voxelparticles.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>

class Shader;
class Mesh;
class VoxelParticles;

/**
 * @brief Менеджер ресурсов
 * 
 */
class ResourceManager {
    static std::string _path;

    static std::map<std::string, Shader*> _shaders;
    static std::map<std::string, VoxelModel*> _rowmodels;
    static std::map<std::string, Texture*> _textures;
    static std::map<std::string, Particles*> _particles;
    static std::map<std::string, FT_Face> _faces;

    static FT_Library _ft;
public:
    static void init(std::string str);
    static void cleanup();

    static void deleteShaders();
    static void deleteModels();
    static void deleteTextures();
    static void deleteParticles();

    static void loadShaders();
    static void loadTextures();
    static void loadModels();
    static void loadParticlesSystems();
    static void loadFonts();

    static void loadShader(std::string str);
    static void loadTexture(std::string str);
    static void loadModel(std::string str, std::string type);
    static void loadVoxelParticles(std::string str);
    static void loadFont(std::string str);

    static void prepareModel(std::string str);

    static void addShader(Shader* shader, std::string name);
    static void addTexture(Texture* texture, std::string name);
	static void addModel(VoxelModel* row, std::string name);
	static void addMesh(Mesh* mesh);
    static void addParticles(Particles* particles, std::string name);

    static Shader* getShader(std::string name);
    static Texture* getTexture(std::string name);
    static VoxelModel* getModel(std::string name);
    static Particles* getParticles(std::string name);
    static FT_Face getFont(std::string name);
};