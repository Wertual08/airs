#include "airs/Spriteset.h"



namespace airs
{
	static constexpr const char* Shader()
	{
		return R"{SHADER_SOURCE}(
#shader vertex
#version 330 core
uniform int Time = 0;
uniform usamplerBuffer TextureBuffer;
layout(location = 0) in ivec3 InTexMetrics;
layout(location = 1) in vec3 InPosition;
layout(location = 2) in vec2 InSize;
layout(location = 3) in vec3 InTransform;
layout(location = 4) in uint InColor;
flat out int BaseIndex;
flat out ivec2 PixelsSize;
flat out vec2 Position;
flat out float Depth;
flat out vec2 Size;
flat out vec2 Layout;
flat out float Angle;
flat out vec4 Color;
void main()
{
	Position = InPosition.xy;
	Depth = InPosition.z;
	Size = InSize;
	Color = vec4(InColor >> 24u, (InColor >> 16u) & 0xffu, (InColor >> 8u) & 0xffu, InColor & 0xffu) / 255.0f;
	Layout = InTransform.xy;
	Angle = InTransform.z;
	PixelsSize = ivec2(texelFetch(TextureBuffer, InTexMetrics.x).r,
		texelFetch(TextureBuffer, InTexMetrics.x + 1).r);
	if (InTexMetrics.x >= 0)
	{
		if (InTexMetrics.z > 0) BaseIndex = InTexMetrics.x + Time / InTexMetrics.z %
			InTexMetrics.y * PixelsSize.x * PixelsSize.y + 2;
		else if (InTexMetrics.z < 0) BaseIndex = InTexMetrics.x + (InTexMetrics.y - 1 + Time / InTexMetrics.z %
			InTexMetrics.y) * PixelsSize.x * PixelsSize.y + 2;
		else BaseIndex = InTexMetrics.x + 2;
	}
	else BaseIndex = 2;
}

#shader geometry
#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
uniform vec4 Camera = vec4(0.0f, 0.0f, 1.0f, 1.0f);
flat in int BaseIndex[1];
flat in ivec2 PixelsSize[1];
flat in vec2 Position[1];
flat in float Depth[1];
flat in vec2 Size[1];
flat in vec2 Layout[1];
flat in float Angle[1];
flat in vec4 Color[1];
flat out int PixelIndex;
flat out ivec2 RectSize;
centroid out vec2 TexPixel;
flat out float FragDepth;
flat out vec4 FragColor;
void main()
{
	if (BaseIndex[0] < 0) return;
	PixelIndex = BaseIndex[0];
	RectSize = PixelsSize[0];
	FragDepth = Depth[0];
	FragColor = Color[0];
	float cs = cos(Angle[0]);
	float sn = sin(Angle[0]);
	float hw = Size[0].x / 2.0f;
	float hh = Size[0].y / 2.0f;
	float xoffcosp = (-hw - Layout[0].x) * cs;
	float xoffcosn = (hw - Layout[0].x) * cs;
	float yoffcosp = (-hh - Layout[0].y) * cs;
	float yoffcosn = (hh - Layout[0].y) * cs;
	float xoffsinp = (-hw - Layout[0].x) * sn;
	float xoffsinn = (hw - Layout[0].x) * sn;
	float yoffsinp = (hh + Layout[0].y) * sn;
	float yoffsinn = (-hh + Layout[0].y) * sn;
	vec2 pos = Position[0] + Camera.xy - Camera.zw;
	gl_Position = vec4((pos + vec2(xoffcosn + yoffsinn, xoffsinn + yoffcosn)) / Camera.zw, 0.0f, 1.0f);
	TexPixel = vec2(float(PixelsSize[0].x), float(PixelsSize[0].y));
	EmitVertex();
	gl_Position = vec4((pos + vec2(xoffcosp + yoffsinn, xoffsinp + yoffcosn)) / Camera.zw, 0.0f, 1.0f);
	TexPixel = vec2(0.0f, float(PixelsSize[0].y));
	EmitVertex();
	gl_Position = vec4((pos + vec2(xoffcosn + yoffsinp, xoffsinn + yoffcosp)) / Camera.zw, 0.0f, 1.0f);
	TexPixel = vec2(float(PixelsSize[0].x), 0.0f);
	EmitVertex();
	gl_Position = vec4((pos + vec2(xoffcosp + yoffsinp, xoffsinp + yoffcosp)) / Camera.zw, 0.0f, 1.0f);
	TexPixel = vec2(0.0f, 0.0f);
	EmitVertex();
	EndPrimitive();
}

#shader fragment
#version 330 core
uniform usamplerBuffer TextureBuffer;
flat in int PixelIndex;
flat in ivec2 RectSize;
centroid in vec2 TexPixel;
flat in float FragDepth;
flat in vec4 FragColor;
out vec4 Color;
void main()
{
	ivec2 offset = ivec2(TexPixel);
	uint pack = texelFetch(TextureBuffer, PixelIndex + offset.x + offset.y * RectSize.x).r;
	Color = vec4(pack >> 24u, (pack >> 16u) & 0xffu, (pack >> 8u) & 0xffu, pack & 0xffu) / 255.0f * FragColor;
	if (Color.a < 0.1f) gl_FragDepth = -10000000.0f;
	else gl_FragDepth = FragDepth;
}
){SHADER_SOURCE}";
	}

	void Spriteset::Extend(size_t capacity)
	{
		size_t treshold = MaxSize();
		if (capacity > treshold) throw std::bad_array_new_length();
		while (Capacity < capacity) if ((Capacity *= 2ull) > treshold) Capacity = treshold;

		uint8_t* temp = new uint8_t[Cursor * sizeof(uint32_t)];

		Buffer::Bind();
		GetSubData(0, Cursor * sizeof(uint32_t), temp);
		Data(Capacity * sizeof(uint32_t), nullptr, Usage::StaticDraw);
		SubData(0, Cursor * sizeof(uint32_t), temp);

		delete[] temp;
	}

	Spriteset::Spriteset(size_t capacity) :
		SpritesShader(Shader()),
		CameraUID(SpritesShader.Uniform("Camera")),
		TimeUID(SpritesShader.Uniform("Time")),
		TextureBuffer(capacity, Usage::StaticDraw, Format::R32UI)
	{
		Capacity = capacity;
		Cursor = 0;
	}
	Spriteset::Spriteset() : Spriteset(1024)
	{
	}
	Spriteset::~Spriteset()
	{
	}

	void Spriteset::Load(const Bitmap& bmp, int32_t count, int32_t delay, float ax, float ay, float an, float sw, float sh)
	{
		size_t size = bmp.Size() + 2ull;
		if (Cursor + size > Capacity) Extend(Cursor + size);
		uint32_t* data = new uint32_t[size];
		memcpy(data, bmp - 2, size * sizeof(uint32_t));
		data[1] /= count;
		Buffer::Bind();
		PixelSubData(Cursor * sizeof(uint32_t), size * sizeof(uint32_t), data);
		delete[] data;
		push_back(Sprite(Cursor, count, delay, static_cast<float>(bmp.Width()) * sw, static_cast<float>(bmp.Height()) / count * sh, ax, ay, an));
		Cursor += size;
	}
	void Spriteset::Load(const Bitmap& bmp)
	{
		size_t size = bmp.Size() + 2ull;
		if (Cursor + size > Capacity) Extend(Cursor + size);
		Buffer::Bind();
		PixelSubData(Cursor * sizeof(uint32_t), size * sizeof(uint32_t), bmp - 2);
		push_back(Sprite(Cursor, 1, 0, static_cast<float>(bmp.Width()), static_cast<float>(bmp.Height()), 0.0f, 0.0f, 0.0f));
		Cursor += size;
	}

	size_t Spriteset::Size() const
	{
		return size();
	}

	void Spriteset::CameraTime(vec4f camera, int32_t time) const
	{
		SpritesShader.Uniform(CameraUID, camera);
		SpritesShader.Uniform(TimeUID, time);
	}

	void Spriteset::Bind() const
	{
		SpritesShader.Bind();
		TextureBuffer::Bind();
	}
	void Spriteset::Unbind() const
	{
		TextureBuffer::Unbind();
		SpritesShader.Unbind();
	}
}