#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <regex.h>

#define couleur(param) printf("\033[%sm",param)




//Structure Personne
struct Personne
{
    char *cin;
	char *nom;
	char *prenom;
	char *telefon;
	char *adress;
};
typedef struct Personne personne;



typedef struct Node {
	personne * data;	
	struct Node *next;
} node;




//Variables globales
node * head = NULL;
char * racine;
char * emplacement = "presidence";
int c1 = 0;








/*
	Ce programme permet de construir des annuaires simples des univirsités
	L'annuaires se compose des repetoires et fichiers textes
	Le premier reperoire la racine represente l'université
	puis les sous repertoires represente les instituts de l'université
	chaque institut composé de trois sous repertoires services, departements et filieres
	sauf la presidence a juste services et departements
	Sous ces repertoires on trouve des fichiers textes, chaqu'une represente une entrée
	cette derniere soit un eseignant, soit fonctionnaire, soit etudiant

	Les fonctions et les procedures utilisés dans notre programme nous permet de manipuler
	cet annuaire :

*/


void menuModifier();
int verifierReg(char * reg , char * chaine);
void ajouterPersonne(personne * newPersonne);
void afficher();
int trier(char * critere) ;
char ** splitLine(char * line, char * sep);
char *concat_string(char *s1,char *s2);
int changerRepertoire(const char * nom);
int creerRepertoire(char * nom);
int initialiserAnnuaire();
int creerInstitut(char * nom);
void supprimerInstitut(char * nom);
void addEntry(char **arr1);
void removeEntry(char **arr1);
void findEntry(char * entryName,const char *name, int level);
void list(const char *name, int level);
void display(const char *name, int level, char * critere);
int executerCmd(char cmd[]);





//Cette fonction permet de verifier si une chaine respecte une expression regulière
int verifierReg(char * reg , char * chaine){
	
	int err;
    regex_t preg;
	err = regcomp (&preg, reg, REG_NOSUB | REG_EXTENDED);
	if(err == 0){
		int match;
	    match = regexec (&preg, chaine, 0, NULL, 0);
	    regfree (&preg);
	    if (match == 0)
		      {
		         return 1;
		      }
		      else if (match == REG_NOMATCH)
		      {
		      	couleur("31");
		      	printf("\t %s n'est pas validé\n",chaine);
		      	printf("\t CIN (AA1111), nom prenom adresse fonction (alpha),telephon (1111111111)\n");
		      	couleur("0");
		        return -1;
		      }

	}else{
		printf("\tProbleme d'application, essayer une autre fois !!\n");
		return -1;
	}

}


//Supprimer un repertoire
int supprimerRep(const char *nom)
{
   DIR *d = opendir(nom);
   size_t path_len = strlen(nom);
   int r = -1;

   if (d)
   {
      struct dirent *p;
      r = 0;
      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }
          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);
          if (buf)
          {
             struct stat statbuf;
             snprintf(buf, len, "%s/%s", nom, p->d_name);
             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = supprimerRep(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }
   if (!r)
   {
      r = rmdir(nom);
   }
   return r;
}




//Ajouter une personne à la liste chainé du programme
void ajouterPersonne(personne * newPersonne) {
	node * newNode = (node *) malloc(sizeof(node));
	newNode->data = newPersonne;
	newNode->next = head;
	head = newNode;
}






//Afficher le contenu de la liste chainé
void afficher() {
	node *ptr = head; 
	printf("+ %-20s%-20s%-20s%-20s%-20s\n\n","CIN","NOM","PRENOM","ADRESSE","TELEPHONE");

	while(ptr) {
		printf("+ %-20s%-20s%-20s%-20s%-20s\n",ptr->data->cin,ptr->data->nom,ptr->data->prenom,ptr->data->adress,ptr->data->telefon);
		ptr = ptr->next;
	}

	printf("\n");
}





