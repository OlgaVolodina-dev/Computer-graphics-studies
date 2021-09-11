#include "worldHandler.h"


void WorldHandler::Init()
{
    terrainHandler.Init();
    tree.Init(terrainHandler);
}

void WorldHandler::Process(Camera & camera)
{
    terrainHandler.Process(camera);
}

void WorldHandler::Draw()
{
    terrainHandler.Draw();
    tree.Draw();
}