
#ifndef _4DAPP_H_
#define _4DAPP_H_

#ifdef MY4DAPP_EXPORTS
#define MY4DAPP_API __declspec(dllexport)
#else
#define MY4DAPP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	MY4DAPP_API void moveCamera(float rotXY, float rotYZ, float rotXZ,
	                            float rotXW, float rotYW, float rotZW,
	                            float movement);
	MY4DAPP_API float cameraPos(int axis);
	MY4DAPP_API float cameraDirection(int axis);
	MY4DAPP_API float cameraUp(int axis);
	MY4DAPP_API void resetCamera();
	MY4DAPP_API void resetIterators();
	MY4DAPP_API bool hasObject();
	MY4DAPP_API bool hasFacet();
	MY4DAPP_API bool hasVertex();
	MY4DAPP_API void nextObject();
	MY4DAPP_API void nextFacet();
	MY4DAPP_API void nextVertex();
	MY4DAPP_API float vertexCoord(int axis);
	MY4DAPP_API float vertexMetadata();
	
#ifdef __cplusplus
}
#endif

#endif
