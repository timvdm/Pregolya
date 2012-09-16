#include "../src/dfs.h"

using namespace Pregolya;

int main()
{
  Graph g;
  GraphMutator m(&g);
  m.construct(8, {{0, 1}, {1, 2}, {1, 3}, {2, 4}, {3, 5}, {4, 6}, {5, 6}, {6, 7}});

}
