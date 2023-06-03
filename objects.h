#pragma once

#include <Windows.h>

typedef struct _movingObj {
	int objId;
	int x, y;
	int delay;

} mObj;

typedef struct _movingObjListNode {
	mObj obj;
	DWORD lastUpdateTick;
	struct _movingObjListNode* nextNode;
	struct _movingObjListNode* prevNode;

} Node;