//Trier le contenu de la liste chainé selon un critere soit nom, prenom, cin ...
int trier(char * critere) {	
	
	node *ptr = head; 
	node *parser;

	if( strcmp(critere,"nom")){
			while(ptr != NULL) {
				parser = ptr->next;
				while(parser != NULL) {

						if(strcmp(parser->data->nom, ptr->data->nom) < 0) {
							personne * permut = parser->data;
							parser->data = ptr->data;
							ptr->data = permut;				
						}
					parser = parser->next; 
				}
				ptr = ptr-> next; 
			}
	}
	else if( strcmp(critere,"prenom")){
		while(ptr != NULL) {
				parser = ptr->next;
				while(parser != NULL) {

						if(strcmp(parser->data->prenom, ptr->data->prenom) < 0) {
							personne * permut = parser->data;
							parser->data = ptr->data;
							ptr->data = permut;				
						}
					parser = parser->next; 
				}
				ptr = ptr-> next; 
			}
	}else if( strcmp(critere,"cin")){
		while(ptr != NULL) {
				parser = ptr->next;
				while(parser != NULL) {

						if(strcmp(parser->data->cin, ptr->data->cin) < 0) {
							personne * permut = parser->data;
							parser->data = ptr->data;
							ptr->data = permut;				
						}
					parser = parser->next; 
				}
				ptr = ptr-> next; 
			}
	}else if( strcmp(critere,"adresse")){
		while(ptr != NULL) {
				parser = ptr->next;
				while(parser != NULL) {

						if(strcmp(parser->data->adress, ptr->data->adress) < 0) {
							personne * permut = parser->data;
							parser->data = ptr->data;
							ptr->data = permut;				
						}
					parser = parser->next; 
				}
				ptr = ptr-> next; 
			}
	}else{
		printf("Nous pouvons pas trier en utilisant %s ,la liste n'est pas trié\n",critere);
	}

	afficher();
	return 0;
}






//La fonction qui divise une chaine de caracteres par caractere donnée (separateur) 
char ** splitLine(char * line, char * sep) {
	int i=0, wordNbr=0;
	static char * splittedLine[100];	
	char * str = strdup(line); 
	char * token;
	while (token = strsep(&str, sep)) {
		if(strlen(token) > 0) {
			splittedLine[wordNbr] = token;
			wordNbr++;
		}
	}
	c1 = wordNbr;
	free(str);
	return splittedLine;
}





//Fonction de concatenation
char *concat_string(char *s1,char *s2)
{
     char *s3=NULL;
     s3=(char *)malloc((strlen(s1)+strlen(s2))*sizeof(char));
     strcpy(s3,s1);
     strcat(s3,s2);
     return s3;
 }





//Cette fonction permet de changer le repertoire vers le chemin "nom"
int changerRepertoire(const char * nom)
{
    int  fd;
    if ((fd = open(nom, O_RDONLY)) < 0) {
        perror(nom);
        return -1;
    }else{
	    if (fchdir(fd) < 0){
	        perror(nom);return -1;
	    }else return 1;
	}
    close(fd);
}






//Cette fontion permet de creer un repetoire de le chemin courant
int creerRepertoire(char * nom){
	if (mkdir(nom, 0777) != 0) {
        perror("");
        return -1;
    } else {
        printf("===\t%s est cree.\n",nom);
        return 1;
    }
}




//Initiialisation d'annuaire
int initialiserAnnuaire(){

	char * rm = "rm -rf ";
	rm = concat_string(rm,racine);
	couleur("34");
	fprintf(stdout,"====================================================================================\n"); 
	fprintf(stdout,"========================= ** SOUFIANE OUDRISS ** ===================================\n");
	fprintf(stdout,"====================================================================================\n");
	couleur("0");
	fprintf(stdout,"=== Initialisation d'annuaire ...\n");
	fprintf(stdout,"=== Creation de la racine (%s) ...\n",racine);
	if( changerRepertoire(racine) != 1){
	    if(creerRepertoire(racine) == 1){
		    changerRepertoire(racine);
		    if(creerRepertoire("presidence")==1){
		    	changerRepertoire("presidence");
		    	if(creerRepertoire("departements")==1){
		    		if(creerRepertoire("services")==1){
		    				return 1;
		    		}else{changerRepertoire("..");changerRepertoire("..");supprimerRep(racine);return -1;}
		    	}else{changerRepertoire("..");changerRepertoire("..");supprimerRep(racine);return -1;}
		    }else{changerRepertoire("..");supprimerRep(racine);return -1;}
		}else{return -1;}
	}else{
		changerRepertoire("presidence");
	}
}






//Creation d'une nouvelle institut
int creerInstitut(char * nom){
		changerRepertoire("..");
		if( creerRepertoire(nom) == 1 ){
			changerRepertoire(nom);
			creerRepertoire("departements");
			creerRepertoire("services");
			creerRepertoire("filieres");
			return 1;
		}else{
			printf("L'institut n'est pas ajoutée !!\n");
			changerRepertoire("presidence");
			return -1;
		}

}




