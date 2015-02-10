// Rotations are absolute.
void calculate(
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
	float*& 4DMetadata);
