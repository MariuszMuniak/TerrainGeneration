#pragma once
#include "RenderUtils.h"

class ModelLoader
{
public:
	static void LoadModelToContext(const char* filePath, RenderContext& context);
};