//SuuprimerInsitut
void supprimerInstitut(char * nom){

	if( strcmp(nom,"*") != 0 ){
		changerRepertoire("..");
		supprimerRep(nom);
		changerRepertoire("presidence");
		printf("\t%s est suprimee.\n",nom);
	}else printf("Attention n'essayer pas *\n");
}





//Ajouter une entree
void addEntry(char **arr1){
		if(changerRepertoire(arr1[1]) == 1){
			if( changerRepertoire(arr1[2]) == 1){
				struct Personne p;
				p.cin = (char *)malloc(100*sizeof(char));
				p.nom = (char *)malloc(100*sizeof(char));
				p.prenom = (char *)malloc(100*sizeof(char));
				p.telefon = (char *)malloc(100*sizeof(char));
				p.adress = (char *)malloc(100*sizeof(char));
				char * info = (char *)malloc(100*sizeof(char));
				int c;

				printf("\tEntrer CIN : ");
				scanf("%s",p.cin);

				do{
					fflush(stdin);
					printf("\tEntrer CIN : ");
					scanf("%s",p.cin);
					while ((c = getchar ()) != '\n' && c != EOF);
				}while( verifierReg("[[:alpha:]]{2}[[:digit:]]",p.cin) != 1 );
				
				char nomFile1[100];
				char * nomFile2 = concat_string(p.cin,".txt\0");
				FILE* file = NULL;
				strcpy(nomFile1, nomFile2); 
				file = fopen(nomFile1,"w");
				
				do{
					fflush(stdin);
					printf("\tEntrer Nom : ");
					scanf("%s",p.nom);
				}while( verifierReg("[[:alpha:]]",p.nom) != 1 );

				do{
					fflush(stdin);
					printf("\tEntrer Prenom : ");
					scanf("%s",p.prenom);
					while ((c = getchar ()) != '\n' && c != EOF);
				}while( verifierReg("[[:alpha:]]",p.prenom) != 1 );


				do{
					fflush(stdin);
					printf("\tEntrer Telephon : ");
					scanf("%s",p.telefon);
					while ((c = getchar ()) != '\n' && c != EOF);
				}while( verifierReg("[[:digit:]]{10}",p.telefon) != 1 );


				do{
					fflush(stdin);
					printf("\tEntrer Adresse : ");
					scanf("%s",p.adress);
					while ((c = getchar ()) != '\n' && c != EOF);
				}while( verifierReg("[[:alnum:]]",p.telefon) != 1 );
				
				
				
				fprintf(file, "CIN:%s\nNOM:%s\nPRENOM:%s\nTELEPHONE:%s\nADRESSE:%s\n",p.cin,p.nom,p.prenom,p.telefon,p.adress);

				if( strcmp(arr1[1],"services") == 0){

					do{
						fflush(stdin);
						printf("\tEntrer fonction : ");
						scanf("%s",info);
						while ((c = getchar ()) != '\n' && c != EOF);
					}while( verifierReg("[[:alpha:]]",info) != 1 );
					fprintf(file, "FONCTION:%s\n",info);

				}else if( strcmp(arr1[1],"filieres") == 0){

					do{
						fflush(stdin);
						printf("\tEntrer semstre : ");
						scanf("%s",info);
						while ((c = getchar ()) != '\n' && c != EOF);
					}while( verifierReg("[[:digit:]]",info) != 1 );
					fprintf(file, "SEMSTRE:%s\n",info);

				}else{
					fprintf(file, "DEPARTEMENT:%s\n",arr1[2]);
				}

				
				

				fclose(file);
				changerRepertoire("..");changerRepertoire("..");
			}else changerRepertoire("..");
		}	
		
}





//Supprimer une entree
void removeEntry(char **arr1){
		if(changerRepertoire(arr1[1]) == 1){
			if( changerRepertoire(arr1[2]) == 1){
				char * nomFile2 = concat_string(arr1[3],".txt\0");

				if(remove(nomFile2) == 0)
					printf("%s est supprimee\n",arr1[3] );
				else printf("%s n'est pas supprimée\n",arr1[3]);

				changerRepertoire("..");changerRepertoire("..");
			}else changerRepertoire("..");
		}
}





