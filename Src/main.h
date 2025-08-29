#ifndef _MAIN_H
#define _MAIN_H


#include<iostream>
#include<vector>
#include"Renderer.h"
#include"Textures.h"

BufferElementLayout layout = {
    0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0

};
BufferElementLayout Texlayout = {
    1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0

};
const long double pi = 3.14159265359;

bool arr_bool[10] = { false, false, false, false, false, false, false, false, false, true };
//data
const char* names[9] = { "Mercury", "Venus" ,"Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto" };
int Planet_a = 0, Planet_b = 1, centre, orbits = 10;

const float arr_a[9] = { 0.387f, 0.723f, 1.000f, 1.524f, 5.204f, 9.573f, 19.165f, 30.178f, 39.236f };// semi-major axis
const float arr_b[9] = { 0.370f, 0.722f, 0.999f , 1.517f, 5.191f, 9.547f, 19.123f, 30.175f, 36.892f };// semi-minor axis *10^6
const float arr_e[9] = { 0.2056f, 0.0068f, 0.0167f, 0.0935f, 0.0487f, 0.0520f, 0.0469f, 0.0097f, 0.2444f }; //ecentricity

const float arr_T[9] = { 0.241f, 0.615f, 1.000f, 1.881f, 11.862f, 29.457f, 84.011f, 164.79f, 247.94f }; //Time period in earth years

const float arr_red[9] = { 0.53f, 1.00f, 0.00f, 1.00f, 0.79f, 1.00f, 0.00f, 0.00f, 1.00f };
const float arr_green[9] = { 0.53f, 0.85f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.16f, 1.00f };
const float arr_blue[9] = { 0.53f, 0.00f, 0.20f, 0.00f, 1.00f, 0.80f, 0.90f, 1.00f, 1.00f };

class planet
{
private:
    std::string name;

    Renderer* renderer_ptr;
    std::string path;
    


    float r; // distance from foci to the point on elipce
    BufferElementLayout layout = {
    0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0

    };




public:
    float red, green, blue;
    float a, b, e, T;
    std::vector<float> Orbit_points;
    RendererObject Orbit_obj;
    RendererObject Planet_obj;
    Texture tex;



    planet(int index, Renderer* renderer_adrs) : Orbit_obj(index), Planet_obj(index + 9, NULL, NULL, NULL, NULL, NULL,"C:/dev/Solar System Simulator/Res/shader/TexShader.shader" ), tex(path) {
        renderer_ptr = renderer_adrs;
        name = names[index];
        path = "C:/dev/Solar System Simulator/Res/Planet_Sprites/"+name+".png";
        tex.reconfigure(path);
        a = arr_a[index];
        b = arr_b[index];
        e = arr_e[index];
        T = arr_T[index];
        red = arr_red[index];
        green = arr_green[index];
        blue = arr_blue[index];
        CalcOrbit();
        Orbit_obj.vb_pos.UpdateData(Orbit_points.data(), 2 * Elipse_verticies_count * sizeof(float));
        Orbit_obj.AddPosAtribute(layout);
        Orbit_obj.shader_program.SetUniform4f("u_colour", red, green, blue, 1.0f);



        Planet_obj.AddPosAtribute(layout);
        Planet_obj.AddTexAtribute(Texlayout);
        Planet_obj.shader_program.SetUniform4f("u_colour", red, green, blue, 1.0f);


    };
    void Animate(double t) {

        double theta = (2 * pi * t) / T;
        r = (a * (1 - (e * e)) / (1 - (e * cos(theta))));
        float x = r * cos(theta);
        float y = r * sin(theta);

        Planet_obj.vb_pos.UpdateData(((*renderer_ptr).CircleCalc(x, y, (a / 38.7), 10)).data(), 2 * (2 + 10) * sizeof(float));
        Planet_obj.vb_tex.UpdateData((*renderer_ptr).CircleTex((*renderer_ptr).CircleCalc(x, y, (a / 38.7), 10)).data(), 2 * (2 + 10) * sizeof(float));
        Planet_obj.AddPosAtribute(layout);
        Planet_obj.AddTexAtribute(Texlayout);

        (*renderer_ptr).DrawPoint(Planet_obj, 10, &tex);



    }

