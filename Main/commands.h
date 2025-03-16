struct data{
	int d, m, a;
};
typedef struct data Data;

union tipo{
    int num;
    char data;
    char logico;
    char character[50];
    char memo[50];
};
typedef union tipo tipo;

struct registros{
    tipo tipoDado;
    struct registros *prox;
};
typedef struct registros reg;

struct Status{
	char info;
	struct status *prox;
};
typedef struct Status Status;

struct campo{
	reg *pAtual;
    char nomeCampo[50];
    char tipoDado[10]; //NUMERIC, DATE, LOGICAL, CHARACTER, MEMO
    int tam;
    int dec;
    reg *dados;
    struct campo *prox;
};
typedef struct campo Campo;

struct DBFFile{
	char nomearq[50];
	Data data;
	char hora[6];
	Status status;
	Campo *campos;
	struct DBFFile *ant, *prox;
};
typedef struct DBFFile DBF;

struct armazenamento{
    char unidade[2]; // 'D' ou 'C'
    DBF *arq; // Lista encadeada de arquivos dentro do drive
    struct armazenamento *ant, *prox;
};
typedef struct armazenamento Armaz;

struct unidade{
	Armaz *u;
};
typedef struct unidade Unidade;

//INIT CABE�A DAS UNIDADES C: E D:
void Init(Unidade **U){
	*U = (Unidade*)malloc(sizeof(Unidade));
}

//CRIA DISCOS
void InitDiscos(Unidade **unid){
	
	Armaz *C, *D;
	
	C = (Armaz*)malloc(sizeof(Armaz));
	D = (Armaz*)malloc(sizeof(Armaz));
	
	D -> ant = C;
	D -> prox = NULL;
	D -> arq = NULL;
	strcpy(D -> unidade, "D:");
	
	C -> prox = D;
	C -> ant = NULL;
	C -> arq = NULL;
	strcpy(C -> unidade, "C:");
	
	(*unid) -> u = C;
	printf("Unidades criadas!\n");
}

//INSERINDO NO INICIO
void MODIFY_STRUCTURE(DBF**dbf, char *arq, char *nomeCampo, char tipo[10], int tam, int dec){
	
    Campo *novoCampo = (Campo*)malloc(sizeof(Campo));
    
    strcpy(novoCampo -> nomeCampo, nomeCampo);
    strcpy(novoCampo -> tipoDado, tipo);
    novoCampo -> tam = tam;
    novoCampo -> dec = dec;
    novoCampo -> pAtual = novoCampo -> dados = NULL;
    
    DBF *aux = *dbf;
    while(aux != NULL && stricmp(aux -> nomearq, arq) != 0)
    	aux = aux -> prox;
    
    if(aux != NULL){
    	novoCampo -> prox = aux -> campos;
    	aux -> campos = novoCampo;
    }
}

void LiberarCampos(Campo *C){
	
	Campo *aux;
	
	while(C != NULL){
		
		aux = C;
		C = C -> prox;
		free(aux);
	}
}

void SET_DEFAULT_TO(Unidade **unid, char *unidade){
	
	if(stricmp(unidade, "C:") == 0 || stricmp(unidade, "D:") == 0){
		if(stricmp((*unid) -> u -> unidade, unidade) != 0){
			if((*unid) -> u -> prox == NULL)
				(*unid) -> u = (*unid) -> u -> ant;
			else
				(*unid) -> u = (*unid) -> u -> prox;
		}
		printf("Disco alterado!\n");
	}
	else
		printf("Unidade invalida!\n");
}

void CREATE(Armaz**disco, char *nome, int dia, int mes, int ano, char *hora){
	
    DBF *novoarq = (DBF*)malloc(sizeof(DBF));
    
    strcpy(novoarq -> nomearq, nome);
    novoarq -> data.d = dia;
    novoarq -> data.m = mes;
    novoarq -> data.a = ano;
    strcpy(novoarq -> hora, hora);
	novoarq -> status.info = 'A';
    novoarq -> campos = NULL;
    novoarq -> ant = NULL;
    novoarq -> prox = NULL;
    
    if((*disco) -> arq == NULL)
    	(*disco) -> arq = novoarq;
    
    else{
    	(*disco) -> arq -> ant = novoarq;
    	novoarq -> prox = (*disco) -> arq;
    	(*disco) -> arq = novoarq;
    }
}

