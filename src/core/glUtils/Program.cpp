//
// Created by liyh on 2019/9/11.
//

#include "Program.h"

Program::Program()
		: m_program(0) {
}

Program::~Program() {
	clean();
}

void Program::clean() {
	if (!m_program) return;
	// 强制从当前上下文卸载, 否则只是标记删除(等待状态机没有使用时才删除)
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	if (currentProgram == m_program) {
		glUseProgram(0);
	}
	glDeleteProgram(m_program);
	m_program = 0;
}

bool Program::Build(const char *vertexSource, const char *fragmentSource,
					const std::vector<std::string> &attribLocationNames, std::vector<GLint> &attribLocations,
					const std::vector<std::string> &uniformLocationNames, std::vector<GLint> &uniformLocations) {
	// 删除
	clean();

	auto createShader = [=](GLenum shaderType, const char *source) -> GLuint {
		auto shader = glCreateShader(shaderType);
		assert(shader);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (!status) {
			// 编译失败
			GLint infoLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			assert(infoLen);
			m_error.reserve(infoLen);
			glGetShaderInfoLog(shader, infoLen, nullptr, (GLchar *) m_error.c_str());
			glDeleteShader(shader);
			shader = 0;
		}
		return 0;
	};

	auto vs = createShader(GL_VERTEX_SHADER, vertexSource);
	if (!vs) return false;

	auto fs = createShader(GL_FRAGMENT_SHADER, fragmentSource);
	if (!fs) return false;

	m_program = glCreateProgram();
	assert(m_program);
	glAttachShader(m_program, vs);
	glAttachShader(m_program, fs);
	glLinkProgram(m_program);
	GLint linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		GLint infoLen;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLen);
		assert(infoLen);
		m_error.reserve(infoLen);
		glGetProgramInfoLog(m_program, infoLen, nullptr, (GLchar *) m_error.c_str());
		glDeleteProgram(m_program);
		m_program = 0;
	}

	// 直接标记删除
	glDeleteShader(vs);
	glDeleteShader(fs);

	if (!m_program) return false;

	attribLocations.clear();
	attribLocations.reserve(attribLocationNames.size());
	for (const std::string &name : attribLocationNames) {
		auto loc = glGetAttribLocation(m_program, name.c_str());
		attribLocations.push_back(loc);
	}

	uniformLocations.clear();
	uniformLocations.reserve(uniformLocationNames.size());
	for (const std::string &name : uniformLocationNames) {
		auto loc = glGetUniformLocation(m_program, name.c_str());
		uniformLocations.push_back(loc);
	}
}

void Program::Use(const std::vector<const Attribute *> *attribData, const std::vector<const Uniform *> *uniformData) {

	// glVertexAttribPointer()

}