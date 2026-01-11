#include <trickyLib/tricky.hpp>

int main(int argc, char *argv[]) {

    Tricky::Tablero triky;

    Tricky::Player tanuk{"Tanuk"};
    Tricky::Player player2{"Player2"};

    triky.setPlayer({tanuk, player2});

    triky.start();

    return 0;
}