void DIR(Armaz*a){
	
	int cont = 0, total = 0;
	
	if(a -> arq == NULL)
		printf("Disco vazio!\n");
	
	else{
		
		printf("%s %12s %14s %7s\n", "Database Files", "# Records", "Last Update", "Size");
		
		DBF *aux = a -> arq;
		while(aux != NULL){
			
			Campo *c = a -> arq -> campos;
	
			while(c != NULL){
				total += c -> tam;
				c = c -> prox;
			}
			
			printf("%s %10d %9.2d/%.2d/%d %7d\n", aux -> nomearq, cont, aux -> data.d, aux -> data.m, aux -> data.a, total);
			cont++;
			aux = aux -> prox;
		}
	}
}

void USE(DBF**arq, char *nomearq){
	while(*arq != NULL && stricmp((*arq) -> nomearq, nomearq) != 0)
		*arq = (*arq) -> prox;
}

void LIST_STRUCTURE(Armaz *a, DBF *arq){
	
	int total = 0, cont = 1;
	
	if(arq != NULL){
		
		printf("Structure for database: %s\\%s\n", a -> unidade, arq -> nomearq);
	    printf("Number of data records: %5d\n", 0);
	    printf("Date of last update   : %.2d/%.2d/%d\n", arq -> data.d, arq -> data.m, arq -> data.a);
	    
	    Campo *c = arq -> campos;
	    
	    if(c != NULL){
	    	printf("------------------------------------------\n");
		    printf("%-5s   %-10s %-10s %3s %4s\n", "Field", "FieldName", "Type", "Width", "Dec");
		    printf("------------------------------------------\n");
		    while(c != NULL){
		        printf("%5d   %-10s %-10s %5d %3d\n", cont, c -> nomeCampo, c -> tipoDado, c -> tam, c -> dec);
		        total += c -> tam;
		        cont++;
		        c = c -> prox;
		    }
		    printf("** Total ** %23d\n", total);
		    printf("------------------------------------------\n");
	    }
	    
	    else
	    	printf("Nenhum campo registrado.\n");
	}
	
	else
		printf("Erro: Nenhum arquivo encontrado!\n");
}

void APPEND(DBF *arq, char **valores) {
    
    if(arq != NULL){
    	
        int i = 0;
        Campo *campoAtual = arq -> campos;
    
        while(campoAtual != NULL){
            reg *novoReg = (reg*)malloc(sizeof(reg));
            novoReg -> prox = NULL;
            
            // Preenchendo o novo registro baseado no tipo do campo
            if(stricmp(campoAtual -> tipoDado, "NUMERIC") == 0)
                novoReg -> tipoDado.num = atoi(valores[i]);
            
            else if(stricmp(campoAtual -> tipoDado, "CHARACTER") == 0)
                strncpy(novoReg -> tipoDado.character, valores[i], campoAtual -> tam);
    
            // Inserindo o registro na lista encadeada
            if(campoAtual -> dados == NULL)
				campoAtual -> pAtual = campoAtual -> dados = novoReg;
            
            else{
                reg *temp = campoAtual -> dados;
                while(temp -> prox != NULL)
                    temp = temp -> prox;
                
                temp -> prox = novoReg;
            }
            
            campoAtual = campoAtual -> prox;
            i++;
        }
        
        printf("Registro adicionado com sucesso!\n");
    }
}