//Rechercher une entrée
void findEntry(char * entryName,const char *name, int level)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            findEntry(entryName,path, level + 1);
        }
        else{
        	if( strcmp(entry->d_name,entryName) == 0){
        		char path[1024];
            	int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            	printf("Entree trouvee :  %s (%s)\n",  entry->d_name,path);
            	FILE* file = fopen(path,"r");
            	char ligne[200];
            	while(fgets(ligne, 200 , file) != NULL)
					{
					        printf("\t%s", ligne);
					}

				fclose(file);
        	}
        }
    } while (entry = readdir(dir));
    closedir(dir);
}





//Lister l'annuaire
void list(const char *name, int level)
{
   
    int count = 0;
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            if(level == 0){
            	 fprintf(stdout,"====================================================================================\n");
           		 printf("%*s %s\n", level*3, "", entry->d_name);
           		}else printf("%*s %s\n", level*3, "", entry->d_name);
            list(path, level + 1);
        }
        else{
            	printf("%*s* %s\n", level*3, "", entry->d_name);

        }
    } while (entry = readdir(dir));
    closedir(dir);
}





//Afficher les entrees en les triant par le critere
void display(const char *name, int level, char * critere)
{
    DIR *dir;
    int test = 0;
    struct dirent *entry;
    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            display(path, level + 1,critere);
        }
        else{

        	if( strcmp(critere,"") == 0){
        		char path[1024];
            	int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            	printf("%s (%s) :\n",  entry->d_name,path);
            	FILE* file = fopen(path,"r");
            	char ligne[200];
            	while(fgets(ligne, 200 , file) != NULL)
					{
					        printf("\t%s", ligne);
					}
				fclose(file);
			}else{
				test = 1;
				char path[1024];
            	int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            	FILE* file = fopen(path,"r");
            	char ligne[200];
            	personne * newPersonne = (personne *) malloc(sizeof(personne));
            	while(fgets(ligne, 200 , file) != NULL)
					{
					        char ** data = splitLine(ligne,":");
					        if( strcmp(data[0],"CIN") == 0){
					        	newPersonne->cin = (char*)malloc(sizeof(char)*sizeof(data[1]));
					        	strncpy(newPersonne->cin,data[1],strlen(data[1])-1);
					        }else if( strcmp(data[0],"NOM") == 0){
					        	newPersonne->nom = (char*)malloc(sizeof(char)*sizeof(data[1]));
					        	strncpy(newPersonne->nom,data[1],strlen(data[1])-1);
					        }else if( strcmp(data[0],"PRENOM") == 0){
					        	newPersonne->prenom = (char*)malloc(sizeof(char)*sizeof(data[1]));
					        	strncpy(newPersonne->prenom,data[1],strlen(data[1])-1);
					        }else if( strcmp(data[0],"TELEPHONE") == 0){
					        	newPersonne->telefon = (char*)malloc(sizeof(char)*sizeof(data[1]));
					        	strncpy(newPersonne->telefon,data[1],strlen(data[1])-1);	
					        }else if( strcmp(data[0],"ADRESSE") == 0){
					        	newPersonne->adress = (char*)malloc(sizeof(char)*sizeof(data[1]));
					        	strncpy(newPersonne->adress,data[1],strlen(data[1])-1);  	
					        }
					}
				ajouterPersonne(newPersonne);
				fclose(file);
			}
			
        	
        }
    } while (entry = readdir(dir));
    closedir(dir);

    if(test == 1){
    	trier(critere);
    	head = NULL;
    }
}






