#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>

class Maze
{
public:
    enum { WALL = -1, EMPTY = 9999 };
    enum NeibourhoodType { von_Neumann = 4, Moore = 8 };
    explicit Maze();
    explicit Maze(const std::vector<std::vector<int> > &map);
    void load_map_from_file(const std::string &path_to_file,
                            std::pair<int,int> &start,
                            std::pair<int,int> &end);
    inline auto width() const noexcept { return !m_map.empty() ? m_map[0].size(): 0; }
    inline auto height() const noexcept { return m_map.size(); }
    inline int path_map_at(int x, int y) const noexcept
    {
        return is_valid_pos(x,y) ? m_path_map[y][x] : WALL;
    }
    inline const std::vector<std::pair<int,int> >& get_curr_wave() const noexcept
    {
        return m_wave_curr;
    }
    void set_map(const std::vector<std::vector<int> > &map);
    void set_neighbourhood_type(NeibourhoodType nt);
    inline NeibourhoodType get_neighbourhood_type() const noexcept
    { return m_neighbourhood_type; }
    bool solve_wave_algorithm(std::pair<int,int> start,
                              std::pair<int,int> target,
                              std::vector<std::pair<int,int> > &path,
                              void(*display)());
private:
    std::vector<std::vector<int> > m_map;
    std::vector<std::vector<int> > m_path_map;
    std::vector<std::vector<std::pair<int,int> > > m_prev;
    std::vector<std::pair<int,int> > m_wave_curr;
    NeibourhoodType m_neighbourhood_type { von_Neumann };
    bool m_has_map_changed {true};

    bool is_valid_pos(int row, int col) const noexcept;
};

#endif // MAZE_H
