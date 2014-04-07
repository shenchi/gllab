#include "Material.hpp"
#include <cassert>
#include <cstdio>

using namespace std;

Material* Material::CreateMaterial(Program *program) {
	assert(program != 0); // TODO add default material
	return new Material(program);
}

void Material::setTexture(const string& name, Texture *texture) {
	if (m_texNames.find(name) == m_texNames.end()) {
		GLint loc = m_program->getUniformLocation(name.c_str());
		if (loc < 0) return;

		m_texNames[name] = make_pair(m_textures.size(), loc);
		m_textures.push_back(texture);
	} else {
		size_t idx = m_texNames[name].first;
		m_textures[idx] = texture;
	}
}

void Material::setUniformBuffer(const string& name, UniformBuffer *uniformBuffer) {
	if (m_ubNames.find(name) == m_ubNames.end()) {
		GLuint bindIdx = m_program->getUniformBlockIndex(name.c_str());
		if (GL_INVALID_INDEX == bindIdx) return;

		m_ubNames[name] = make_pair(m_uniformBuffers.size(), bindIdx);
		m_uniformBuffers.push_back(uniformBuffer);
	} else {
		size_t idx = m_ubNames[name].first;
		m_uniformBuffers[idx] = uniformBuffer;
	}
}

void Material::bind() {
	glUseProgram(m_program->getProgram());

	for (auto& name : m_texNames) {
		pair<size_t, GLint> &t = name.second;

		glUniform1ui(t.second, t.first);
		glActiveTexture(t.first);
		Texture* tex = m_textures[t.first];
		glBindTexture(tex->getTarget(), tex->getTexture());
	}

	for (auto& name : m_ubNames) {
		pair<size_t, GLuint> &b = name.second;

		glUniformBlockBinding(m_program->getProgram(), b.second, b.first);
		UniformBuffer *ub = m_uniformBuffers[b.first];
		glBindBufferBase(GL_UNIFORM_BUFFER, b.first, ub->getUniformBuffer());
	}
	
}