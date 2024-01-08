#ifndef SUFRAMCE_H
#define SUFRAMCE_H

typedef struct {
	char type;
	union {
		char c;
		char *s;
	}data;
	
}Suframce_token;


#endif