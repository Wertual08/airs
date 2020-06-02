#include "airs/Program.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "gl/glew.h"



namespace airs
{
	Program::Program(Program&& s) noexcept
	{
		std::swap(ProgramID, s.ProgramID);
	}
	Program::Program() : ProgramID(0)
	{
	}
	Program::Program(const std::string& shader) : Program()
	{
		Link(shader);
	}
	Program::Program(std::istream& stream) : Program()
	{
		Link(std::string(std::istreambuf_iterator<char>(stream), {}));
	}
	Program::~Program()
	{
		if (ProgramID) glDeleteProgram(ProgramID);
		ProgramID = 0;
	}

	void Program::Attach(const Shader& shader)
	{
		glAttachShader(ProgramID, shader.ID());
	}
	void Program::Detach(const Shader& shader)
	{
		glDetachShader(ProgramID, shader.ID());
	}

	void Program::Link()
	{
		int32_t Success;
		glLinkProgram(ProgramID);
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetProgramInfoLog(ProgramID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Program error: Shader program linking failed:\n" + templog);
		}
	}
	void Program::Link(const std::string& shader)
	{
		if (!ProgramID) ProgramID = glCreateProgram();
		if (!ProgramID) throw std::runtime_error("airs::Program error: Program program creation failed.\n");

		std::vector<Shader> Shaders;
		
		Shader::Variation CurrentShader = Shader::None;
		std::stringstream fin(shader);
		std::string line, source;
		while (getline(fin, line))
		{
			if (line.find("#shader") == 0)
			{
				if (CurrentShader != Shader::None)
				{
					Shaders.emplace_back(CurrentShader, source);
					source.clear();
				}
				if (line.find("vertex") != std::string::npos) CurrentShader = Shader::Vertex;
				else if (line.find("geometry") != std::string::npos) CurrentShader = Shader::Geometry;
				else if (line.find("fragment") != std::string::npos) CurrentShader = Shader::Fragment;
				else if (line.find("compute") != std::string::npos) CurrentShader = Shader::Compute;
				else if (line.find("end") != std::string::npos) { CurrentShader = Shader::None; break; }
				else CurrentShader = Shader::None;
			}
			else if (CurrentShader != Shader::None) source += line + '\n';
		}
		if (!source.empty()) Shaders.emplace_back(CurrentShader, source);

		for (const Shader& shader : Shaders) Attach(shader);
		Link();
		for (const Shader& shader : Shaders) Detach(shader);
	}
	std::istream& operator>>(std::istream& is, Program& shader)
	{
		shader.Link(std::string(std::istreambuf_iterator<char>(is), {}));
		return is;
	}

