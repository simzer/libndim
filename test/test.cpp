
#include "../src/matrix.h"
#include "../app/4DApp.h"

#include <cstdio>
#include <iostream>

int main(int argc, char* argv[])
{
	ndim::Matrix4D a = ndim::Matrix4D::rotator(0, 1, .2);
	std::cout << std::string(a.inverse() * a);


	float rotXY = 0;
	float rotYZ = 0;
	float rotXZ = 0;
	float rotXW = 0;
	float rotYW = 0;
	float rotZW = 0;
	float movement = 0;

	moveCamera(rotXY, rotYZ, rotXZ, rotXW, rotYW, rotZW, movement);

	printf("Camera position: %f %f %f\n", cameraPos(0), cameraPos(1), cameraPos(2));
	printf("Camera direction: %f %f %f\n", cameraDirection(0), cameraDirection(1), cameraDirection(2));
	printf("Camera up: %f %f %f\n", cameraUp(0), cameraUp(1), cameraUp(2));

	resetIterators();
	while (hasObject()) {
		while (hasFacet()) {
			printf("Next facet:\n");
			while (hasVertex()) {
				printf("\tcoords: { %f, %f, %f }, meta: %f\n",
					vertexCoord(0),
					vertexCoord(1),
					vertexCoord(2),
					vertexMetadata());
				nextVertex();
			}
			nextFacet();
		}
		nextObject();
	}

	return 0;
}

