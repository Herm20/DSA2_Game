#include "AppClass.h"
void Application::InitVariables(void)
{
	m_sProgrammer = "Herman McElveen/ Edward Opich/ Brandon Rodriguez/ James Meeks";
	playerMat = IDENTITY_M4;
	playerMovement = ZERO_V3;


	player = new Model();
	player->Load("FFmodel\\cube.obj");
	playerRB = new MyRigidBody(player->GetVertexList());

	//steve
	crateOb = new Model();
	crateOb->Load("FFmodel\\BoxCrate.obj");
	crateObRB = new MyRigidBody(crateOb->GetVertexList());
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	player->SetModelMatrix(playerMat);
	playerRB->SetModelMatrix(playerMat);
	//m_pMeshMngr->AddAxisToRenderList(playerMat);

	/// If a collision happens at 0,0,0 the program will break no idea why
	matrix4 mCrate = glm::translate(vector3(3.0f, 0.0f, 0-3.0f));
	crateOb->SetModelMatrix(mCrate);
	crateObRB->SetModelMatrix(mCrate);
	//m_pMeshMngr->AddAxisToRenderList(mCrate);

	bool bColliding = playerRB->IsColliding(crateObRB);

	player->AddToRenderList();
	playerRB->AddToRenderList();

	crateOb->AddToRenderList();
	crateObRB->AddToRenderList();

	m_pMeshMngr->Print("Colliding: ");
	if (bColliding)
		m_pMeshMngr->PrintLine("YES!", C_RED);
	else
		m_pMeshMngr->PrintLine("no", C_YELLOW);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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