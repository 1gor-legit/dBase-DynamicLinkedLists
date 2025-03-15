#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>
#include "commands.h"

int main(){
	
	Unidade *unid;
	Init(&unid);
	InitDiscos(&unid);
    DBF *arqaux;
    
    //Escolhendo unidade de armazenamento
    SETDEFAULTTO(&unid, "D:");
    
    //Criando DBF
    CREATE(&(unid->u), "CLIENTES.DBF", 2, 1, 2025, "10:00");
    CREATE(&(unid->u), "PRODUTOS.DBF", 11, 3, 2025, "16:13");
	
    //Adicionando campos ao DBF
    addCampo(&(unid -> u -> arq), "CLIENTES.DBF", "FONE", "CHARACTER", 10, 0);
    addCampo(&(unid -> u -> arq), "CLIENTES.DBF", "NOME", "CHARACTER", 20, 0);
    addCampo(&(unid -> u -> arq), "CLIENTES.DBF", "CODIGO", "NUMERIC", 8, 0);
    
    //COMANDOS DO dBASE
    DIR(unid -> u);
    
    arqaux = unid -> u -> arq;
    USE(&arqaux, "CLIENTES.DBF");
	
    LISTSTRUCTURE(unid -> u, arqaux);
    
    //Liberando mem�ria
    LiberarCampos(unid -> u -> arq -> campos);
    free(unid -> u -> arq);
    free(unid -> u);
    
    QUIT();
    return 0;
}