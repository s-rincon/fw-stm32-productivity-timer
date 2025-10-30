#include "alarm_simulation.h"

#include <stdint.h>
#include <stdbool.h>

#include "main.h"

#define ALARM_DURATION_MS           10000  // Total alarm duration in milliseconds (10 seconds)
#define ALARM_BEEP_DURATION_MS      800    // GPIO ON duration
#define ALARM_SILENCE_DURATION_MS   200    // GPIO OFF duration
#define ALARM_TOTAL_CYCLES          (ALARM_DURATION_MS / (ALARM_BEEP_DURATION_MS + ALARM_SILENCE_DURATION_MS))  // 10 cycles

typedef struct alarm_ {
    uint32_t current_tick;
    uint32_t cycles_left;
    bool beep_active;

} alarm_t;

static alarm_t alarm = {
    .beep_active = false,
    .current_tick = 0,
    .cycles_left = 0
};

void alarm_start(void) {
    alarm.beep_active = true;
    alarm.cycles_left = ALARM_TOTAL_CYCLES;
    alarm.current_tick = HAL_GetTick() + ALARM_BEEP_DURATION_MS;
    HAL_GPIO_WritePin(LED_USER_GPIO_Port, LED_USER_Pin, GPIO_PIN_SET);
}

void alarm_stop(void) {
    alarm.cycles_left = 0;
}

void alarm_task(void) {
    if (alarm.cycles_left == 0) {
        return;
    }

    uint32_t current_tick = HAL_GetTick();
    if (current_tick > alarm.current_tick) {
        if (alarm.beep_active) {
            // Finished beeping, switch to silence
            alarm.beep_active = false;
            alarm.current_tick = current_tick + ALARM_SILENCE_DURATION_MS;
            HAL_GPIO_WritePin(LED_USER_GPIO_Port, LED_USER_Pin, GPIO_PIN_RESET);
            alarm.cycles_left--;

        } else {
            // Finished silence, switch to beep
            alarm.beep_active = true;
            alarm.current_tick = current_tick + ALARM_BEEP_DURATION_MS;
            HAL_GPIO_WritePin(LED_USER_GPIO_Port, LED_USER_Pin, GPIO_PIN_SET);
            
        }
    }

}