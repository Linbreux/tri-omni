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
/* naar wens. Deze code is beschikbaar op github.             	      */
/* ****************************************************************** */

#include <pigpio.h>
#include <math.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>

//Wezel voegt some text toe he lolol
//Test Thib

// motor 1
const int PWM1 = 12;
const int INI1_1 = 23;
const int INI2_1 = 24;

// motor 2
const int PWM2 = 13;
const int INI1_2 = 5;
const int INI2_2 = 6;

// motor 3
const int PWM3 = 18;
const int INI1_3 = 9;
const int INI2_3 = 11;

int Init_Motor_Control(void){
	if (gpioInitialise() < 0) return 1;
	// Initialisation of outputs
	gpioSetPWMrange(PWM1, 255);
	gpioSetMode(PWM1, PI_OUTPUT);
	gpioSetMode(INI1_1, PI_OUTPUT);
	gpioSetMode(INI1_2, PI_OUTPUT);
	
	gpioSetPWMrange(PWM2, 255);
	gpioSetMode(PWM2, PI_OUTPUT);
	gpioSetMode(INI1_2, PI_OUTPUT);
	gpioSetMode(INI2_2, PI_OUTPUT);
	
	gpioSetPWMrange(PWM3, 255);
	gpioSetMode(PWM3, PI_OUTPUT);
	gpioSetMode(INI1_3, PI_OUTPUT);
	gpioSetMode(INI2_3, PI_OUTPUT);
	return 0;
}

//motor aansturen
 void Motor_Control(int motor_nr,int velocity, int direction){
	 velocity *= 3;
	int PinPwm , PinOut1 , PinOut2 ;
	
	if (motor_nr == 1) {
		 PinPwm = PWM1 ;
		 PinOut1 = INI1_1;
		 PinOut2 = INI2_1;
	} else if(motor_nr == 2) {
		 PinPwm = PWM2;
		 PinOut1 = INI1_2;
		 PinOut2 = INI2_2;
	} else if(motor_nr == 3){ 
		 PinPwm = PWM3;
		 PinOut1 = INI1_3;
		 PinOut2 = INI2_3;
	}
	
	if (direction == 1) 	{
		gpioWrite(PinOut1,1);
		gpioWrite(PinOut2,0);
	} else if (direction == -1) { 
		gpioWrite(PinOut1,0);
		gpioWrite(PinOut2,1);
	} else {
		gpioWrite(PinOut1,0);
		gpioWrite(PinOut2,0);		
	}
	//zorg dat waarde hoger dan 100 naar 100 worden geplaatst
	if (velocity > 100)velocity = 100;
	
	//func(GPIO_pin, freq, duty)
	//PWMfreq: 0 (off) or 1-125M (1-187.5M for the BCM2711)
	//PWMduty: 0 (off) to 1000000 (1M)(fully on
	if(gpioHardwarePWM(PinPwm, 50000 ,velocity*10000)==0){	
		//informatie printen
		//if (velocity ==0)return 0;
		printf("pin: %d -- Motor snelheid ingesteld op %d/100\n",PinPwm,velocity);
	}else{
		printf("Kon de uitgang niet aansturen, zorg dat je als super user runt\n");
	}

}

void Set_Motor(int motor, int signed_pwm){
	int mypwm = signed_pwm;
	int direction = 1;
	if (mypwm < 0 ) {
		mypwm = -mypwm;
		direction = -1;
	} else if (mypwm ==0) {
		direction = 0 ;
	}
	Motor_Control(motor, mypwm, direction);
}

//speed tussen 0 en 100
//40 is min in praktijk
void set_motor_speed(int motor_nr, double speed){
	Set_Motor(motor_nr, (int)speed);
}

double M1;
double M2;
double M3;

