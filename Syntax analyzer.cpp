#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
 
//------------------------------------------------------------
// Declaration des variables 
//------------------------------------------------------------

FILE* file;
char Car_Cour;
char *p , chaine[20];

//------------------------------------------------------------
// DECLARATION DES CLASSES LEXICALES
//--------------------------------------------------------------

typedef enum {
ID_TOKEN,NUM_TOKEN, PROGRAM_TOKEN,CONST_TOKEN, VAR_TOKEN,BEGIN_TOKEN,END_TOKEN,IF_TOKEN,THEN_TOKEN,WHILE_TOKEN,DO_TOKEN,
READ_TOKEN,WRITE_TOKEN,PV_TOKEN,PT_TOKEN,PLUS_TOKEN,MOINS_TOKEN,MULT_TOKEN,DIV_TOKEN,VIR_TOKEN,AFF_TOKEN,INF_TOKEN,
INFEG_TOKEN,SUP_TOKEN,SUPEG_TOKEN,EG_TOKEN,DIFF_TOKEN,PO_TOKEN,PF_TOKEN,FIN_TOKEN,ERREUR_TOKEN,COMMENTAIRE_TOKEN,ELSE_TOKEN, EOF_TOKEN}Class_Lex;
Class_Lex Sym_Cour;

//------------------------------------------------------------------------------
// DECLARATION DES CLASSES DES ERREURS
//------------------------------------------------------------------------------

typedef enum{ 
ERR_CAR_INC,ERR_FICH_VID,ERR_PROGRAM,ERR_ID,ERR_PV,ERR_PT,ERR_EGAL,ERR_NUM,ERR_CONST,ERR_BEGIN,ERR_END,ERR_AFF,ERR_IF,ERR_WHILE,ERR_DO,
ERR_WRITE,ERR_READ,ERR_THEN,ERR_PO,ERR_PF,ERR_MOINS,ERR_PLUS,ERR_MULT,ERR_DIV,ERR_INF,ERR_SUP,ERR_INFEG,ERR_SUPEG,ERR_DIFF,ERR_CONST_VAR_BEGIN,
ERR_INST,ERR_COND}Erreurs;

//------------------------------------------------------------------------------
// DECLARATION DU TABLEAU DES ERREURS
//------------------------------------------------------------------------------
typedef struct { Erreurs CODE_ERR ;char mes[40]; } Code_Erreurs;
Code_Erreurs MES_ERR[40]={{ERR_CAR_INC ,"caract?re inconnu." },{ERR_FICH_VID,"fichier est vide"},{ERR_PROGRAM,"Erreur : PROGRAM"},{ERR_ID,"Erreur: ID"},
{ERR_PV,"Erreur: ';'  "},{ERR_PT," Erreur:'.' "},{ERR_EGAL,"Erreur : '=' "},{ERR_NUM,"Erreur : NUM"},{ERR_CONST,"Erreur : CONST"},
{ERR_CONST_VAR_BEGIN,"Erreur  : CONST VAR BEGIN"},{ERR_BEGIN,"Erreur :  BEGIN"},{ERR_END,"Erreur : END"},{ERR_AFF,"Erreur : AFFECTATION"},
{ERR_IF,"Erreur : IF"},{ERR_WHILE,"Erreur : WHILE"},{ERR_DO,"Erreur : DO"},{ERR_WRITE,"Erreur : WRITE"},{ERR_READ,"Erreur: READ"},
{ERR_THEN,"Erreur : THEN"},{ERR_PO,"Erreur : '('"},{ERR_PF,"Erreur : ')'"},{ERR_MOINS,"Erreur : '-'"},{ERR_PLUS,"Erreur : '+'"},{ERR_MULT,"Erreur : '*'"},
{ERR_DIV,"Erreur : '/'"},{ERR_INF,"Erreur :'<'"},{ERR_SUP,"Erreur: '>'"},	{ERR_INFEG,"Erreur : '<='"},{ERR_SUPEG,"Erreur : '>='"},
{ERR_COND,"Erreur : COND"},{ERR_DIFF,"Erreur : '<>'"}};




//------------------------------------------------------------
// Declaration des fonctions 
//------------------------------------------------------------

