#include "states/drogue_descent.h"

StateId State::DrogueDescent::getNewState(const StateInput &input,
                                          StateAuxilliaryInfo &) {
    int8_t num_checks = 0;
    if (input.altitude < M_MAIN_DEPLOY_ALTITUDE) {
        num_checks++;
    } else {
        num_checks = 0;
    }

    if (num_checks >= M_DEPLOY_CHECKS) {
        return StateId::MAIN_DESCENT;
    } else {
        return StateId::DROGUE_DESCENT;
    }
}
