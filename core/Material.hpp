#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "common.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "UniformBuffer.hpp"

#include <unordered_map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

class Material {
	Material(const Material& ) {}

	Material(Program *program): m_program(program) {}
public:
	static Material* CreateMaterial(Program *program = 0);

	bool setUniform(const std::string& name, int size, const void* data);
	bool setUniform(const std::string& name, GLint data);
	bool setUniform(const std::string& name, float data);
	bool setUniform(const std::string& name, const glm::vec4& data);
	bool setUniform(const std::string& name, const glm::vec3& data);
	bool setUniform(const std::string& name, const glm::mat4& data);
	// etc..

	// void setUniformBlock(const std::string& name, int size, const void* data);

	bool setTexture(const std::string& name, Texture *texture);


	const Program * getProgram() const { return m_program; }

	void bind();

private:
	Program *m_program;

	std::unordered_map<std::string, size_t > m_texNames;
	// std::unordered_map<std::string, std::pair<size_t, GLuint> > m_ubNames;

	std::vector<Texture*> m_textures;
	std::vector<UniformBuffer*> m_uniformBuffers;
};

#endif

