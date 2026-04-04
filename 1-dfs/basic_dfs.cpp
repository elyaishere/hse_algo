#include "graph.h"
#include <cstdlib>
#include <ctime>

// Fills graph randomly
void Fill(Graph &gr) {
    std::srand(std::time({}));
    auto size = gr.Size();

    for (decltype(size) i = 0; i < size; ++i)
        for (decltype(size) j = i + 1; j < size; ++j)
            gr[i, j] = gr[j, i] = rand() % 2;
}

int main() {
    const size_t size = 5;
    Graph gr(size);

    // gr[0, 1] = 1, gr[1, 0] = 1;
    // gr[0, 2] = 1, gr[2, 0] = 1;
    // gr[1, 3] = 1, gr[3, 1] = 1;
    // gr[1, 4] = 1, gr[4, 1] = 1;

    Fill(gr);

    gr.PrintTree();
    std::cout << std::endl;
    gr.RemoveLoops();
    gr.FillVertexDepth();
    gr.PrintTree(true);
}
