#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pokemon.h"
#include "Defs.h"



void freepokemon(Pokemon *pok){
	if(pok!=NULL){
	free(pok->PokemonName);
	free(pok->species);
	free(pok);
	}
}
void freepokemonType(PokemonType *pokemonType){
	if(pokemonType!=NULL){
	free(pokemonType->TypeName);
	if(pokemonType->num_effective_against_me>0){
		free(pokemonType->effective_against_me);
	}
	if(pokemonType->num_effective_against_others>0){
			free(pokemonType->effective_against_others);
	}

	free(pokemonType);
	}
}


Pokemon* initPokemon( char *PokemonName, char *species ,PokemonType *type, float Height,float Weight,int Power)
{
	Pokemon* pok = (Pokemon*)malloc(sizeof(Pokemon));
	if (pok==NULL){
		return NULL;
	}
	pok->PokemonName=(char*)malloc(strlen(PokemonName)+1);
	if (pok->PokemonName==NULL){
			return NULL;
		}

	strcpy(pok->PokemonName,PokemonName);
	pok->species=(char*)malloc(strlen(species)+1);
	if (pok->species==NULL){
			return NULL;
		}
	strcpy(pok->species,species);
	pok->Type=*type;
	pok->bio.Height=Height;
	pok->bio.Weight=Weight;
	pok->bio.Power=Power;
	type->NuminType++;
	return pok;
}


PokemonType* initPokemonType( char * name)
{
	PokemonType *newPokemonType = (PokemonType*)malloc(sizeof(PokemonType));
	if (newPokemonType==NULL){
		return NULL;
	}
	newPokemonType->TypeName=(char*)malloc(sizeof(name)+1);
	if (newPokemonType->TypeName==NULL){
			return NULL;
		}
	if (name!=NULL){
	strcpy(newPokemonType->TypeName,name);
	}
	newPokemonType->num_effective_against_me=0;
	newPokemonType->num_effective_against_others=0;
	newPokemonType ->NuminType=0;
	return newPokemonType;
}
status add_effective_against_me(PokemonType* me,PokemonType* other){
	me->num_effective_against_me++;
	if (me->num_effective_against_me==1){
		me->effective_against_me=(PokemonType**)malloc(sizeof(PokemonType*));

	}
	me->effective_against_me=(PokemonType**)realloc(me->effective_against_me,me->num_effective_against_me*sizeof(PokemonType*));
	if (me->effective_against_me==NULL){
			return failure;
		}
	else{
	me->effective_against_me[me->num_effective_against_me-1]=other;
	}
	return success;

}
status add_effective_against_others(PokemonType* me,PokemonType* other){
	me->num_effective_against_others++;
	if (me->num_effective_against_others==1){
		me->effective_against_others=(PokemonType**)malloc(sizeof(PokemonType*));

	}
	else{
	me->effective_against_others=(PokemonType**)realloc(me->effective_against_others,me->num_effective_against_others*sizeof(PokemonType*));
	}
	if (me->effective_against_others==NULL){
			return failure;
		}
	me->effective_against_others[me->num_effective_against_others-1]=other;
	return success;

}
status delete_effective_against_me(PokemonType* me,PokemonType* other){
	for(int i=0;i<me->num_effective_against_me;i++){
		if(!strcmp(me->effective_against_me[i]->TypeName,other->TypeName)){
			for (int j=i;j<me->num_effective_against_me-1;j++){
				me->effective_against_me[j]=me->effective_against_me[j+1];
			}
			me->num_effective_against_me--;
			me->effective_against_me=(PokemonType**)realloc(me->effective_against_me,me->num_effective_against_me*sizeof(PokemonType*));
			return(success);
			break;
		}

	}
	return(failure);
}
status delete_efective_against_others(PokemonType* me,PokemonType* other){
	for(int i=0;i<me->num_effective_against_others;i++){
		if(!strcmp(me->effective_against_others[i]->TypeName,other->TypeName)){
			for (int j=i;j<me->num_effective_against_others-1;j++){
				me->effective_against_others[j]=me->effective_against_others[j+1];
			}
			me->num_effective_against_others--;
			me->effective_against_others=(PokemonType**)realloc(me->effective_against_others,me->num_effective_against_others*sizeof(PokemonType*));
			return(success);
			break;
		}

	}
	return(failure);}
status print_Pokemon(Pokemon* pokemon){


	printf("%s :\n",pokemon->PokemonName);
	printf("%s, %s Type.\n",pokemon->species,pokemon->Type.TypeName);
	printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n",pokemon->bio.Height,pokemon->bio.Weight,pokemon->bio.Power);
	printf("\n");
	return(success);
}
status print_PokemonType(PokemonType* type){
	printf("Type %s -- %d pokemons\n",type->TypeName,type->NuminType);
	if(type->num_effective_against_me>0){
		printf("\tThese types are super-effective against %s:",type->TypeName);
		for(int i=0;i<type->num_effective_against_me;i++){
			printf("%s",type->effective_against_me[i]->TypeName);
			if(i<type->num_effective_against_me-1){
				printf(" ,");
			}
		}
		printf("\n");

	}
	if(type->num_effective_against_others>0){
			printf("\t%s moves are super-effective against:",type->TypeName);
			for(int i=0;i<type->num_effective_against_others;i++){
				printf("%s",type->effective_against_others[i]->TypeName);
				if(i<type->num_effective_against_others-1){
					printf(" ,");
				}
			}
			printf("\n");

		}
	printf("\n");

	return(success);
}
