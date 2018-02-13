#include "maze.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Maze::Maze()
{}

Maze::Maze(const std::vector<std::vector<int> > &map):
    m_map{map}, m_path_map{map}
{}

void Maze::load_map_from_file(const std::string &path_to_file,
                        std::pair<int,int> &start,
                        std::pair<int,int> &end)
{
    std::ifstream fi(path_to_file);
    if(fi.is_open())
    {
        m_map.clear();
        int y{0};
        while(!fi.eof())
        {
            std::vector<int> row;
            std::string line;
            std::getline(fi, line);
            for(size_t x{0}; x < line.size(); ++x)
            {
                auto ch = line[x];
                if(ch == ' ' || ch == 'S' || ch == 'E')
                {
                    row.push_back(EMPTY);
                    if(ch == 'S')
                        start = {x, y};
                    else if(ch == 'E')
                        end = {x, y};
                }
                else if(ch == 'X')
                {
                    row.push_back(WALL);
                }
            }
            m_map.push_back(row);
            ++y;
        }
        m_path_map = m_map;
        m_has_map_changed = true;
        fi.close();
    }
}

void Maze::set_map(const std::vector<std::vector<int> > &map)
{
    m_map = map;
    m_has_map_changed = true;
}

void Maze::set_neighbourhood_type(Maze::NeibourhoodType nt)
{
    m_neighbourhood_type = nt;
    m_has_map_changed = true;
}

bool Maze::solve_wave_algorithm(std::pair<int, int> start,
                                    std::pair<int, int> target,
                                    std::vector<std::pair<int, int> > &path,
                                    void(*display)())
{
    std::vector<std::pair<int,int> > old_wave;

    const int dx[] {1, -1, 0, 0, 1, -1, -1, 1};
    const int dy[] {0, 0, 1, -1, 1, 1, -1, -1};
    bool is_path_founded {true};
    int step {0};
    m_path_map[start.second][start.first] = step;
    old_wave.push_back(start);
    while(old_wave.size() > 0)
    {
        ++step;
        m_wave_curr.clear();
        for(auto it = old_wave.begin(); it != old_wave.end(); ++it)
        {
            for(int dir{0}; dir < m_neighbourhood_type; ++dir)
            {
                auto nx = it->first + dx[dir];
                auto ny = it->second + dy[dir];

                //TODO add checking if array indexes are out of bounds
                if(is_valid_pos(nx,ny) && m_path_map[ny][nx] == EMPTY)
                {
                   m_wave_curr.push_back({nx,ny});
                   m_path_map[ny][nx] = step;
                   display();
                   if(nx == target.first && ny == target.second)
                   {
                       is_path_founded = true;
                       goto done;
                   }
                }
            }
        }
        old_wave = m_wave_curr;
    }
    done:
    if(is_path_founded)
    {
        m_wave_curr.clear();
        path.clear();
        auto curr_pos = target;
        path.push_back(curr_pos);
        while(curr_pos != start)
        {
            for(int dir = 0; dir < m_neighbourhood_type; ++dir)
            {
                auto nx = curr_pos.first + dx[dir];
                auto ny = curr_pos.second + dy[dir];

                if(is_valid_pos(nx,ny) && m_path_map[ny][nx] != WALL &&
                   m_path_map[curr_pos.second][curr_pos.first] - m_path_map[ny][nx] == 1)
                {                    
                    path.push_back({nx,ny});
                    curr_pos = {nx,ny};
                    break;
                }
            }
        }
        path.push_back(curr_pos);
        std::reverse(path.begin(), path.end());
    }
    return is_path_founded;
}

bool Maze::is_valid_pos(int row, int col) const noexcept
{
    return !m_map.empty() && row >= 0 &&
           row < int(m_map.size()) &&
           col >= 0 && int(m_map[row].size());
}
