
#ifdef MY4DAPP_EXPORTS
#define MY4DAPP_API __declspec(dllexport)
#else
#define MY4DAPP_API __declspec(dllimport)
#endif

// Rotations are absolute.
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
	float*& metadata);
