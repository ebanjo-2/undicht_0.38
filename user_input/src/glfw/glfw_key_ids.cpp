#include "core.h"
#ifdef USE_GLFW

#include "key_ids.h"
#include "glfw_key_ids.h"
#include <GLFW/glfw3.h>

namespace undicht {

    namespace user_input {

        namespace glfw {

            void initKeyIds() {

                // letters
                UND_KEY_A = GLFW_KEY_A;
                UND_KEY_B = GLFW_KEY_B;
                UND_KEY_C = GLFW_KEY_C;
                UND_KEY_D = GLFW_KEY_D;
                UND_KEY_E = GLFW_KEY_E;
                UND_KEY_F = GLFW_KEY_F;
                UND_KEY_G = GLFW_KEY_G;
                UND_KEY_H = GLFW_KEY_H;
                UND_KEY_I = GLFW_KEY_I;
                UND_KEY_J = GLFW_KEY_J;
                UND_KEY_K = GLFW_KEY_K;
                UND_KEY_L = GLFW_KEY_L;
                UND_KEY_M = GLFW_KEY_M;
                UND_KEY_N = GLFW_KEY_N;
                UND_KEY_O = GLFW_KEY_O;
                UND_KEY_P = GLFW_KEY_P;
                UND_KEY_Q = GLFW_KEY_Q;
                UND_KEY_R = GLFW_KEY_R;
                UND_KEY_S = GLFW_KEY_S;
                UND_KEY_T = GLFW_KEY_T;
                UND_KEY_U = GLFW_KEY_U;
                UND_KEY_V = GLFW_KEY_V;
                UND_KEY_W = GLFW_KEY_W;
                UND_KEY_X = GLFW_KEY_X;
                UND_KEY_Y = GLFW_KEY_Y;
                UND_KEY_Z = GLFW_KEY_Z;

                UND_KEY_LSHIFT = GLFW_KEY_LEFT_SHIFT;
                UND_KEY_SPACE = GLFW_KEY_SPACE;

            }

        } // glfw

    } // user_input

} // undicht

#endif // USE_GLFW
