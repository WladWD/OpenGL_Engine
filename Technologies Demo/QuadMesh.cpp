#include "QuadMesh.h"
#include <array>

using namespace SSAO;

QuadMesh::QuadMesh() {
	initializeVertexBuffer();
	initializeIndexBuffer();
	initializeVAO();
}

QuadMesh::~QuadMesh() { 
	if (glIsVertexArray(vao)) {
		glDeleteVertexArrays(1, &vao);
	}

	if (glIsBuffer(vertexBuffer)) {
		glDeleteBuffers(1, &vertexBuffer);
	}

	if (glIsBuffer(indexBuffer)) {
		glDeleteBuffers(1, &indexBuffer);
	}
}

void QuadMesh::bindVAO() const {
	glBindVertexArray(vao);
}

void SSAO::QuadMesh::draw() const {
	glDrawElementsInstancedBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr, 1, 0);
}

GLuint SSAO::QuadMesh::getVAO() const {
	return vao;
}

void SSAO::QuadMesh::enableVertexAttribArray() const {
	const auto size = sizeof(QuadMesh::Mesh);
	const uint32_t attribCount = 4;

	std::array<uint32_t, attribCount> componentCount{ 3, 3, 3, 2 };
	std::array<GLenum, attribCount> componentType{ GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT };
	std::array<GLenum, attribCount> offset {
		offsetof(QuadMesh::Mesh, position),
		offsetof(QuadMesh::Mesh, normal), 
		offsetof(QuadMesh::Mesh, tangent), 
		offsetof(QuadMesh::Mesh, tex0) };

	for (size_t i = 0; i < attribCount; ++i) {
		glVertexAttribPointer(i, componentCount[i],
			componentType[i], GL_FALSE, size,
			reinterpret_cast<const void*>(offset[i]));
		glEnableVertexAttribArray(i);
	}
}

void SSAO::QuadMesh::initializeVertexBuffer() {
	std::array<QuadMesh::Mesh, 4> vertexBuffer = { {
		{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0, 0.0f), glm::vec2(1.0f, 1.0f) }
	} };

	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(QuadMesh::Mesh), vertexBuffer.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SSAO::QuadMesh::initializeIndexBuffer() {
	std::array<uint8_t, 6> indexBuffer = {
		2, 3, 1, 1, 0, 2
	};

	glGenBuffers(1, &this->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(uint8_t), indexBuffer.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SSAO::QuadMesh::initializeVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	enableVertexAttribArray();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
