#include "application.hpp"

int main(int argc, char *argv[]) {
    auto app = Application::get_instance(argc, argv);
    app->run();
    return 0;
}
