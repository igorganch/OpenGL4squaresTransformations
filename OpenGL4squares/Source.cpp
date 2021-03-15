#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderinit.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

bool SPressed;
bool WPressed;
bool QPressed;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& ourShader);
void init(Shader& ourShader);
void render();

//void transformations(Shader& ourShader, float x, float y, float z);

const unsigned int screen_width = 1200;
const unsigned int screen_height = 800;
const GLuint NumVertices = 36;
GLuint VBO; //(Vertex Buffer Object) stores vertex array data in high performace grapgic memory on the server side which promotes effecient data transfer 
GLuint VAO; //(Vertex Array Object)
GLuint EBO; //(Element Buffer Object) to create the indices 
int random;

int main()
{
    SPressed = false;
    WPressed = false;
    QPressed = false;
    random = 100;
    glfwInit(); //Initialize library

    //Set some Hints for the next glfwCreateWindow // Specidying which verision of opengl to use 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Set the OpenGL version to 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); //Set the OpenGL version to 4.x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Set OpenGL core profile the one i have downloaded

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  
    GLFWwindow* window;                                     //Name of program
    window = glfwCreateWindow(screen_width, screen_height, "4 squares view Ihar Gancharenka", NULL, NULL);  //NULL and Null is glfwMonitor and is used to create a full screen
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // set the window to render 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   
  
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("shader.vs", "shader.fs"); // you can name your shader files however you like

    init(ourShader);

    // render loop
    bool tf = false;
    float object[][3] = { {-0.8f, -0.8f, 0.0f }, {0.8f, -0.8f, 0.0f}, {-0.8f, 0.8f, 0.0f}, {0.8f, 0.8f, 0.0f} };
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----


        processInput(window, ourShader);

        //   tranformations(ourShader);
        render();
        ourShader.use();

        glBindVertexArray(VAO);

        // calculate the model matrix for each object and pass it to shader before drawing

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

        ourShader.use();
        //We specify the uniform variables in the Vertex Shader
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // We pass thev variables to the shaders (3 different ways)
      //  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);


        for (unsigned int i = 0; i < 4; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            if (i == random) {
                if (WPressed != true) {
                    float angle = glfwGetTime() * 45.0f;
                    model = glm::rotate(model, glm::radians(angle), glm::vec3(object[i][0], object[i][1], object[i][2]));
                    model = glm::translate(model, glm::vec3(object[i][0], object[i][1], object[i][2]));
                }
            }
            else {

                model = glm::translate(model, glm::vec3(object[i][0], object[i][1], object[i][2]));

            }
            if (SPressed == true && i == random) {
                model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));

            }
            if (WPressed == true && i == random) {
                float angle = glfwGetTime() * 45.0f;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
            }
            



            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }







        //Set the values of the matrices



        // glfw: swap buffers
        glfwSwapBuffers(window); // Renders the window with the new contents/data 

        glfwPollEvents(); // searches for user input
    }

    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    glfwTerminate();  //closes the window 
    return 0;
}
/*
void transformations(Shader& ourShader, float x , float y ,float z)
{
    //Add Projection
        //Initialize the Matrice (model, view, projection)
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    //Set the values of the matrices
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(x, y, z)); // Creating a translation matrix
    projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

    ourShader.use();
    //We specify the uniform variables in the Vertex Shader
    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model"); //Multiplies the matrices to perform the translations
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    // We pass thev variables to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    ourShader.setMat4("projection", projection);

}

*/



void render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*                         // R     G     B
 static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //sets the colour
 glClearBufferfv(GL_COLOR, 0, black); // Clear buffer and assign a colour
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // not sure but something useufull
 //glClear(GL_COLOR_BUFFER_BIT);
 glBindVertexArray(VAO);
 //glDrawArrays(GL_TRIANGLES, 0, NumVertices);
 glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0); // THis creates the trinagles in the object
// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Shows the trinagles in the object */
}



void init(Shader& ourShader)
{
    float vertices[] = {
        // X       Y     Z      R    G      B 
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,


    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*   glGenVertexArrays(1, &VAO); // Generates Vertex Array Object names.
    glGenBuffers(1, &VBO); // generates buffer objects
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER specifies that the VBO will store vertex array data (Defines the buffers type)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies the vertices to the VBO

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // draws the triangles

    // position attribute pointer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //The next step is to pass the Vertex Attributes to the Shader


    glEnableVertexAttribArray(0); // We then enable the Vertex Attribute Array and set the index to 0
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    */
    // transformations(ourShader , 0.0f , 0.0f, -3.0f);
}

// user input
void processInput(GLFWwindow* window, Shader& ourShader)
{
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {  //Rotation

        random = 0 + (std::rand() % (3 - 0 + 1));


    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {// Scale 
        SPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {// Move to center of the wndow 
        WPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {// Reset cubes position to original coordinates 
        SPressed = false;
        WPressed = false;
        QPressed = true;
        random = 100;


    }
    else {
        int i = 1;
    }
}

// glfw: viewport to window adjustment
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


