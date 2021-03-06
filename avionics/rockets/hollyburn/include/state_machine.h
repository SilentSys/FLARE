#pragma once

#include <unordered_map> //for std::unordered_map (hash map)

#include "states/armed.h"
#include "states/ascent_to_apogee.h"
#include "states/drogue_descent.h"
#include "states/landed.h"
#include "states/mach_lock.h"
#include "states/main_descent.h"
#include "states/pressure_delay.h"
#include "states/standby.h"
#include "states/winter_contingency.h"

class StateMachine {
  private:
    /* Configuration constants */
    constexpr static int STANDBY_LAUNCH_CHECKS = 4;
    constexpr static int ARMED_LAUNCH_CHECKS = 4;
    constexpr static int MOTOR_BURNOUT_CHECKS = 4;
    constexpr static int MACH_LOCK_CHECKS = 10;
    constexpr static int MACH_UNLOCK_CHECKS = 10;
    constexpr static int APOGEE_CHECKS = 5;
    constexpr static int MAIN_DEPLOY_CHECKS = 5;

    constexpr static float LAUNCH_THRESHOLD = 25;                // m
    constexpr static float MACH_LOCK_VELOCITY_THRESHOLD = 155;   // m/s
    constexpr static float MACH_UNLOCK_VELOCITY_THRESHOLD = 150; // m/s
    constexpr static float MAIN_DEPLOY_ALTITUDE = 488;           // m == 1500 ft

    constexpr static long LAND_CHECK_TIME_INTERVAL = 10000; // ms
    constexpr static int LAND_CHECKS = 6;
    constexpr static float LAND_VELOCITY_THRESHOLD = 4;

    constexpr static long APOGEE_PRESSURE_DELAY = 3000;
    constexpr static long LANDING_TIME_INTERVAL = 10000;
    constexpr static long TOGGLE_CAMERA_INTERVAL = 200;

    /* States */
    State::Standby standby = State::Standby(
        StateId::ASCENT_TO_APOGEE, STANDBY_LAUNCH_CHECKS, LAUNCH_THRESHOLD);

    State::Armed armed = State::Armed(StateId::ASCENT_TO_APOGEE,
                                      ARMED_LAUNCH_CHECKS, LAUNCH_THRESHOLD);

    State::AscentToApogee coast = State::AscentToApogee(
        APOGEE_CHECKS, MACH_LOCK_CHECKS, MACH_LOCK_VELOCITY_THRESHOLD);

    State::MachLock mach_lock =
        State::MachLock(MACH_UNLOCK_CHECKS, MACH_UNLOCK_VELOCITY_THRESHOLD);

    State::PressureDelay pres_delay =
        State::PressureDelay(APOGEE_PRESSURE_DELAY);

    State::DrogueDescent drogue =
        State::DrogueDescent(MAIN_DEPLOY_ALTITUDE, MAIN_DEPLOY_CHECKS);

    State::MainDescent main = State::MainDescent(
        LAND_CHECK_TIME_INTERVAL, LAND_CHECKS, LAND_VELOCITY_THRESHOLD);

    State::Landed landed = State::Landed();

    State::WinterContingency contingency{};

    std::unordered_map<StateId, IState *> hash_map = {
        {StateId::STANDBY, &standby},
        {StateId::ARMED, &armed},
        {StateId::ASCENT_TO_APOGEE, &coast},
        {StateId::MACH_LOCK, &mach_lock},
        {StateId::PRESSURE_DELAY, &pres_delay},
        {StateId::DROGUE_DESCENT, &drogue},
        {StateId::MAIN_DESCENT, &main},
        {StateId::LANDED, &landed},
        {StateId::WINTER_CONTINGENCY, &contingency}};

    StateId current_state = StateId::STANDBY;

  public:
    StateMachine() : current_state(StateId::STANDBY) {}

    void update(const StateInput state_input, StateAuxilliaryInfo state_aux) {
        current_state =
            hash_map[current_state]->getNewState(state_input, state_aux);
    }

    /**
     * Arm the state machine. Only allowed if in STANDBY.
     * Returns true if arming was successful, false otherwise (i.e. not in
     * STANDBY)
     */
    bool arm() {
        if (current_state == StateId::STANDBY) {
            current_state = StateId::ARMED;
            return true;
        }
        return false;
    }

    /**
     * Disarm the state machine. Only allowed if in ARMED.
     * Returns true if disarming was successful, false otherwise (i.e. not in
     * ARMED)
     */
    bool disarm() {
        if (current_state == StateId::ARMED) {
            current_state = StateId::STANDBY;
            return true;
        }
        return false;
    }

    /**
     * Set the state machine to a failsafe state
     */
    void abort() { current_state = StateId::WINTER_CONTINGENCY; }

    const StateId &getState() const { return current_state; }
};
