#include "ObjScene.hpp"


#include "DdsLoader.hpp"
#include "imgui.h"
#include "../../../engine/loaders/obj/obj_loader.h"

void ObjScene::Init()
{
	transform.SetScale(glm::vec3(1, 1, 1));
	transform.SetPosition(glm::vec3(0, 0, 0));
	transform.SetRotation(45, glm::vec3(1, 1, 0));


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	char* _vertexShaderBuffer = 0;
	char* _fragmentShaderBuffer = 0;
	
	/**/
	vertexShader.LoadShader("assets/obj/TransformVertexShader.vertexshader", GL_VERTEX_SHADER);
	framgentShader.LoadShader("assets/obj/TextureFragmentShader.fragmentshader", GL_FRAGMENT_SHADER);
	
	vertexShader.CompileShader(_vertexShaderBuffer);
	framgentShader.CompileShader(_fragmentShaderBuffer);

	vertexShader.CreateShaderProgram();

	programID = vertexShader.GetProgramID();

	framgentShader.CreateShaderProgram(programID); // same program for both shader
	

	matrixID = glGetUniformLocation(programID, "MVP");

	/* Texture */
	// Load the texture
	DdsLoader::LoadFile("assets/obj/uvmap.dds", texture);

	// Get a handle for our "myTextureSampler" uniform
	textureID = glGetUniformLocation(programID, "myTextureSampler");

	/* load obj file */
	bool res = loadOBJ("assets/obj/cube.obj", vertices, uvs, normals);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	


	
}

void ObjScene::Update(float _dt, glm::mat4 _mvp)
{
	/* create to texture to render */
	GLuint _idTexture;
	glGenTextures(1, &_idTexture);

	// Binding de la texture pour pouvoir la modifier.
	glBindTexture(GL_TEXTURE_2D, _idTexture);

	// Création de la texture 2D vierge de la taille de votre fenêtre OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Paramètrage de notre texture (étirement et filtrage)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	/* create FBO */
	glGenFramebuffers(1, &fbo);

	// On bind le FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Affectation de notre texture au FBO
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _idTexture, 0);

	// Affectation d'un drawbuffer au FBO
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);


	// render to texture
	// Activation et binding la texture
	glBindTexture(GL_TEXTURE_2D, _idTexture);
	glActiveTexture(GL_TEXTURE0);

	// Activation du FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, 1024, 768);

	// Changement de la couleur de background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Rafraichissement des buffers (reset)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// =================================================================

	
	//framgentShader.Use(); // todo static method
	glUseProgram(programID);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &_mvp[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(textureID, 0);

	
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// =================================================================


	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	ImVec2 wsize = ImGui::GetWindowSize();
	ImGui::Image(reinterpret_cast<ImTextureID>(_idTexture), wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Text("Shader Text");               // Display some text (you can use a format strings too)
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void ObjScene::Clean()
{
	glDeleteFramebuffers(1, &fbo);

}

string ObjScene::GetName()
{
	return "Obj Scene";
}