// partie analyseur lexicale :
void lire_mot();    						//  Lecture des mots
void lire_nombre();							//  Lecture des nombres
void Lire_Car();            				//  Lecture d'un caractere 
void Lire_special();						//  Lecture des symboles spécials
void Sym_Suiv();							//  Traiter selon les categores caractére par caractére
const char* AfficherToken(Class_Lex code);  //  Affechage des codes tokens
void Erreur(Erreurs ERR);					//  codage  des Erreurs

// partie analyseur syntaxique
void Test_Symbole(Class_Lex ct, Erreurs code_erreur);
void PROGRAM();
void BLOCK();
void CONSTS();
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void TANTQUE();
void ECRIRE();
void LIRE();
void COND();
void EXPR();
void TERM();
void FACT();

//------------------------------------------------------------
// Programme Principale 
//------------------------------------------------------------

int main()
{
printf(" **********************************************\n");
printf(" *   A N A L Y S E U R    S Y N T A X I Q U E *\n");
printf(" **********************************************\n \n");

                                                       
    file= fopen("testSyntaxique.text", "r+");				// Ouvrir un script écrit en langage Pascale
    Lire_Car();	                                    // Lire le premier caractère
    Sym_Suiv();  									// coder le premier symbole
    PROGRAM();										// debut de  l'analyse synatxique
	if (Sym_Cour==EOF_TOKEN)
	printf("BRAVO: le programme est correcte!!!");
	else printf("PAS BRAVO: fin de programme erronée!!!!");
	getch();
    return 1;
}

//------------------------------------------------------------
//AfficherToken(Class_Lex token)
//------------------------------------------------------------
const char* AfficherToken(Class_Lex token) {         // Affechage des codes tokens
   
   switch (token) 
   {
      case PROGRAM_TOKEN : return "PROGRAM_TOKEN";
      case NUM_TOKEN : return "NUM_TOKEN";
      case CONST_TOKEN : return "CONST_TOKEN";
      case VAR_TOKEN : return "VAR_TOKEN";
      case BEGIN_TOKEN : return "BEGIN_TOKEN";
      case END_TOKEN : return "END_TOKEN";
      case IF_TOKEN : return "IF_TOKEN";
      case THEN_TOKEN : return "THEN_TOKEN";
      case WHILE_TOKEN : return "WHILE_TOKEN";
      case DO_TOKEN : return "DO_TOKEN";
      case READ_TOKEN : return "READ_TOKEN";
      case WRITE_TOKEN : return "WRITE_TOKEN";
      case PV_TOKEN : return "PV_TOKEN";
      case PT_TOKEN : return "PT_TOKEN";
      case PLUS_TOKEN : return "PLUS_TOKEN";
      case MOINS_TOKEN : return "MOINS_TOKEN";
      case MULT_TOKEN : return "MULT_TOKEN";
      case DIV_TOKEN : return "DIV_TOKEN";
      case VIR_TOKEN : return "VIR_TOKEN";
      case AFF_TOKEN : return "AFF_TOKEN";
      case INF_TOKEN : return "INF_TOKEN";
      case INFEG_TOKEN : return "INFEG_TOKEN";
      case SUP_TOKEN : return "SUP_TOKEN";
      case SUPEG_TOKEN : return "SUPEG_TOKEN";
      case DIFF_TOKEN : return "DIFF_TOKEN";
      case PO_TOKEN : return "PO_TOKEN";
      case PF_TOKEN : return "PF_TOKEN";
      case FIN_TOKEN : return "FIN_TOKEN";
      case ID_TOKEN : return "ID_TOKEN";
      case ERREUR_TOKEN : return "ERREUR_TOKEN";
      case EOF_TOKEN : return "EOF_TOKEN";
      case EG_TOKEN : return "EG_TOKEN";
      case COMMENTAIRE_TOKEN : return "COMMENTAIRE_TOKEN";
      
   }
}



//------------------------------------------------------------
//Lire_Car()	
//------------------------------------------------------------


void Lire_Car()				      // cette fonction permet de lire caractére par caractére
{   	       
Car_Cour=fgetc(file);
}

//------------------------------------------------------------
//lire_nombre()	
//------------------------------------------------------------

void lire_nombre(){
  while(isdigit(Car_Cour))
  {
  	*p++ = Car_Cour;
    Lire_Car();  
  }
  
  if(!isalpha(Car_Cour))
  {
 	Sym_Cour=NUM_TOKEN;		 // codage des nombres
  }
  
}
//------------------------------------------------------------
//lire_mot()	
//------------------------------------------------------------


