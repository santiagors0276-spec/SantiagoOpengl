#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ============================
// VERTEX SHADER
// ============================
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";


// ============================
// FRAGMENT SHADER
// ============================
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform int mode;\n"
"\n"
"void main()\n"
"{\n"
"   float spacing = 15.0;\n"
"\n"
"   // 0 = Triangulo naranja\n"
"   if(mode == 0)\n"
"   {\n"
"       FragColor = vec4(1.0, 0.5, 0.0, 1.0);\n"
"   }\n"
"\n"
"   // 1 = Lineas diagonales\n"
"   else if(mode == 1)\n"
"   {\n"
"       if (mod(gl_FragCoord.x + gl_FragCoord.y, spacing) < 1.0)\n"
"           FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"       else\n"
"           discard;\n"
"   }\n"
"\n"
"   // 2 = Puntos\n"
"   else if(mode == 2)\n"
"   {\n"
"       if(mod(gl_FragCoord.x, spacing) < 2.0 && mod(gl_FragCoord.y, spacing) < 2.0)\n"
"           FragColor = vec4(1.0,1.0,1.0,1.0);\n"
"       else\n"
"           discard;\n"
"   }\n"
"}\n\0";


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Triangulo Interactivo", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Error al crear ventana\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Error al cargar GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 800);

    // ============================
    // Vertices del triangulo equilatero
    // ============================
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * sqrt(3) / 3, 0.0f,
         0.5f, -0.5f * sqrt(3) / 3, 0.0f,
         0.0f,  0.5f * sqrt(3) * 2 / 3, 0.0f
    };

    // ============================
    // Vertex Shader
    // ============================
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ============================
    // Fragment Shader
    // ============================
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ============================
    // Shader Program
    // ============================
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ============================
    // VAO y VBO
    // ============================
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ============================
    // Variable de modo
    // ============================
    int mode = 0;

    // ============================
    // Loop principal
    // ============================
    while (!glfwWindowShouldClose(window))
    {
        // Detectar teclas
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            mode = 1;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            mode = 2;

        // Fondo
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Enviar modo al shader
        glUniform1i(glGetUniformLocation(shaderProgram, "mode"), mode);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ============================
    // Liberar memoria
    // ============================
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}