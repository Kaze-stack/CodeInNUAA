#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include "myLibrary.h"
#include "ConstList.h"

#include "Sphere.h"

#define numVAOs 1
#define numVBOS 2
#define PI 3.1415926

float cameraX,cameraY,cameraZ;
float pyraLocX,pyraLocY,pyraLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOS];

GLuint mvLoc,projLoc;
GLuint iceTexture,earthTexture,sunTexture;;
int width,height;
float aspect;
glm::mat4 pMat,vMat,mMat,mvMat;
stack<glm::mat4> GraphicStack;

Sphere *model;

float toRad(float degree)
{
	return (degree/180.0)*PI;
}

string readShaderSource(const char *filePath)
{
	string content;
	string line;
	fstream file;
	file.open(filePath,ios::in);
	if(!file) exit(FAIL);
	while(!file.eof())
	{
		char buffer;
		file.get(buffer);
		if(!file) break;
		if(buffer=='\n')
		{
			content+=line;
			content+='\n';
			line.clear();
			continue;
		}
		else
		{
			line+=buffer;
		}
	}
	file.close();
	
	return content;
}

GLuint createShaderProgram()
{
	string vShaderStr=readShaderSource(vShaderFile);
	string fShaderStr=readShaderSource(fShaderFile);
	
	const char *vshaderSource=vShaderStr.c_str();
	const char *fshaderSource=fShaderStr.c_str();
	
	GLuint vShader=glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader=glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vShader,1,&vshaderSource,NULL);
	glShaderSource(fShader,1,&fshaderSource,NULL);
	
	glCompileShader(vShader);
	glCompileShader(fShader);
	
	GLuint vfProgram=glCreateProgram();
	glAttachShader(vfProgram,vShader);
	glAttachShader(vfProgram,fShader);
	glLinkProgram(vfProgram);

	return vfProgram;
}

void WindowHint()
{
	if (!glfwInit()) exit(FAIL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
}

void SetupVertices()
{
	vector<int> ind=model->getIndex();
	vector<glm::vec3> vert=model->getNormVector();
	vector<glm::vec2> tex=model->getTexCoord();
	
	int numIndex=model->getNumIndices();
	vector<float> position(3*numIndex);
	vector<float> texCoord(2*numIndex);
	for(int i=0;i<numIndex;i++)
	{
		position[i*3+0]=vert[ind[i]].x;
		position[i*3+1]=vert[ind[i]].y;
		position[i*3+2]=vert[ind[i]].z;
		
		texCoord[i*2+0]=tex[ind[i]].x;
		texCoord[i*2+1]=tex[ind[i]].y;
	}
	glGenVertexArrays(numVAOs,vao);
	glBindVertexArray(vao[0]);
	
	glGenBuffers(numVBOS,vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,position.size()*sizeof(float),&position[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,texCoord.size()*sizeof(float),&texCoord[0],GL_STATIC_DRAW);
}

void init(GLFWwindow *window)
{
	renderingProgram=createShaderProgram();
	cameraX=0.0;
	cameraY=0.0;
	cameraZ=12.0;
	pyraLocX=0.0;
	pyraLocY=0.0;
	pyraLocZ=0.0;
	model=new Sphere(1080);
	SetupVertices();
	glfwGetFramebufferSize(window,&width,&height);
	aspect=(1.0*width)/(1.0*height);
	pMat=glm::perspective(toRad(60.0),aspect,0.1f,1000.0f);
	iceTexture=loadTexture(IceTexturePath);
	earthTexture=loadTexture(EarthTexturePath);
	sunTexture=loadTexture(SunTexturePath);
}

void window_Reshape(GLFWwindow *window,int n_width,int n_height)
{
	glfwGetFramebufferSize(window,&n_width,&n_height);
	aspect=(1.0*n_width)/(1.0*n_height);
	pMat=glm::perspective(toRad(60.0),aspect,0.1f,1000.0f);
}

GLuint loadTexture(const char *filePath)
{
	GLuint textureID;
	textureID=SOIL_load_OGL_texture(filePath,
									SOIL_LOAD_AUTO,
									SOIL_CREATE_NEW_ID,
									SOIL_FLAG_INVERT_Y);
	if(textureID==0)
	{
		cout<<"Fail Load Image: "<<filePath<<endl;
		exit(FAIL);
	}
	return textureID;
}

void display(GLFWwindow *window,double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	
	mvLoc=glGetUniformLocation(renderingProgram,"mv_matrix");
	projLoc=glGetUniformLocation(renderingProgram,"proj_matrix");
	glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(pMat));
	
	vMat=glm::translate(glm::mat4(1.0f),glm::vec3(-cameraX,-cameraY,-cameraZ));
	GraphicStack.push(vMat);
	
	//Sun
	GraphicStack.push(GraphicStack.top());
	GraphicStack.top()*=glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
	GraphicStack.push(GraphicStack.top());
	GraphicStack.top()*=glm::rotate(glm::mat4(1.0f),(float)(currentTime*0.6),glm::vec3(0.0f,1.0f,0.0f));
	GraphicStack.top()*=glm::scale(glm::mat4(1.0f),glm::vec3(1.5f,1.5f,1.5f));
	glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(GraphicStack.top()));
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(1);
	
	glBindTexture(GL_TEXTURE_2D,sunTexture);
	glUniform1i(glGetUniformLocation(renderingProgram,"samp"),0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDrawArrays(GL_TRIANGLES,0,model->getNumIndices());
	GraphicStack.pop();
	
	//Earth
	GraphicStack.push(GraphicStack.top());
	GraphicStack.top()*=glm::translate(glm::mat4(1.0f),glm::vec3(sin(currentTime*0.8)*6.0,0.0f,cos(currentTime*0.8)*6.0));
	GraphicStack.push(GraphicStack.top());
	GraphicStack.top()*=glm::scale(glm::mat4(1.0f),glm::vec3(0.8f,0.8f,0.8f));
	GraphicStack.top()*=glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
	glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(GraphicStack.top()));
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(1);
	
	glBindTexture(GL_TEXTURE_2D,earthTexture);
	glUniform1i(glGetUniformLocation(renderingProgram,"samp"),0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDrawArrays(GL_TRIANGLES,0,model->getNumIndices());
	GraphicStack.pop();
	
	//Ice
	GraphicStack.push(GraphicStack.top());
	GraphicStack.top()*=glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,sin(currentTime)*2.0,cos(currentTime)*2.0));
	GraphicStack.push(GraphicStack.top());
	GraphicStack.top()*=glm::scale(glm::mat4(1.0f),glm::vec3(0.4f,0.4f,0.4f));
	GraphicStack.top()*=glm::rotate(glm::mat4(1.0f),(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
	glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(GraphicStack.top()));
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(1);
	
	glBindTexture(GL_TEXTURE_2D,iceTexture);
	glUniform1i(glGetUniformLocation(renderingProgram,"samp"),0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDrawArrays(GL_TRIANGLES,0,model->getNumIndices());
	GraphicStack.pop();
	
	while(!GraphicStack.empty())
	{
		GraphicStack.pop();
	}
}

#pragma clang diagnostic pop
