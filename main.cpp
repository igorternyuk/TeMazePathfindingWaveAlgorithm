#include <iostream>
#include "maze.hpp"
#include <GL/glut.h>
#include <algorithm>

Maze maze;
std::pair<int,int> start,end;
bool path_founded {false};
std::vector<std::pair<int,int> > path;
const int cell_size {20};

void display();

void timer(int i = 0);

int main(int argc, char *argv[])
{
    if(argc >= 1)
    {
        maze.load_map_from_file(argv[1],start,end);
    }
    else
    {
        std::cout << "Pass input file with maze as command line parameter" << std::endl;
        return 0;
    }
    const int window_width = maze.width() * cell_size;
    const int window_height = maze.height() * cell_size;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Wave algorithm");
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,window_width,window_height,0,-1,1);
    glutDisplayFunc(display);
    glutTimerFunc(300, timer, 0);
    glutMainLoop();
    return 0;
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    //This piece of code draws the maze
    for(size_t y{0}; y < maze.height(); ++y)
    {
        for(size_t x{0}; x < maze.width(); ++x)
        {
            if(maze.path_map_at(x,y) == Maze::WALL)
                glColor3f(1,1,1);
            else if(maze.path_map_at(x,y) == Maze::EMPTY)
                glColor3f(0,0,0);
            else
                glColor3f(maze.path_map_at(x,y) / 24.f,0,0);
            glVertex2f(x * cell_size, y * cell_size);
            glVertex2f((x + 1) * cell_size, y * cell_size);
            glVertex2f((x + 1) * cell_size, (y + 1) * cell_size);
            glVertex2f(x * cell_size, (y + 1) * cell_size);
        }
    }
    //This lines of code draw current wave
    glColor3f(0,1.0f,0.5f);

    auto wave = maze.get_curr_wave();
    for(auto it = wave.begin(); it != wave.end(); ++it)
    {
        auto x = it->first;
        auto y = it->second;
        glVertex2f(x * cell_size, y * cell_size);
        glVertex2f((x + 1) * cell_size, y * cell_size);
        glVertex2f((x + 1) * cell_size, (y + 1) * cell_size);
        glVertex2f(x * cell_size, (y + 1) * cell_size);
    }
    //This lines of code draw requiered path if exists
    if(path_founded)
    {
        glColor3f(0,0.85f,0);
        for(auto it = path.begin(); it != path.end(); ++it)
        {
            auto x = it->first;
            auto y = it->second;
            glVertex2f(x * cell_size, y * cell_size);
            glVertex2f((x + 1) * cell_size, y * cell_size);
            glVertex2f((x + 1) * cell_size, (y + 1) * cell_size);
            glVertex2f(x * cell_size, (y + 1) * cell_size);
        }
    }
    glEnd();
    glutSwapBuffers();
}

void timer(int)
{
    path_founded = maze.solve_wave_algorithm(start, end, path, display);
    if(path_founded)
    {
        auto str = std::string("Distance:") + std::to_string(path.size());
        glutSetWindowTitle(str.c_str());
    }
    else
    {
        glutSetWindowTitle("Path not found");
    }
    display();
}