void lire_mot(){
  while(isdigit(Car_Cour) || isalpha(Car_Cour))
  {
  	*p++ = Car_Cour;
    Lire_Car();
    // codage des mots
    if(!strcmp(chaine,"program"))    Sym_Cour = PROGRAM_TOKEN;
    else if(!strcmp(chaine,"begin")) Sym_Cour = BEGIN_TOKEN;
    else if(!strcmp(chaine,"end"))   Sym_Cour = END_TOKEN;
    else if(!strcmp(chaine,"if"))    Sym_Cour = IF_TOKEN;
    else if(!strcmp(chaine,"then"))  Sym_Cour = THEN_TOKEN;
    else if(!strcmp(chaine,"read"))  Sym_Cour = READ_TOKEN;
    else if(!strcmp(chaine,"write")) Sym_Cour = WRITE_TOKEN;
    else if(!strcmp(chaine,"const")) Sym_Cour = CONST_TOKEN;
    else if(!strcmp(chaine,"var"))   Sym_Cour = VAR_TOKEN;
    else if(!strcmp(chaine,"while")) Sym_Cour = WHILE_TOKEN;
    else if(!strcmp(chaine,"do"))    Sym_Cour = DO_TOKEN;
    else Sym_Cour = ID_TOKEN;
  
      }
}

//------------------------------------------------------------
//Lire_special()	
//------------------------------------------------------------
void Lire_special()
   {
      switch (Car_Cour)  // codage des symboles spécials
	  {
        case '(' : Sym_Cour = PO_TOKEN; 
				   Lire_Car(); 
				   break;
        case ')' : Sym_Cour = PF_TOKEN; 
		           Lire_Car(); 
				   break;
        case ';' : Sym_Cour = PV_TOKEN; 
				   Lire_Car();
				   break;
        case ',' : Sym_Cour = VIR_TOKEN;
				   Lire_Car();
				    break;
        case '=' : Sym_Cour = EG_TOKEN; 
		           Lire_Car();
				   break;
        case '+' : Sym_Cour = PLUS_TOKEN;
		           Lire_Car(); 
				   break;
        case '-' : Sym_Cour = MOINS_TOKEN;
		           Lire_Car();
				   break;
        case '*' : Sym_Cour = MULT_TOKEN;
				   Lire_Car(); 
				   break;
        case '/' : Sym_Cour = DIV_TOKEN;
			 	   Lire_Car(); 
				   break;
        case '.' : Sym_Cour = PT_TOKEN;
				   Lire_Car();
				   break;   
        case ':' : Car_Cour = fgetc(file);
                  	if(Car_Cour == '=') Sym_Cour = AFF_TOKEN; 
                  	Lire_Car();
                  	break;        	
        case '>' : Car_Cour = fgetc(file); 
                  	if(Car_Cour == '=') Sym_Cour = SUPEG_TOKEN;
                  	else Sym_Cour = SUP_TOKEN;
                  	Lire_Car();
					break;     	
        case '<' : Car_Cour = fgetc(file); 
                  	if(Car_Cour == '=') Sym_Cour = INFEG_TOKEN;
                  	else if (Car_Cour == '>') Sym_Cour = DIFF_TOKEN;
                         else Sym_Cour = INF_TOKEN; 
                    Lire_Car();
					break;             
        case '{' :  Lire_Car();				// les commantires en pascale : {*Commantaire*}
        			if(Car_Cour=='*')
					{
						while(Car_Cour!=EOF)
						{
							Lire_Car();
        					if(Car_Cour=='*')
							{
        						Lire_Car();
        						if(Car_Cour=='}')
								{
        							Sym_Cour = COMMENTAIRE_TOKEN;
									break;
								}		
							}		
						}
					}	
					Lire_Car();
					break;
					
        case EOF : Sym_Cour = EOF_TOKEN;
					break;
        
        default:
        	Sym_Cour =ERREUR_TOKEN ;
			break;
       }
   }



//------------------------------------------------------------
//Sym_Suiv()	
//------------------------------------------------------------


