#include "airs/Shader.h"
#include <fstream>
#include <sstream>
#include "gl/glew.h"



namespace airs
{
	Shader::Shader(Shader&& s) noexcept 
	{
		std::swap(ProgramID, s.ProgramID);
	}
	Shader::Shader() : ProgramID(glCreateProgram())
	{
		if (!ProgramID) throw std::runtime_error("airs::Shader error: Shader program creation failed.\n");
	}
	Shader::Shader(const std::string& vs, const std::string& gs, const std::string& fs) : Shader()
	{
		Load(vs, gs, fs);
	}
	Shader::Shader(const std::string& vs, const std::string& fs) : Shader()
	{
		Load(vs, fs);
	}
	Shader::Shader(const std::string& shader) : Shader()
	{
		Load(shader);
	}
	Shader::Shader(std::istream& stream) : Shader()
	{
		Load(std::string(std::istreambuf_iterator<char>(stream), {}));
	}
	Shader::~Shader()
	{
		if (ProgramID) glDeleteProgram(ProgramID);
		ProgramID = 0;
	}

	void Shader::Load(const std::string& vs, const std::string& gs, const std::string& fs)
	{
		uint32_t VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (!VertexShaderID) throw std::runtime_error("airs::Shader error: Vertex shader creation failed.\n");
		uint32_t GeometryShaderID = gs.empty() ? 0 : glCreateShader(GL_GEOMETRY_SHADER);
		if (!gs.empty() && !GeometryShaderID) throw std::runtime_error("airs::Shader error: Geometry shader creation failed.\n");
		uint32_t FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (!FragmentShaderID) throw std::runtime_error("airs::Shader error: Fragment shader creation failed.\n");

		const char* cvs = vs.c_str();
		const char* cgs = gs.c_str();
		const char* cfs = fs.c_str();

		int32_t Success;

		glShaderSource(VertexShaderID, 1, &cvs, 0);
		glCompileShader(VertexShaderID);
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetShaderInfoLog(VertexShaderID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: Vertex shader compilation failed:\n" + templog);
		}

		if (!gs.empty())
		{
			glShaderSource(GeometryShaderID, 1, &cgs, 0);
			glCompileShader(GeometryShaderID);
			glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				int32_t lenght;
				std::string templog;
				glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &lenght);
				templog.resize(lenght);
				glGetShaderInfoLog(GeometryShaderID, lenght, 0, &templog[0]);
				throw std::runtime_error("airs::Shader error: Geometry shader compilation failed:\n" + templog);
			}
		}

		glShaderSource(FragmentShaderID, 1, &cfs, 0);
		glCompileShader(FragmentShaderID);
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetShaderInfoLog(FragmentShaderID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: Fragment shader compilation failed:\n" + templog);
		}

		glAttachShader(ProgramID, VertexShaderID);
		if (!gs.empty()) glAttachShader(ProgramID, GeometryShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetProgramInfoLog(ProgramID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: Shader program linking failed:\n" + templog);
		}

		glDetachShader(ProgramID, VertexShaderID);
		if (!gs.empty()) glDetachShader(ProgramID, GeometryShaderID);
		glDetachShader(ProgramID, FragmentShaderID);
		glDeleteShader(VertexShaderID);
		if (!gs.empty()) glDeleteShader(GeometryShaderID);
		glDeleteShader(FragmentShaderID);
	}
	void Shader::Load(const std::string& vs, const std::string& fs)
	{
		Load(vs, "", fs);
	}
	void Shader::Load(const std::string& shader)
	{
		enum
		{
			NONE,
			VERTEX,
			GEOMETRY,
			FRAGMENT
		} CurrentShader = NONE;

		std::string vs = "";
		std::string gs = "";
		std::string fs = "";

		std::stringstream fin(shader);
		std::string line;
		while (getline(fin, line))
		{
			if (line.find("#shader") == 0)
			{
				if (line.find("vertex") != std::string::npos) CurrentShader = VERTEX;
				else if (line.find("geometry") != std::string::npos) CurrentShader = GEOMETRY;
				else if (line.find("fragment") != std::string::npos) CurrentShader = FRAGMENT;
				else CurrentShader = NONE;
			}
			else
			{
				if (CurrentShader == VERTEX) vs += line + "\n";
				else if (CurrentShader == GEOMETRY) gs += line + "\n";
				else if (CurrentShader == FRAGMENT) fs += line + "\n";
			}
		}

		uint32_t VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (!VertexShaderID) throw std::runtime_error("airs::Shader error: Vertex shader creation failed.\n");
		uint32_t GeometryShaderID = gs.empty() ? 0 : glCreateShader(GL_GEOMETRY_SHADER);
		if (!gs.empty() && !GeometryShaderID) throw std::runtime_error("airs::Shader error: Geometry shader creation failed.\n");
		uint32_t FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (!FragmentShaderID) throw std::runtime_error("airs::Shader error: Fragment shader creation failed.\n");

		const char* cvs = vs.c_str();
		const char* cgs = gs.c_str();
		const char* cfs = fs.c_str();

		int32_t Success;

		glShaderSource(VertexShaderID, 1, &cvs, 0);
		glCompileShader(VertexShaderID);
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetShaderInfoLog(VertexShaderID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: Vertex shader compilation failed:\n" + templog);
		}

		if (!gs.empty())
		{
			glShaderSource(GeometryShaderID, 1, &cgs, 0);
			glCompileShader(GeometryShaderID);
			glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				int32_t lenght;
				std::string templog;
				glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &lenght);
				templog.resize(lenght);
				glGetShaderInfoLog(GeometryShaderID, lenght, 0, &templog[0]);
				throw std::runtime_error("airs::Shader error: Geometry shader compilation failed:\n" + templog);
			}
		}

		glShaderSource(FragmentShaderID, 1, &cfs, 0);
		glCompileShader(FragmentShaderID);
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetShaderInfoLog(FragmentShaderID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: Fragment shader compilation failed:\n" + templog);
		}

		glAttachShader(ProgramID, VertexShaderID);
		if (!gs.empty()) glAttachShader(ProgramID, GeometryShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetProgramInfoLog(ProgramID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: Shader program linking failed:\n" + templog);
		}

		glDetachShader(ProgramID, VertexShaderID);
		if (!gs.empty()) glDetachShader(ProgramID, GeometryShaderID);
		glDetachShader(ProgramID, FragmentShaderID);
		glDeleteShader(VertexShaderID);
		if (!gs.empty()) glDeleteShader(GeometryShaderID);
		glDeleteShader(FragmentShaderID);
	}
	std::istream& operator>>(std::istream& is, Shader& shader)
	{
		shader.Load(std::string(std::istreambuf_iterator<char>(is), {}));
		return is;
	}

	int32_t Shader::Uniform(const std::string& uniform) const
	{
		glUseProgram(ProgramID);
		int32_t t = glGetUniformLocation(ProgramID, uniform.c_str());
		glUseProgram(0);
		return t;
	}
	void Shader::Uniform(int32_t location, float x) const
	{
		glUseProgram(ProgramID);
		glUniform1f(location, x);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, float x, float y) const
	{
		glUseProgram(ProgramID);
		glUniform2f(location, x, y);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, float x, float y, float z) const
	{
		glUseProgram(ProgramID);
		glUniform3f(location, x, y, z);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, float x, float y, float z, float w) const
	{
		glUseProgram(ProgramID);
		glUniform4f(location, x, y, z, w);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec2f v) const
	{
		glUseProgram(ProgramID);
		glUniform2f(location, v.x, v.y);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec3f v) const
	{
		glUseProgram(ProgramID);
		glUniform3f(location, v.x, v.y, v.z);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec4f v) const
	{
		glUseProgram(ProgramID);
		glUniform4f(location, v.x, v.y, v.z, v.w);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, int32_t x) const
	{
		glUseProgram(ProgramID);
		glUniform1i(location, x);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, int32_t x, int32_t y) const
	{
		glUseProgram(ProgramID);
		glUniform2i(location, x, y);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, int32_t x, int32_t y, int32_t z) const
	{
		glUseProgram(ProgramID);
		glUniform3i(location, x, y, z);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const
	{
		glUseProgram(ProgramID);
		glUniform4i(location, x, y, z, w);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec2i v) const
	{
		glUseProgram(ProgramID);
		glUniform2i(location, v.x, v.y);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec3i v) const
	{
		glUseProgram(ProgramID);
		glUniform3i(location, v.x, v.y, v.z);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec4i v) const
	{
		glUseProgram(ProgramID);
		glUniform4i(location, v.x, v.y, v.z, v.w);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, uint32_t x) const
	{
		glUseProgram(ProgramID);
		glUniform1ui(location, x);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, uint32_t x, uint32_t y) const
	{
		glUseProgram(ProgramID);
		glUniform2ui(location, x, y);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, uint32_t x, uint32_t y, uint32_t z) const
	{
		glUseProgram(ProgramID);
		glUniform3ui(location, x, y, z);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
	{
		glUseProgram(ProgramID);
		glUniform4ui(location, x, y, z, w);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec2ui v) const
	{
		glUseProgram(ProgramID);
		glUniform2ui(location, v.x, v.y);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec3ui v) const
	{
		glUseProgram(ProgramID);
		glUniform3ui(location, v.x, v.y, v.z);
		glUseProgram(0);
	}
	void Shader::Uniform(int32_t location, vec4ui v) const
	{
		glUseProgram(ProgramID);
		glUniform4ui(location, v.x, v.y, v.z, v.w);
		glUseProgram(0);
	}

	int32_t Shader::FastUniform(const std::string& uniform) const
	{
		return glGetUniformLocation(ProgramID, uniform.c_str());
	}
	void Shader::FastUniform(int32_t location, float x) const
	{
		glUniform1f(location, x);
	}
	void Shader::FastUniform(int32_t location, float x, float y) const
	{
		glUniform2f(location, x, y);
	}
	void Shader::FastUniform(int32_t location, float x, float y, float z) const
	{
		glUniform3f(location, x, y, z);
	}
	void Shader::FastUniform(int32_t location, float x, float y, float z, float w) const
	{
		glUniform4f(location, x, y, z, w);
	}
	void Shader::FastUniform(int32_t location, vec2f v) const
	{
		glUniform2f(location, v.x, v.y);
	}
	void Shader::FastUniform(int32_t location, vec3f v) const
	{
		glUniform3f(location, v.x, v.y, v.z);
	}
	void Shader::FastUniform(int32_t location, vec4f v) const
	{
		glUniform4f(location, v.x, v.y, v.z, v.w);
	}
	void Shader::FastUniform(int32_t location, int32_t x) const
	{
		glUniform1i(location, x);
	}
	void Shader::FastUniform(int32_t location, int32_t x, int32_t y) const
	{
		glUniform2i(location, x, y);
	}
	void Shader::FastUniform(int32_t location, int32_t x, int32_t y, int32_t z) const
	{
		glUniform3i(location, x, y, z);
	}
	void Shader::FastUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const
	{
		glUniform4i(location, x, y, z, w);
	}
	void Shader::FastUniform(int32_t location, vec2i v) const
	{
		glUniform2i(location, v.x, v.y);
	}
	void Shader::FastUniform(int32_t location, vec3i v) const
	{
		glUniform3i(location, v.x, v.y, v.z);
	}
	void Shader::FastUniform(int32_t location, vec4i v) const
	{
		glUniform4i(location, v.x, v.y, v.z, v.w);
	}
	void Shader::FastUniform(int32_t location, uint32_t x) const
	{
		glUniform1ui(location, x);
	}
	void Shader::FastUniform(int32_t location, uint32_t x, uint32_t y) const
	{
		glUniform2ui(location, x, y);
	}
	void Shader::FastUniform(int32_t location, uint32_t x, uint32_t y, uint32_t z) const
	{
		glUniform3ui(location, x, y, z);
	}
	void Shader::FastUniform(int32_t location, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const
	{
		glUniform4ui(location, x, y, z, w);
	}
	void Shader::FastUniform(int32_t location, vec2ui v) const
	{
		glUniform2ui(location, v.x, v.y);
	}
	void Shader::FastUniform(int32_t location, vec3ui v) const
	{
		glUniform3ui(location, v.x, v.y, v.z);
	}
	void Shader::FastUniform(int32_t location, vec4ui v) const
	{
		glUniform4ui(location, v.x, v.y, v.z, v.w);
	}

	void Shader::Bind() const
	{
		glUseProgram(ProgramID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}