	int32_t Program::Uniform(const std::string& uniform) const
	{
		glUseProgram(ProgramID);
		int32_t t = glGetUniformLocation(ProgramID, uniform.c_str());
		glUseProgram(0);
		return t;
	}
	void Program::Uniform(int32_t location, float x) const
	{
		glUseProgram(ProgramID);
		glUniform1f(location, x);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, float x, float y) const
	{
		glUseProgram(ProgramID);
		glUniform2f(location, x, y);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, float x, float y, float z) const
	{
		glUseProgram(ProgramID);
		glUniform3f(location, x, y, z);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, float x, float y, float z, float w) const
	{
		glUseProgram(ProgramID);
		glUniform4f(location, x, y, z, w);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec2f v) const
	{
		glUseProgram(ProgramID);
		glUniform2f(location, v.x, v.y);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec3f v) const
	{
		glUseProgram(ProgramID);
		glUniform3f(location, v.x, v.y, v.z);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec4f v) const
	{
		glUseProgram(ProgramID);
		glUniform4f(location, v.x, v.y, v.z, v.w);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, int32_t x) const
	{
		glUseProgram(ProgramID);
		glUniform1i(location, x);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, int32_t x, int32_t y) const
	{
		glUseProgram(ProgramID);
		glUniform2i(location, x, y);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, int32_t x, int32_t y, int32_t z) const
	{
		glUseProgram(ProgramID);
		glUniform3i(location, x, y, z);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const
	{
		glUseProgram(ProgramID);
		glUniform4i(location, x, y, z, w);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec2i v) const
	{
		glUseProgram(ProgramID);
		glUniform2i(location, v.x, v.y);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec3i v) const
	{
		glUseProgram(ProgramID);
		glUniform3i(location, v.x, v.y, v.z);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec4i v) const
	{
		glUseProgram(ProgramID);
		glUniform4i(location, v.x, v.y, v.z, v.w);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, uint32_t x) const
	{
		glUseProgram(ProgramID);
		glUniform1ui(location, x);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, uint32_t x, uint32_t y) const
	{
		glUseProgram(ProgramID);
		glUniform2ui(location, x, y);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, uint32_t x, uint32_t y, uint32_t z) const
	{
		glUseProgram(ProgramID);
		glUniform3ui(location, x, y, z);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
	{
		glUseProgram(ProgramID);
		glUniform4ui(location, x, y, z, w);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec2ui v) const
	{
		glUseProgram(ProgramID);
		glUniform2ui(location, v.x, v.y);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec3ui v) const
	{
		glUseProgram(ProgramID);
		glUniform3ui(location, v.x, v.y, v.z);
		glUseProgram(0);
	}
	void Program::Uniform(int32_t location, vec4ui v) const
	{
		glUseProgram(ProgramID);
		glUniform4ui(location, v.x, v.y, v.z, v.w);
		glUseProgram(0);
	}

	int32_t Program::FastUniform(const std::string& uniform) const
	{
		return glGetUniformLocation(ProgramID, uniform.c_str());
	}
	void Program::FastUniform(int32_t location, float x) const
	{
		glUniform1f(location, x);
	}
	void Program::FastUniform(int32_t location, float x, float y) const
	{
		glUniform2f(location, x, y);
	}
	void Program::FastUniform(int32_t location, float x, float y, float z) const
	{
		glUniform3f(location, x, y, z);
	}
	void Program::FastUniform(int32_t location, float x, float y, float z, float w) const
	{
		glUniform4f(location, x, y, z, w);
	}
	void Program::FastUniform(int32_t location, vec2f v) const
	{
		glUniform2f(location, v.x, v.y);
	}
	void Program::FastUniform(int32_t location, vec3f v) const
	{
		glUniform3f(location, v.x, v.y, v.z);
	}
	void Program::FastUniform(int32_t location, vec4f v) const
	{
		glUniform4f(location, v.x, v.y, v.z, v.w);
	}
	void Program::FastUniform(int32_t location, int32_t x) const
	{
		glUniform1i(location, x);
	}
	void Program::FastUniform(int32_t location, int32_t x, int32_t y) const
	{
		glUniform2i(location, x, y);
	}
	void Program::FastUniform(int32_t location, int32_t x, int32_t y, int32_t z) const
	{
		glUniform3i(location, x, y, z);
	}
	void Program::FastUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const
	{
		glUniform4i(location, x, y, z, w);
	}
	void Program::FastUniform(int32_t location, vec2i v) const
	{
		glUniform2i(location, v.x, v.y);
	}
	void Program::FastUniform(int32_t location, vec3i v) const
	{
		glUniform3i(location, v.x, v.y, v.z);
	}
	void Program::FastUniform(int32_t location, vec4i v) const
	{
		glUniform4i(location, v.x, v.y, v.z, v.w);
	}
	void Program::FastUniform(int32_t location, uint32_t x) const
	{
		glUniform1ui(location, x);
	}
	void Program::FastUniform(int32_t location, uint32_t x, uint32_t y) const
	{
		glUniform2ui(location, x, y);
	}
	void Program::FastUniform(int32_t location, uint32_t x, uint32_t y, uint32_t z) const
	{
		glUniform3ui(location, x, y, z);
	}
	void Program::FastUniform(int32_t location, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
	{
		glUniform4ui(location, x, y, z, w);
	}
	void Program::FastUniform(int32_t location, vec2ui v) const
	{
		glUniform2ui(location, v.x, v.y);
	}
	void Program::FastUniform(int32_t location, vec3ui v) const
	{
		glUniform3ui(location, v.x, v.y, v.z);
	}
	void Program::FastUniform(int32_t location, vec4ui v) const
	{
		glUniform4ui(location, v.x, v.y, v.z, v.w);
	}

	void Program::Bind() const
	{
		glUseProgram(ProgramID);
	}
	void Program::Unbind() const
	{
		glUseProgram(0);
	}
}