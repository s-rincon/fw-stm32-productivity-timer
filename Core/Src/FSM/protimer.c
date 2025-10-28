#include "protimer.h"

static void protimer_idle_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {

}

static void protimer_time_set_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {

}

static void protimer_countdown_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {

}

static void protimer_pause_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {

}

static void protimer_stats_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {

}

static void protimer_state_machine(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return;
    }

    switch (mobj->active_state) {
        case PROTIMER_STATE_IDLE:
            protimer_idle_state_handler(mobj, evt);
            break;

        case PROTIMER_STATE_TIME_SET:
            protimer_time_set_state_handler(mobj, evt);
            break;

        case PROTIMER_STATE_COUNTDOWN:
            protimer_countdown_state_handler(mobj, evt);
            break;

        case PROTIMER_STATE_PAUSE:
            protimer_pause_state_handler(mobj, evt);
            break;

        case PROTIMER_STATE_STATS:
            protimer_stats_state_handler(mobj, evt);
            break;

        default:
            // Unknown state, reset to IDLE
            mobj->active_state = PROTIMER_STATE_IDLE;
            break;
    }
    
}

void protimer_dispatch(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return;
    }
    // Dispatch the event to the state machine and handle transition if any
}

void protimer_init(protimer_t * const mobj) {
    if (mobj == NULL) {
        return;
    }

    // Initialize the protimer and handle initial transition to IDLE state
}