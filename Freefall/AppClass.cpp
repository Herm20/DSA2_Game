#include "AppClass.h"
void Application::InitVariables(void)
{
	m_sProgrammer = "Herman McElveen, Edward Opich, Brandon Rodriguez, James Meeks";
	playerMat = IDENTITY_M4;
	playerMovement = vector3(0.0f, 2.0f, 2.0f);
	
	srand(std::time(NULL)); // seed random start time 
	
	// entity manager
	entMan = EntityManagerFF::GetInstance();

	// add the character to the entity manager
	entMan->AddEntity("FFmodel\\FreeFallMan.obj", "FFMan");
	entMan->SetAxisVisibility(true, "FFMan");

	// render plane
	entMan->AddEntity("FFmodel\\Ground.obj", "ground");
	entMan->SetModelMatrix(glm::translate(glm::vec3(0.0f, 0.0f, -600.0f)) * glm::rotate(IDENTITY_M4, 90.0f, 1.0f, 0.0f, 0.0f));
	entMan->SetAxisVisibility(true, "ground");

	//spawn obstacles and add them to the entity list
	for (int x = 0; x < 110; x++)
	{
		entMan->AddEntity("FFmodel\\RoundCrate.obj", "Debris_" + std::to_string(x));
		entMan->SetAxisVisibility(true, "Debris_" + std::to_string(x));
		mCrateMat.push_back(IDENTITY_M4);
		entMan->entityList[x + 2]->move = -100.0f + (-1 * (x + 2) * 10);
		entMan->entityList[x + 2]->horiStart = (rand() % 30) - 15;
		entMan->entityList[x + 2]->vertStart = (rand() % 30) - 15;
		moveVec.push_back(vector3(0.0f, 0.0f, 0.0f));
	}
}
void Application::Update(void)
{
	/// Implement the octree, clearing and deleting it first then remaking it
	if (root != nullptr)
	{
		entMan->ClearDimensionSetAll();
		SafeDelete(root);
	}
	if (octOptimize == true)
	{
		root = new Octree(m_uOctantLevels, 5);
	}
	///

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	// character movement
	entMan->SetModelMatrix(playerMat, "FFMan");

	/// debris movement
	for (int x = 0; x < mCrateMat.size(); x++)
	{
		mCrateMat[x] = glm::translate(moveVec[x])  * glm::scale(vector3(1.5f));
		entMan->SetModelMatrix(mCrateMat[x], "Debris_" + std::to_string(x));
		moveVec[x] = vector3(entMan->entityList[x + 2]->horiStart, entMan->entityList[x + 2]->vertStart, entMan->entityList[x + 2]->move);
	}

	for (int x = 2; x < entMan->entityList.size(); x++)
	{
		// move debris objects forward
		entMan->entityList[x]->move += 1.0f;

		// randomize respawn location of debris
		if (entMan->entityList[x]->move >= 10)
		{
			entMan->entityList[x]->horiStart = (rand() % 30) - 15;
			entMan->entityList[x]->horiStart = (rand() % 30) - 15;
			entMan->entityList[x]->move = -200;
		}
	}

	///
	
	// check collisions in the entity manager after a certain point
	entMan->Update();

	// render the entities
	entMan->AddEntityToRenderList(-1, true);

	// end the game if lives are gone
	if (entMan->lives < 1)
	{
		exit(0);
	}
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	if (octOptimize == true)
	{
		root->Display(m_uOctantID, C_GREEN);
	}
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}