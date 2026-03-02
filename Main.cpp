#include <iostream>      // Permite imprimir mensajes en consola
#include <cmath>         // Permite usar sqrt()
#include <glad/glad.h>   // Carga funciones modernas de OpenGL
#include <GLFW/glfw3.h>  // Manejo de ventanas y contexto OpenGL


// ============================
// VERTEX SHADER
// ============================
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"      // Recibe posición del vértice (x,y,z)
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"        // Convierte vec3 en vec4 para OpenGL
"}\0";


// ============================
// FRAGMENT SHADER (Solo líneas)
// ============================
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"                      // Color final del pixel
"void main()\n"
"{\n"
"   float spacing = 15.0;\n"                 // Distancia entre líneas\n"
"\n"
"   // Creamos patrón diagonal con coordenadas del pixel\n"
"   if (mod(gl_FragCoord.x + gl_FragCoord.y, spacing) < 1.0)\n"
"       FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"  // Línea blanca\n"
"   else\n"
"       discard;\n"                          // Elimina el pixel (no se dibuja)\n"
"}\n\0";


int main()
{
    // Inicializa GLFW (librería de ventana)
    glfwInit();

    // Configuramos versión de OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Indicamos que usamos perfil moderno (core)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creamos una ventana de 800x800
    GLFWwindow* window = glfwCreateWindow(800, 800, "Triangulo Solo Lineas", NULL, NULL);

    // Verificamos que la ventana se creó correctamente
    if (window == NULL)
    {
        std::cout << "Error al crear ventana\n";
        glfwTerminate();
        return -1;
    }

    // Activamos el contexto OpenGL
    glfwMakeContextCurrent(window);

    // Cargamos funciones de OpenGL con GLAD
    if (!gladLoadGL())
    {
        std::cout << "Error al cargar GLAD\n";
        return -1;
    }

    // Definimos el área visible de dibujo
    glViewport(0, 0, 800, 800);


    // ============================
    // Definimos vértices del triángulo equilátero
    // ============================
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * sqrt(3) / 3, 0.0f,   // Vértice inferior izquierdo
         0.5f, -0.5f * sqrt(3) / 3, 0.0f,   // Vértice inferior derecho
         0.0f,  0.5f * sqrt(3) * 2 / 3, 0.0f// Vértice superior
    };


    // ============================
    // Crear y compilar Vertex Shader
    // ============================
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);   // Creamos shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Enviamos código
    glCompileShader(vertexShader); // Compilamos


    // ============================
    // Crear y compilar Fragment Shader
    // ============================
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    // ============================
    // Crear programa y enlazar shaders
    // ============================
    GLuint shaderProgram = glCreateProgram();   // Creamos programa
    glAttachShader(shaderProgram, vertexShader); // Adjuntamos vertex
    glAttachShader(shaderProgram, fragmentShader); // Adjuntamos fragment
    glLinkProgram(shaderProgram); // Enlazamos

    // Eliminamos shaders individuales (ya están dentro del programa)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // ============================
    // Crear VAO y VBO
    // ============================
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO); // Genera VAO
    glGenBuffers(1, &VBO);      // Genera VBO

    glBindVertexArray(VAO);     // Activamos VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Activamos VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Enviamos datos

    // Configuramos atributo posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desactivamos para seguridad
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // ============================
    // Bucle principal
    // ============================
    while (!glfwWindowShouldClose(window))
    {
        // Fondo oscuro
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

        // Limpiamos pantalla
        glClear(GL_COLOR_BUFFER_BIT);

        // Activamos programa shader
        glUseProgram(shaderProgram);

        // Activamos VAO
        glBindVertexArray(VAO);

        // Dibujamos triángulo
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Intercambiamos buffers
        glfwSwapBuffers(window);

        // Procesamos eventos
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