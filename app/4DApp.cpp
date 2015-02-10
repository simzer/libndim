
#include "4DApp.h"

#include <vector>

#include "../src/vector.h"
#include "../src/matrix.h"
#include "../src/cube.h"
#include "../src/camera.h"

using namespace ndim;

class Game {
public:
	Camera camera;
	Cube4D object;
	Game();
};

struct InterfaceHelper {
	std::vector<int> objectIDs;
	std::vector<int> vertexNums;
	std::vector<float> vertexCoordinates;
	std::vector<float> metadata;
	void clear();
};

static Game game;
static InterfaceHelper helper;

Game::Game() {
	camera.center = Vector4D({ -10, 0, 0, 0 });
	camera.directions = Matrix4D::unit();
}

void InterfaceHelper::clear() {
	objectIDs.clear();
	vertexNums.clear();
	vertexCoordinates.clear();
	metadata.clear();
}

MY4DAPP_API void calculate(
	float rotXY, float rotYZ, float rotXZ, float rotXW, float rotYW, float rotZW,
	float movement,
	float& camXLeft, float& camYLeft, float& camZLeft,
	float& camXRight, float& camYRight, float& camZRight,
	float& camRotXLeft, float& camRotYLeft, float& camRotZLeft,
	float& camRotXRight, float& camRotYRight, float& camRotZRight,
	int& objectNum,
	int*& objectIDs,
	int*& vertexNums,
	float*& vertexCoordinates,
	float*& metadata)
{
	helper.clear();

	game.camera.move(movement);

	Matrix<4> camRot =
		  Matrix<4>::rotator(0, 1, rotXY)
		* Matrix<4>::rotator(1, 2, rotYZ)
		* Matrix<4>::rotator(0, 2, rotXZ)
		* Matrix<4>::rotator(0, 3, rotXW)
		* Matrix<4>::rotator(1, 3, rotYW)
		* Matrix<4>::rotator(2, 3, rotZW);

	game.camera.rotate(camRot);

	camXLeft = camXRight = game.camera.center[0];
	camYLeft = camYRight = game.camera.center[1];
	camZLeft = camZRight = game.camera.center[2];

	camRotXLeft = camRotXRight = game.camera.directions[0][0];
	camRotYLeft = camRotYRight = game.camera.directions[0][1];
	camRotZLeft = camRotZRight = game.camera.directions[0][2];

	objectNum = game.object.facets.size();

	for (auto &facet : game.object.facets) {
		helper.objectIDs.push_back(0);
		helper.vertexNums.push_back(facet.vertices.size());
		for (auto &vertex : facet.vertices) {
			auto projected = game.camera.project(vertex);
			for (int i = 0; i < 3; i++) {
				helper.vertexCoordinates.push_back(projected[i]);
			}
			helper.metadata.push_back(NAN); // todo: t.b.d
		}
	}

	objectIDs = helper.objectIDs.data();
	vertexNums = helper.vertexNums.data();
	vertexCoordinates = helper.vertexCoordinates.data();
	metadata = helper.metadata.data();
}