//Execution d'une commande
int executerCmd(char cmd[]){
	
	int res = 1;	
	char **arr1 = splitLine(cmd," ");
	int i = 0;
	

		//La commande add
		if( strcmp(arr1[0],"add") == 0 ){

			if( c1 == 2){
				if( creerInstitut(arr1[1]) == 1){
					emplacement = (char *) malloc (sizeof(char) * sizeof(arr1[1]));
					strcpy(emplacement,arr1[1]);
				}
			}else if( c1 == 3){

				if(changerRepertoire(arr1[1]) == 1){
						creerRepertoire(arr1[2]);
						changerRepertoire("..");
				}else{
					fprintf(stderr, "%s n'est pas correct ! (services, departements ou bien filieres)\n",arr1[1]);
				}

			}else{
				printf("La commande n'est pas correct\n");
			}








		//La commande remove
		}else if( strcmp(arr1[0],"remove") == 0 ){

			if( c1 == 2){
				supprimerInstitut(arr1[1]);
				strcpy(emplacement,"presidence");
			}else if( c1 == 3 ){

					if(changerRepertoire(arr1[1]) == 1){
						char * rep = ( char * )malloc( sizeof(char)*(sizeof(arr1[2])-1) );
						strcpy( rep ,arr1[2]);
						supprimerRep(rep);
						changerRepertoire("..");
						printf("%s est supprimé\n",rep);
					}else{
						fprintf(stderr, "%s n'est pas correct ! (services, departements ou bien filieres)\n",arr1[1]);
						changerRepertoire("..");
					}

			}else{
				printf("La commande n'est pas correct\n");
			}









		//La commande rmoveEntry
		}else if( strcmp(arr1[0],"removeEntry") == 0 ){

			if( c1 == 4){
				removeEntry(arr1);
			}else printf("La commande n'est pas correct\n");







		//La commande select
		}else if( strcmp(arr1[0],"select") == 0 ){

			if(c1 == 2){
				changerRepertoire("..");
				if( changerRepertoire(arr1[1]) == 1){
					emplacement = (char *) malloc (sizeof(char) * sizeof(arr1[1]));
					strcpy(emplacement,arr1[1]);
					printf("Vous avez selectionne %s\n",emplacement);
				}else changerRepertoire(emplacement);
			}else{
				printf("La commande n'est pas correct\n");
			}







		//La commande modify
		}else if( strcmp(arr1[0],"modify") == 0 ){

				if( c1 == 2){

					changerRepertoire("..");
					if( rename (emplacement,arr1[1]) == 0){
						printf("%s est modifié à %s\n",emplacement,arr1[1]);
						strcpy(emplacement,arr1[1]);
					}
					else
						perror("");
			
					changerRepertoire(emplacement);


				}else if( c1 == 4){

					if(changerRepertoire(arr1[1]) == 1){
						if(rename(arr1[2],arr1[3]) == 0){ 
							printf("%s est modifié à %s\n",arr1[2],arr1[3]);
						}else perror("");
						changerRepertoire("..");
					}else printf("Verifier l'ancien nom est ce qu'il est correct (%s,%s) !\n",arr1[1],arr1[2]);

				}else printf("La commande n'est pas correct !\n");







		//La commande modifyEntry
		}else if( strcmp(arr1[0],"modifyEntry") == 0 ){

				if( c1 == 4){

					char * entryName = arr1[3];
					changerRepertoire("..");
					changerRepertoire("..");
					findEntry(concat_string(entryName,".txt"),racine, 0);
					changerRepertoire(racine);
					changerRepertoire(emplacement);

					printf("\nEntrer les nouveaux informations\n");
					addEntry(arr1);

				}else  printf("La commande n'est pas correct !\n");





		//La commande ls
		}else if( strcmp(arr1[0],"ls") == 0 ){

			if(c1 == 1){
					list(".", 0);
			}else if( c1 == 2){
				if( strcmp(arr1[1],"*") == 0){
					changerRepertoire("..");
					changerRepertoire("..");
					list(racine, 0);
					changerRepertoire(racine);
					changerRepertoire(emplacement);
				}else{
					if( strcmp(arr1[1],"filieres") || strcmp(arr1[1],"departements") || strcmp(arr1[1],"services"))
						list(arr1[1],0);
					else printf("%s n'est pas correct ( filieres, departements ou bien services)",arr1[1]);
				}

			}else if( c1 == 3){
				char * chemin;
				chemin = (char*)malloc(sizeof(char)*255);
				strcpy(chemin,arr1[1]);
				strcat(chemin,"/");
				strcat(chemin,arr1[2]);
				list(chemin,0);
			}else 
				printf("La commande n'est pas correct\n");






			//La comande display
			}else if( strcmp(arr1[0],"display") == 0 ){

			if( c1 == 3){
				char * chemin;
				chemin = (char*)malloc(sizeof(char)*255);
				strcpy(chemin,arr1[1]);
				strcat(chemin,"/");
				strcat(chemin,arr1[2]);
				display(chemin,0,"");
			}else if( c1 == 4){
				char * chemin;
				chemin = (char*)malloc(sizeof(char)*255);
				strcpy(chemin,arr1[1]);
				strcat(chemin,"/");
				strcat(chemin,arr1[2]);
				display(chemin,0,arr1[3]);
			}else 
				printf("La commande n'est pas correct\n");






		//La commande addEntry
		}else if( strcmp(arr1[0],"addEntry") == 0 ){
			if(c1 == 3){
				addEntry(arr1);
			}else
				printf("La commande n'est pas correct\n");





		//La commande find
		}else if( strcmp(arr1[0],"find") == 0 ){

			if( c1 == 2){
					char * entryName = arr1[1];
					changerRepertoire("..");
					changerRepertoire("..");
					findEntry(concat_string(entryName,".txt"),racine, 0);
					changerRepertoire(racine);
					changerRepertoire(emplacement);

			}else printf("La commande n'est pas correct");




		//La commande help
		}else if( strcmp(arr1[0],"help") == 0 ){
			menuModifier();




		//La commande exit
		}else if( strcmp(arr1[0],"exit") == 0 ){
			res = -1;
		}else{
			printf("la commande n'existe pas.\n");
		}

	return res;	

}




