/*#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 QPushButton button1 ("test");
 QPushButton button2 ("other", &button1);

 button1.show();

 return app.exec();
}*/

#include <iostream>
#include "headers/board.hpp"
#include "headers/players.hpp"
#include "headers/pieces.hpp"
#include "headers/game.hpp"

int main(void){
    std::cout << "\n           ~ ✰ FAIRY CHESS ✰ ~\n";
    std::vector<Piece *> positions(64, nullptr);
    play(positions);
    return 0;
}