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

using namespace undicht;
using namespace graphics;
using namespace user_input;
using namespace tools;
using namespace et1;



struct Knoten {

    double leitwert_self;
    std::vector<double> leitwerte_andere; // verbindende leitwerte zu den anderen knoten

};

double leitwert(double widerstand) {

    return 1.0f / widerstand;
}

template<unsigned int m, unsigned int n>
void printLinearEquations(std::array<std::array<double, n>, m> A, std::array<double, m> b) {

    for(int row = 0; row < m; row++) {

        for(int col = 0; col < n; col++) {

            std::cout << std::setw(8) << std::setprecision(3) << A[row][col];

        }

        std::cout << " | ";

        std::cout << b[row] << std::endl;

    }

    std::cout << std::endl;

}

template<unsigned int m, unsigned int n>
void swapRows(std::array<std::array<double, n>, m>& A, std::array<double, m>& b, int row1, int row2) {
    // swaps the rows 1 and 2 in A and b
    // a type of valid operation used in the gauss algorithm

    std::array<double, n> temp_row;
    double temp_b;

    temp_row = A[row1];
    temp_b = b[row1];

    A[row1] = A[row2];
    b[row1] = b[row2];

    A[row2] = temp_row;
    b[row2] = temp_b;


}

template<unsigned int m, unsigned int n>
void subtRows(std::array<std::array<double, n>, m>& A, std::array<double, m>& b, double f, int row1, int row2) {
    // subtracts row 1 from 2 (in A and b)
    // beforehand row 1 gets multiplied by f (temporary)
    // a type of valid operation used in the gauss algorithm

    for(int i = 0; i < n; i++) {
        // going through every factor in A

        A[row2][i] -= A[row1][i] * f;
    }

    b[row2] -= b[row1] * f;

}

template<unsigned int m, unsigned int n>
void scaleRow(std::array<std::array<double, n>, m>& A, std::array<double, m>& b, double f, int row) {
    // multiplies the row in A and b by f
    // a type of valid operation used in the gauss algorithm

    for(int i = 0; i < n; i++) {
        // going through every factor in the row

        A[row][i] *= f;
    }

    b[row] *= f;

}



template<unsigned int m, unsigned int n>
void solveLinearEquations(std::array<std::array<double, n>, m>& A, std::array<double, m>& b) {
    /** solves the system of linear equations Ax = b for x
    *   a11 * x1 + a12 * x2 + ... + a1n * xn = b1
    *   ...
    *   am1 * x1 + am2 * x2 + ... + amn * xn = bm
    * @param m: number of equations (rows)
    * @param n: number of unknown variables (columns) */

    // Gauss

    printLinearEquations<m,n>(A, b);

    for(int i = 0; i < std::min(n,m); i++) {
        // eliminating the ith unknown
        // in every row below the ith

        // making sure the ith row contains the variable
        for(int r = i; r < m; r++) {

            if(std::fabs(A[r][i]) > 0.00000001) {
                // the rth row contains the ith unknown

                //std::cout << "swapping " << r << " and " << i << "\n";
                swapRows<m,n>(A, b, r, i);
                break;
            }

        }

        // eleminating the ith unknown variable from the rows further down
        for(int r = i + 1; r < m; r++) {

            // the unknown was already eliminated from this row
            if(std::fabs(A[i][i]) < 0.00000001)
                break;

            double f = A[r][i] / A[i][i];

            //std::cout << "subtracting " << i << " * " << f << " from " << r << "\n";

            subtRows<m,n>(A,b, f, i, r);
        }

    }

    printLinearEquations<m,n>(A, b);


    // eliminating the known variables in the rows above
    for(int i = std::min(n,m) - 1; i >= 0; i--) {
        // eliminating the ith known

        for(int r = i - 1; r >= 0; r--) {
            // in the rth row

            // this unknown has been totally eliminated
            // its either equal to 0 or the system has no valid solution
            if(std::fabs(A[i][i]) < 0.00000001)
                break;

            // factor to multiply the ith row with to eleminate
            // its known variable in the rth row
            double f = A[r][i] / A[i][i];
            subtRows<m,n>(A, b, f, i, r);

        }


    }

    printLinearEquations<m,n>(A, b);

    // making sure every unknown has a 1 in front of it
    for(int i = 0; i < std::min(n,m); i++) {
        // there may be more rows than unknowns

        // this unknown has been totally eliminated
        // its either equal to 0 or the system has no valid solution
        if(std::fabs(A[i][i]) < 0.00000001)
            break;

        // the factor with which the row needs to be multiplied so that the unknown has a 1 in front of it
        double f = 1.0 / A[i][i];

        scaleRow<m,n>(A, b, f, i);

    }

    printLinearEquations<m,n>(A, b);

}




int main() {


    std::array<std::array<double, 3>,3> A = {{{ 3, 2,-1},
                                              { 2,-2, 4},
                                              {-1,0.5,-1},

                                           }};

    std::array<double,3> b = {1,-2,0};

    solveLinearEquations<3,3>(A,b);


    return 0;
}