void LIST(DBF *arq){
    
	if (arq != NULL || arq -> campos != NULL){
	    
	    printf("%s %9s %3s %19s\n", "Record#", "CODIGO", "NOME", "FONE");
	
	    int cont = 1, temRegistro = 1;
	    
	    while(temRegistro){
	    	
	    	temRegistro = 0;
	
	        Campo *campo = arq -> campos;
	        while (campo != NULL){
	        	
	            reg *registro = campo -> dados;
	            for (int i = 1; i < cont && registro != NULL; i++)
	                registro = registro -> prox;
	
	            if (registro != NULL)
	            	temRegistro = 1;
	
	            campo = campo -> prox;
	        }
	        
	        if(temRegistro){
	        	
	        	printf("      %d", cont);
	        	
	        	Campo *campo = arq -> campos;
	            while (campo != NULL){
	            	
	                reg *registro = campo -> dados;
                	for(int i = 1; i < cont && registro != NULL; i++)
                		registro = registro -> prox;
	
	                if (registro != NULL){
	                    if (stricmp(campo -> tipoDado, "NUMERIC") == 0)
	                        printf("   %7d ", registro -> tipoDado.num);
	                    
	                    else if (stricmp(campo -> tipoDado, "CHARACTER") == 0)
	                        printf("%-20s", registro -> tipoDado.character);
	                }
					
					else
	                    printf("%-20s", " ");

                	campo = campo -> prox;
	        	}
	        
	        	printf("\n");
	        	cont++;
	    	}
	
		    if (cont == 1)
		        printf("Nenhum registro encontrado.\n");
		}
	}
}

void CLEAR(){
	system("cls");
}

void LOCATE(Campo *c, char *campo, char *conteudo){

	while(c != NULL && stricmp(c -> nomeCampo, campo) != 0){
		printf("%s\n", c -> nomeCampo);
		c = c -> prox;
	}

	if(c != NULL){
		int record = 1;
		reg *r = c -> dados;
		while(r != NULL &&  stricmp(r -> tipoDado.character, conteudo) != 0){
			r = r -> prox;
			record++;
		}
		
		printf("Record =     %d\n", record);
	}

	else
		printf("Registro nao encontrado!\n");
}

void GOTO(DBF *arq, int record){

	int totalRegistros = 0;
    Campo *campo = arq -> campos;
	reg *registro = campo -> dados;

    if (campo != NULL)
        while (registro != NULL){
            totalRegistros++;
            registro = registro -> prox;
        }

    if (record < 1 || record > totalRegistros)
        printf("Registro fora do alcance!\n");

	else{
		while (campo != NULL){
			registro = campo -> dados;
			for (int i = 1; i < record && registro != NULL; i++)
				registro = registro -> prox;
			
			campo -> pAtual = registro;
			campo = campo -> prox;
		}
		printf("Registro %d acessado!\n", record);
	}
}

void DISPLAY(DBF *arq){

	int record = 1;

	Campo *c = arq -> campos;
	if(c != NULL)
		printf("Record#    ");

	while(c != NULL){

		if(stricmp(c -> tipoDado, "NUMERIC") == 0)
			printf("%-7s", c -> nomeCampo);

		if(stricmp(c -> tipoDado, "CHARACTER") == 0)
			printf("%-20s", c -> nomeCampo);
		
		c = c -> prox;
	}
	printf("\n");

	reg *busca = arq -> campos -> dados;

	if(stricmp(arq -> campos -> tipoDado, "NUMERIC") == 0)
		while(busca != NULL && busca != arq -> campos -> pAtual){
			record++;
			busca = busca -> prox;
		}
	
	if(stricmp(arq -> campos -> tipoDado, "CHARACTER") == 0)
		while(busca != NULL && stricmp(busca -> tipoDado.character, arq -> campos -> pAtual -> tipoDado.character) != 0){
			record++;
			busca = busca -> prox;
		}

	printf("      %-5d", record);

	c = arq -> campos;
	while(c != NULL){

		reg *registro = c -> pAtual;
        
		if(stricmp(c -> tipoDado, "NUMERIC") == 0)
			printf("   %-4d", registro -> tipoDado);

		if(stricmp(c -> tipoDado, "CHARACTER") == 0)
			printf("%-20s", registro);

        record++;
		c = c -> prox;
	}
}

void QUIT(){
	printf("\nENCERRANDO O PROGRAMA!");
	exit(0);
}