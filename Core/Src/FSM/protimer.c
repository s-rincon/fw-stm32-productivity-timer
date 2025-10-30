#include "protimer.h"

#include <string.h>

#include "lcd_simulation.h"
#include "alarm_simulation.h"
#include "main.h"

static void protimer_set_active_state(protimer_t * const mobj, protimer_state_t state) {
    if (mobj == NULL) {
        return;
    }

    mobj->active_state = state;
}

static protimer_event_status_t protimer_idle_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return PROTIMER_EVENT_IGNORED;
    }

    switch (evt->signal) {
        case PROTIMER_SIGNAL_ENTRY:
            mobj->current_time = 0;
            mobj->elapsed_time = 0;
            display_time(0);
            display_message("Set Time");
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_EXIT:
            alarm_stop();
            display_clear();
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_INC_TIME:
            mobj->current_time += 60;
            protimer_set_active_state(mobj, PROTIMER_STATE_TIME_SET);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_START_PAUSE:
            protimer_set_active_state(mobj, PROTIMER_STATE_STATS);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_TIME_TICK:
            // \todo implement tick
            return PROTIMER_EVENT_HANDLED;

        default:
            return PROTIMER_EVENT_IGNORED;
    }
}

static protimer_event_status_t protimer_time_set_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return PROTIMER_EVENT_IGNORED;
    }

    switch (evt->signal) {
        case PROTIMER_SIGNAL_ENTRY:
            display_message("Set Time");
            display_time(mobj->current_time);
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_EXIT:
            display_clear();
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_INC_TIME:
            mobj->current_time += 60;
            display_time(mobj->current_time);
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_DEC_TIME:
            if (mobj->current_time >= 60) {
                mobj->current_time -= 60;
                display_time(mobj->current_time);
                display_show();
                return PROTIMER_EVENT_HANDLED;
            }
            return PROTIMER_EVENT_IGNORED;

        case PROTIMER_SIGNAL_START_PAUSE:
            if (mobj->current_time > 0) {
                protimer_set_active_state(mobj, PROTIMER_STATE_COUNTDOWN);
                return PROTIMER_EVENT_TRANSITION;
            }
            return PROTIMER_EVENT_IGNORED;

        case PROTIMER_SIGNAL_ABORT:
            protimer_set_active_state(mobj, PROTIMER_STATE_IDLE);
            return PROTIMER_EVENT_TRANSITION;

        default:
            return PROTIMER_EVENT_IGNORED;
    }
    return PROTIMER_EVENT_IGNORED;
}

static protimer_event_status_t protimer_countdown_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return PROTIMER_EVENT_IGNORED;
    }

    switch (evt->signal) {
        case PROTIMER_SIGNAL_ENTRY:
            display_message("Working...");
            display_time(mobj->current_time);
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_EXIT:
            mobj->worked_time += mobj->elapsed_time;
            mobj->elapsed_time = 0;
            display_clear();
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_ABORT:
            protimer_set_active_state(mobj, PROTIMER_STATE_IDLE);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_START_PAUSE:
            protimer_set_active_state(mobj, PROTIMER_STATE_PAUSE);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_TIME_TICK:
            static uint8_t time_tick_ss = 0;
            if (++time_tick_ss < 10) {
                return PROTIMER_EVENT_IGNORED;
            }

            time_tick_ss = 0;
            mobj->elapsed_time++;
            mobj->current_time--;

            if (mobj->current_time == 0) {
                alarm_start();
                protimer_set_active_state(mobj, PROTIMER_STATE_IDLE);
                return PROTIMER_EVENT_TRANSITION;

            } else {
                display_time(mobj->current_time);
                display_show();
                return PROTIMER_EVENT_HANDLED;
            }

        default:
            return PROTIMER_EVENT_IGNORED;
    }
}