void Sym_Suiv()  
{
	memset (chaine, 0, sizeof (chaine));
	p= chaine;
	
	while( isspace(Car_Cour) && Car_Cour!=EOF  )		//PASSER LES SEPARATEURS \t ,\n , espace , .....
	{        
    Lire_Car();    							// lire car_cour jusqu'à passer les séparateurs
    }
  	//**** TRAITER SELON LA CATEGORIE*****//

   if(isdigit(Car_Cour))	   			 // *** CATEGORIE DE NOMBRES***//
  		  lire_nombre();				//  *** si car_cour est un chiffre : lire_nombre()		
  		  
  		  
    else 
   {
    if(isalpha(Car_Cour))				// *** CATEGORIE DE MOTS***//
			lire_mot();     		   // *** si car_cour est une lettre : lire_mot()
			
			
    else Lire_special();			   //***CATEGORIE DES SPECIAUX***//
   }
   
}

//------------------------------------------------------------
//Erreur(Erreurs ERR)	
//------------------------------------------------------------
void Erreur(Erreurs ERR)
{
int ind_err=ERR;
printf( "Erreur numero %d \t  %s \n", ind_err, MES_ERR[ind_err].mes);
getch();
exit(1);
}


//------------------------------------------------------------
//	Test_Symbole()
//------------------------------------------------------------
void Test_Symbole(Class_Lex ct, Erreurs code_erreur){
	
	if(Sym_Cour==ct){
		Sym_Suiv();
	}
	else 
	  Erreur(code_erreur);
	
}
//------------------------------------------------------------
//	PROGRAM()
//------------------------------------------------------------

void PROGRAM()
{
Test_Symbole(PROGRAM_TOKEN, ERR_PROGRAM);
Test_Symbole(ID_TOKEN, ERR_ID);
Test_Symbole(PV_TOKEN, ERR_PV);
BLOCK();
Test_Symbole(PT_TOKEN, ERR_PT);
}
//------------------------------------------------------------
//	BLOCK();
//------------------------------------------------------------

void BLOCK()
{
	CONSTS();
	VARS();
	INSTS();
}

//------------------------------------------------------------
//CONSTS();    // CONSTS ::= const ID = NUM ; { ID = NUM ; } | e
//------------------------------------------------------------
void CONSTS() 
{	Sym_Suiv();
	switch (Sym_Cour) 
	{
	  case CONST_TOKEN : 
	  		  Test_Symbole(CONST_TOKEN,ERR_CONST);
	          Test_Symbole(ID_TOKEN, ERR_ID);
	          Test_Symbole(EG_TOKEN, ERR_EGAL);
	          Test_Symbole(NUM_TOKEN, ERR_NUM);                 
	          Test_Symbole(PV_TOKEN, ERR_PV); 
	          while (Sym_Cour ==ID_TOKEN){
	          Sym_Suiv();
	          Test_Symbole(EG_TOKEN, ERR_EGAL);
	          Test_Symbole(NUM_TOKEN, ERR_NUM);
	          Test_Symbole(PV_TOKEN,ERR_PV); };
			  
			   break;
	  case VAR_TOKEN: break;
	  case BEGIN_TOKEN: break;
	  default: Erreur(ERR_CONST_VAR_BEGIN);break;
	}
}



//------------------------------------------------------------
//	VARS()    //VARS ::= var ID { , ID } ; | e
//------------------------------------------------------------

void VARS()
{
	switch(Sym_Cour)
	{
		case VAR_TOKEN:Sym_Suiv();
						Test_Symbole(ID_TOKEN,ERR_ID);
						while(VIR_TOKEN==Sym_Cour)
            {
						  	Sym_Suiv();
						  	Test_Symbole(ID_TOKEN,ERR_ID);
						}
            Test_Symbole(PV_TOKEN,ERR_PV);
            break;
						  
		case BEGIN_TOKEN: break;
	}
	
}


//------------------------------------------------------------
//	INSTS()
//------------------------------------------------------------

void INSTS(){
	Test_Symbole(BEGIN_TOKEN,ERR_BEGIN);
	INST();
	while(Sym_Cour==PV_TOKEN){
		Sym_Suiv();
		INST();
	}
}

//------------------------------------------------------------
//	INST()
//------------------------------------------------------------

void INST(){
	
	switch(Sym_Cour){
		
		
		case BEGIN_TOKEN: INSTS();break;
		case ID_TOKEN:  
					AFFEC()	;break; 
						
		case IF_TOKEN: SI();break;
		case WHILE_TOKEN: TANTQUE();break;
		case WRITE_TOKEN: ECRIRE();break;
		case READ_TOKEN: LIRE();break;
		default: Erreur(ERR_INST);break;
	}
}

