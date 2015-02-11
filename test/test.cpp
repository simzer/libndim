
#include "../app/4DApp.h"

#include <cstdio>

int main(int argc, char* argv[])
{
	float rotXY = 0;
	float rotYZ = 0;
	float rotXZ = 0;
	float rotXW = 0;
	float rotYW = 0;
	float rotZW = 0;
	float movement = 0;
	float camXLeft;
	float camYLeft;
	float camZLeft;
	float camXRight;
	float camYRight;
	float camZRight;
	float camRotXLeft;
	float camRotYLeft;
	float camRotZLeft;
	float camRotXRight;
	float camRotYRight;
	float camRotZRight;
	int objectNum;
	int* objectIDs;
	int* vertexNums;
	float* vertexCoordinates;
	float* metadata;

	calculate(
		rotXY, rotYZ, rotXZ, rotXW, rotYW, rotZW,
		movement,
		camXLeft, camYLeft, camZLeft,
		camXRight, camYRight, camZRight,
		camRotXLeft, camRotYLeft, camRotZLeft,
		camRotXRight, camRotYRight, camRotZRight,
		objectNum,
		objectIDs,
		vertexNums,
		vertexCoordinates,
		metadata);

	printf("Camera position: %f %f %f\n", camXLeft, camYLeft, camZLeft);
	printf("Camera direction: %f %f %f\n", camRotXLeft, camRotYLeft, camRotZLeft);
	printf("Num of facets: %d\n", objectNum);

	int icoord = 0;
	int ifacet = 0;
	for (int i = 0; i < objectNum; i++) {
		printf("Facet %d (Id: %d):\n", i, objectIDs[i]);
		int vertices = vertexNums[i];
		for (int j = 0; j < vertices; j++) {
			printf("\tcoords: { %f, %f, %f }, meta: %f\n", 
				vertexCoordinates[icoord++], 
				vertexCoordinates[icoord++], 
				vertexCoordinates[icoord++],
				metadata[ifacet++]);
		}
	}

	return 0;
}

