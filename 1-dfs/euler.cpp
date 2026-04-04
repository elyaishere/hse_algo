#include "graph.h"

int main() {
    const size_t size = 6;
    Graph gr(size);

    gr[0, 2] = 1, gr[2, 0] = 1;
    gr[0, 3] = 1, gr[3, 0] = 1;
    gr[1, 2] = 1, gr[2, 1] = 1;
    gr[1, 3] = 1, gr[3, 1] = 1;
    gr[1, 4] = 1, gr[4, 1] = 1;
    gr[1, 5] = 1, gr[5, 1] = 1;
    gr[4, 5] = 1, gr[5, 4] = 1;

    // 0 - 2
    // |   |
    // 3 - 1
    //     | \
    //     5 - 4

    gr.PrintEulerCycle();
}
