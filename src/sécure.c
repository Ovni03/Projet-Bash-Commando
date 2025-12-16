/**
 * @file sécure.c
 * @brief Contenant 04 fonction pour qui lit en toute sécurité les reponses insereés sur "stdin" par l'utilisateur
 * @date 24 Sept 2025
 * @author RAKOTOARIVONY Zo Mamitiana Olivier
 * 
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @fn lireLigne
 * @brief Lit une chaîne de caractère 
 * @param 1 : Tableau de char pour stocké les chaines
 * @param 2 : Taille du tableau de char
*/
void lireLigne( char *line , int taille)
{
	int size = 0;
	
		if (fgets(line , taille , stdin))
		{
			size = strlen(line);
				if ( size > 0 && line[size-1] == '\n' ) // Supprime "\n" inserez automatiquement par fgets
				{
					line[size-1]='\0';
				}	
		}
}


/**
 * @fn lireEntier
 * @brief Lire un entier 
 * @return Return l'entier entré
 */
int lireEntier()
{
	char entier[50]="";
	
	lireLigne(entier , sizeof(entier));

	return atoi(entier);
}


/**
 * @fn lireFloat
 * @brief Lit un float
 * @return Return le float entré
 */
float lireFloat()
{
	char reel[50]="";	
	lireLigne(reel , sizeof(reel));

	return strtof(reel , NULL);
}


/**
 * @fn lireDouble
 * @brief Lit un double
 * @return Return un variable de type double 
 */
double lireDouble()
{
	char reel[50]="";
	lireLigne(reel , sizeof(reel));

	return strtod(reel , NULL);
} 
