#pragma once
#include "Bindable.h"
#include "math.h"
#include "Shader.h"
#include <istream>
#include <string>



namespace airs
{
	class Program : public Bindable
	{
	private:
		uint32_t ProgramID;

	public:
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;
		Program(Program&& s) noexcept;
		Program();
		Program(const std::string& shader);
		Program(std::istream& stream);
		~Program();

		void Attach(const Shader& shader);
		void Detach(const Shader& shader);

		void Link();
		void Link(const std::string& shader);
		friend std::istream& operator>>(std::istream& is, Program& shader);

		int32_t Uniform(const std::string& uniform) const;
		void Uniform(int32_t location, float x) const;
		void Uniform(int32_t location, float x, float y) const;
		void Uniform(int32_t location, float x, float y, float z) const;
		void Uniform(int32_t location, float x, float y, float z, float w) const;
		void Uniform(int32_t location, vec2f v) const;
		void Uniform(int32_t location, vec3f v) const;
		void Uniform(int32_t location, vec4f v) const;
		void Uniform(int32_t location, int32_t x) const;
		void Uniform(int32_t location, int32_t x, int32_t y) const;
		void Uniform(int32_t location, int32_t x, int32_t y, int32_t z) const;
		void Uniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const;
		void Uniform(int32_t location, vec2i v) const;
		void Uniform(int32_t location, vec3i v) const;
		void Uniform(int32_t location, vec4i v) const;
		void Uniform(int32_t location, uint32_t x) const;
		void Uniform(int32_t location, uint32_t x, uint32_t y) const;
		void Uniform(int32_t location, uint32_t x, uint32_t y, uint32_t z) const;
		void Uniform(int32_t location, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const;
		void Uniform(int32_t location, vec2ui v) const;
		void Uniform(int32_t location, vec3ui v) const;
		void Uniform(int32_t location, vec4ui v) const;

		int32_t FastUniform(const std::string& uniform) const;
		void FastUniform(int32_t location, float x) const;
		void FastUniform(int32_t location, float x, float y) const;
		void FastUniform(int32_t location, float x, float y, float z) const;
		void FastUniform(int32_t location, float x, float y, float z, float w) const;
		void FastUniform(int32_t location, vec2f v) const;
		void FastUniform(int32_t location, vec3f v) const;
		void FastUniform(int32_t location, vec4f v) const;
		void FastUniform(int32_t location, int32_t x) const;
		void FastUniform(int32_t location, int32_t x, int32_t y) const;
		void FastUniform(int32_t location, int32_t x, int32_t y, int32_t z) const;
		void FastUniform(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w) const;
		void FastUniform(int32_t location, vec2i v) const;
		void FastUniform(int32_t location, vec3i v) const;
		void FastUniform(int32_t location, vec4i v) const;
		void FastUniform(int32_t location, uint32_t x) const;
		void FastUniform(int32_t location, uint32_t x, uint32_t y) const;
		void FastUniform(int32_t location, uint32_t x, uint32_t y, uint32_t z) const;
		void FastUniform(int32_t location, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const;
		void FastUniform(int32_t location, vec2ui v) const;
		void FastUniform(int32_t location, vec3ui v) const;
		void FastUniform(int32_t location, vec4ui v) const;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}