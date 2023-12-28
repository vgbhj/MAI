#include "dag.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.yaml>" << std::endl;
        return 1;
    }

    DAG_J dagj;
    dagj.createDAG(argv[1]);
    dagj.runCompile();
    return 0;
}
