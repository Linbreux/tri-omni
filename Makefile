
all:
	gcc -Wall -lpigpio -lcairo `pkg-config --cflags --libs gtk+-3.0` -lm  motorcontrol.c

