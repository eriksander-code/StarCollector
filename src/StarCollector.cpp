#include "StarCollector.h"

void StarCollector::InitEnvironment()
{
    SetVSync(false);

    SetBackgroundColor(ENGINE::WINDOW::BLACK_BACKGROUND);
    GameIni.ReadIniArchive("../config/config.ini");
    GameOrder = GameIni.GetConfigValue("GameOrder");
    GameStage = GameIni.GetConfigValue("GameStage");
    GameTime = GameIni.GetConfigValue("GameTime");     // 300 Segundos
    QtyStars = GameIni.GetConfigValue("QtyStars");
    PlayerSize = GameIni.GetConfigValue("PlayerSize");
    StarSize = GameIni.GetConfigValue("StarSize");
    RangeOfCollect = GameIni.GetConfigValue("RangeOfCollect");
    StarCollect = GameIni.GetConfigValue("StarCollect");
    VelocityIncrease = GameIni.GetConfigValue("VelocityIncrease");
    ParallaxInc =  GameIni.GetConfigValue("ParallaxIncrease");

    MyHUD.SetCustomRenderFunction([&](){
        ImGui::Begin("StarCollector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::Text("PONTUACAO............: %u", StarCollect);
            ImGui::Text("PONTUACAO TOTAL......: %u", StarCollect+Score);
            ImGui::Text("STATUS DE JOGO.......: %s", GameStat.c_str());
            ImGui::Text("ORDA.................: %u", GameOrder);
            ImGui::Text("TEMPO RESTANTE.......: %.2f", (GameTime-glfwGetTime()));
        }
        ImGui::End();
    });
    MyHUD.Start(GetWindowHandle());
}

void StarCollector::Initialize() 
{    
    //MySound.SCPlaySound("../assets/music.mp3");
    StarActors.CreateNewObjects(QtyStars, -GetWindowAspectRatio(), GetWindowAspectRatio(), -1.0f, 1.0f, -1.0f, 1.0f, 10.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    GlobalShader.CreateBasicShaders();
    Player.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO),1,1,0,1);
    PlayerT.BasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, "../assets/rocket_t.png", false, false);
    GlobalShader.Use();
    GlobalShader.SetInt("texture0", 0);

    Star.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO),1,1,0,1);
    StarT.BasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/star_2.png", true, false);
    GlobalShader.Use();
    GlobalShader.SetInt("texture1", 1);

    Background.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO),1,1,0,1);
    BackgroundT.BasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/background_low.png", true, true);
    GlobalShader.Use();
    GlobalShader.SetInt("texture2", 2);
}

void StarCollector::Input(GLFWwindow* window, float Delta) 
{    
    if (Keyboard::isKeyPressed(GLFW_KEY_S)) {
        if (MyActors.ObjectArray.at(PlayerID).position.y > -1) {
            //MyActors.MoveObject(ENGINE::AXIS::Y_AXIS, ENGINE::DIRECTION::BACKWARD, Delta, PlayerID);
            MyActors.ObjectArray.at(PlayerID).velocity.y -= VelocityIncrease * Delta;
            
            if (PlayerDegreesRot > 0) 
                {PlayerDegreesRot -= 360 * Delta; }
            else if (PlayerDegreesRot < 0) 
                {PlayerDegreesRot += 360 * Delta; };
            
            ParallaxY.y += ParallaxInc*Delta;
        } else {
            MyActors.ObjectArray.at(PlayerID).position.y = -1;
        }
    } else if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
        if (MyActors.ObjectArray.at(PlayerID).position.y < 1) {
            //MyActors.MoveObject(ENGINE::AXIS::Y_AXIS, ENGINE::DIRECTION::FORWARD, Delta, PlayerID);
            MyActors.ObjectArray.at(PlayerID).velocity.y += VelocityIncrease * Delta;
            
            if (PlayerDegreesRot > 180) 
                {PlayerDegreesRot -= 360 * Delta; }
            else if (PlayerDegreesRot < 180) 
                {PlayerDegreesRot += 360 * Delta; };
            
            ParallaxY.y -= ParallaxInc*Delta;
        } else {
            MyActors.ObjectArray.at(PlayerID).position.y = 1;
        }
    } else {
        MyActors.ObjectArray.at(PlayerID).velocity.y = MyActors.ObjectArray.at(PlayerID).velocity.y * 0.999f;
        ParallaxY.y = ParallaxY.y * 0.8f;
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
        if (MyActors.ObjectArray.at(PlayerID).position.x > -GetWindowAspectRatio()) {
            //MyActors.MoveObject(ENGINE::AXIS::X_AXIS, ENGINE::DIRECTION::BACKWARD, Delta, PlayerID);
            MyActors.ObjectArray.at(PlayerID).velocity.x -= VelocityIncrease * Delta;
            
            if (PlayerDegreesRot > 270) 
                {PlayerDegreesRot -= 360 * Delta; }
            else if (PlayerDegreesRot < 270) 
                {PlayerDegreesRot += 360 * Delta; };
            
            ParallaxX.y += ParallaxInc*Delta;
        } else {
            MyActors.ObjectArray.at(PlayerID).position.x = -GetWindowAspectRatio();
        }
    } else if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
        if (MyActors.ObjectArray.at(PlayerID).position.x < GetWindowAspectRatio()) {
            //MyActors.MoveObject(ENGINE::AXIS::X_AXIS, ENGINE::DIRECTION::FORWARD, Delta, PlayerID);
            MyActors.ObjectArray.at(PlayerID).velocity.x += VelocityIncrease * Delta;
            
            if (PlayerDegreesRot > 90) 
                {PlayerDegreesRot -= 360 * Delta; }
            else if (PlayerDegreesRot < 90) 
                {PlayerDegreesRot += 360 * Delta; };
            
                ParallaxX.y -= ParallaxInc*Delta;
        } else {
            MyActors.ObjectArray.at(PlayerID).position.x = GetWindowAspectRatio();
        }
    } else {
        MyActors.ObjectArray.at(PlayerID).velocity.x = MyActors.ObjectArray.at(PlayerID).velocity.x * 0.999f;
        ParallaxX.y = ParallaxX.y * 0.8f; 
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_SPACE)) {
        if ((MyActors.ObjectArray.at(PlayerID).velocity.x < 1.0f) &&
            (MyActors.ObjectArray.at(PlayerID).velocity.y < 1.0f)) {
                MyActors.ObjectArray.at(PlayerID).velocity.x += 0.2f * Delta;
                MyActors.ObjectArray.at(PlayerID).velocity.y += 0.2f * Delta;
            }
    } else {
        if ((MyActors.ObjectArray.at(PlayerID).velocity.x > 0.5f) &&
            (MyActors.ObjectArray.at(PlayerID).velocity.y > 0.5f)) {
                MyActors.ObjectArray.at(PlayerID).velocity.x -= 0.2f * Delta;
                MyActors.ObjectArray.at(PlayerID).velocity.y -= 0.2f * Delta;
            }
    }
}

