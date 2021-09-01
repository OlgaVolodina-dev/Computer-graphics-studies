#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "engine.h"
#include <chrono>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

Engine& GetEngine()
{
    static Engine engine;
    return engine;
}
void processInput(GLFWwindow* window, float elapsed_milliseconds)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        GetEngine().GetCamera().ProcessKeyboard(Camera::FORWARD, elapsed_milliseconds);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        GetEngine().GetCamera().ProcessKeyboard(Camera::BACKWARD, elapsed_milliseconds);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        GetEngine().GetCamera().ProcessKeyboard(Camera::LEFT, elapsed_milliseconds);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        GetEngine().GetCamera().ProcessKeyboard(Camera::RIGHT, elapsed_milliseconds);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        GetEngine().ShowDepth(true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        GetEngine().ShowDepth(false);
}

float lastX = 400, lastY = 300;

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) // initially set to true
    {
        std::cout << "!" << std::endl;
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    GetEngine().GetCamera().ProcessMouse(xoffset, yoffset);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    start = std::chrono::steady_clock::now();
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    GLenum err = glewInit();

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        auto timePoint = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - start);
        start = timePoint;
        // input
        // -----
        processInput(window, elapsed.count());
        
        // render
        // ------
        GetEngine().SetWindowSize(width, height);
        GetEngine().Draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