//------------------------------------------------------------
//	AFFEC()   //AFFEC ::= ID := EXPR
//------------------------------------------------------------

void AFFEC(){
	switch(Sym_Cour){
		case AFF_TOKEN: 
						Test_Symbole(AFF_TOKEN,ERR_AFF);
						break;
		default:
						
						break;
	}
}

//------------------------------------------------------------
//	SI()  //SI ::= if COND then INST
//------------------------------------------------------------

void SI(){
	
	Test_Symbole(IF_TOKEN,ERR_IF);
	COND();
	Test_Symbole(THEN_TOKEN,ERR_THEN);
	INST();
	switch(Sym_Cour){
			case ELSE_TOKEN :   
			                    INST(); 
								break;
			default : break;
		}
}


//------------------------------------------------------------
//	TANQUE
//------------------------------------------------------------

void TANTQUE(){
	Test_Symbole(WHILE_TOKEN,ERR_WHILE);
	COND();
	Test_Symbole(DO_TOKEN,ERR_DO);
	INST();
} 

//------------------------------------------------------------
//	ECRIRE()//ECRIRE ::= write ( EXPR { , EXPR } )
//------------------------------------------------------------


void ECRIRE(){
	Test_Symbole(WRITE_TOKEN,ERR_WRITE);
	Test_Symbole(PO_TOKEN,ERR_PO);
	EXPR();
	
	while(VIR_TOKEN==Sym_Cour){
		Sym_Suiv();
		EXPR();
	}
	Test_Symbole(PF_TOKEN,ERR_PF);
}
//------------------------------------------------------------
//	LIRE()
//------------------------------------------------------------
void LIRE(){
	Test_Symbole(READ_TOKEN,ERR_READ);
	Test_Symbole(PO_TOKEN,ERR_PO);
	Test_Symbole(ID_TOKEN,ERR_ID);
	while(VIR_TOKEN==Sym_Cour){
		Sym_Suiv();
		Test_Symbole(ID_TOKEN,ERR_ID);
	}
	Test_Symbole(PF_TOKEN,ERR_PF);
}


//------------------------------------------------------------
//	//COND ::= EXPR [= | <> | < | > | <= | >=] EXPR
//------------------------------------------------------------



void COND(){
	Sym_Suiv();
	EXPR();
	switch(Sym_Cour){
		case EG_TOKEN:     Test_Symbole(EG_TOKEN,ERR_EGAL);
							break;
		case DIFF_TOKEN:   Test_Symbole(DIFF_TOKEN,ERR_DIFF);					
							break;
		case INF_TOKEN:    Test_Symbole(INF_TOKEN,ERR_INF);							
							break;
		case SUP_TOKEN:    Test_Symbole(SUP_TOKEN,ERR_SUP);							
							break;
		case INFEG_TOKEN : Test_Symbole(INFEG_TOKEN,ERR_INFEG);						
							break;
		case SUPEG_TOKEN : Test_Symbole(SUPEG_TOKEN,ERR_SUPEG);						
							break;
		default: Erreur(ERR_COND); break;
	}
	EXPR();
	
}	


//------------------------------------------------------------
// EXPR ::= TERM { [+ | -] TERM }	
//------------------------------------------------------------

void EXPR() {  
	  TERM();   
	   while(Sym_Cour==PLUS_TOKEN||Sym_Cour==MOINS_TOKEN){     
		    Sym_Suiv();      
			TERM();    
			
		}
}
//------------------------------------------------------------
//	TERM ::= FACT { [* | /] FACT }
//------------------------------------------------------------

void TERM() {
	FACT(); 
	while(Sym_Cour==MULT_TOKEN||Sym_Cour==DIV_TOKEN) {
		Sym_Suiv();
		FACT();
	} 
}



//------------------------------------------------------------
//	FACT ::= ID | NUM | ( EXPR )
//------------------------------------------------------------
void FACT() {   
	switch(Sym_Cour) {        
		case ID_TOKEN: Sym_Suiv();
						break;  
					  
		case NUM_TOKEN: 
						Sym_Suiv(); 
						break;   
						 
		case PO_TOKEN: Sym_Suiv(); 
			           EXPR();  
			           Test_Symbole(PF_TOKEN,ERR_PF);
			           break;
		case PF_TOKEN: Sym_Suiv(); break;
   }
}


