void StarCollector::ProcessPhysics(float Delta) 
{
    
    if (MyActors.ObjectArray.at(PlayerID).position.x + MyActors.ObjectArray.at(PlayerID).velocity.x*Delta < GetWindowAspectRatio() &&
        MyActors.ObjectArray.at(PlayerID).position.x + MyActors.ObjectArray.at(PlayerID).velocity.x*Delta > -GetWindowAspectRatio()) { 
        MyActors.ObjectArray.at(PlayerID).position.x += MyActors.ObjectArray.at(PlayerID).velocity.x*Delta; 
    } else {
        MyActors.ObjectArray.at(PlayerID).velocity.x = 0;
    }

    if (MyActors.ObjectArray.at(PlayerID).position.y + MyActors.ObjectArray.at(PlayerID).velocity.y*Delta < 1 &&
        MyActors.ObjectArray.at(PlayerID).position.y + MyActors.ObjectArray.at(PlayerID).velocity.y*Delta > -1) { 
        MyActors.ObjectArray.at(PlayerID).position.y += MyActors.ObjectArray.at(PlayerID).velocity.y*Delta;   
    } else {
        MyActors.ObjectArray.at(PlayerID).velocity.y = 0;
    }
}

void StarCollector::Processing() 
{
    if ((i32)glfwGetTime() % 501 == 0) {
        //MySound.SCPlaySound("../assets/music.mp3");
    }

    //////////////////////////////////////////////////////////////////////
    // Logica de jogo
    if (glfwGetTime() < GameTime) {
        for (int i = 0; i < StarActors.ObjectArray.size(); i++) {
            if (((MyActors.ObjectArray.at(PlayerID).position.x <= StarActors.ObjectArray.at(i).position.x + RangeOfCollect) &&
                (MyActors.ObjectArray.at(PlayerID).position.x >= StarActors.ObjectArray.at(i).position.x - RangeOfCollect)) &&
                ((MyActors.ObjectArray.at(PlayerID).position.y <= StarActors.ObjectArray.at(i).position.y + RangeOfCollect) &&
                (MyActors.ObjectArray.at(PlayerID).position.y >= StarActors.ObjectArray.at(i).position.y - RangeOfCollect))) {
                    auto it = StarActors.ObjectArray.begin() + i;
                    StarCollect += 1;
                    MySound.SCPlaySound("../assets/pop_sound.mp3");
                    StarActors.ObjectArray.erase(it);

                    if (StarCollect % 10 == 0) {
                        GameTime += 10.f;
                    }
            }
        }
        if (StarCollect >= QtyStars) {
            GameTime = glfwGetTime();
        }
    } else {
        if (StarCollect < QtyStars) {
            GameStat = "Perdeu!";

        } else 
        if (StarCollect >= QtyStars) {

            if (glfwGetTime() >= GameTime + 5) {
                    if (GameStage <= 0.85f) {
                        GameStage = 0.85f;
                    } else {
                        GameStage -= 0.06125f;
                    }

                    if (QtyStars >= 4000.0f) {
                        QtyStars = 4000.0f;
                    } else {
                        QtyStars *= 1.35f;
                    }

                    if (PlayerSize <= 25.85f) {
                        PlayerSize = 25.85f;
                    } else {
                        PlayerSize *=  GameStage;
                    }

                    if (StarSize <= 25.85f) {
                        StarSize = 25.85f;
                    } else {
                        StarSize *=  GameStage;
                    }

                    if (RangeOfCollect <= 0.025f) {
                        RangeOfCollect = 0.025f;
                    } else {
                        RangeOfCollect *=  GameStage;
                    }

                    if ((MyActors.ObjectArray.at(PlayerID).velocity.x <= 0.025f) &&
                        (MyActors.ObjectArray.at(PlayerID).velocity.y <= 0.025f) &&
                        (MyActors.ObjectArray.at(PlayerID).velocity.z <= 0.025f)) {
                        MyActors.ObjectArray.at(PlayerID).velocity = glm::vec3(0.025f);
                    }

                    Score += StarCollect;
                    StarCollect = 0;

                    GameOrder++;
                    GameStat = "Rodando";
                    
                    StarActors.CreateNewObjects(QtyStars, -GetWindowAspectRatio(), GetWindowAspectRatio(), -1.0f, 1.0f, -1.0f, 1.0f, 10.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                    GameTime = glfwGetTime() + GameIni.GetConfigValue("TimeBonusPerRound");
            } else {
                GameStat = "Ganhou!";
            }
        }
    }

    ////////////////////////////////////////////////////////////////////
    // Projeção
    MyCamera.Start();
    MyCamera.SetProjection(ENGINE::CAMERA::ORTHOGRAPHIC, GetWindowWidth(), GetWindowHeight());
    MyCamera.Translate(0.0f, 0.0f, -1.0f);
    MyCamera.Zoom(0.975f);

    ////////////////////////////////////////////////////////////////////
    // Modelo
    PlayerModel = glm::mat4(1.0f);
    BackgroundModel = glm::mat4(1.0f);

    // Atente-se quanto as matrizes de parametros!
    PlayerModel = glm::translate(PlayerModel, MyActors.ObjectArray.at(PlayerID).position);
    PlayerModel = glm::rotate(PlayerModel, glm::radians(PlayerDegreesRot), glm::vec3(0.0f, 0.0f, 1.0f));
    PlayerModel = glm::scale(PlayerModel, glm::vec3(PlayerSize/1000));

    BackgroundModel = glm::scale(BackgroundModel, glm::vec3(16.0f, 8.0f, -1.0f));
    BackgroundModel = glm::translate(BackgroundModel, glm::vec3(-MyActors.ObjectArray.at(PlayerID).position.x/10, 
                                                                -MyActors.ObjectArray.at(PlayerID).position.y/10, 1.0f));

    ////////////////////////////////////////////////////////////////////
    // Enviando pra GPU
    GlobalShader.SetMat4("projection", MyCamera.GetProjectionMat4());
    GlobalShader.SetMat4("view", MyCamera.GetCameraMat4());
}

void StarCollector::Render() 
{    
    //////////////////////////////////////////////////////////////////////////
    // Render Data

    GlobalShader.Use();
    GlobalShader.SetMat4("model", BackgroundModel);
    GlobalShader.SetInt("Actor", 2);
    glBindTexture(GL_TEXTURE_2D, BackgroundT.GetTextureID());
    glBindVertexArray(Background.GetVAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    GlobalShader.Use();
    glBindTexture(GL_TEXTURE_2D, StarT.GetTextureID());
    glBindVertexArray(Star.GetVAO());

    for (int i = 0; i < StarActors.ObjectArray.size(); i++) {
        StarModel = glm::mat4(1.0f);
        StarModel = glm::translate(StarModel, StarActors.ObjectArray.at(i).position);
        StarModel = glm::translate(StarModel, glm::vec3(0.0f, sin((f32)glfwGetTime()/5)/50, 0.0f));
        StarModel = glm::rotate(StarModel, glm::radians((f32)glfwGetTime()*50), glm::vec3(0.0f, 0.0f, 1.0f));
        //StarModel = glm::rotate(StarModel, glm::radians((f32)glfwGetTime()*StarActors.ObjectArray.at(i).position.y), glm::vec3(0.0f, 0.0f, 1.0f));
        //StarModel = glm::translate(StarModel, glm::vec3(sin((f32)glfwGetTime()*StarActors.ObjectArray.at(i).position.x), cos((f32)glfwGetTime()*StarActors.ObjectArray.at(i).position.y), 1.0f));
        StarModel = glm::scale(StarModel, glm::vec3(StarSize/1000));

        GlobalShader.SetMat4("model", StarModel);
        GlobalShader.SetInt("Actor", 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    }

    GlobalShader.Use();
    GlobalShader.SetMat4("model", PlayerModel);
    GlobalShader.SetInt("Actor", 0);
    glBindTexture(GL_TEXTURE_2D, PlayerT.GetTextureID());
    glBindVertexArray(Player.GetVAO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    MyHUD.Run(GetWindowWidth(), GetWindowHeight());
}

void StarCollector::Release() 
{
    Player.ReleaseBuffers();
    Star.ReleaseBuffers();
    Background.ReleaseBuffers();
    MyHUD.Release();
}