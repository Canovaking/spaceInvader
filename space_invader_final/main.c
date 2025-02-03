#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "projectile.h"
#include "enemy.h"
#include "graphics.h"

#define LF_DW   6750
#define RG_UP   7500


uint8_t menu, game, level = 1;
uint16_t score = 0, highscore = 0;
volatile uint8_t count, lives, lose = 0, win = 0;

/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer;

Player player;

/* Statics */
const Timer_A_UpModeConfig upConfig = {
    TIMER_A_CLOCKSOURCE_ACLK,                   // 32kHz
    TIMER_A_CLOCKSOURCE_DIVIDER_12,             // divider by / 12 ~= 3kHz
    120,                                        // 120/3000 kHz ~= 40ms
    TIMER_A_TAIE_INTERRUPT_DISABLE,             // Disable Timer interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,         // Enable CCR0 interrupt
    TIMER_A_DO_CLEAR                            // Clear value
};

Timer_A_CompareModeConfig compareConfig_PWM = {
    TIMER_A_CAPTURECOMPARE_REGISTER_4,          // Use CCR3
    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
    TIMER_A_OUTPUTMODE_TOGGLE_SET,
    5000
};

void _delayTimerInit() {

    MAP_Timer_A_clearTimer(TIMER_A0_BASE);

    /* Initializing ACLK to LFXT (effectively 32khz) */
    MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Configuring Timer_A1 for Up Mode */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);

    /* Enabling interrupts and starting the timer */
    MAP_Interrupt_enableInterrupt(INT_TA0_0);
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

void _hwInit() {
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Configuring P1.1 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  // left
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);  // right
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableInterrupt(INT_PORT4);

    graphics_init();
    _delayTimerInit();

    /* Configures Pin 6.0 and 4.4 as ADC input */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
     * with internal 2.5v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}


/* Initializes all the game structure */
void _gameInit() {
    game = 1;
    menu = 0;

    count = 0;

    /* Initialize player */
    player_init(&player);

    /* Initialize the enemy manager */
    switch(level){
        case 1:
            // Initialize the enemy manager /
            enemy_manager_init(2, 5, 0); // 5 invaders per row, 2 rows
            break;
        case 2:
            // Initialize the enemy manager /
            enemy_manager_init(2, 5, 1); // 5 invaders per row, 2 rows
            break;
        case 3:
            // Initialize the enemy manager */
            enemy_manager_init(2, 5, 2); // 5 invaders per row, 2 rows
            break;
    }
    projectile_manager_init();

    Graphics_clearDisplay(&g_sContext);

    // Draw player
    graphics_draw_player(player.x, player.y);

    // Draw score
    graphics_update_screen(score,lives,level);
}


//screen wipe, then initialize the menu screen
void _menuInit() {
    menu = 1;
    game = 0;

    Graphics_clearDisplay(&g_sContext);

    score = 0;
    lives = 3;
    level = 1;

    graphics_init_menu();
}

int main(void) {

    /* Initialize hardware */
    _hwInit();

    /* generate seed for random function */
    srand( time(NULL) );
    //setvbuf(stdout, NULL, _IONBF, 0);  // Disable buffering
    _menuInit();

    while(1)
    {
        MAP_PCM_gotoLPM0();
        if (game) {
            // Move the enemies and update their positions
            enemy_manager_update();

            // Draw all active enemies and projectiles
            enemy_manager_draw();

            projectile_manager_update();

            projectile_manager_draw();
            // Check for collisions or if enemies have reached the player
            if (enemy_manager_has_reached_player()) {
                lives -=1;
                Graphics_clearDisplay(&g_sContext);
                _gameInit();

            }
            if(lives <= 0){
                graphics_lose();
                game = 0;
                lose = 1;
            }

            // Handle collision detection between bullets and enemies
            projectile_manager_check_collisions(&player);

            graphics_update_screen(score,lives,level);

            if(is_every_enemy_dead()){
                if(level==3){
                    graphics_win();
                    game = 0;
                    win = 1;
                }else{
                    level++;
                    Graphics_clearDisplay(&g_sContext);
                    _gameInit();
                }
            }
            // Add a delay to manage game speed
            __delay_cycles(4000000);

        }
    }
}

// left button
void PORT1_IRQHandler(void)
{
    //if button pressed, either start game, do a projectile or go back to the menu
    uint32_t status_p1;
    status_p1 = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

    if(menu && (status_p1 & GPIO_PIN1)) {
        _gameInit();
    }else{
        if((game==1) && (menu==0) && (status_p1 & GPIO_PIN1)) {
            projectile_manager_spawn(player.x + (PLAYER_WIDTH / 2), player.y - 3, PROJECTILE_TYPE_PLAYER);
        }else{
            if (!game && !menu &&(status_p1 & GPIO_PIN1)) {
                _menuInit();
            }
            if (lose){
                lose = 0;
                _menuInit();
            }
            if(win){
                win = 0;
                _menuInit();
            }
        }
    }


    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
}

// This is the TIMERA interrupt vector service routine.
void TA0_0_IRQHandler(void) {
    if (game) {
        count += 1;
    }
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void) {
    uint64_t status;

    /* Returns the status of a the ADC interrupt register masked with the
     * enabled interrupts. */
    status = MAP_ADC14_getEnabledInterruptStatus();
    /* Clears the indicated ADCC interrupt sources. */
    MAP_ADC14_clearInterruptFlag(status);

    // disable the interrupt to compute the results
    MAP_Interrupt_disableInterrupt(INT_ADC14);

    if (game && (lives > 0)) {
        // helper variables
        uint8_t x = player.x;
        uint8_t y = player.y;

        /* ADC_MEM1 conversion completed
         * if count>=1 means that one timer interrupt has occurred;
         * that means, that a delay has been waited */
        if(count >= 1 && (status & ADC_INT1)) {
            /* Returns the conversion result for the specified memory channel in
             * the format assigned by the ADC14_setResultFormat (unsigned binary
             * by default) function. Then stores the value in a buffer.*/
            resultsBuffer = ADC14_getResult(ADC_MEM0);

            /* go left */
            if (resultsBuffer <= LF_DW) {
                graphics_hide_player(x,y);
                x -= 1;
                player.x = x;
                player_move(&player,x,y);
                count = 0;
            }
            /* go right */
            else if (resultsBuffer >= RG_UP) {
                graphics_hide_player(x,y);
                x += 1;
                player.x = x;
                player_move(&player,x,y);
                count = 0;
            }
        }
    }
    // enable interrupt again
    MAP_Interrupt_enableInterrupt(INT_ADC14);
}
