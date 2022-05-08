#include "Matrix.h"
#include "Structs.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Need file name" << std::endl;
        exit(1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream myfile;
    myfile.open(argv[1]);
    if (!myfile.is_open())
    {
        std::cout << "file not opened" << std::endl;
        exit(1);
    }
    // std::stringstream ss(myfile.rdbuf());
    // myfile.close();

    int num_ver, num_edge, num_triangle;
    myfile >> num_ver >> num_edge >> num_triangle;
    std::vector<std::array<float, 3>> vertices;
    for (int i = 0; i < num_ver; i++)
    {
        std::array<float, 3> buf;
        for (int j = 0; j < 3; j++)
        {
            myfile >> buf[j];
        }
        vertices.push_back(buf);
    }

    std::vector<std::array<int, 2>> edges;
    for (int i = 0; i < num_edge; i++)
    {
        std::array<int, 2> buf;
        for (int j = 0; j < 2; j++)
        {
            myfile >> buf[j];
            buf[j]--;
        }
        if (buf[0] > buf[1])
            std::swap(buf[0], buf[1]);
        edges.push_back(buf);
    }

    std::vector<std::array<int, 3>> triangles;
    for (int i = 0; i < num_triangle; i++)
    {
        std::array<int, 3> buf;
        for (int j = 0; j < 3; j++)
        {
            myfile >> buf[j];
            buf[j]--;
        }
        triangles.push_back(buf);
    }

    boundary_matrix bm = make_boundary(edges, triangles);
    Matrix matrix(bm.num_rows, bm.num_cols, 0);
    matrix.populate(bm.num_rows, bm.num_cols, bm.mat);
    // matrix.print();
    std::cout << bm.num_cols - matrix.get_rank() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time Taken is = " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    // matrix.print();
    // Matrix *ker_space = matrix.get_ker();
    // ker_space->print();
    // int **array;
    // array = new int *[3];
    // for (int i = 0; i < 3; i++)
    //     array[i] = new int[5];
}

struct boundary_matrix make_boundary(std::vector<std::array<int, 2>> edges, std::vector<std::array<int, 3>> triangles)
{
    boundary_matrix bm = {edges.size(), triangles.size(), new int *[edges.size()]};
    for (int i = 0; i < edges.size(); i++)
    {
        bm.mat[i] = new int[triangles.size()];
    }

    for (int i = 0; i < edges.size(); i++)
    {
        for (int j = 0; j < triangles.size(); j++)
        {
            bm.mat[i][j] = 0;
        }
    }

    for (int i = 0; i < triangles.size(); i++)
    {
        std::array<std::array<int, 2>, 3> old_edges;
        old_edges[0] = edges[triangles[i][0]];
        old_edges[1] = edges[triangles[i][1]];
        old_edges[2] = edges[triangles[i][2]];
        std::array<int, 3> ver;
        ver[0] = old_edges[0][0];
        ver[1] = old_edges[0][1];
        if (ver[0] == old_edges[1][0] || ver[1] == old_edges[1][0])
        {
            ver[2] = old_edges[1][1];
        }
        else
        {
            ver[2] = old_edges[1][0];
        }

        std::array<std::array<int, 2>, 3> new_edges;
        new_edges[0] = {ver[0], ver[1]};
        new_edges[1] = {ver[1], ver[2]};
        new_edges[2] = {ver[2], ver[0]};

        for (auto &new_edge : new_edges)
        {
            int j = 0;
            for (auto &old_edge : old_edges)
            {
                if (new_edge[0] == old_edge[0])
                {
                    if (new_edge[1] == old_edge[1])
                    {
                        bm.mat[triangles[i][j]][i] = 1;
                    }
                }
                else if (new_edge[0] == old_edge[1])
                {
                    if (new_edge[1] == old_edge[0])
                    {
                        bm.mat[triangles[i][j]][i] = -1;
                    }
                }
                j++;
            }
        }
        // if (e1[0] == e2[1] || e1[1] == e2[0])
        // {
        //     bm.mat[triangles[i][0]][i] = 1;
        //     bm.mat[triangles[i][1]][i] = 1;
        //     bm.mat[triangles[i][2]][i] = -1;
        // }
        // if (e1[0] == e3[1] || e1[1] == e3[0])
        // {
        //     bm.mat[triangles[i][0]][i] = 1;
        //     bm.mat[triangles[i][1]][i] = -1;
        //     bm.mat[triangles[i][2]][i] = 1;
        // }
        // if (e2[0] == e3[1] || e2[1] == e3[0])
        // {
        //     bm.mat[triangles[i][0]][i] = -1;
        //     bm.mat[triangles[i][1]][i] = 1;
        //     bm.mat[triangles[i][2]][i] = 1;
        // }
    }
    return bm;
}