#include "AppClass.h"
void Application::InitVariables(void)
{
	m_sProgrammer = "Herman McElveen, Edward Opich, Brandon Rodriguez, James Meeks";
	playerMat = IDENTITY_M4;
	playerMovement = vector3(0.0f, 2.0f, 2.0f);
	//playerMat = glm::translate(IDENTITY_M4, vector3(0.0f, 20.0f, 2.0f));
	
	
	srand(std::time(NULL));
	//float horiStart = (rand() % 20) - 10;
	//float vertStart = (rand() % 8) - 4;
	
	// entity manager
	entMan = EntityManagerFF::GetInstance();

		//player = new Model();
		//player->Load("FFmodel\\FreeFallMan.obj");
		//playerRB = new MyRigidBody(player->GetVertexList());

	// add the character to the entity manager
	entMan->AddEntity("FFmodel\\FreeFallMan.obj", "FFMan");
	entMan->SetAxisVisibility(true, "FFMan");

	// render plane
	entMan->AddEntity("FFmodel\\Ground.obj", "ground");
	entMan->SetModelMatrix(glm::translate(glm::vec3(0.0f, 0.0f, -600.0f)) * glm::rotate(IDENTITY_M4, 90.0f, 1.0f, 0.0f, 0.0f));
	entMan->SetAxisVisibility(true, "ground");

	for (int x = 0; x < 25; x++)
	{
		entMan->AddEntity("FFmodel\\RoundCrate.obj", "Debris_" + std::to_string(x));
		entMan->SetAxisVisibility(true, "Debris_" + std::to_string(x));
		mCrateMat.push_back(IDENTITY_M4);
		entMan->entityList[x + 2]->move = -100.0f + (-1 * (x + 2) * 10);
		entMan->entityList[x + 2]->horiStart = (rand() % 30) - 15;
		entMan->entityList[x + 2]->vertStart = (rand() % 30) - 15;
		moveVec.push_back(vector3(0.0f, 0.0f, 0.0f));
	}
	
		//crate movement
		//for (uint i = 0; i < 1; i++)
		//{
		//	//entMan->AddEntity("Minecraft\\BoxCrate.obj", "Crate_0");
		//	//entMan->SetAxisVisibility(true, "Crate_0");
		//	//crateOb.push_back(Model());
		//	//crateOb[i].Load("FFmodel\\BoxCrate.obj");
		//	//crateObRB = new MyRigidBody(crateOb[0].GetVertexList());
		//	//mCrateMat.push_back(IDENTITY_M4);
		//}	

		//mCrateMat[0] = glm::translate(vector3(0.0f, 0.0f, -100.0f));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	// character movement
	entMan->SetModelMatrix(playerMat, "FFMan");

	// debris movement
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

		/*m_pMeshMngr->Print("Colliding: ");
		if (bColliding)
			m_pMeshMngr->PrintLine("YES!", C_RED);
		else
			m_pMeshMngr->PrintLine("no", C_YELLOW);
			*/
	
	// check collisions in the entity manager after a certain point
	entMan->Update();

	// render the entities
	entMan->AddEntityToRenderList(-1, true);

	if (entMan->lives < 1)
	{
		// end the game
		exit(0);
	}
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//draw ground plane
	//m_pMeshMngr->AddPlaneToRenderList(), C_WHITE, 1);
	
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