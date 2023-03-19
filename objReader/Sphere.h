#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

class Sphere{
private:
	int numVertices;
	int numIndices;
	std::vector<glm::vec3> Vertex;
	std::vector<int> Index;
	std::vector<glm::vec2> TexCoord;
	std::vector<glm::vec3> NormVector;
	
protected:
	void _Initialize(int precision);
	float _toRadians(float degree);
	
public:
	Sphere(int precision);
	~Sphere();
	int getNumVertices() const;
	int getNumIndices() const;
	std::vector<glm::vec3> getVertex() const;
	std::vector<int> getIndex() const;
	std::vector<glm::vec2> getTexCoord() const;
	std::vector<glm::vec3> getNormVector() const;
};

#endif
