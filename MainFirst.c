#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pokemon.h"
#include "Defs.h"




void free_all(bool error, int numberOfTypes,int numberOfPokemons,PokemonType *Typesarray[numberOfTypes], Pokemon *Pokemonarray[numberOfPokemons]){
	//free all the types and pokemons
	if (error==true){
		printf("Problem Memory ");
	}
	for(int i=0; i<numberOfPokemons;i++){
		freepokemon(Pokemonarray[i]);
	}
	for(int i=0; i<numberOfTypes;i++){
		freepokemonType(Typesarray[i]);
	}


}
bool ismeline(char* str){
	if (strstr(str, "effective-against-me:") != NULL) {
	    return true;
	}
    return false;


}
bool isotherline(char* str){
	if (strstr(str, "effective-against-other:") != NULL) {
		    return true;
		}
    return false;

}
status strtotypes(int numberOfTypes,char str[300],char *TypesNames[numberOfTypes],int start){
		   int index=0;
		   int place=-1;
		   char tname[300]="";

		   for(int i=start;i<strlen(str);i++){
			   if (str[i]!=','){

				   tname[index]=str[i];
				   index++;
			   }
			   else{
				   tname[index]=NULL;
				   place+=1;
				   TypesNames[place]=(char*)malloc(strlen(tname)+1);
				   if(TypesNames[place]==NULL){
					   return(failure);
				   }
				   strcpy(TypesNames[place],tname);
				   index=0;
				   *tname="";

			   }
		   }
		   tname[index]=NULL;
		   TypesNames[place+1]= (char*)malloc(strlen(tname)+1);
		   if(TypesNames[place]==NULL){
			   return(failure);
		   }
		   strcpy(TypesNames[place+1],tname);
		   for(int i= place+2; i<numberOfTypes;i++){
			   TypesNames[i]='\0';
		   }
		   return(success);


}

