#ifndef MYPROGRAM_H
#define MYPROGRAM_H

#include "Program.h"

class StarCollector : public Program {
private:
    //////////////////////////////////////////////////////////////////////////////////
    // Config
    IniReader GameIni;

    //////////////////////////////////////////////////////////////////////////////////
    // ACTORS
    BOX_OBJECT_MANAGER MyActors;
    BOX_OBJECT_MANAGER StarActors;

    // HUD
    
    // Background
    Mesh        Background;
    Texture     BackgroundT;
    glm::mat4   BackgroundModel       = glm::mat4(1.0f);  
    glm::vec2   ParallaxX = glm::vec2(0.0f, 0.0f);  // pos vel
    glm::vec2   ParallaxY = glm::vec2(0.0f, 0.0f);
    f32         ParallaxInc;

    // Stars Attributes
    Mesh        Star;
    Texture     StarT;
    glm::mat4   StarModel       = glm::mat4(1.0f);  
    //u32         StarID          = StarActors.CreateOneObject(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f,
    //                                                       glm::vec3(5.0f, 5.0f, 5.0f), 
    //                                                       glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    // Player Attributes
    u32         Score = 0;
    f32         PlayerDegreesRot = 0;
    Mesh        Player;
    Texture     PlayerT;
    glm::mat4   PlayerModel     = glm::mat4(1.0f);
    u32         PlayerID        = MyActors.CreateOneObject(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f,
                                                           glm::vec3(0.0f, 0.0f, 0.0f), 
                                                           glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    // Global Attributes
    Shader      GlobalShader;       // Serão apenas cubos

    std::string GameStat;
    u32         GameOrder;
    f32         GameStage;
    f32         GameTime;     // 300 Segundos
    i32         QtyStars;
    f32         PlayerSize;
    f32         StarSize;
    f32         RangeOfCollect;
    f32         VelocityIncrease;
    u32         StarCollect;

    //////////////////////////////////////////////////////////////////////////////////
    // Vertex Data

    // Vertex
    u32 MyEBO[6] {
        0, 1, 2,
        2, 3, 0,
    };

    // Default Vertex
    Mesh::vertex MyVertex[4] {         // Padrão
        // X      Y       Z  |  R      G     B  |    U     V
        {-0.50f, -0.50f,  0.0f, 1.0f,  1.0f,  0.0f,  0.00f, 0.00f}, // bottom left
        { 0.50f, -0.50f,  0.0f, 0.0f,  0.85f, 0.0f,  1.00f, 0.00f}, // bottom right
        { 0.50f,  0.50f,  0.0f, 0.85f, 0.0f,  0.0f,  1.00f, 1.00f},  // top right
        {-0.50f,  0.50f,  0.0f, 0.0f,  0.0f,  0.85f, 0.00f, 1.00f}, // top left 
    };

public:
        
    void Input(GLFWwindow* window, float Delta);
    void ProcessPhysics(float Delta);
    void InitEnvironment();
    void Initialize();
    void Processing();
    void Render();
    void Release();
};

#endif //MYPROGRAM_H