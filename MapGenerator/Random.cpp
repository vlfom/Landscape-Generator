#include "stdafx.h" 

#include "Rand_Mersenn.h"
#include "Random.h"

float Rand_generate(int a) {
	switch(a) {
		case 0:
			return Rand_an((float)MERSENN_genrand_real2(),0.5f) ;
			break ;
	}
	return 0;
}

float Rand_an(float a,float b) {
	if(a>b)
		return a-b;
	else 
		return -a;
}
