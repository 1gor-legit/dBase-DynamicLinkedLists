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
        gotoxy(25, 20);
        gets(command);
        TransformaMAIUSCULA(command);

        // CASO SET DEFAUL TO
        char aux[16];
        for (int i = 0; i < 15; i++)
            aux[i] = command[i];
        
        if (strcmp(aux, "SET DEFAULT TO") == 0)
            SET_DEFAULT_TO(&unid, command);
        // CASO SET DEFAUL TO
        
        // CASO CREATE
        if (strcmp(command, "CREATE") == 0){
            TelaCREATE(unid);
            CREATE(&(unid->u), "CLIENTES.DBF", 2, 1, 2025, "10:00");
            CREATE(&(unid->u), "PRODUTOS.DBF", 11, 3, 2025, "16:13");
        }
        // CASO CREATE

        if (strcmp(command, "DIR") == 0){}

        if (strcmp(command, "QUIT") == 0){}

        if (strcmp(command, "USE") == 0){}

        if (strcmp(command, "LIST STRCTURE") == 0){}

        if (strcmp(command, "APPEND") == 0){}

        if (strcmp(command, "LIST") == 0){}

        if (strcmp(command, "CLEAR") == 0){}

        if (strcmp(command, "LOCATE") == 0){}

        if (strcmp(command, "GOTO") == 0){}

        if (strcmp(command, "DISPLAY") == 0){}

        if (strcmp(command, "EDIT") == 0){}

        if (strcmp(command, "DELETE") == 0){}

        if (strcmp(command, "RECALL") == 0){}

        if (strcmp(command, "SET DELETED") == 0){}

        if (strcmp(command, "PACK") == 0){}

        if (strcmp(command, "ZAP") == 0){}
		
	}while(strcmp(command, "EXIT") != 0);
	
    //Adicionando campos ao DBF
    MODIFY_STRUCTURE(&(unid -> u -> arq), "CLIENTES.DBF", "FONE", "CHARACTER", 11, 0);
    MODIFY_STRUCTURE(&(unid -> u -> arq), "CLIENTES.DBF", "NOME", "CHARACTER", 20, 0);
    MODIFY_STRUCTURE(&(unid -> u -> arq), "CLIENTES.DBF", "CODIGO", "NUMERIC", 8, 0);
    
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