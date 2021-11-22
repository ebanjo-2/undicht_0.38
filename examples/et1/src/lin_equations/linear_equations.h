#ifndef LINEAR_EQUATIONS_H
#define LINEAR_EQUATIONS_H

#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>

namespace et1 {

    class LinearSystem {
        // looks like this: Ax = b
        // were A and b are known (A is a matrix, b a vector of same height)
        // and x is an unknown vector

        public:

            unsigned int rows = 0; // number of equations / height of matrix A
            unsigned int cols = 0; // number of unknowns / width of matrix A

            double* A = 0; // rows * cols matrix
            double* b = 0; // vector with rows entries

        public:


            LinearSystem(int m, int n) {
                /** @param m: number of equations (rows)
                * @param n: number of unknown variables (columns) */
                rows = m;
                cols = n;

                A = new double[rows * cols];
                b = new double[rows];

            }

            virtual ~LinearSystem() {

                delete[] A;
                delete[] b;

            }

            void setRow(int m, const std::vector<double>& values, double b_value) {

                for(int i = 0; i < cols;i++){

                    getARow(m)[i] = values[i];
                }

                b[m] = b_value;
            }

            double* getARow(int m) {

                return A + (m * cols);
            }

    };

    void printLinearEquations(LinearSystem& ls) {

        for(int row = 0; row < ls.rows; row++) {

            for(int col = 0; col < ls.cols; col++) {

                std::cout << std::setw(8) << std::setprecision(3) << ls.getARow(row)[col];

            }

            std::cout << " | ";

            std::cout << ls.b[row] << std::endl;

        }

        std::cout << std::endl;

    }

    void swapRows(LinearSystem& ls, int row1, int row2) {
        // swaps the rows 1 and 2 in A and b
        // a type of valid operation used in the gauss algorithm

        double* temp_row = new double[ls.cols];
        double temp_b;

        memcpy(temp_row, ls.getARow(row1), ls.cols);
        temp_b = ls.b[row1];

        memcpy(ls.getARow(row1), ls.getARow(row2), ls.cols);
        ls.b[row1] = ls.b[row2];

        memcpy(ls.getARow(row2), temp_row, ls.cols);
        ls.b[row2] = temp_b;

        delete[] temp_row;
    }


    void subtRows(LinearSystem&ls, double f, int row1, int row2) {
        // subtracts row 1 from 2 (in A and b)
        // beforehand row 1 gets multiplied by f (temporary)
        // a type of valid operation used in the gauss algorithm

        for(int i = 0; i < ls.cols; i++) {
            // going through every factor in A

            ls.getARow(row2)[i] -= ls.getARow(row1)[i] * f;
        }

        ls.b[row2] -= ls.b[row1] * f;

    }

    void scaleRow(LinearSystem& ls, double f, int row) {
        // multiplies the row in A and b by f
        // a type of valid operation used in the gauss algorithm

        for(int i = 0; i < ls.cols; i++) {
            // going through every factor in the row

            ls.getARow(row)[i] *= f;
        }

        ls.b[row] *= f;

    }



    void solveLinearEquations(LinearSystem& ls) {
        /** solves the system of linear equations Ax = b for x
        * n = number of unknown variables, m = number of equations
        *   a11 * x1 + a12 * x2 + ... + a1n * xn = b1
        *   ...
        *   am1 * x1 + am2 * x2 + ... + amn * xn = bm */

        // Gauss

        printLinearEquations(ls);

        for(int i = 0; i < std::min(ls.cols, ls.rows); i++) {
            // eliminating the ith unknown
            // in every row below the ith

            // making sure the ith row contains the variable
            for(int r = i; r < ls.rows; r++) {

                if(std::fabs(ls.getARow(r)[i]) > 0.00000001) {
                    // the rth row contains the ith unknown

                    //std::cout << "swapping " << r << " and " << i << "\n";
                    swapRows(ls, r, i);
                    break;
                }

            }

            // eleminating the ith unknown variable from the rows further down
            for(int r = i + 1; r < ls.rows; r++) {

                // the unknown was already eliminated from this row
                if(std::fabs(ls.getARow(i)[i]) < 0.00000001)
                    break;

                double f = ls.getARow(r)[i] / ls.getARow(i)[i];

                //std::cout << "subtracting " << i << " * " << f << " from " << r << "\n";

                subtRows(ls, f, i, r);
            }

        }

        printLinearEquations(ls);


        // eliminating the known variables in the rows above
        for(int i = std::min(ls.cols, ls.rows) - 1; i >= 0; i--) {
            // eliminating the ith known

            for(int r = i - 1; r >= 0; r--) {
                // in the rth row

                // this unknown has been totally eliminated
                // its either equal to 0 or the system has no valid solution
                if(std::fabs(ls.getARow(i)[i]) < 0.00000001)
                    break;

                // factor to multiply the ith row with to eleminate
                // its known variable in the rth row
                double f = ls.getARow(r)[i] / ls.getARow(i)[i];
                subtRows(ls, f, i, r);

            }


        }

        printLinearEquations(ls);

        // making sure every unknown has a 1 in front of it
        for(int i = 0; i < std::min(ls.cols, ls.rows); i++) {
            // there may be more rows than unknowns

            // this unknown has been totally eliminated
            // its either equal to 0 or the system has no valid solution
            if(std::fabs(ls.getARow(i)[i]) < 0.00000001)
                break;

            // the factor with which the row needs to be multiplied so that the unknown has a 1 in front of it
            double f = 1.0 / ls.getARow(i)[i];

            scaleRow(ls, f, i);

        }

        printLinearEquations(ls);

    }


} // et1

#endif // LINEAR_EQUATIONS_H
