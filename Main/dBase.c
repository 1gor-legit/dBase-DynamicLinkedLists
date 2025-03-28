#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>
#include <conio2.h>
#include "commands.h"

int main(){
	
    DBF *arqaux;
    char command[50];
    Unidade *unid;
	Init(&unid);
	InitDiscos(&unid);

    TelaPrincipal(unid);

    do{
        textbackground(0);
        textcolor(15);
        gotoxy(27, 20);
        gets(command);
        TransformaMAIUSCULA(command);

        if (strncmp(command, "SET DEFAULT TO ", 15) == 0)
            SET_DEFAULT_TO(&unid, command);
        
        else if (strncmp(command, "CREATE ", 7) == 0)
            CREATE(&unid, command);

        else if (strcmp(command, "DIR") == 0){}

        else if (strcmp(command, "QUIT") == 0){}

        else if (strcmp(command, "USE") == 0){}

        else if (strcmp(command, "LIST STRCTURE") == 0){}

        else if (strcmp(command, "APPEND") == 0){}

        else if (strcmp(command, "LIST") == 0){}

        else if (strcmp(command, "CLEAR") == 0){}

        else if (strcmp(command, "LOCATE") == 0){}

        else if (strcmp(command, "GOTO") == 0){}

        else if (strcmp(command, "DISPLAY") == 0){}

        else if (strcmp(command, "EDIT") == 0){}

        else if (strcmp(command, "DELETE") == 0){}

        else if (strcmp(command, "RECALL") == 0){}

        else if (strcmp(command, "SET DELETED") == 0){}

        else if (strcmp(command, "PACK") == 0){}

        else if (strcmp(command, "ZAP") == 0){}
		
	}while(strcmp(command, "EXIT") != 0);
    
    DIR(unid -> u);
    
    USE(&(unid -> u -> arq), "CLIENTES.DBF");
    
    char *valores[] = {"123", "Joao Silva", "99706-5566"};
	APPEND(unid -> u -> arq, valores);
	
	char *valores1[] = {"140", "Igor Silva", "99730-2876"};
	APPEND(unid -> u -> arq, valores1);
	
	char *valores2[] = {"169", "Eduardo Silva", "99671-9766"};
	APPEND(unid -> u -> arq, valores2);
	
	char *valores3[] = {"222", "Lucas Malagueta", "11111-1111"};
	APPEND(unid -> u -> arq, valores3);

    return 0;
}