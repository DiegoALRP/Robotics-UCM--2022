#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
//#include <wiringPISPI.h>
#include <softPwm.h>
#include <mcp3004.h>

#define RANGE		100
#define INPUT_PIN_1 5
#define INPUT_PIN_2 6

#define RUEDA_IZQ 4
#define RUEDA_DER 0

#define RUEDA_DER_ADELANTE 6
#define RUEDA_DER_ATRAS 20
#define RUEDA_IZQ_ADELANTE 20
#define RUEDA_IZQ_ATRAS 6

#define ADELANTE 0
#define DERECHA 1
#define IZQUIERDA 2

/*void inicializaValores() {

    wiringPiSetup();

    mcp3004Setup(100, 0);


    pinMode(INPUT_PIN_1, INPUT);
    pinMode(INPUT_PIN_2, INPUT);
    pinMode(RUEDA_DER, PWM_OUTPUT);
    pinMode(RUEDA_IZQ, PWM_OUTPUT);

    softPwmCreate(RUEDA_DER, 0, RANGE);
    softPwmCreate(RUEDA_IZQ, 0, RANGE);

    parar_ruedas();
}*/

void mueve_adelante() {

	softPwmWrite(RUEDA_DER, RUEDA_DER_ADELANTE);
	softPwmWrite(RUEDA_IZQ, RUEDA_IZQ_ADELANTE);
}

void gira_izquierda() {

	softPwmWrite(RUEDA_DER, RUEDA_DER_ADELANTE);
	softPwmWrite(RUEDA_IZQ, 0);
}

void gira_izquierda_atras() {

	softPwmWrite(RUEDA_DER, RUEDA_DER_ADELANTE);
	softPwmWrite(RUEDA_IZQ, RUEDA_IZQ_ATRAS);
}

void gira_derecha() {

	softPwmWrite(RUEDA_DER, 0);
	softPwmWrite(RUEDA_IZQ, RUEDA_IZQ_ADELANTE);
}

void gira_derecha_atras() {

	softPwmWrite(RUEDA_DER, RUEDA_DER_ATRAS);
	softPwmWrite(RUEDA_IZQ, RUEDA_IZQ_ADELANTE);
}

void mueve_atras() {

	softPwmWrite(RUEDA_DER, RUEDA_DER_ATRAS);
	softPwmWrite(RUEDA_IZQ, RUEDA_IZQ_ATRAS);
}

void parar_ruedas() {

	softPwmWrite(RUEDA_DER, 0);
	softPwmWrite(RUEDA_IZQ, 0);
}

void gira_derecha_90() {

	gira_derecha_atras();
	delay(700);
}

void gira_izquierda_90() {

	gira_izquierda_atras();
	delay(700);
}

int main ()
{
    int result_1;
    int result_2;
    int proximidad_1;
    int proximidad_2;

    wiringPiSetup();

    mcp3004Setup(100, 0);


    pinMode(INPUT_PIN_1, INPUT);
    pinMode(INPUT_PIN_2, INPUT);
    pinMode(RUEDA_DER, PWM_OUTPUT);
    pinMode(RUEDA_IZQ, PWM_OUTPUT);

    softPwmCreate(RUEDA_DER, 0, RANGE);
    softPwmCreate(RUEDA_IZQ, 0, RANGE);

    parar_ruedas();

    printf("Hola");

    int diff = 0;
    int countDer = 0;
    int countIzq = 0;
    int count = 0;

    for (;;) {

        //result_1 = digitalRead(INPUT_PIN_1);
        proximidad_1 = analogRead(100);
        proximidad_2 = analogRead(101);

        printf("\nproximidad 1: %d \n", proximidad_1);
        printf("proximidad 2: %d \n", proximidad_2);

        diff = proximidad_1*2 - proximidad_2*2;

	if (proximidad_1 > 500 && proximidad_2 > 500) {

	    mueve_atras();
	    delay(400);

	    if (rand() & 1) {

		gira_derecha_90();
	    }
	    else {
	    
	    	gira_izquierda_90();
	    }
	}
	else if ((proximidad_1 < 150 && proximidad_2 < 105) || count > 1) {
	
	    mueve_adelante();
	    count = 0;
	    
	    delay(200);
	}
        else if (diff < -100) {

            gira_izquierda_90();
            countIzq++;
            
            if (countDer == 1 && countIzq == 1) {
            
            	count++;
            	countDer = 0;
            }
            else {
            
            	count = 0;
            	countDer = 0;
            	countIzq = 0;
            }
        }
        else if (diff > 100) {

            gira_derecha_90();
            countDer++;
            
            if (countDer == 1 && countIzq == 1) {
            
            	count++;
            	countIzq = 0;
            }
            else {
            
            	count = 0;
            	countDer = 0;
            	countIzq = 0;
            }
        }
        else {

            mueve_adelante();
        }

        delay(200);
    }
}