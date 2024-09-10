#include "GL.hpp"
#include "gl_errors.hpp"
#include "gl_compile_program.hpp"

//draw a half-screen linearity check image:
void draw_linearity_check() {
	static GLuint program = gl_compile_program(
		"#version 330\n"
		"out vec2 position;\n"
		"void main() {\n"
		"	vec2 Position = vec2(4 * (gl_VertexID & 1) - 1,  2 * (gl_VertexID & 2));\n"
		"	position = 0.5 * Position + vec2(0.5);\n"
		"	gl_Position = vec4(Position, 0.0, 1.0);\n"
		"}\n"
	,
		"#version 330\n"
		"in vec2 position;\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"	vec3 color = vec3(position.x);\n"
		"	float stripe = fract(5.0 * position.y);\n"
		"	if (stripe < 0.5) {\n"
		"		int xbits = int(gl_FragCoord.x);\n"
		"		int ybits = int(gl_FragCoord.y);\n"
		"		int tbits = \n"
		"			  (xbits & 0x8) >> 3\n"
		"			| (xbits & 0x4)     \n"
		"			| (xbits & 0x2) << 3\n"
		"			| (xbits & 0x1) << 6\n"
		"			| (ybits & 0x8) >> 2\n"
		"			| (ybits & 0x4) << 1\n"
		"			| (ybits & 0x2) << 4\n"
		"			| (ybits & 0x1) << 7\n"
		"		;\n"
		"		float thresh = (float(tbits) + 0.5) / 256.0;\n"
		"		color.r = (color.r > thresh ? 1.0 : 0.0);\n"
		"		color.g = (color.g > thresh ? 1.0 : 0.0);\n"
		"		color.b = (color.b > thresh ? 1.0 : 0.0);\n"
		"	}\n"
		"	fragColor = vec4(color, 1.0);\n"
		"}\n"
	);
	GL_ERRORS();

	static GLuint vao = 0;
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}

	glBindVertexArray(vao);

	glUseProgram(program);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	GL_ERRORS();
}