/*
double x_speed = 2;
double y_speed = 6;
double z_speed = 20;

const double a = cos(240*(M_PI/180.0));
const double b = cos(120*(M_PI/180.0));
const double c = cos(0);
const double d = sin(240*(M_PI/180.0));
const double e = sin(120*(M_PI/180.0));
const double f = sin(0);
const double g = 1;
const double h = 1;
const double i = 1;
const double det = a*e*i + b*f*g + c*d*h - c*e*g - a*f*h - b*d*i;

const double a_invers = (e*i - f*h) / det;
const double b_invers = (h*c - i*b) / det;
const double c_invers = (b*f - c*e) / det;
const double d_invers = (g*f - d*i) / det;
const double e_invers = (a*i - g*c) / det;
const double f_invers = (d*c - a*f) / det;
const double g_invers = (d*h - g*e) / det;
const double h_invers = (g*b - a*h) / det;
const double i_invers = (a*e - d*b) / det;
*/

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

void set_robot_speed(double x_speed, double y_speed, double z_speed){

	// Bereken hoekverschuiving
	hoekB = acos((pow(afstand_wielen_tot_mid,2) + pow(motor1_afstand,2) - pow(z,2))/(2*motor1_afstand*afstand_wielen_tot_mid));
	hoekY = acos((pow(motor2_afstand,2) + pow(afstand_wielen_tot_mid,2) - pow(z,2))/(2*afstand_wielen_tot_mid*motor2_afstand));
	hoekX = atan(mid.x/(afstand_wielen_tot_mid+mid.y));

	// locale variabel naar globale
	X_SPEED = x_speed;
	Y_SPEED = y_speed;

	// rotatiesnelheid berekenen per motor
	double A = sin(M_PI/2-hoekB)*(motor1_afstand/afstand_wielen_tot_mid)*z_speed;
	double B = sin(M_PI/2-hoekY)*(motor2_afstand/afstand_wielen_tot_mid)*z_speed;
	double C = sin(M_PI/2-hoekX)*(motor3_afstand/afstand_wielen_tot_mid)*z_speed;

	// snelheid motoren berekenen aan de hand van inverse matrix. 
	M1 = (double) -1/3*x_speed 	+ 1.0/sqrt(3) * y_speed 	+ 1.0/3* A;
 	M2 = (double) -1/3*x_speed 	+ -1.0/sqrt(3) * y_speed 	+ 1.0/3* B;
	M3 = (double) 2/3*x_speed 	+ 0 * y_speed 			+ 1.0/3* C;

	// print motorsneldheden	
	printf("m1: %lf, m2: %lf,m3: %lf\n",M1,M2,M3);

	// stuur de output van elke motor aan.
	set_motor_speed(1, M1);
	set_motor_speed(2, M2);
	set_motor_speed(3, M3);
}

// functie om makkelijk input te vragen
int vraag_waarde_op(){
	double waarde;
	scanf("%lf",&waarde);
	return waarde;
}
// manueel aansturen van PWM
int man_pwm(int motor){
	printf("Waarde motor %d: ", motor);
	Set_Motor(1,vraag_waarde_op());
	
}
// zet geselecteerde motor af
int close_pwm(int motor){
	Set_Motor(motor,0);
	return 0;
}