    void PlotOrbit() {
        (*renderer_ptr).DrawLoop(Orbit_obj);

    }

    void CalcOrbit() {

        double dtheta = (2 * pi) / Elipse_verticies_count;

        for (double theta = 0; theta < (2 * pi); theta += dtheta) {

            r = (a * (1 - (e * e)) / (1 - (e * cos(theta))));
            float x = r * cos(theta);
            float y = r * sin(theta);
            Orbit_points.push_back(x);
            Orbit_points.push_back(y);
        }

    }

};

template<size_t N>
void InitPlanets(int index, planet* (&arr)[N], Renderer* render_ptr, RendererObject* sun) {
    glm::mat4 proj;

    if (index == 8) { //if pluto
        proj = glm::ortho(-1 * (arr[8]->a), arr[8]->a + 20, -1 * (arr[8]->b + 5), arr[8]->b + 5);
    }
    else
    {
        if (index <= 3) // inner planet
        {
            float padding = (index / 10) + 0.2;
            proj = glm::ortho(-1 * (arr[index]->a + padding), arr[index]->a + padding, -1 * (arr[index]->b + padding), arr[index]->b + padding);
        }
        else
        {
            float padding = (index - 2);
            proj = glm::ortho(-1 * (arr[index]->a + padding), arr[index]->a + padding, -1 * (arr[index]->b + padding), arr[index]->b + padding);
        }
    }
    for (planet* ptr : arr) {
        (*render_ptr).SetProjectionMatrix(proj, (*ptr).Orbit_obj, (*ptr).Planet_obj);

    }
    (*render_ptr).SetProjectionMatrix(proj, (*sun));

}

void spirograph(int index, planet& a, planet& b, Renderer* render_ptr, RendererObject& line) {
    double t, dt, tmax, theta, r;
    float verticies[4];
    glm::mat4 proj;

    line.shader_program.SetUniform4f("u_colour", 1.0f, 1.0f, 1.0f, 1.0f);
    line.AddPosAtribute(layout);
    // create matrix.
    if (index == 8) { //if pluto
        proj = glm::ortho(-1 * (a.a), a.a + 20, -1 * (a.b + 5), a.b + 5);
    }
    else
    {
        if (index <= 3) // inner planet
        {
            float padding = (index / 10) + 0.2;
            proj = glm::ortho(-1 * (a.a + padding), a.a + padding, -1 * (a.b + padding), a.b + padding);
        }
        else
        {
            float padding = (index - 2);
            proj = glm::ortho(-1 * (a.a + padding), a.a + padding, -1 * (a.b + padding), a.b + padding);
        }
    }

    (*render_ptr).SetProjectionMatrix(proj, line, a.Orbit_obj, b.Orbit_obj);

    if ((a.T) > (b.T)) {
        tmax = (a.T) * 10;
    }
    else { tmax = b.T * 10; }
    dt = tmax / 1234;
    a.PlotOrbit();
    b.PlotOrbit();
    for (t = 0;t < tmax;t += dt) {

        theta = (2 * pi * t) / a.T;
        r = (a.a * (1 - (a.e * a.e)) / (1 - (a.e * cos(theta))));
        verticies[0] = r * cos(theta); verticies[1] = r * sin(theta);

        theta = (2 * pi * t) / b.T;
        r = (b.a * (1 - (b.e * b.e)) / (1 - (b.e * cos(theta))));
        verticies[2] = r * cos(theta); verticies[3] = r * sin(theta);

        line.vb_pos.UpdateData(verticies, 4 * sizeof(float));


        (*render_ptr).DrawLine(line, 2);

    }


}

