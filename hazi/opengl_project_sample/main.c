#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define WIDTH 640
#define HEIGHT 640
#define N 100
#define M 3

GLuint vao[3], vbo[3];

GLfloat points[24] = {
	// gorbe 1
	// p1
	-0.75f, 0.5f, 0.0f,
	-0.9f, -0.5f, 0.0f,
	// p2
	-0.4f, 0.2f, 0.0f,
	-0.7f, -0.4f, 0.0f,
	// gorbe 2
	// p3
	0.5f, 0.0f, 0.0f,
	0.75f, -0.3f, 0.0f,
	// gorbe 3
	// p4
	0.65f, 0.25f, 0.0f,
	0.4f, 0.75f, 0.0f

};




GLint dragged = -1;

GLfloat HermiteCurve[3 * N * M];




GLfloat dist2_2d(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y) {

	GLfloat dx = P1x - P2x;
	GLfloat dy = P1y - P2y;
	return dx * dx + dy * dy;
}

void drawHermite(int n) {
	GLfloat t;
	GLint i;
	GLfloat step = 2.0f / (N - 1);
	HermiteCurve[3 * n * N] = points[n * 6 + 0];
	HermiteCurve[3 * n * N + 1] = points[n * 6 + 1];
	HermiteCurve[3 * n * N + 2] = 0.0f;

	for (i = 1; i < N - 1; i++) {
		t = -1 + i * step;
		HermiteCurve[(n * N + i) * 3] = points[n * 6 + 0] * (1.0f / 4.0f * t * t * t - 3.0f / 4.0f * t + 1.0f / 2.0f) + points[n * 6 + 6] * (-1.0f / 4.0f * t * t * t + 3.0f / 4.0f * t + 1.0f / 2.0f) + (points[n * 6 + 0] - points[n * 6 + 3]) * (1.0f / 4.0f * t * t * t - 1.0f / 4.0f * t * t - 1.0f / 4.0f * t + 1.0f / 4.0f) + (points[n * 6 + 6] - points[n * 6 + 9]) * (1.0f / 4.0f * t * t * t + 1.0f / 4.0f * t * t - 1.0f / 4.0f * t - 1.0f / 4.0f);
		HermiteCurve[(n * N + i) * 3 + 1] = points[n * 6 + 1] * (1.0f / 4.0f * t * t * t - 3.0f / 4.0f * t + 1.0f / 2.0f) + points[n * 6 + 7] * (-1.0f / 4.0f * t * t * t + 3.0f / 4.0f * t + 1.0f / 2.0f) + (points[n * 6 + 1] - points[n * 6 + 4]) * (1.0f / 4.0f * t * t * t - 1.0f / 4.0f * t * t - 1.0f / 4.0f * t + 1.0f / 4.0f) + (points[n * 6 + 7] - points[n * 6 + 10]) * (1.0f / 4.0f * t * t * t + 1.0f / 4.0f * t * t - 1.0f / 4.0f * t - 1.0f / 4.0f);
		HermiteCurve[(n * N + i) * 3 + 2] = 0.0f;
	}

	HermiteCurve[3 * (n * N + i)] = points[n * 6 + 6];
	HermiteCurve[3 * (n * N + i) + 1] = points[n * 6 + 7];
	HermiteCurve[3 * (n * N + i) + 2] = 0.0f;
}

void recalcHermite() {
	drawHermite(0);
	drawHermite(1);
	drawHermite(2);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * M * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLint getActivePoint(GLfloat* p, GLfloat sensitivity, GLfloat x, GLfloat y) {

	GLfloat s = sensitivity * sensitivity;
	GLfloat xNorm = -1 + x / (WIDTH / 2);
	GLfloat yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

	for (GLint i = 0; i < 8; i++)
		if (dist2_2d(p[i * 3], p[i * 3 + 1], xNorm, yNorm) < s)
			return i;

	return -1;
}



void cursorPosCallback(GLFWwindow* window, double x, double y) {

	if (dragged >= 0) {

		GLfloat xNorm = -1 + x / (WIDTH / 2);
		GLfloat yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

		points[3 * dragged] = xNorm;	 // x coordinate
		points[3 * dragged + 1] = yNorm; // y coordinate
	}
	recalcHermite();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double x, y;

		glfwGetCursorPos(window, &x, &y);
		dragged = getActivePoint(points, 0.1f, x, y);
		recalcHermite();
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		dragged = -1;
}


int main() {
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;

	const char* vertex_shader =
		"#version 330\n"
		"in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 330\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4(1.0, 0.0, 1.0, 1.0);"
		"}";

	GLuint vert_shader, frag_shader;
	GLuint shader_programme;

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Drag&Drop", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenBuffers(3, vbo);
	glGenVertexArrays(3, vao);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * M * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);

	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, frag_shader);
	glAttachShader(shader_programme, vert_shader);
	glLinkProgram(shader_programme);

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glPointSize(15.0f);

	while (!glfwWindowShouldClose(window)) {
		recalcHermite();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_programme);

		glBindVertexArray(vao[0]);
		glDrawArrays(GL_POINTS, 0, 8);
		glBindVertexArray(0);


		glBindVertexArray(vao[1]);
		glDrawArrays(GL_LINE_STRIP, 0, N * M);
		glBindVertexArray(0);


		glBindVertexArray(vao[2]);
		glDrawArrays(GL_LINES, 0, 8);
		glBindVertexArray(0);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