//MAIN
int main(int argc,char * argv[]){


		if( argc == 2){


			racine = (char*)malloc(sizeof(char)*sizeof(argv[1]));
			strcpy(racine,argv[1]);
			if(initialiserAnnuaire()==1){
				printf( "=== Initialisation d'annuaire est termine.\n");
				menuModifier();
				int res = 0;


				do{
					fflush(stdout);
					fflush(stdin);
					fflush(stderr);
					char cmd[200];
					couleur("36");
					fprintf(stdout,"\n====================================================================================\n");
					printf("%-12s>>  ",emplacement);
					scanf("%[^\n]",cmd);
					fprintf(stdout,"====================================================================================\n\n");
					couleur("0");
					int c;
					while ((c = getchar ()) != '\n' && c != EOF);
		 
					res = executerCmd(cmd);
					memset (cmd, 0, sizeof (cmd));
				}while( res != -1 );

			}else{
				printf("=== Initialisation d'annuaire n'est pas termine.\n");
			}



		}else printf("Syntax : ./annuaire [nom d'univirste]");

	couleur("0");
	return 0;
}















//Afficher le menu d'aide
void menuModifier(){
	fprintf(stdout,"====================================================================================\n"); 
	fprintf(stdout,"====================== ** Manupilation d'institut ** ===============================\n");
	fprintf(stdout,"====================================================================================\n");
	fprintf(stdout,"=== 1) Ajouter/Supprimer institut ou bien service/departement/filiere     ==========\n");
	fprintf(stdout,"===    - add/remove [institut]                                            ==========\n"); 
	fprintf(stdout,"===    - add/remove [services|departements|filieres] [nom]                ==========\n"); 
	fprintf(stdout,"=== 2) Ajouter/Supprimer/modifier une entree                              ==========\n");
	fprintf(stdout,"===    - addEntry [service|departement|filiere] [nom]                     ==========\n");
	fprintf(stdout,"===    - modifyEntry [services|departements|filieres] [nom] [cin]         ==========\n");
	fprintf(stdout,"===    - removeEntry [services|departements|filieres] [nom] [cin]         ==========\n"); 
	fprintf(stdout,"=== 3) Modifier institut ou bien service/departement/filiere              ==========\n");
	fprintf(stdout,"===    - modify [nouveau nom]                                             ==========\n"); 
	fprintf(stdout,"===    - modify [service|departement|filiere] [ancienNOM] [nouveauNOM]    ==========\n");      
	fprintf(stdout,"=== 4) Rechercher dans l'annuaire                                         ==========\n");        
	fprintf(stdout,"===    - find [cin]                                                       ==========\n");
	fprintf(stdout,"=== 5) Lister dans l'annuaire                                             ==========\n"); 
	fprintf(stdout,"===    - ls                                                               ==========\n");       
	fprintf(stdout,"===    - ls *                                                             ==========\n");
	fprintf(stdout,"===    - ls [filieres|services|departements] [nom]                        ==========\n");
	fprintf(stdout,"=== 5) Afficher des entrees                                               ==========\n"); 
	fprintf(stdout,"===    - display [filieres|services|departements] [nom]                   ==========\n"); 
	fprintf(stdout,"===    - display [filieres|services|departements] [nom] [critere]         ==========\n");         
	fprintf(stdout,"=== 5) Vous voulez aide !                                                 ==========\n");
    fprintf(stdout,"===    - help                                                             ==========\n");  
    couleur("34");   
	fprintf(stdout,"====================================================================================\n");
	fprintf(stdout,"====================================================================================\n");
	couleur("0");
}