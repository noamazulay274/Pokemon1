#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"
#ifndef POCEDEX_H
#define POCEDEX_H

typedef struct BiologicalInfo_t
{
	float Height;
	float Weight;
	int Power;

}BiologicalInfo;

typedef struct PokemonType_t
{
	char *TypeName;
	int NuminType;
	struct PokemonType_t **effective_against_me;
	struct PokemonType_t **effective_against_others;
	int num_effective_against_me;
	int num_effective_against_others;

}PokemonType;


typedef struct Pokemon_t
{
	char *PokemonName;
	char *species;
	PokemonType Type;
	BiologicalInfo bio;

}Pokemon;



PokemonType* initPokemonType( char * );
Pokemon* initPokemon( char *, char *,PokemonType*, float ,float ,int  );
status add_effective_against_me(PokemonType*,PokemonType*);
status add_effective_against_others(PokemonType*,PokemonType*);
status delete_effective_against_me(PokemonType*,PokemonType*);
status delete_efective_against_others(PokemonType*,PokemonType*);
status print_Pokemon(Pokemon*);
status print_PokemonType(PokemonType*);

#endif