static protimer_event_status_t protimer_pause_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return PROTIMER_EVENT_IGNORED;
    }

    switch (evt->signal) {
        case PROTIMER_SIGNAL_ENTRY:
            display_message("Paused");
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_EXIT:
            display_clear();
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_START_PAUSE:
            protimer_set_active_state(mobj, PROTIMER_STATE_COUNTDOWN);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_ABORT:
            protimer_set_active_state(mobj, PROTIMER_STATE_IDLE);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_INC_TIME:
            mobj->current_time += 60;
            protimer_set_active_state(mobj, PROTIMER_STATE_TIME_SET);
            return PROTIMER_EVENT_TRANSITION;

        case PROTIMER_SIGNAL_DEC_TIME:
            if (mobj->current_time >= 60) {
                mobj->current_time -= 60;
                protimer_set_active_state(mobj, PROTIMER_STATE_TIME_SET);
                return PROTIMER_EVENT_TRANSITION;
            }
            return PROTIMER_EVENT_IGNORED;

        default:
            return PROTIMER_EVENT_IGNORED;
    }
}

static protimer_event_status_t protimer_stats_state_handler(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return PROTIMER_EVENT_IGNORED;
    }

    switch (evt->signal) {
        case PROTIMER_SIGNAL_ENTRY:
            display_time(mobj->worked_time);
            display_message("Worked Time:");
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_EXIT:
            display_clear();
            display_show();
            return PROTIMER_EVENT_HANDLED;

        case PROTIMER_SIGNAL_TIME_TICK:
            static uint8_t time_tick_ss = 0;
            if (++time_tick_ss >= 30) {
                time_tick_ss = 0;
                protimer_set_active_state(mobj, PROTIMER_STATE_IDLE);
                return PROTIMER_EVENT_TRANSITION;
            }
            return PROTIMER_EVENT_IGNORED;

        default:
            return PROTIMER_EVENT_IGNORED;
    }
}

static protimer_event_status_t protimer_state_machine(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return PROTIMER_EVENT_IGNORED;
    }

    switch (mobj->active_state) {
        case PROTIMER_STATE_IDLE:
            return protimer_idle_state_handler(mobj, evt);

        case PROTIMER_STATE_TIME_SET:
            return protimer_time_set_state_handler(mobj, evt);

        case PROTIMER_STATE_COUNTDOWN:
            return protimer_countdown_state_handler(mobj, evt);

        case PROTIMER_STATE_PAUSE:
            return protimer_pause_state_handler(mobj, evt);

        case PROTIMER_STATE_STATS:
            return protimer_stats_state_handler(mobj, evt);

        default:
            // Unknown state, reset to IDLE
            mobj->active_state = PROTIMER_STATE_IDLE;
            return PROTIMER_EVENT_IGNORED;
    }
    
}

/**
 * @brief Perform the initial transition of the state machine
 * 
 * Executes the initial transition from the implicit start state to the IDLE state.
 * This includes executing transition actions and dispatching the entry event to 
 * the initial state (IDLE).
 * 
 * @param[in,out] mobj Pointer to the protimer object
 * 
 * @details Transition sequence:
 *          1. Execute transition action (initialize worked_time to 0)
 *          2. Set active_state to PROTIMER_STATE_IDLE
 *          3. Dispatch ENTRY event to IDLE state
 */
static void protimer_initial_transition(protimer_t * const mobj) {
    if (mobj == NULL) {
        return;
    }

    // Execute transition action
    mobj->worked_time = 0;

    // Transition to IDLE state
    protimer_set_active_state(mobj, PROTIMER_STATE_IDLE);

    // Optionally, you can dispatch an ENTRY event to the new state here
    protimer_event_t entry_event = { .signal = PROTIMER_SIGNAL_ENTRY };
    protimer_dispatcher(mobj, &entry_event);
}

void protimer_dispatcher(protimer_t * const mobj, const protimer_event_t * const evt) {
    if ((mobj == NULL) || (evt == NULL)) {
        return;
    }

    protimer_state_t source_state = mobj->active_state;
    protimer_event_status_t status = protimer_state_machine(mobj, evt);
    protimer_state_t target_state = mobj->active_state;

    if (status == PROTIMER_EVENT_TRANSITION) {
        protimer_event_t exit_event = { .signal = PROTIMER_SIGNAL_EXIT };
        mobj->active_state = source_state;
        protimer_state_machine(mobj, &exit_event);

        protimer_event_t entry_event = { .signal = PROTIMER_SIGNAL_ENTRY };
        mobj->active_state = target_state;
        protimer_state_machine(mobj, &entry_event);
    }
}

void protimer_init(protimer_t * const mobj) {
    if (mobj == NULL) {
        return;
    }

    memset(mobj, 0, sizeof(protimer_t));

    protimer_initial_transition(mobj);
}
