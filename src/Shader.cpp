#include "airs/Shader.h"
#include "gl/glew.h"
#include <stdexcept>



namespace airs
{
	inline std::string VariationToString(Shader::Variation type)
	{
		switch (type)
		{
		case Shader::None: return "None";
		case Shader::Vertex: return "Vertex";
		case Shader::Geometry: return "Geometry";
		case Shader::Fragment: return "Fragment";
		case Shader::Compute: return "Compute";
		default: return "Invalid";
		}
	}

	Shader::Shader(Shader&& s) noexcept
	{
		std::swap(ShaderID, s.ShaderID);
		std::swap(ShaderType, s.ShaderType);
	}
	Shader::Shader(Variation type) : ShaderID(0), ShaderType(type) 
	{
	}
	Shader::Shader(Variation type, const std::string& src) : Shader(type)
	{
		Compile(src);
	}
	Shader::~Shader()
	{
		if (ShaderID)
		{
			glDeleteShader(ShaderID);
			ShaderID = 0;
		}
	}

	void Shader::Compile(const std::string& src)
	{
		if (!ShaderID) ShaderID = glCreateShader(ShaderType);
		if (!ShaderID) throw std::runtime_error("airs::Program error: " +
			VariationToString(ShaderType) + " shader creation failed.\n");

		const char* csrc = src.c_str();
		int32_t Success;
		glShaderSource(ShaderID, 1, &csrc, 0);
		glCompileShader(ShaderID);
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			int32_t lenght;
			std::string templog;
			glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &lenght);
			templog.resize(lenght);
			glGetShaderInfoLog(ShaderID, lenght, 0, &templog[0]);
			throw std::runtime_error("airs::Shader error: " + VariationToString(ShaderType) + " shader compilation failed:\n" + templog);
		}
	}

	uint32_t Shader::ID() const
	{
		return ShaderID;
	}
	Shader::Variation Shader::Type() const
	{
		return ShaderType;
	}
}