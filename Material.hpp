#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "common.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "UniformBuffer.hpp"

#include <unordered_map>
#include <string>
#include <vector>

class Material {
	Material(const Material& ) {}

	Material(Program *program): m_program(program) {}
public:
	static Material* CreateMaterial(Program *program = 0);

	void setTexture(const std::string& name, Texture *texture);
	void setUniformBuffer(const std::string& name, UniformBuffer *uniformBuffer);

	const Program * getProgram() const { return m_program; }

	void bind();

private:
	Program *m_program;

	std::unordered_map<std::string, std::pair<size_t, GLint> > m_texNames;
	std::unordered_map<std::string, std::pair<size_t, GLuint> > m_ubNames;

	std::vector<Texture*> m_textures;
	std::vector<UniformBuffer*> m_uniformBuffers;
};

#endif