//https://www.cairographics.org/samples/
//Groot deel gewoon overgenomen. on_draw wordt onderaan aageroepen
gboolean on_draw (GtkWidget *widget,GdkEventExpose *event,gpointer data){


	//"convert" the G*t*kWidget to G*d*kWindow (no, it's not a GtkWindow!)
	GdkWindow* window = gtk_widget_get_window(widget);  

	cairo_region_t * cairoRegion = cairo_region_create();

	GdkDrawingContext * drawingContext;
	drawingContext = gdk_window_begin_draw_frame (window,cairoRegion);

	{ 
		// say: "I want to start drawing"
		cairo_t * cr = gdk_drawing_context_get_cairo_context (drawingContext);

		{ // Hier teken ik de layout
			
			// Posties van de motoren in de virtuele wereld (niet de werkelijke maten)
			double M1_X, M1_Y;
			double M2_X, M2_Y;
			double M3_X, M3_Y;

			// Positie mid punt virtuele wereld
			double MID_X, MID_Y;

			// declareren
			M2_X = 150; M2_Y = 150;
		      	M1_X = 150+260*2; M1_Y = 150;
			M3_X = 150+260; M3_Y = 600;	

			MID_X = M3_X; MID_Y = 300;
			

			cairo_set_line_width (cr, 10.0);
			
			//middenpunt aanmaken
			cairo_arc (cr, MID_X, MID_Y, 5, 0, 2*M_PI);

			//
			double lengte_wiel = 25;
			cairo_move_to(cr, M2_X+sin(M2_hoek)*lengte_wiel, M2_Y-cos(M1_hoek)*lengte_wiel);
			cairo_rel_line_to (cr, -25, 43.3);
			// lijn motor1 tot mid
			cairo_move_to(cr, M1_X-sin(M1_hoek)*lengte_wiel, M1_Y-cos(M1_hoek)*lengte_wiel);
			cairo_rel_line_to (cr, 25, 43.3);
			//lijn motor3 tot mid
			cairo_move_to(cr, M3_X-28, M3_Y);
			cairo_rel_line_to (cr, 56, 0.0);

			cairo_stroke(cr);
			cairo_set_line_width (cr, 2.0);
			
			// 
			cairo_move_to(cr, M1_X, M1_Y);
			cairo_line_to (cr, MID_X, MID_Y);
			
			cairo_move_to(cr, M2_X, M2_Y);
			cairo_line_to (cr, MID_X, MID_Y);

			cairo_move_to(cr, M3_X, M3_Y);
			cairo_line_to (cr, MID_X, MID_Y);


			cairo_set_source_rgba (cr, 1, 0.2, 0.2, 1);
			
			cairo_stroke(cr);

			double zw_punt_x = MID_X + mid.x*300/afstand_wielen_tot_mid;
			double zw_punt_y = MID_Y - mid.y*300/afstand_wielen_tot_mid;
			cairo_set_source_rgba (cr, 0.2, 0.2, 1,0.8 );
			cairo_set_line_width (cr, 1.0);

			cairo_stroke(cr);

			cairo_move_to(cr, zw_punt_x ,zw_punt_y );
			cairo_arc (cr, zw_punt_x , zw_punt_y , 5, 0, 2*M_PI);
			cairo_fill(cr);
			
			cairo_move_to(cr, zw_punt_x ,zw_punt_y );
			cairo_line_to(cr, M1_X,M1_Y);
			cairo_move_to(cr, zw_punt_x ,zw_punt_y );
			cairo_line_to(cr, M2_X,M2_Y);
			cairo_move_to(cr, zw_punt_x ,zw_punt_y );
			cairo_line_to(cr, M3_X,M3_Y);
			cairo_set_source_rgba (cr, 0.2, 0.2, 1, 1);
			
			cairo_stroke(cr);


			cairo_set_line_width (cr, 1.0);
			
			/*
			cairo_move_to(cr, M1_X,M1_Y);
			cairo_rel_line_to(cr, - cos(M_PI/6+M_PI/2+hoekB)*M1*3,sin(M_PI/6+M_PI/2+hoekB)*M1*3);

			cairo_move_to(cr, M2_X,M2_Y);
			cairo_rel_line_to(cr, cos(-M_PI/6-M_PI/2+hoekY)*M2*3,-sin(M_PI/6-M_PI/2-hoekY)*M2*3);

			cairo_move_to(cr, M3_X,M3_Y);
			cairo_rel_line_to(cr, cos(hoekX)*M3*3,sin(hoekX)*M3*3);
			*/

			cairo_move_to(cr, M1_X,M1_Y);
			cairo_rel_line_to(cr, - cos(M_PI/6-M_PI/2)*M1*3,sin(M_PI/6-M_PI/2)*M1*3);

			cairo_move_to(cr, M2_X,M2_Y);
			cairo_rel_line_to(cr, cos(-M_PI/6-M_PI/2)*M2*3,-sin(M_PI/6-M_PI/2)*M2*3);

			cairo_move_to(cr, M3_X,M3_Y);
			cairo_rel_line_to(cr, M3*3,0);

			cairo_set_source_rgba (cr, 0.2, 1, 0.2,0.8 );
			cairo_set_line_width (cr, 4.0);
			cairo_stroke(cr);

			cairo_move_to(cr, MID_X,MID_Y);
			cairo_rel_line_to(cr,X_SPEED,Y_SPEED);
		
			cairo_set_source_rgba (cr, 1, 0.5,0 ,0.8 );
			cairo_set_line_width (cr, 4.0);
			cairo_stroke(cr);



		}

		// say: "I'm finished drawing
		gdk_window_end_draw_frame(window,drawingContext);
	}

	// cleanup
	cairo_region_destroy(cairoRegion);

	return FALSE;
}

