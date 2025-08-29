#include<iostream>
#include"main.h"
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"imgui.h"
#include "imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"VAO.h"
#include"IBO.h"
#include"Shaders.h"
#include"Renderer.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include<initializer_list>
#include"Textures.h"

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);


    GLFWwindow* window = glfwCreateWindow(900, 700, "Solar System Simulator - test", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window \n";
        glfwTerminate();
        return -1;
    }






    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 900, 700);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    ImGui::CreateContext();


    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");









    Renderer render;



    // RendererObject square(0, indicies, sizeof(indicies), positions, sizeof(positions));
    // square.AddAtribute(layout);
    // square.shader_program.SetUniform4f("u_colour", 1.0f, 0.0f, 0.0f, 1.0f);

    int count = 300;
    Texture Suntex("C:/dev/Solar System Simulator/Res/Planet_Sprites/Sun.png");

    RendererObject Sun(1, NULL, NULL, render.CircleCalc(0.0f, 0.0f, 0.05f, count).data(),  render.CircleTex(render.CircleCalc(0.0f, 0.0f, 0.05f, count)).data(), sizeof(float) * ((2 * count) + 4),"C:/dev/Solar System Simulator/Res/shader/TexShader.shader" );
    Sun.AddPosAtribute(layout);
    Sun.AddTexAtribute(Texlayout);
    Sun.shader_program.SetUniform1i("u_Texture", 0);

    float RectanglePosCoords[] = {
        // Positions            // Texture Coords
        -1.0f, -1.0f,   // Top-left
         1.0f, -1.0f,   // Top-right
         1.0f, 1.0f,   // Bottom-right
        -1.0f, 1.0f,
    };
    float BackgroundTexCords[] = {
     0.0f,  0.0f,
     1.0f,  0.0f,
     1.0f,  1.0f,
     0.0f,  1.0f
    };
    Texture backgroundtex("C:/dev/Solar System Simulator/Res/Background.jpg");


    // RendererObject Background(2, NULL, NULL, BackgroundPosCords, NULL, sizeof(float)*8+4);
    // // backgroundtex.Bind();
    // // Background.shader_program.SetUniform1i("u_Texture", 0);
    // Background.AddPosAtribute(layout);
    // Background.shader_program.SetUniform4f("u_colour", 1, 0.43, 0, 1.0);
    // // Background.AddTexAtribute(Texlayout);

    RendererObject Background(2, NULL, NULL, RectanglePosCoords, BackgroundTexCords, sizeof(float) * 8, "C:/dev/Solar System Simulator/Res/shader/TexShader.shader");
    Background.shader_program.SetUniform1i("u_Texture", 0);
    Background.AddPosAtribute(layout);
    Background.AddTexAtribute(Texlayout);














    RendererObject PtolomyPoint(98, NULL, NULL, NULL, NULL, NULL,"C:/dev/Solar System Simulator/Res/shader/TexShader.shader");
    RendererObject line(99);


    int largest_index = 3;

    planet Mercury(0, &render), Venus(1, &render), Earth(2, &render), Mars(3, &render), Jupiter(4, &render), Saturn(5, &render), Uranus(6, &render), Neptune(7, &render), Pluto(8, &render);
    planet* Planet_ptr[9] = { &Mercury, &Venus, &Earth, &Mars, &Jupiter, &Saturn, &Uranus, &Neptune, &Pluto };
    double t = 0, t_max;

    bool Info1 = false, Info2 = false, Info3 = false;
    std::string Info1txt, Info2txt, Info3txt;
    Info1txt = ParseText("C:/dev/Solar System Simulator/Res/info/1.txt");
    Info2txt = ParseText("C:/dev/Solar System Simulator/Res/info/2.txt");
    Info3txt = ParseText("C:/dev/Solar System Simulator/Res/info/3.txt");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        switch (largest_index)
        {
        case 8:
            InitPlanets(8, Planet_ptr, &render, &Sun);
            t_max = Pluto.T;
            break;

        case 7:
            InitPlanets(7, Planet_ptr, &render, &Sun);
            t_max = Neptune.T;
            break;
        case 6:
            InitPlanets(6, Planet_ptr, &render, &Sun);
            t_max = Uranus.T;
            break;
        case 5:
            InitPlanets(5, Planet_ptr, &render, &Sun);
            t_max = Saturn.T;
            break;
        case 4:
            InitPlanets(4, Planet_ptr, &render, &Sun);
            t_max = Jupiter.T;
            break;
        case 3:
            InitPlanets(3, Planet_ptr, &render, &Sun);
            t_max = Mars.T;
            break;
        case 2:
            InitPlanets(2, Planet_ptr, &render, &Sun);
            t_max = Earth.T;
            break;
        case 1:
            InitPlanets(1, Planet_ptr, &render, &Sun);
            t_max = Venus.T;
            break;
        default:
            InitPlanets(0, Planet_ptr, &render, &Sun);
            t_max = Mercury.T;
            break;
        }
        glm::mat4 Background_Proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        render.SetProjectionMatrix(Background_Proj, Background);

        float dt = t_max / 1000;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        render.DrawRectangle(Background, &backgroundtex);







        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Eliptical orbits and animations")) {

                ImGui::Begin("Eliptical orbits and animations");





                render.DrawPoint(Sun, 300, &Suntex);


                if (arr_bool[0]) { Mercury.PlotOrbit();largest_index = 0;if (arr_bool[9]) { Mercury.Animate(t);t_max = Mercury.T; } }
                if (arr_bool[1]) { Venus.PlotOrbit();largest_index = 1; if (arr_bool[9]) { Venus.Animate(t);t_max = Venus.T; } }
                if (arr_bool[2]) { Earth.PlotOrbit();largest_index = 2;if (arr_bool[9]) { Earth.Animate(t);t_max = Earth.T;  } }
                if (arr_bool[3]) { Mars.PlotOrbit();largest_index = 3;if (arr_bool[9]) { Mars.Animate(t);t_max = Mars.T;  } }
                if (arr_bool[4]) { Jupiter.PlotOrbit();largest_index = 4;if (arr_bool[9]) { Jupiter.Animate(t);t_max = Jupiter.T;  } }
                if (arr_bool[5]) { Saturn.PlotOrbit();largest_index = 5;if (arr_bool[9]) { Saturn.Animate(t);t_max = Saturn.T;  } }
                if (arr_bool[6]) { Uranus.PlotOrbit();largest_index = 6;if (arr_bool[9]) { Uranus.Animate(t);t_max = Uranus.T;  } }
                if (arr_bool[7]) { Neptune.PlotOrbit();largest_index = 7; if (arr_bool[9]) { Neptune.Animate(t);t_max = Neptune.T; } }
                if (arr_bool[8]) { Pluto.PlotOrbit();largest_index = 8;if (arr_bool[9]) { Pluto.Animate(t);t_max = Pluto.T;  } }

                ImGui::SliderFloat("Time Step", &dt, 0.000241f, 0.248f);
                //ImGui::InputDouble("Delta T", &deltat);

                ImGui::Checkbox("Mercury", &arr_bool[0]);
                ImGui::SameLine();
                ImGui::Checkbox("Animate", &arr_bool[9]);
                ImGui::Checkbox("venus", &arr_bool[1]);
                ImGui::Checkbox("earth", &arr_bool[2]);
                ImGui::Checkbox("mars", &arr_bool[3]);
                ImGui::Checkbox("jupiter", &arr_bool[4]);
                ImGui::Checkbox("saturn", &arr_bool[5]);
                ImGui::Checkbox("uranus", &arr_bool[6]);
                ImGui::Checkbox("neptune", &arr_bool[7]);
                ImGui::Checkbox("pluto", &arr_bool[8]);

                if (ImGui::Button("Info")) {

                    Info1 = !Info1;
                }
                if (Info1) {
                    ImGui::Begin("Info");
                    ImGui::Text(Info1txt.c_str());
                    ImGui::End();
                }


                ImGui::End();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Spirographs")) {

                ImGui::Begin("Spirographs");
                ImGui::Columns(2, "Spirographs");

                ImGui::Text("Choose The Further Planet :");
                ImGui::ListBox("##", &Planet_a, names, 9);

                ImGui::NextColumn();
                ImGui::Text("Choose The Closer Planet :");
                ImGui::ListBox("##box 2", &Planet_b, names, 9);

                if (Planet_b>Planet_a){
                    int temp = Planet_b;
                    Planet_b = Planet_a;
                    Planet_a = temp;
                }
                if(Planet_a==Planet_b){
                    if (Planet_a==8){Planet_b-=1;}
                    else{Planet_a+=1;}
                }


                spirograph(Planet_a, (*Planet_ptr[Planet_a]), (*Planet_ptr[Planet_b]), &render, line);
                ImGui::Columns();

                if (ImGui::Button("Info")) {

                    Info2 = !Info2;
                }
                if (Info2) {
                    ImGui::Begin("Info");
                    ImGui::Text(Info2txt.c_str());
                    ImGui::End();
                }

                ImGui::End();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Ptolomys")) {



                if (arr_bool[0]) { largest_index = 0; }
                if (arr_bool[1]) { largest_index = 1; }
                if (arr_bool[2]) { largest_index = 2; }
                if (arr_bool[3]) { largest_index = 3; }
                if (arr_bool[4]) { largest_index = 4; }
                if (arr_bool[5]) { largest_index = 5; }
                if (arr_bool[6]) { largest_index = 6; }
                if (arr_bool[7]) { largest_index = 7; }
                if (arr_bool[8]) { largest_index = 8; }
                Ptolomys(&render, line, PtolomyPoint, largest_index, orbits, (*Planet_ptr[centre]),
                    (arr_bool[0] ? Mercury : (*Planet_ptr[centre])),
                    (arr_bool[1] ? Venus : (*Planet_ptr[centre])),
                    (arr_bool[2] ? Earth : (*Planet_ptr[centre])),
                    (arr_bool[3] ? Mars : (*Planet_ptr[centre])),
                    (arr_bool[4] ? Jupiter : (*Planet_ptr[centre])),
                    (arr_bool[5] ? Saturn : (*Planet_ptr[centre])),
                    (arr_bool[6] ? Uranus : (*Planet_ptr[centre])),
                    (arr_bool[7] ? Neptune : (*Planet_ptr[centre])),
                    (arr_bool[8] ? Pluto : (*Planet_ptr[centre]))
                );




                ImGui::Begin("Ptolomys");
                ImGui::Combo("##combo", &centre, names, 9, 9);
                ImGui::Checkbox("Mercury", &arr_bool[0]);
                ImGui::SameLine();
                ImGui::Checkbox("venus", &arr_bool[1]);
                ImGui::SameLine();
                ImGui::Checkbox("earth", &arr_bool[2]);
                ImGui::SameLine();
                ImGui::Checkbox("mars", &arr_bool[3]);

                ImGui::Checkbox("jupiter", &arr_bool[4]);
                ImGui::SameLine();
                ImGui::Checkbox("saturn", &arr_bool[5]);
                ImGui::SameLine();
                ImGui::Checkbox("uranus", &arr_bool[6]);
                ImGui::SameLine();
                ImGui::Checkbox("neptune", &arr_bool[7]);
                ImGui::SameLine();
                ImGui::Checkbox("pluto", &arr_bool[8]);
                ImGui::SliderInt("Number of orbits for center", &orbits, 10, 100);

                if (ImGui::Button("Info")) {

                    Info3 = !Info3;
                }
                if (Info3) {
                    ImGui::Begin("Info");
                    ImGui::Text(Info3txt.c_str());
                    ImGui::End();
                }
                ImGui::End();

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }



        // Ptolomys( &render, line, PtolomyPoint,8, 10, Jupiter, Saturn, Uranus, Pluto );
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();

        t += dt;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

}