void Build(int numberOfTypes,int numberOfPokemons,char *path,PokemonType *Typesarray[numberOfTypes], Pokemon *Pokemonarray[numberOfPokemons]) //read the file and create the objects
{
	   char str[301];//the longest str
	   FILE *fptr;
	   fptr = fopen(path,"r");
	   fscanf(fptr, "%s", str);
	   fscanf(fptr, "%s", str);
	   char *TypesNames[numberOfTypes];
	   status stat=strtotypes(numberOfTypes, str,TypesNames,0);
	   if (stat==failure){
		   free_all(true, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
		   exit(0);
	   }

	   for (int i=0;i<numberOfTypes;i++){//create all types
	   Typesarray[i]=initPokemonType(TypesNames[i]);
	   if(Typesarray[i]==NULL){
		   free_all(true, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
		   exit(0);
	   }
	   }
	   char *effectiveagainst[numberOfTypes];
	   char name[300];
	   while (fscanf(fptr, "%s", str)!=EOF && (strcmp(str,"Pokemons")))  //read and use the efective agaist lines
	   {
		   strcpy(name,str);
    	   fscanf(fptr, "%s", str);
    	   if(ismeline(str)==true){ //the file in "effective-against-me:" line

    	   	        		   stat=strtotypes(numberOfTypes, str, effectiveagainst,21);
    	   	        		   if (stat==failure){
    	   	        		   		   free_all(true, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
    	   	        		   		   exit(0);
    	   	        		   	   }
    	   	        		   for (int i=0;i<numberOfTypes;i++){
    	   	        			   if(!strcmp(name,TypesNames[i])){
    	   	        				   for (int j=0;j<numberOfTypes;j++){
    	    	   	        			   for (int k=0; k<numberOfTypes;k++){
												   if(effectiveagainst[j]!=NULL && !strcmp(effectiveagainst[j],TypesNames[k])){
													   add_effective_against_me(Typesarray[i], Typesarray[k]);
												   }

    	    	   	        			   }
    	   	        				   }

    	   	        			   }
    	   	        		   }

    	   		           for(int i=0;i<numberOfTypes;i++)
    	   		           		   {
    	   		           			   free(effectiveagainst[i]);//the strtotypes function used dynamic array and we free him

    	   		           		   }
    	   	        	   }
    	   	   else if(isotherline(str)==true){//the file in "effective-against-other:" line

    	       	   	        		   stat=strtotypes(numberOfTypes, str, effectiveagainst,24);
    	       	   	        		   if (stat==failure){
    	       	   	        		   		   free_all(true, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
    	       	   	        		   		   exit(0);
    	       	   	        		   	   }
    	       	   	        		   for (int i=0;i<numberOfTypes;i++){
    	       	   	        			   if(!strcmp(name,TypesNames[i])){
    	       	   	        				   for (int j=0;j<numberOfTypes;j++){
    	       	    	   	        			   for (int k=0; k<numberOfTypes;k++){
    	   												   if(effectiveagainst[j]!=NULL && !strcmp(effectiveagainst[j],TypesNames[k])){
    	   													   add_effective_against_others(Typesarray[i], Typesarray[k]);
    	   												   }

    	       	    	   	        			   }
    	       	   	        				   }

    	       	   	        			   }
    	       	   	        		   }

    	       	   		           for(int i=0;i<numberOfTypes;i++)
    	       	   		           		   {
    	       	   		           			   free(effectiveagainst[i]);//the strtotypes function used dynamic array and we free him

    	       	   		           		   }
    	       	   	        	   }



	   }

	   char *newPokemon[6];

	   for(int i=0;i<numberOfPokemons;i++){// create all the pokemons
		   fscanf(fptr, "%s", str);
		   stat=strtotypes(6, str, newPokemon,0);
		   if (stat==failure){
		   		   free_all(true, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
		   		   exit(0);
		   	   }
		   for (int k=0;k<numberOfTypes;k++){
			   if (!strcmp(TypesNames[k],newPokemon[5])){
				   Pokemonarray[i]=initPokemon(newPokemon[0], newPokemon[1], Typesarray[k],atof(newPokemon[2]), atof(newPokemon[3]), atoi(newPokemon[4]));
				   if(Pokemonarray[i]==NULL){
					   free_all(true, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
					   exit(0);
				   }
			   }
		   }
		   for(int i=0;i<6;i++)
		   {
			free(newPokemon[i]);//the strtotypes function used dynamic array and we free him

		   }

	   }




	       fclose(fptr);
		   for(int i=0;i<numberOfTypes;i++)
		   {
			   free(TypesNames[i]);//the strtotypes function used dynamic array and we free him

		   }


}



int main(int argc, char *argv[])
{


   int numberOfTypes = atoi(argv[1]);
   int numberOfPokemons =  atoi(argv[2]);
   char *path=argv[3];
   Pokemon *Pokemonarray[numberOfPokemons];
   PokemonType *Typesarray[numberOfTypes];

   Build(numberOfTypes, numberOfPokemons, path,&Typesarray,&Pokemonarray);//read the file and create the objects


 int ch = 15;
 char name1[300];
 char name2[300];
 bool con;



   while( ch != 9 ) {
	  printf("Please choose one of the following numbers:\n");
      printf("1 : Print all Pokemons\n");
      printf("2 : Print all Pokemons types\n");
      printf("3 : Add type to effective against me list\n");
      printf("4 : Add type to effective against others list\n");
      printf("5 : Remove type from effective against me list\n");
      printf("6 : Remove type from effective against others list\n");
      printf("7 : Print Pokemon by name\n");
      printf("8 : Print Pokemons by type\n");
      printf("9 : Exit\n");
      scanf("%s", &ch );
      ch=ch-48;  //convert from ascii to digit


      switch( ch )
      {
          case 1:     //Print all Pokemons
        	  for (int i=0;i<numberOfPokemons;i++){
        	  	  		   print_Pokemon(Pokemonarray[i]);
        	  	  	   }
				  break;

          case 2:     //Print all Pokemons types
        	  for (int i=0;i<numberOfTypes;i++){
        	  		   print_PokemonType(Typesarray[i]);
        	  	   }
				  break;


          case 3:     //Add type to effective against me list
        	  printf("Please enter type name:\n");
              scanf("%s", name1 );
              con=true;
              for (int i=0;i<numberOfTypes;i++){
            	  if(!strcmp(Typesarray[i]->TypeName,name1)){
                	  printf("Please enter type name to add to %s effective against me list:\n",name1);
                      scanf("%s", name2 );
                      for(int k=0; k<Typesarray[i]->num_effective_against_me;k++){

                    	  if(!strcmp(Typesarray[i]->effective_against_me[k]->TypeName,name2))
                    	  {
                        	  printf("This type already exist in the list.\n");
                        	  con=false;
                    	  	  break;
                    	  }

                      }
                      if (con==false){
                    	  break;
                      }
                      for(int k=0; k<numberOfTypes;k++){

									  if(!strcmp(Typesarray[k]->TypeName,name2))
									  {
										  add_effective_against_me(Typesarray[i], Typesarray[k]);
										  print_PokemonType(Typesarray[i]);
			                        	  con=false;
										  break;
									  }

									}


            	  }
              }
              if (con==false){
                  break;}

              printf("Type name doesn't exist.\n");


        	  	  break;


          case 4:    //Add type to effective against others list
        	  printf("Please enter type name:\n");
              scanf("%s", name1 );
              con=true;
              for (int i=0;i<numberOfTypes;i++){
            	  if(!strcmp(Typesarray[i]->TypeName,name1)){
                	  printf("Please enter type name to add to %s effective against others list:\n",name1);
                      scanf("%s", name2 );
                      for(int k=0; k<Typesarray[i]->num_effective_against_others;k++){

                    	  if(!strcmp(Typesarray[i]->effective_against_others[k]->TypeName,name2))
                    	  {
                        	  printf("This type already exist in the list.\n");
                        	  con=false;
                    	  	  break;
                    	  }

                      }
                      if (con==false){
                    	  break;
                      }
                      for(int k=0; k<numberOfTypes;k++){

									  if(!strcmp(Typesarray[k]->TypeName,name2))
									  {
										  add_effective_against_others(Typesarray[i], Typesarray[k]);
										  print_PokemonType(Typesarray[i]);
			                        	  con=false;
										  break;
									  }

									}


            	  }
              }
              if (con==false){
                  break;}

              printf("Type name doesn't exist.\n");


        	  	  break;


          case 5:   //Remove type from effective against me list

        	  printf("Please enter type name:\n");
        	                scanf("%s", name1 );
        	                con=true;
        	                for (int i=0;i<numberOfTypes;i++){
        	              	  if(!strcmp(Typesarray[i]->TypeName,name1)){
        	                  	  printf("Please enter type name to remove from %s effective against me list:\n",name1);
        	                        scanf("%s", name2 );
        	                        for(int k=0; k<Typesarray[i]->num_effective_against_me;k++){

        	                      	  if(!strcmp(Typesarray[i]->effective_against_me[k]->TypeName,name2))
        	                      	  {
        	                      		delete_effective_against_me(Typesarray[i], Typesarray[i]->effective_against_me[k]);
        	                      		print_PokemonType(Typesarray[i]);
        	                      		con= false;
        	                      		break;
        	                      	  }

        	                        }

          	                        if(con==true){

										  printf("Type name doesn't exist in the list.\n");
										  con=false;
										  break;
          	                        }




        	              	  }
        	                }
        	                if (con==false){
        	                    break;}

        	                printf("Type name doesn't exist.\n");
        	                break;




          case 6:   //Remove type from effective against others list
        	  printf("Please enter type name:\n");
        	  scanf("%s", name1 );
        	          	                con=true;
        	          	                for (int i=0;i<numberOfTypes;i++){
        	          	              	  if(!strcmp(Typesarray[i]->TypeName,name1)){
        	          	                  	  printf("Please enter type name to remove from %s effective against others list:\n",name1);
        	          	                        scanf("%s", name2 );
        	          	                        for(int k=0; k<Typesarray[i]->num_effective_against_others;k++){

        	          	                      	  if(!strcmp(Typesarray[i]->effective_against_others[k]->TypeName,name2))
        	          	                      	  {
        	          	                      		delete_efective_against_others(Typesarray[i], Typesarray[i]->effective_against_others[k]);
        	          	                      		print_PokemonType(Typesarray[i]);
        	          	                      		con= false;
        	          	                      		break;
        	          	                      	  }

        	          	                        }
        	          	                        if(con==true){

													  printf("Type name doesn't exist in the list.\n");
													  con=false;
													  break;
        	          	                        }

        	          	              	  }
        	          	                }
        	          	                if (con==false){
        	          	                    break;}

        	        	                printf("Type name doesn't exist.\n");
        	          	                break;


          case 7:    //Print Pokemon by name
        	  con=true;
        	  printf("Please enter Pokemon name:\n");
              scanf("%s", name1 );
              for(int i=0; i<numberOfPokemons;i++){
            	  if(!strcmp(name1,Pokemonarray[i]->PokemonName)){
            		  print_Pokemon(Pokemonarray[i]);
            		  con=false;
            		  break;
            	  }

              }
              if (con==false){
            	  break;
              }
        	  printf("The Pokemon doesn't exist.\n");
				  break;



          case 8:    //Print Pokemons by type\n
        	  con=true;
			  printf("Please enter type name:\n");
			  scanf("%s", name1 );
			  for(int i=0; i<numberOfTypes;i++){
			              	  if(!strcmp(name1,Typesarray[i]->TypeName)){
			              		  con=false;
			              		  if(Typesarray[i]->NuminType==0){
			              			  printf("There are no Pokemons with this type.\n");
			              		  }
			              		  else{
			              			  printf("There are %d Pokemons with this type:\n",Typesarray[i]->NuminType);
			              			  for (int k=0; k<numberOfPokemons; k++){
			              				  if (!strcmp(Pokemonarray[k]->Type.TypeName,name1)){
			              					  print_Pokemon(Pokemonarray[k]);
			              				  }
			              			  }
			              		  }
			              		  break;
			              	  }

			                }
			  if (con==false){
			              	  break;
			                }
			          	  printf("Type name doesn't exist.\n");
			          	  break;



          case 9:    //Exit
        	  free_all(false, numberOfTypes, numberOfPokemons, Typesarray, Pokemonarray);
        	  printf("All the memory cleaned and the program is safely closed.\n");
        	  exit(0);
				  break;

          default:
        	  ch=10;
        	  printf("Please choose a valid number.\n");
        	  break;


      }
   }
   return 0;
}
