
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
	void resetCamera();
};

class InterfaceHelper {
public:
	bool logEnabled;
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
	                float movX, float movY, float movZ, float movW);
	void log();
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
	auto outer = new Cube4D(5);
	auto inner = new Cube4D(3);
	auto gate = new Cube4D(1, Vector4D({ 0, 0, 0, 2 }));
	objects.push_back(outer);
	objects.push_back(inner);
	objects.push_back(gate);
	resetCamera();
	camera.isValidPosition = [=](Vector4D p, Vector4D d) {
		/*return outer->isInside(pos) && !(inner->isInside(pos) || gate->isInside(pos))
			? false : true;*/
		return outer->isHit(p, d) && !(inner->isHit(p, d, 0.02) || gate->isHit(p, d, 0.02))
			? false : true;
	};
}

void Game::resetCamera()
{
	camera.center = Vector4D({ -10, 0, 0, 0 });
	camera.directions = Matrix4D::unit();

	moveCamera(0.0001, 0.001, 0.0002, 0.0005, .0003, 0.0004,
		0.00001, 0.0002, 0.001, 0.00002);
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
	logEnabled = false;
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
	float movX, float movY, float movZ, float movW)
{
	game.camera.move(Vector<4>({movX, movY, movZ, movW}));
	Matrix<4> camRot =
		Matrix<4>::rotator(0, 1, rotXY) // -dy
		* Matrix<4>::rotator(1, 2, rotYZ)
		* Matrix<4>::rotator(0, 2, rotXZ) // dx
		* Matrix<4>::rotator(0, 3, rotXW)
		* Matrix<4>::rotator(1, 3, rotYW)
		* Matrix<4>::rotator(2, 3, rotZW)
		;

	game.camera.rotate(camRot);

	if (logEnabled) 
		if (rotXY != 0 || rotYZ != 0 || rotXZ != 0 ||
			rotXW != 0 || rotYW != 0 || rotZW != 0 ||
			movX != 0 || movY != 0 || movZ != 0 || movW != 0)
		{
			logFile 
				<< rotXY << "," << rotYZ << "," << rotXZ << ","
				<< rotXW << "," << rotYW << "," << rotZW << ","
				<< movX << movY << movZ << movW << "\n";
			log();
		}
}

void InterfaceHelper::log()
{
	//logFile << "Camera position:\n" << std::string(game.camera.center) << "\n";
	//logFile << "Camera direction:\n" << std::string(game.camera.directions);
	//logFile << std::string(game.camera.project(Vector4D({ 0, 0, 0, 0 }))) << "\n";
	
	resetIterators();
	while (hasObject()) {
		while (hasFacet()) {
			logFile << "Next facet:\n";
			while (hasVertex()) {
				logFile << "coords: " << std::string(helper.projected) << "\n";
				nextVertex();
			}
			nextFacet();
		}
		nextObject();
	}

}

extern "C" MY4DAPP_API 
void moveCamera(float rotXY, float rotYZ, float rotXZ,
                float rotXW, float rotYW, float rotZW,
                float movX, float movY, float movZ, float movW)
{
	helper.moveCamera(rotXY, rotYZ, rotXZ,
	                  rotXW, rotYW, rotZW,
	                  movX, movY, movZ, movW);
}

extern "C" MY4DAPP_API 
float cameraPos(int axis)
{
	//return game.camera.center[axis];
	return 0;
}

extern "C" MY4DAPP_API 
float cameraDirection(int axis) {
	//return game.camera.directions[0][axis];
	return (axis == 0) ? 1.0 : 0.0;
}

extern "C" MY4DAPP_API
float cameraUp(int axis) {
	//return game.camera.directions[1][axis];
	return (axis == 1) ? 1.0 : 0.0;
}

extern "C" MY4DAPP_API
void resetCamera() {
	game.resetCamera();
}

extern "C" MY4DAPP_API void resetIterators()		{ helper.reset(); }
extern "C" MY4DAPP_API bool hasObject()				{ return helper.hasObject(); }
extern "C" MY4DAPP_API bool hasFacet()				{ return helper.hasFacet(); }
extern "C" MY4DAPP_API bool hasVertex()				{ return helper.hasVertex(); }
extern "C" MY4DAPP_API void nextObject()			{ helper.nextObject(); }
extern "C" MY4DAPP_API void nextFacet()				{ helper.nextFacet(); }
extern "C" MY4DAPP_API void nextVertex()			{ helper.nextVertex(); }
extern "C" MY4DAPP_API float vertexCoord(int axis)	{ return helper.vertexCoord(axis); }
extern "C" MY4DAPP_API float vertexMetadata()		{ return 0; }
extern "C" MY4DAPP_API void enableLog(bool enable)	{ helper.logEnabled = enable; }
extern "C" MY4DAPP_API void triggerLog()			{ helper.log(); }
