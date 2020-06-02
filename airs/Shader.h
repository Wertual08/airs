#include <cstdint>
#include <string>



namespace airs
{
	class Shader
	{
	public:
		enum Variation
		{
			None,
			Vertex = 0x8b31,
			Geometry = 0x8dd9,
			Fragment = 0x8b30,
			Compute = 0x91b9,
		};

	private:
		uint32_t ShaderID;
		Variation ShaderType;

	public:
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&& s) noexcept;
		Shader(Variation type);
		Shader(Variation type, const std::string& src);
		~Shader();

		void Compile(const std::string& src);

		uint32_t ID() const;
		Variation Type() const;
	};
}