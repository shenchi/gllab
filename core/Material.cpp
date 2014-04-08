#include "Material.hpp"
#include <cassert>

#include <glm/gtc/type_ptr.hpp>

using namespace std;

Material* Material::CreateMaterial(Program *program) {
	assert(program != 0); // TODO add default material
	assert(program->getProgram() != 0);

	Material *mat = new Material(program);

	const vector< Program::BlockDesc >& blockList = program->getUniformBlockList();
	mat->m_uniformBuffers.clear();
	for (auto& block : blockList) {
		size_t idx = mat->m_uniformBuffers.size();
		UniformBuffer *ub = UniformBuffer::CreateUniformBuffer(block.second);
		if (!ub) {
			delete mat;
			return 0;
		}
		glUniformBlockBinding(program->getProgram(), idx, idx);
		mat->m_uniformBuffers.push_back(ub);
	}

	return mat;
}

bool Material::setUniform(const string& name, int size, const void* data) {
	const UniformDesc* desc = m_program->getUniform(name);
	if (!desc) return false;
	if (desc->blockIndex < 0) {
		return false;
	} else {
		// if (desc->size < size) return false;
		m_uniformBuffers[desc->blockIndex]->setData(desc->location, size, data);
	}
	return true;
}

bool Material::setUniform(const std::string& name, GLint data) {
	const UniformDesc* desc = m_program->getUniform(name);
	if (!desc) return false;
	if (desc->blockIndex < 0) {
		glUseProgram(m_program->getProgram());
		glUniform1i(desc->location, data);
	} else {
		// if (desc->size < sizeof(GLuint)) return false;
		m_uniformBuffers[desc->blockIndex]->setData(desc->location, sizeof(GLuint), &data);
	}
	return true;
}

bool Material::setUniform(const std::string& name, float data) {
	const UniformDesc* desc = m_program->getUniform(name);
	if (!desc) return false;
	if (desc->blockIndex < 0) {
		glUseProgram(m_program->getProgram());
		glUniform1f(desc->location, data);
	} else {
		// if (desc->size < sizeof(float)) return false;
		m_uniformBuffers[desc->blockIndex]->setData(desc->location, sizeof(float), &data);
	}
	return true;
}

bool Material::setUniform(const std::string& name, const glm::vec4& data) {
	const UniformDesc* desc = m_program->getUniform(name);
	if (!desc) return false;
	if (desc->blockIndex < 0) {
		glUseProgram(m_program->getProgram());
		glUniform4fv(desc->location, 1, glm::value_ptr(data));
	} else {
		// if (desc->size < sizeof(glm::vec4)) return false;
		m_uniformBuffers[desc->blockIndex]->setData(desc->location, sizeof(glm::vec4), glm::value_ptr(data));
	}
	return true;
}

bool Material::setUniform(const std::string& name, const glm::vec3& data) {
	const UniformDesc* desc = m_program->getUniform(name);
	if (!desc) return false;
	if (desc->blockIndex < 0) {
		glUseProgram(m_program->getProgram());
		glUniform4fv(desc->location, 1, glm::value_ptr(data));
	} else {
		// if (desc->size < sizeof(glm::vec3)) return false;
		m_uniformBuffers[desc->blockIndex]->setData(desc->location, sizeof(glm::vec3), glm::value_ptr(data));
	}
	return true;
}

bool Material::setUniform(const std::string& name, const glm::mat4& data) {
	const UniformDesc* desc = m_program->getUniform(name);
	if (!desc) return false;
	if (desc->blockIndex < 0) {
		glUseProgram(m_program->getProgram());
		glUniformMatrix4fv(desc->location, 1, GL_FALSE, glm::value_ptr(data));
	} else {
		// if (desc->size < sizeof(glm::mat4)) return false;
		m_uniformBuffers[desc->blockIndex]->setData(desc->location, sizeof(glm::mat4), glm::value_ptr(data));
	}
	return true;
}

bool Material::setTexture(const string& name, Texture *texture) {
	if (m_texNames.find(name) == m_texNames.end()) {
		if (!m_program->getUniform(name)) return false;

		if (!setUniform(name, (GLint)m_textures.size())) return false;
		m_texNames[name] = m_textures.size();
		m_textures.push_back(texture);
	} else {
		size_t idx = m_texNames[name];
		m_textures[idx] = texture;
	}
	return true;
}


void Material::bind() {
	glUseProgram(m_program->getProgram());

	for (size_t idx = 0; idx < m_uniformBuffers.size(); ++idx) {

		UniformBuffer *ub = m_uniformBuffers[idx];
		ub->bind(idx);
	}

	for (auto& t : m_texNames) {

		Texture* tex = m_textures[t.second];
		tex->bind(t.second);
	}
	
}