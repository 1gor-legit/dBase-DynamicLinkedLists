#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>
#include "commands.h"

int main(){
	
	Unidade *unid;
	Init(&unid);
	InitDiscos(&unid);
    DBF *arqaux;
    
    SET_DEFAULT_TO(&unid, "D:");
    
    CREATE(&(unid->u), "CLIENTES.DBF", 2, 1, 2025, "10:00");
    CREATE(&(unid->u), "PRODUTOS.DBF", 11, 3, 2025, "16:13");
	
    //Adicionando campos ao DBF
    MODIFY_STRUCTURE(&(unid -> u -> arq), "CLIENTES.DBF", "FONE", "CHARACTER", 11, 0);
    MODIFY_STRUCTURE(&(unid -> u -> arq), "CLIENTES.DBF", "NOME", "CHARACTER", 20, 0);
    MODIFY_STRUCTURE(&(unid -> u -> arq), "CLIENTES.DBF", "CODIGO", "NUMERIC", 8, 0);
    
    //Comandos do dBase
    DIR(unid -> u);
    
    USE(&(unid -> u -> arq), "CLIENTES.DBF");
	
    LIST_STRUCTURE(unid -> u, unid -> u -> arq);
    
    char *valores[] = {"123", "Joao Silva", "99706-5566"};
	APPEND(unid -> u -> arq, valores);
	
	char *valores1[] = {"140", "Igor Silva", "99730-2876"};
	APPEND(unid -> u -> arq, valores1);
	
	char *valores2[] = {"169", "Eduardo Silva", "99671-9766"};
	APPEND(unid -> u -> arq, valores2);
	
	char *valores3[] = {"222", "Lucas Malagueta", "11111-1111"};
	APPEND(unid -> u -> arq, valores3);

    /*TESTANDO O COMANDO "PACK"

    CLEAR();
    LIST(unid -> u -> arq);
    LOCATE(unid -> u -> arq, "NOME", "Igor Silva");
    GOTO(unid -> u -> arq, 2);
    DELETE(unid -> u -> arq);
    PACK(unid -> u -> arq);
    LIST(unid -> u -> arq);
    getchar();*/
    
    //Liberando memoria
    LiberarCampos(unid -> u -> arq -> campos);
    free(unid -> u -> arq);
    free(unid -> u);
    
    QUIT();
    return 0;
}