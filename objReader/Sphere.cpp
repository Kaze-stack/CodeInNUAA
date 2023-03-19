#include "Sphere.h"
using namespace std;
void Sphere::_Initialize(int precision)
{
	numVertices=(precision+1)*(precision+1);
	numIndices=precision*precision*6;
	
	Vertex.resize(numVertices,glm::vec3());
	TexCoord.resize(numVertices,glm::vec2());
	NormVector.resize(numVertices,glm::vec3());
	Index.resize(numIndices,0);
	
	for(int i=0;i<=precision;i++)
	{
		float x,y,z;
		for(int j=0;j<=precision;j++)
		{
			y=cos(_toRadians(180.0-i*180.0/precision));
			x=-cos(_toRadians(j*360.0/precision))*abs(cos(asin(y)));
			z=sin(_toRadians(j*360.0/precision))*abs(cos(asin(y)));
			Vertex[i*(precision+1)+j]=glm::vec3(x,y,z);
			TexCoord[i*(precision+1)+j]=glm::vec2(j*1.0/precision,i*1.0/precision);
			NormVector[i*(precision+1)+j]=glm::vec3(x,y,z);
		}
	}
	
	for(int i=0;i<precision;i++)
	{
		for(int j=0;j<precision;j++)
		{
			Index[6*(i*precision+j)+0]=i*(precision+1)+j;
			Index[6*(i*precision+j)+1]=i*(precision+1)+j+1;
			Index[6*(i*precision+j)+2]=(i+1)*(precision+1)+j;
			Index[6*(i*precision+j)+3]=i*(precision+1)+j+1;
			Index[6*(i*precision+j)+4]=(i+1)*(precision+1)+j+1;
			Index[6*(i*precision+j)+5]=(i+1)*(precision+1)+j;
		}
	}
}

float Sphere::_toRadians(float degree)
{
	return (degree/180.0)*3.1415926;
}

Sphere::Sphere(int precision)
{
	_Initialize(precision);
}

Sphere::~Sphere()
{
}

int Sphere::getNumVertices() const
{
	return numVertices;
}

int Sphere::getNumIndices() const
{
	return numIndices;
}

std::vector<glm::vec3> Sphere::getVertex() const
{
	return Vertex;
}

std::vector<int> Sphere::getIndex() const
{
	return Index;
}

std::vector<glm::vec2> Sphere::getTexCoord() const
{
	return TexCoord;
}

std::vector<glm::vec3> Sphere::getNormVector() const
{
	return NormVector;
}

