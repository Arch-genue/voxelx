#include "gameobject.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../voxels/voxel.h"
#include "../loaders/resourcemanager.h"


GameObject::GameObject(const char* model) : TransformObject() {
	_id = 9999999;
	
	_campos = glm::vec3(0);
	setPosition(glm::vec3(0));

    setVisible(true);

	_voxelmodel = ResourceManager::getModel(model);
	_sizes = _voxelmodel->getSize();
	
	BoxCollider* collider = new BoxCollider(this, glm::vec3(_position),_voxelmodel->getSize());

	_physicsobject = new PhysicsObject(this, collider, 10.0f);
	setPosition(glm::vec3(0));

    // !CREATE MESH!
	if (_voxelmodel->getMesh() == nullptr) {
		_voxelmodel->setMesh(Renderer::render(_voxelmodel));
	}

	_mesh = _voxelmodel->getMesh();
}
GameObject::GameObject(const char *model, glm::vec3 position) : TransformObject() {
	_id = 9999999;
	
	_campos = glm::vec3(0);

    setVisible(true);

	_voxelmodel = ResourceManager::getModel(model);
	_sizes = _voxelmodel->getSize();
	
	BoxCollider* collider = new BoxCollider(this, glm::vec3(position),_voxelmodel->getSize());

	_physicsobject = new PhysicsObject(this, collider, 10.0f);
	setPosition(position);

    // !CREATE MESH!
	if (_voxelmodel->getMesh() == nullptr) {
		_voxelmodel->setMesh(Renderer::render(_voxelmodel));
	}

	_mesh = _voxelmodel->getMesh();
}
GameObject::~GameObject() {}

void GameObject::setID(uint id) {
	if (_id != 9999999){
		std::cerr << "ID already set!" << std::endl;
	} else {
		_id = id;
	}
}

void GameObject::setGameManager(GameManager* gamemanager) {
	_gm = gamemanager;
}
GameManager* GameObject::getGameManager() {
	return _gm;
}

void GameObject::attachCamera(Camera* cam, glm::vec3 stdpos) {
	_camera = cam;
	_campos = stdpos;
	_camera->setPosition(getPosition() + _campos);
}

PhysicsObject* GameObject::getPhysicsObject() {
	return _physicsobject;
}

void GameObject::setPosition(glm::vec3 position) {
	TransformObject::setPosition(position);
	
	_physicsobject->setPosition(position);
}

void GameObject::draw() {
	if (isVisible()) {
		_mesh->draw(GL_TRIANGLES, getMatrix(), ResourceManager::getShader("voxel"));
		// _boundingboxmesh->draw(GL_LINES);
	}
};

void GameObject::setPhysics(PHYSICS physics) {
	switch(physics) {
		case NO_PHYSICS:
			_physicsobject->getCollider()->setMax(glm::vec3(0, 0, 0)); 
			_physicsobject->setPhysics(physics);
		break;
		case STATIC_PHYSICS:
		case DYNAMIC_PHYSICS:
			glm::vec3 sizes = _voxelmodel->getSize();
			_physicsobject->getCollider()->setMax(glm::vec3(sizes.x, sizes.y, sizes.z));

			std::vector<line> lines = _physicsobject->getVertices();
			float vertices[72];

			uint d = 0;
			for (size_t i = 0; i < lines.size(); i++) {
                vertices[d] 	= lines[i].vertex1.x;
				vertices[d + 1] = lines[i].vertex1.y;
				vertices[d + 2] = lines[i].vertex1.z;

				vertices[d + 3] = lines[i].vertex2.x;
				vertices[d + 4] = lines[i].vertex2.y;
				vertices[d + 5] = lines[i].vertex2.z;
				
				d += 6;
            }
			
			int attrs[2] = { 3,  0 };
			_boundingboxmesh = new Mesh(vertices, 24, attrs);
			// if (_camera != nullptr) {
			// 	float rayvertices[6] = {
			// 		// x, y, z
			// 		0.0f, 0.0f, 0.0f,
			// 		_camera->getFrontVector().x, _camera->getFrontVector().y, _camera->getFrontVector().z
			// 	};
					
			// 	int rayattrs[2] = { 3,  0 };

			// 	_raymesh = new Mesh(rayvertices, 2, rayattrs);
			// }
			// _physicsobject->setMass(_mass);
			_physicsobject->setPhysics(physics);
		break;
		// default:
		// 	_physicsobject->getCollider()->setSize(glm::vec3(0, 0, 0));
		// break;
	}
}

// TODO SIMPLE COLLISION
// glm::ivec3 GameObject::checkCollision(BOUNDINGBOX b) {
// 	glm::ivec3 collisionside(1);
// 	if (_bbox.max.x < b.min.x || _bbox.min.x > b.max.x) collisionside.x = 0;

// 	if (_bbox.max.y < b.min.y || _bbox.min.y > b.max.y) { 
// 		collisionside.y = 0; _onGround = false; 
// 	} else 
// 		_onGround = true;

// 	if (_bbox.max.z < b.min.z || _bbox.min.z > b.max.z) collisionside.z = 0;
// 	return collisionside;
// }
// bool GameObject::checkGround() {
// 	return _onGround;
// }

// BOUNDINGBOX GameObject::getBBOX() {
// 	return _bbox;
// }




VoxelModel* GameObject::getVoxelModel() {
	return _voxelmodel;
    // if (_visible == false) return false;

	// VoxelModel* voxels = _mesh->getVoxels();
	// glm::vec3 halfSize = glm::vec3(0.1f) * 0.5f;
	// for (int i = 0; i < voxels->getVoxelsCount(); i++) {
	// 	Voxel* voxel = voxels->getVoxel(i);
	// 	glm::vec3 position = voxel->getPosition();
	// 	//std::cout << voxs->voxels[i].position.x << " " << voxs->voxels[i].position.y << " " << voxs->voxels[i].position.z << std::endl;
	// 	// if (voxs->voxels[i].position == pos) {
	// 	// 	return &voxs->voxels[i];
	// 	// }
	// 	glm::vec3 voxelMin = position - halfSize;
    // 	glm::vec3 voxelMax = position + halfSize;
	// 	if (point.x >= voxelMin.x && point.x <= voxelMax.x &&
    //         point.y >= voxelMin.y && point.y <= voxelMax.y &&
    //         point.z >= voxelMin.z && point.z <= voxelMax.z) return true;
	// }
	// return false;
}

void GameObject::setMesh(Mesh* mesh) {
	_mesh = mesh;
}

Mesh* GameObject::getMesh() {
	return _mesh;
}