#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// ====================================================
// VERTEX SHADER
// ====================================================

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"uniform float angle;\n"
"out vec2 TexCoord;\n"

"void main()\n"
"{\n"

"   float c = cos(angle);\n"
"   float s = sin(angle);\n"

"   // Rotacion sobre el eje Y\n"
"   mat3 rotation = mat3(\n"
"       c, 0.0, s,\n"
"       0.0, 1.0, 0.0,\n"
"      -s, 0.0, c\n"
"   );\n"

"   vec3 rotatedPos = rotation * aPos;\n"

"   // Alejar el triangulo\n"
"   rotatedPos.z -= 2.0;\n"

"   // Perspectiva\n"
"   float fov = 1.5;\n"
"   float perspective = fov / (-rotatedPos.z);\n"

"   gl_Position = vec4(\n"
"       rotatedPos.x * perspective,\n"
"       rotatedPos.y * perspective,\n"
"       rotatedPos.z / 3.0,\n"
"       1.0\n"
"   );\n"

"   TexCoord = aTexCoord;\n"
"}\0";


// ====================================================
// FRAGMENT SHADER
// ====================================================

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D tex;\n"

"void main()\n"
"{\n"
"   FragColor = texture(tex, TexCoord);\n"
"}\n\0";


int main()
{

    // ====================================================
    // INICIALIZAR GLFW
    // ====================================================

    if (!glfwInit())
    {
        std::cout << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );


    // ====================================================
    // CREAR VENTANA
    // ====================================================

    GLFWwindow* window = glfwCreateWindow(
        800,
        800,
        "Triangulo con Textura",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);


    // ====================================================
    // INICIALIZAR GLAD
    // ====================================================

    if (!gladLoadGL())
    {
        std::cout << "Error al inicializar GLAD" << std::endl;

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    glViewport(0, 0, 800, 800);


    // ====================================================
    // CREAR VERTEX SHADER
    // ====================================================

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL
    );

    glCompileShader(vertexShader);


    // ====================================================
    // CREAR FRAGMENT SHADER
    // ====================================================

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL
    );

    glCompileShader(fragmentShader);


    // ====================================================
    // CREAR SHADER PROGRAM
    // ====================================================

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);


    // Ya no necesitamos los shaders individuales

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // ====================================================
    // VERTICES
    // ====================================================

    // x, y, z,     coordenada X, coordenada Y

    GLfloat vertices[] =
    {
        // ==========================================
        // Vertices principales
        // ==========================================

        -0.5f,
        -0.5f * float(sqrt(3)) / 3,
        0.0f,
        0.0f,
        0.0f,

        0.5f,
        -0.5f * float(sqrt(3)) / 3,
        0.0f,
        1.0f,
        0.0f,

        0.0f,
        0.5f * float(sqrt(3)) * 2 / 3,
        0.0f,
        0.5f,
        1.0f,


        // ==========================================
        // Vertices interiores
        // ==========================================

        -0.5f / 2,
        0.5f * float(sqrt(3)) / 6,
        0.0f,
        0.25f,
        0.75f,

        0.5f / 2,
        0.5f * float(sqrt(3)) / 6,
        0.0f,
        0.75f,
        0.75f,

        0.0f,
        -0.5f * float(sqrt(3)) / 3,
        0.0f,
        0.5f,
        0.0f
    };


    // ====================================================
    // INDICES
    // ====================================================

    GLuint indices[] =
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };


    // ====================================================
    // VAO, VBO Y EBO
    // ====================================================

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);


    // ====================================================
    // VBO
    // ====================================================

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // ====================================================
    // EBO
    // ====================================================

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // ====================================================
    // ATRIBUTO DE POSICION
    // ====================================================

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ====================================================
    // ATRIBUTO DE TEXTURA
    // ====================================================

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // ====================================================
    // CARGAR IMAGEN
    // ====================================================

    int width;
    int height;
    int nrChannels;


    // Voltear imagen verticalmente
    stbi_set_flip_vertically_on_load(true);


    unsigned char* data = stbi_load(
        "ImagenMariposas.png",
        &width,
        &height,
        &nrChannels,
        4
    );


    // ====================================================
    // COMPROBAR SI LA IMAGEN CARGO
    // ====================================================

    if (data)
    {
        std::cout << "====================================" << std::endl;
        std::cout << "Imagen cargada correctamente" << std::endl;
        std::cout << "Ancho: " << width << std::endl;
        std::cout << "Alto: " << height << std::endl;
        std::cout << "Canales originales: " << nrChannels << std::endl;
        std::cout << "====================================" << std::endl;
    }
    else
    {
        std::cout << "====================================" << std::endl;
        std::cout << "ERROR: No se pudo cargar la imagen" << std::endl;
        std::cout << "Archivo buscado: ImagenMariposas.png" << std::endl;
        std::cout << "====================================" << std::endl;
    }


    // ====================================================
    // CREAR TEXTURA
    // ====================================================

    GLuint texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);


    // ====================================================
    // CONFIGURACION DE LA TEXTURA
    // ====================================================

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );


    // ====================================================
    // CARGAR IMAGEN EN OPENGL
    // ====================================================

    if (data)
    {
        // Forzamos la imagen a 4 canales RGBA

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );


        // Crear mipmaps

        glGenerateMipmap(GL_TEXTURE_2D);


        std::cout << "Textura creada correctamente" << std::endl;
    }
    else
    {
        std::cout << "La textura no fue creada porque la imagen no cargo."
            << std::endl;
    }


    // ====================================================
    // LIBERAR MEMORIA DE LA IMAGEN
    // ====================================================

    stbi_image_free(data);


    // ====================================================
    // MAIN LOOP
    // ====================================================

    while (!glfwWindowShouldClose(window))
    {

        // ====================================================
        // COLOR DE FONDO
        // ====================================================

        glClearColor(
            0.07f,
            0.13f,
            0.17f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);


        // ====================================================
        // USAR SHADER
        // ====================================================

        glUseProgram(shaderProgram);


        // ====================================================
        // ROTACION
        // ====================================================

        float time = (float)glfwGetTime();

        float angle = time * 0.8f;


        GLuint angleLocation =
            glGetUniformLocation(
                shaderProgram,
                "angle"
            );


        glUniform1f(
            angleLocation,
            angle
        );


        // ====================================================
        // ACTIVAR TEXTURA
        // ====================================================

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(
            GL_TEXTURE_2D,
            texture
        );


        GLuint textureLocation =
            glGetUniformLocation(
                shaderProgram,
                "tex"
            );


        glUniform1i(
            textureLocation,
            0
        );


        // ====================================================
        // DIBUJAR TRIANGULO
        // ====================================================

        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,
            9,
            GL_UNSIGNED_INT,
            0
        );


        // ====================================================
        // MOSTRAR RESULTADO
        // ====================================================

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    // ====================================================
    // LIMPIAR
    // ====================================================

    glDeleteTextures(
        1,
        &texture
    );

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteBuffers(
        1,
        &EBO
    );

    glDeleteProgram(
        shaderProgram
    );


    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}