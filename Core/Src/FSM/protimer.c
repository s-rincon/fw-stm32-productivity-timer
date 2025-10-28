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

static void protimer_initial_transition(protimer_t * const mobj) {
    if (mobj == NULL) {
        return;
    }

    // Execute transition action
    mobj->worked_time = 0;

    // Transition to IDLE state
    mobj->active_state = PROTIMER_STATE_IDLE;

    // Optionally, you can dispatch an ENTRY event to the new state here
    protimer_event_t entry_event = { .signal = PROTIMER_SIGNAL_ENTRY };
    protimer_dispatch(mobj, &entry_event);
}

void protimer_dispatch(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return;
    }
}

void protimer_init(protimer_t * const mobj) {
    if (mobj == NULL) {
        return;
    }

    memset(mobj, 0, sizeof(protimer_t));

    protimer_initial_transition(mobj);
}