void PtolomyPlot(int orbits, planet& centre, planet& relitive, Renderer* render_ptr, RendererObject& line) {

    double r, t, dt, tmax, theta;
    tmax = centre.T * orbits;
    dt = tmax / 1000;
    line.shader_program.SetUniform4f("u_colour", relitive.red, relitive.green, relitive.blue, 1.0f);


    float arr[2];
    std::vector<float> data;

    for (t = 0;t < tmax;t += dt) {
        // calculate coordinates for the vertex for the centre planet
        theta = (2 * pi * t) / centre.T;
        r = (centre.a * (1 - (centre.e * centre.e)) / (1 - (centre.e * cos(theta))));
        arr[0] = r * cos(theta); arr[1] = r * sin(theta);

        // calculate verticies for the relitive orbit
        theta = (2 * pi * t) / relitive.T;
        r = (relitive.a * (1 - (relitive.e * relitive.e)) / (1 - (relitive.e * cos(theta))));
        data.push_back(((r * cos(theta)) - arr[0])); data.push_back(((r * sin(theta)) - arr[1]));
    }
    line.vb_pos.UpdateData(data.data(), 2 * sizeof(float) * 1000);
    (*render_ptr).DrawLineStrip(line, 1000);

}

template<typename T>
void Ptolomys(Renderer* render_ptr, RendererObject& line, RendererObject& point, int index, int orbits, planet& centre, T& first) {
    glm::mat4 proj;
    float rad = 0.5;
    if (index == 8) { //if pluto
        proj = glm::ortho(-1 * (arr_a[index] + centre.a), arr_a[index] + centre.a + 20, -1 * (arr_b[index] + centre.b + 5), arr_b[index] + centre.b + 5);
    }
    else
    {
        if (index <= 3) // inner planet
        {
            float padding = (index / 10) + 0.5;
            rad = 0.1;

            proj = glm::ortho(-1 * (arr_a[index] + centre.a + padding), (arr_a[index] + centre.a + padding), -1 * (arr_b[index] + centre.b + padding), arr_b[index] + centre.b + padding);
        }
        else
        {
            float padding = (index)-2;
            proj = glm::ortho(-1 * (arr_a[index] + centre.a + padding), (arr_a[index] + centre.a + padding), -1 * (arr_b[index] + centre.b + padding), arr_b[index] + centre.b + padding);


        }
    }

    (*render_ptr).SetProjectionMatrix(proj, line, point);



    point.AddPosAtribute(layout);
    point.AddTexAtribute(Texlayout);
    point.shader_program.SetUniform4f("u_colour", centre.red, centre.green, centre.blue, 1.0f);

    point.vb_pos.UpdateData((*render_ptr).CircleCalc(0, 0, rad).data(), sizeof(float) * ((2 * 100) + 4));
    point.vb_tex.UpdateData((*render_ptr).CircleTex((*render_ptr).CircleCalc(0, 0, rad)).data(), sizeof(float) * ((2 * 100) + 4));


    (*render_ptr).DrawPoint(point, 100, &(centre.tex));


    line.AddPosAtribute(layout);

    PtolomyPlot(orbits, centre, first, render_ptr, line);
}

template<typename T, typename... Rest>
void Ptolomys(Renderer* render_ptr, RendererObject& line, RendererObject& point, int index, int orbits, planet& centre, T& first, Rest&... rest) {
    glm::mat4 proj;
    if (index == 8) { //if pluto
        proj = glm::ortho(-1 * (arr_a[index] + centre.a), arr_a[index] + centre.a + 20, -1 * (arr_b[index] + centre.b + 5), arr_b[index] + centre.b + 5);
    }
    else
    {
        if (index <= 3) // inner planet
        {
            float padding = (index / 10) + 0.2;

            proj = glm::ortho(-1 * (arr_a[index] + centre.a + padding), (arr_a[index] + centre.a + padding), -1 * (arr_b[index] + centre.b + padding), arr_b[index] + centre.b + padding);
        }
        else
        {
            float padding = (index)-2;
            proj = glm::ortho(-1 * (arr_a[index] + centre.a + padding), (arr_a[index] + centre.a + padding), -1 * (arr_b[index] + centre.b + padding), arr_b[index] + centre.b + padding);


        }
    }



    (*render_ptr).SetProjectionMatrix(proj, line);

    line.AddPosAtribute(layout);

    PtolomyPlot(orbits, centre, first, render_ptr, line);

    Ptolomys(render_ptr, line, point, index, orbits, centre, rest...);

}

std::string ParseText(const char* filepath) {
    std::ifstream stream(filepath); // parses in the file
    std::string line;

    std::stringstream buffer;
    buffer << stream.rdbuf(); // passes the file into the string stream buffer

    return buffer.str();

}



#endif

