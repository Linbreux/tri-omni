/* ****************************************************************** */
/*  FILE : motorcontrol.c                                             */
/*                                                                    */
/* Outputs:                                                           */
/*  Pin 11, 13 and 15 	to control motor1 with hardware PWM0.         */
/*  Pin 08, 10 and 12 	to control motor2 with hardware PWM1.         */
/*  Pin 19, 21 and 23 	to control motor3 with hardware PWM2.         */
/*       ^ < - PWM on                                                 */
/*                                                                    */
/*     help:  https://pinout.xyz/pinout/wiringpi                      */
/*                                                                    */
/*  Date: 02/03/2021              Olivier Lenaerts- Brecht Gijsens    */
/* Het programma van dhr Baeten als template gebruikt een aangepast   */
/* naar wens			                        	      */
/* ****************************************************************** */

#include <pigpio.h>
#include <math.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>


// motor 1
const int PWM1 = 19;
const int INI1_1 = 23;
const int INI2_1 = 24;

// motor 2
const int PWM2 = 13;
const int INI1_2 = 6;
const int INI2_2 = 5;

// motor 3
const int PWM3 = 20;
const int INI1_3 = 11;
const int INI2_3 = 9;

int Init_Motor_Control(void);

 void Motor_Control(int motor_nr,int velocity, int direction);


void Set_Motor(int motor, int signed_pwm);

//speed tussen 0 en 100
//40 is min in praktijk
void set_motor_speed(int motor_nr, double speed);

double M1;
double M2;
double M3;

//structure voor coördinaten
typedef struct Coordinaat{
	double x;
	double y;
}coordinaat;

// Aanmaken coördinaat middelpunt
coordinaat mid;

// X- en Y snelheid
double X_SPEED;
double Y_SPEED;

// Afstand van wielen tot middenpunt
const double afstand_wielen_tot_mid = 6.0;

// hulpvar voor goneometrie
double z;

// Afstanden van motor tot zwaartepunt
double motor1_afstand; 
double motor2_afstand;
double motor3_afstand; 

// hoekverschuiving bij verplaatsen van zwaartepunt (M1, M2, M3)
double hoekB, hoekY, hoekX;

double M1_hoek = M_PI/6;
double M2_hoek = 5*M_PI/6;
double M3_hoek = 3*M_PI/2;

double x_speed, y_speed,z_speed;

void set_robot_speed(double x_speed, double y_speed, double z_speed);

// functie om makkelijk input te vragen
int vraag_waarde_op();

// manueel aansturen van PWM
int man_pwm(int motor);
// zet geselecteerde motor af
int close_pwm(int motor);


//https://www.cairographics.org/samples/
//Groot deel gewoon overgenomen. on_draw wordt onderaan aageroepen
gboolean on_draw (GtkWidget *widget,GdkEventExpose *event,gpointer data);

double hoek;

gboolean herteken(gpointer data);


int softPWM(int argc, char **argv);
//variable aanmaken

