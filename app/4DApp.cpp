
#include "4DApp.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "../src/vector.h"
#include "../src/matrix.h"
#include "../src/cube.h"
#include "../src/camera.h"

using namespace ndim;

class Game {
public:
	Camera camera;
	std::vector<Object*> objects;
	Game();
};

class InterfaceHelper {
public:
	InterfaceHelper();
	~InterfaceHelper();
	void reset();
	void nextObject();
	void nextFacet();
	void nextVertex();
	bool hasObject();
	bool hasFacet();
	bool hasVertex();
	float vertexCoord(int axis);
	void moveCamera(float rotXY, float rotYZ, float rotXZ,
	                float rotXW, float rotYW, float rotZW,
	                float movement);
private:
	std::ofstream logFile;
	Vector3D projected;
	std::vector<Object*>::iterator actObject;
	std::vector<Facet>::iterator actFacet;
	std::vector<Vertex>::iterator actVertex;
};

static Game game;
static InterfaceHelper helper;

Game::Game() 
{
	camera.center = Vector4D({ -10, 0, 0, 0 });
	camera.directions = Matrix4D::unit();
	objects.push_back(new Cube4D());
}

InterfaceHelper::InterfaceHelper()
{
	logFile.open("c:/Users/Laszlo/Desktop/4d/4DApp.log");
	reset();
}

InterfaceHelper::~InterfaceHelper()
{
	logFile.close();
}

void InterfaceHelper::reset() {
	actObject = game.objects.begin();
	actFacet = (*actObject)->facets.begin();
	actVertex = (*actFacet).vertices.begin();
	projected = game.camera.project(*actVertex);
}

bool InterfaceHelper::hasObject() {
	return actObject != game.objects.end();
}

void InterfaceHelper::nextObject() {
	++actObject;
	if (actObject != game.objects.end()) {
		actFacet = (*actObject)->facets.begin();
		actVertex = (*actFacet).vertices.begin();
		projected = game.camera.project(*actVertex);
	}
}

bool InterfaceHelper::hasFacet() {
	return actFacet != (*actObject)->facets.end();
}

void InterfaceHelper::nextFacet() {
	++actFacet;
	if (actFacet != (*actObject)->facets.end()) {
		actVertex = (*actFacet).vertices.begin();
		projected = game.camera.project(*actVertex);
	}
}

bool InterfaceHelper::hasVertex() {
	return actVertex != (*actFacet).vertices.end();
}

void InterfaceHelper::nextVertex() {
	++actVertex;
	if (actVertex != (*actFacet).vertices.end()) {
		projected = game.camera.project(*actVertex);
	}
}

float InterfaceHelper::vertexCoord(int axis) {
	return projected[axis];
}

void InterfaceHelper::moveCamera(float rotXY, float rotYZ, float rotXZ,
	float rotXW, float rotYW, float rotZW,
	float movement)
{
	game.camera.move(movement);
	Matrix<4> camRot =
		Matrix<4>::rotator(0, 1, rotXY) // -dy
		* Matrix<4>::rotator(1, 2, rotYZ)
		* Matrix<4>::rotator(0, 2, rotXZ) // dx
		* Matrix<4>::rotator(0, 3, rotXW)
		* Matrix<4>::rotator(1, 3, rotYW)
		* Matrix<4>::rotator(2, 3, rotZW)
		;
	
	game.camera.rotate(camRot);

	if (rotXY != 0 || rotYZ != 0 || rotXZ != 0 || rotXW != 0 || rotYW != 0 || rotZW != 0 || movement != 0) {
		logFile << rotXY << ","
			<< rotYZ << ","
			<< rotXZ << ","
			<< rotXW << ","
			<< rotYW << ","
			<< rotZW << ","
			<< movement << "\n";
		logFile << std::string(game.camera.center) << "\n";
		logFile << std::string(game.camera.directions);
		logFile << std::string(game.camera.project(Vector4D({ 0, 0, 0, 0 }))) << "\n";
	}
}

extern "C" MY4DAPP_API 
void moveCamera(float rotXY, float rotYZ, float rotXZ,
                float rotXW, float rotYW, float rotZW,
                float movement)
{
	helper.moveCamera(rotXY, rotYZ, rotXZ,
	                  rotXW, rotYW, rotZW,
	                  movement);
}

extern "C" MY4DAPP_API 
float cameraPos(int axis)
{
	return game.camera.center[axis];
}

extern "C" MY4DAPP_API 
float cameraDirection(int axis) {
	return game.camera.directions[0][axis];
}

extern "C" MY4DAPP_API
float cameraUp(int axis) {
	return game.camera.directions[1][axis];
}

extern "C" MY4DAPP_API void resetIterators() { helper.reset(); }
extern "C" MY4DAPP_API bool hasObject()      { return helper.hasObject(); }
extern "C" MY4DAPP_API bool hasFacet()       { return helper.hasFacet(); }
extern "C" MY4DAPP_API bool hasVertex()      { return helper.hasVertex(); }
extern "C" MY4DAPP_API void nextObject()     { helper.nextObject(); }
extern "C" MY4DAPP_API void nextFacet()      { helper.nextFacet(); }
extern "C" MY4DAPP_API void nextVertex()     { helper.nextVertex(); }
extern "C" MY4DAPP_API float vertexCoord(int axis) { return helper.vertexCoord(axis); }
extern "C" MY4DAPP_API float vertexMetadata()      { return 0; }