double hoek;

gboolean herteken(gpointer data){

	coordinaat motor1_mid, motor2_mid, motor3_mid;
	motor1_mid.x = afstand_wielen_tot_mid * cos(30.0*(M_PI/180));
	motor1_mid.y = afstand_wielen_tot_mid * sin(30.0*(M_PI/180));
	
	motor2_mid.x = afstand_wielen_tot_mid * cos(150.0*(M_PI/180));
	motor2_mid.y = afstand_wielen_tot_mid * sin(150.0*(M_PI/180));

	motor3_mid.x = afstand_wielen_tot_mid * cos(270.0*(M_PI/180));
	motor3_mid.y = afstand_wielen_tot_mid * sin(270.0*(M_PI/180));
	

	//mid.x = z*cos(hoek);
	//mid.y = z*sin(hoek);
	//printf("hoek: %lf",hoek);
	//hoek = hoek+0.1;


	//stel zwaartepunt in voor z rotatie
	coordinaat motor1_co, motor2_co, motor3_co;

	motor1_co.x = -mid.x + motor1_mid.x;
	motor1_co.y = -mid.y + motor1_mid.y;
	motor1_afstand = sqrt(pow(motor1_co.x,2) + pow(motor1_co.y,2));

	motor2_co.x = -mid.x + motor2_mid.x;
	motor2_co.y = -mid.y + motor2_mid.y;
	motor2_afstand = sqrt(pow(motor2_co.x,2) + pow(motor2_co.y,2));

	motor3_co.x = -mid.x + motor3_mid.x;
	motor3_co.y = -mid.y + motor3_mid.y;
	motor3_afstand = sqrt(pow(motor3_co.x,2) + pow(motor3_co.y,2));

	set_robot_speed(x_speed,y_speed,z_speed);

	gtk_widget_queue_draw(data);
	return TRUE;
}


int main(int argc, char **argv){
 
	Init_Motor_Control();
	//robot snelheid in de x en y richting 
	
	GtkWidget *window;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
	g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL);

	printf("Geef zwaartepunt op, x <enter> y <enter>\n");
	
	mid.x = vraag_waarde_op();
	mid.y = vraag_waarde_op();
	
	hoek = atan(mid.y/mid.x);
	z = sqrt(pow(mid.x,2)+pow(mid.y,2));

	//set_robot_speed(x_speed, y_speed, z_speed);		
	printf("Geef x <enter>, y <enter>, z <enter>\n");
	x_speed = vraag_waarde_op();
	y_speed = vraag_waarde_op();
	z_speed = vraag_waarde_op();
		

	GtkDrawingArea* drawingArea;
	{
		drawingArea = (GtkDrawingArea*) gtk_drawing_area_new();
		gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)drawingArea);

		g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), NULL);    
	}  
	g_timeout_add(100, herteken, drawingArea);
	gtk_widget_show_all(window);
	gtk_main();
	printf("Press ENTER key to Continue\n");  
	getchar();   
	//tweede char nodig omdat bij scanf op enter wordt gedrukt.

	getchar();   
	//sluit PWM van motor #
	close_pwm(1);
	close_pwm(2);
	close_pwm(3);

	gpioTerminate();
	
}
//variable aanmaken

