#include <iostream>
#include <array>
#include <iomanip>

#include <window/window.h>

#include <font_rendering/font_renderer.h>
#include <vector_graphics/vg_renderer.h>

#include <user_interface/button.h>
#include <user_interface/ui_base.h>

#include <mouse_input.h>
#include <key_input.h>

#include <lin_equations/linear_equations.h>

using namespace undicht;
using namespace graphics;
using namespace user_input;
using namespace tools;
using namespace et1;



struct Knoten {

    int id;

};

struct Verbindung {
    // verbindet zwei knoten

    double widerstand = 1;
    int knoten1 = 0;
    int knoten2 = 0;

};

struct Stromquelle {
    // stromquelle zwischen zwei punkten
    // jede reale stromquelle hat einen innenwiderstand parallel zu ihr
    // strom mit positivem vorzeichen fließt in der richtung knt1 -> knt2

    double widerstand = 1;
    double strom = 0; // der strom der ohne weitere last durch den innenwiderstand fließt
    int knoten1 = 0;
    int knoten2 = 0;
};

double leitwert(double widerstand) {

    return 1.0 / widerstand;
}



int main() {

    std::vector<Knoten> knoten;
    knoten.push_back({0});
    knoten.push_back({1});

    std::vector<Verbindung> verbindungen;
    verbindungen.push_back({100,0,1}); // 100 ohm widerstand zwischen knt 0 und 1

    std::vector<Stromquelle> stromquellen;
    stromquellen.push_back({100, 1,0,1});

    LinearSystem gleichungen(3,3);
    gleichungen.setRow(0, {1,1,1}, 6);
    gleichungen.setRow(1, {-15,-9,-12}, -54);
    gleichungen.setRow(2, {0,0,2}, 24);

    solveLinearEquations(gleichungen);


    return 0;
}
