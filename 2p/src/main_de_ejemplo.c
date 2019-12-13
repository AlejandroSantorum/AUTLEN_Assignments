#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../srclib/afnd.h"
#include "../srclib/transforma.h"
#include "../srclib/minimiza.h"

int main(int argc, char ** argv)
{

	AFND * p_afnd;
	AFND * afd;


    p_afnd= AFNDNuevo("afnd", 8, 2);

    	AFNDInsertaSimbolo(p_afnd,"0");
    	AFNDInsertaSimbolo(p_afnd,"1");

    	AFNDInsertaEstado(p_afnd, "A",INICIAL);
    	AFNDInsertaEstado(p_afnd, "B", NORMAL);
    	AFNDInsertaEstado(p_afnd, "C", FINAL);
    	AFNDInsertaEstado(p_afnd, "D", NORMAL);
    	AFNDInsertaEstado(p_afnd, "E", NORMAL);
    	AFNDInsertaEstado(p_afnd, "F", NORMAL);
    	AFNDInsertaEstado(p_afnd, "G", NORMAL);
    	AFNDInsertaEstado(p_afnd, "H", NORMAL);

    	AFNDInsertaTransicion(p_afnd, "A", "0", "B");
    	AFNDInsertaTransicion(p_afnd, "A", "1", "F");
    	AFNDInsertaTransicion(p_afnd, "B", "0", "G");
    	AFNDInsertaTransicion(p_afnd, "B", "1", "C");
    	AFNDInsertaTransicion(p_afnd, "C", "0", "A");
    	AFNDInsertaTransicion(p_afnd, "C", "1", "C");

    	AFNDInsertaTransicion(p_afnd, "D", "0", "C");
    	AFNDInsertaTransicion(p_afnd, "D", "1", "G");
    	AFNDInsertaTransicion(p_afnd, "E", "0", "H");
    	AFNDInsertaTransicion(p_afnd, "E", "1", "F");
    	AFNDInsertaTransicion(p_afnd, "F", "0", "C");
    	AFNDInsertaTransicion(p_afnd, "F", "1", "G");
    	AFNDInsertaTransicion(p_afnd, "G", "0", "G");
    	AFNDInsertaTransicion(p_afnd, "G", "1", "E");
    	AFNDInsertaTransicion(p_afnd, "H", "0", "G");
    	AFNDInsertaTransicion(p_afnd, "H", "1", "C");

	// p_afnd= AFNDNuevo("afnd_ejemplo", 5, 2);
    //
	// AFNDInsertaSimbolo(p_afnd,"a");
    // AFNDInsertaSimbolo(p_afnd,"b");
    //
	// AFNDInsertaEstado(p_afnd, "q0", INICIAL);
	// AFNDInsertaEstado(p_afnd, "q1", FINAL);
	// AFNDInsertaEstado(p_afnd, "q2", NORMAL);
	// AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	// AFNDInsertaEstado(p_afnd, "q4", NORMAL);
    //
	// AFNDInsertaTransicion(p_afnd, "q0", "a", "q1");
	// AFNDInsertaTransicion(p_afnd, "q0", "b", "q1");
    //
    // AFNDInsertaTransicion(p_afnd, "q1", "a", "q0");
	// AFNDInsertaTransicion(p_afnd, "q1", "b", "q2");
    //
    // AFNDInsertaTransicion(p_afnd, "q2", "a", "q0");
	// AFNDInsertaTransicion(p_afnd, "q2", "b", "q2");
    //
    // AFNDInsertaTransicion(p_afnd, "q3", "a", "q1");
	// AFNDInsertaTransicion(p_afnd, "q3", "b", "q4");
    //
    // AFNDInsertaTransicion(p_afnd, "q4", "a", "q3");
	// AFNDCierraLTransicion(p_afnd);

	/*
    p_afnd = AFNDNuevo("af12", 6, 13);
    AFNDInsertaSimbolo(p_afnd, "+");
    AFNDInsertaSimbolo(p_afnd, "-");
    AFNDInsertaSimbolo(p_afnd, ".");
    AFNDInsertaSimbolo(p_afnd, "0");
    AFNDInsertaSimbolo(p_afnd, "1");
    AFNDInsertaSimbolo(p_afnd, "2");
    AFNDInsertaSimbolo(p_afnd, "3");
    AFNDInsertaSimbolo(p_afnd, "4");
    AFNDInsertaSimbolo(p_afnd, "5");
    AFNDInsertaSimbolo(p_afnd, "6");
    AFNDInsertaSimbolo(p_afnd, "7");
    AFNDInsertaSimbolo(p_afnd, "8");
    AFNDInsertaSimbolo(p_afnd, "9");

    AFNDInsertaEstado(p_afnd, "q0",INICIAL);
	AFNDInsertaEstado(p_afnd, "q1", NORMAL);
	AFNDInsertaEstado(p_afnd, "q2", NORMAL);
	AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	AFNDInsertaEstado(p_afnd, "q4", NORMAL);
	AFNDInsertaEstado(p_afnd, "q5", FINAL);

    AFNDInsertaTransicion(p_afnd, "q0", "+", "q1");
    AFNDInsertaTransicion(p_afnd, "q0", "-", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "0", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "1", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "2", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "3", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "4", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "5", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "6", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "7", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "8", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "9", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "0", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "1", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "2", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "3", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "4", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "5", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "6", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "7", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "8", "q4");
    AFNDInsertaTransicion(p_afnd, "q1", "9", "q4");
    AFNDInsertaTransicion(p_afnd, "q4", ".", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "0", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "1", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "2", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "3", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "4", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "5", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "6", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "7", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "8", "q3");
    AFNDInsertaTransicion(p_afnd, "q2", "9", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "0", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "1", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "2", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "3", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "4", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "5", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "6", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "7", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "8", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "9", "q3");
    AFNDInsertaTransicion(p_afnd, "q1", ".", "q2");

    AFNDInsertaLTransicion(p_afnd, "q0", "q1");
    AFNDInsertaLTransicion(p_afnd, "q3", "q5");
    AFNDCierraLTransicion(p_afnd);
	*/

    // afd  = AFNDTransforma(p_afnd);
    AFNDImprime(stdout, p_afnd);
    afd = AFNDMinimiza(p_afnd);
	AFNDImprime(stdout,afd);
    AFNDADot(afd);
	AFNDADot(p_afnd);

	AFNDElimina(afd);
	AFNDElimina(p_afnd);

	return 0;
}
