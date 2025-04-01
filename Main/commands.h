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
typedef struct registros Reg;

struct Status{
	char info;
	struct status *prox;
};
typedef struct Status Status;

struct campo{
	Reg *pAtual;
    char nomeCampo[50];
    char tipo[10]; //NUMERIC, DATE, LOGICAL, CHARACTER, MEMO
    int tam;
    int dec;
    Reg *dados;
    struct campo *prox;
};
typedef struct campo Campo;

struct DBFFile{
	char nomearq[50];
	Data data;
	char hora[6];
	Status *status;
	Campo *campos;
	int DEL;
	struct DBFFile *ant, *prox;
};
typedef struct DBFFile DBF;

struct armazenamento{
    char unidade[3]; // "D:" ou "C:"
    DBF *arq; // Lista encadeada de arquivos dentro do drive
    struct armazenamento *ant, *prox;
};
typedef struct armazenamento Armaz;

struct unidade{
	Armaz *u;
};
typedef struct unidade Unidade;

void TelaPrincipal(Unidade *unid);
void Moldura(int CI, int LI, int CF, int LF, int Frente);
void TelaCREATE();

//INIT DAS UNIDADES C: E D: E FAZ A CABEÇA APONTAR PRA C:
void Init(Unidade **unid){

	Armaz *C, *D;
	
	*unid = (Unidade*)malloc(sizeof(Unidade));
	C = (Armaz*)malloc(sizeof(Armaz));
	D = (Armaz*)malloc(sizeof(Armaz));

	strcpy(D -> unidade, "D:");
	D -> ant = C;
	D -> prox = NULL;
	D -> arq = NULL;

	strcpy(C -> unidade, "C:");
	C -> prox = D;
	C -> ant = NULL;
	C -> arq = NULL;
	
	(*unid) -> u = C;
}

void SET_DEFAULT_TO(Unidade **unid, char command[]){

	char unidade[3];

	if(command[15] == 'D' || command[15] == 'C'){
		
		unidade[0] = command[15];
		unidade[1] = command[16];

		if(stricmp((*unid) -> u -> unidade, unidade) != 0){
			if((*unid) -> u -> prox == NULL)
				(*unid) -> u = (*unid) -> u -> ant;
			else
				(*unid) -> u = (*unid) -> u -> prox;
		}
	}

	else{
		TelaPrincipal(*unid);
		textbackground(0);
		textcolor(15);
		gotoxy(79, 20);
		printf("Unidade invalida!");
		getchar();
	}
}

void CREATE(Unidade **unid, char nomearq[]){

	char aux[20], campo[15], type[15];
	int i, width, dec;

	for (i = 7; nomearq[i] != ' '; i++)
		aux[i - 7] = nomearq[i];
	
	//Nova caixa p/ arquivo .DBF
    DBF *novoarq = (DBF*)malloc(sizeof(DBF));
    strcpy(novoarq -> nomearq, aux);
	strcat(novoarq -> nomearq, ".DBF");
    novoarq -> data.d = 9;
    novoarq -> data.m = 4;
    novoarq -> data.a = 2025;
    strcpy(novoarq -> hora, "10:00");
	novoarq -> status = NULL;
    novoarq -> campos = NULL;
	novoarq -> DEL = 1;
    novoarq -> ant = NULL;
    novoarq -> prox = NULL;

	novoarq -> prox = (*unid) -> u -> arq;
	if ((*unid) -> u -> arq != NULL)
		(*unid) -> u -> arq -> ant = novoarq;
	
	(*unid) -> u -> arq = novoarq;

	TelaCREATE(*unid);
	int y = 13, j = 1;
	TelaCAMPO(y, j);

	gotoxy(29, 13);
	fflush(stdin);
	gets(campo);
	TransformaMAIUSCULA(campo);

	while(stricmp(campo, "\0") != 0){
		gotoxy(41, y);
		fflush(stdin);
		scanf("%s", type);
		TransformaMAIUSCULA(type);
		gotoxy(52, y);
		scanf("%d", &width);
		gotoxy(57, y);
		scanf("%d", &dec);
		MODIFY_STRUCTURE(&((*unid) -> u -> arq), (*unid) -> u -> arq -> nomearq, campo, type, width, dec);
		textbackground(0);
		textcolor(15);
		y += 1;
		j += 1;
		TelaCAMPO(y, j);
		gotoxy(29, y);
		fflush(stdin);
		gets(campo);
		TransformaMAIUSCULA(campo);
	}
}

void DIR(Armaz *a){
	
	int cont = 1, total, y = 9;
	
	if(a -> arq == NULL){
		gotoxy(84, 20);
		printf("Disco vazio!");
	}
	
	else{
		
		gotoxy(26, 8);
		printf("Database Files    # Records    Last Update    Size");
		
		DBF *aux = a -> arq;
		while(aux != NULL){
			
			Campo *c = aux -> campos;
			total = 0;
	
			while(c != NULL){
				total += c -> tam;
				c = c -> prox;
			}
			
			gotoxy(26, y++);
			printf("%-23s%d        %.2d/%.2d/%d     %d", aux -> nomearq, cont, aux -> data.d, aux -> data.m, aux -> data.a, total);
			cont++;
			aux = aux -> prox;
		}
	}
	getchar();
}

void QUIT(){
	exit(0);
}

void USE(Unidade *unid, char *nomearq, DBF**arqAberto){
	char aux[20];
	DBF *arquivos = unid -> u -> arq;

	for (int i = 4; nomearq[i] != ' '; i++)
		aux[i - 4] = nomearq[i];

	while(arquivos != NULL && stricmp(arquivos -> nomearq, aux) != 0)
		arquivos = arquivos -> prox;

	if(arquivos != NULL){
		*arqAberto = arquivos;
		textbackground(15);
		textcolor(0);
		gotoxy(25, 21);
		printf("USE             %c<%s>%c%s                  %c                %c ", 186, unid -> u, 186, (*arqAberto) -> nomearq, 186, 186);
		gotoxy(95, 20);
		textbackground(0);
		textcolor(15);
		gotoxy(45, 22);
		printf("Type BACK to exit from file");
	}
	else{
		gotoxy(25, 20);
		printf("Arquivo %s nao existe!", aux);
		getchar();
	}
}

void LIST_STRUCTURE(Armaz *a, DBF *arqAberto){
	
	int total = 0, cont = 1;
	
	if(arqAberto != NULL){
		
		gotoxy(40, 4);
		printf("Structure for database: %s\\%s", a -> unidade, arqAberto -> nomearq);
	    gotoxy(40, 5);
		printf("Number of data records: %5d", 0);
	    gotoxy(40, 6);
		printf("Date of last update   : %.2d/%.2d/%d", arqAberto -> data.d, arqAberto -> data.m, arqAberto -> data.a);
	    
	    Campo *c = arqAberto -> campos;
		int y = 10;
	    
	    if(c != NULL){
			gotoxy(40, 7);
	    	printf("------------------------------------------\n");
		    gotoxy(40, 8);
			printf("%-5s   %-10s %-10s %3s %4s\n", "Field", "FieldName", "Type", "Width", "Dec");
		    gotoxy(40, 9);
			printf("------------------------------------------\n");
		    while(c != NULL){
				gotoxy(40, y++);
		        printf("%5d   %-10s %-10s %5d %3d", cont, c -> nomeCampo, c -> tipo, c -> tam, c -> dec);
		        total += c -> tam;
		        cont++;
		        c = c -> prox;
		    }
			gotoxy(40, y++);
		    printf("** Total ** %23d", total);
			gotoxy(40, y++);
		    printf("------------------------------------------");
	    }
	    
	    else{
			gotoxy(72, 20);
			printf("Nenhum campo registrado.");
		}
	    	
	}
	
	else{
		gotoxy(64, 20);
		printf("Erro: Nenhum arquivo encontrado!");
	}

	getchar();
}

void APPEND(DBF *arq) {
    
    if(arq != NULL){

		gotoxy(27, 20);
		printf("      ");
    	
        int dadoNum;
		char dadoChar[30];

		Status *novoCampoStatus = (Status*)malloc(sizeof(Status));
		novoCampoStatus -> info = 'T';
		novoCampoStatus -> prox = NULL;

		Status *aux = arq -> status;

		if(aux == NULL)
			arq -> status = novoCampoStatus;
		
		else{
			while(aux -> prox != NULL)
				aux = aux -> prox;
		
			aux -> prox = novoCampoStatus;
		}

        Campo *campoAtual = arq -> campos;
        while(campoAtual != NULL){
            Reg *novoReg = (Reg*)malloc(sizeof(Reg));
            novoReg -> prox = NULL;

			if(stricmp(campoAtual -> tipo, "NUMERIC") == 0){
				gotoxy(27, 20);
				scanf("%d", &dadoNum);
				novoReg -> tipoDado.num = dadoNum;
			}
            
            else if(stricmp(campoAtual -> tipo, "CHARACTER") == 0){
				gotoxy(27, 20);
				fflush(stdin);
				gets(dadoChar);
				strncpy(novoReg -> tipoDado.character, dadoChar, campoAtual -> tam);
			}
    
            if(campoAtual -> dados == NULL)
				campoAtual -> pAtual = campoAtual -> dados = novoReg;
            
            else{
                Reg *temp = campoAtual -> dados;
                while(temp -> prox != NULL)
                    temp = temp -> prox;
                
                temp -> prox = novoReg;
            }
            
            campoAtual = campoAtual -> prox;
        }
        
		gotoxy(64, 20);
        printf("Registro adicionado com sucesso!");
		getchar();
    }
}

void LIST(DBF *arq){
    
	if (arq != NULL && arq -> campos != NULL){
	
	    int cont = 1, i, y = 5;
		Status *statusAtual = arq -> status;

		if(statusAtual != NULL){
			gotoxy(40, 4);
			printf("%s %9s %3s %19s\n", "Record#", "CODIGO", "NOME", "FONE");
		}
		
	    while(statusAtual != NULL){
	        
	        if(statusAtual -> info == 'T'){
	        	
				gotoxy(40, y);
	        	printf("      %d", cont);
	        	
	        	Campo *campo = arq -> campos;
	            while (campo != NULL){

					gotoxy(40, y);
	        		printf("      %d", cont);
	            	
	                Reg *registro = campo -> dados;
                	for(i = 1; i < cont && registro != NULL; i++)
                		registro = registro -> prox;
	
	                if (registro != NULL){
	                    if (stricmp(campo -> tipo, "NUMERIC") == 0){
							printf("   %7d ", registro -> tipoDado.num);
						}
	                    
	                    else if (stricmp(campo -> tipo, "CHARACTER") == 0){
							printf("%-20s", registro -> tipoDado.character);
						}
	                }
					
					else{
						gotoxy(40, y);
						printf("%-20s", " ");
					}
	                    
                	campo = campo -> prox;
	        	}
				
	        	y++;
	    	}
			
			cont++;
			statusAtual = statusAtual -> prox;
		}

		if (cont == 1){
			gotoxy(69, 20);
			printf("Nenhum registro encontrado.");
		}
	}

	getchar();
}

void CLEAR(Unidade *unid, DBF *arqAberto){
	system("cls");
	TelaPrincipal(unid);
	gotoxy(25, 21);
	printf("USE             %c<%s>%c%s                  %c                %c ", 186, unid -> u, 186, arqAberto -> nomearq, 186, 186);     
}

void LOCATE(DBF *arq, char *command){
	char conteudo[20], campo[20];
	int i, j;
	for (i = 11; command[i] != ' '; i++)
		campo[i - 11] = command[i];
	campo[i - 11] = '\0';
	for (j = i + 4; command[j] != '"'; j++)
		conteudo[j - (i + 4)] = command[j];
	conteudo[j - (i + 4)] = '\0';

	printf("%s\n", campo);
	printf("%s", conteudo);
	getchar();
	
	Campo *c = arq -> campos;

	while(c != NULL && stricmp(c -> nomeCampo, campo) != 0){
		c = c -> prox;
	}

	if(c != NULL){
		int record = 1;
		Reg *r = c -> dados;
		Status *statusAtual = arq -> status;

		if(stricmp(c -> tipo, "CHARACTER") == 0)
            while (r != NULL && (statusAtual -> info == 'F' || stricmp(r -> tipoDado.character, conteudo) != 0)) {
                r = r -> prox;
                statusAtual = statusAtual -> prox;
                record++;
            }

        else if(stricmp(c -> tipo, "NUMERIC") == 0) {
            int codigo = atoi(conteudo);
            while(r != NULL && (statusAtual -> info == 'F' || r -> tipoDado.num != codigo)) {
                r = r -> prox;
                statusAtual = statusAtual -> prox;
                record++;
            }
        }

        if(r != NULL && statusAtual -> info == 'T')
            printf("Record =     %d\n", record);
        else
            printf("Registro nao encontrado!\n");
	}

	else
		printf("Campo nao encontrado!\n");

	getchar();
}

void GOTO(DBF *arq, int record){

	int totalRegistros = 0, i;
    Campo *campo = arq -> campos;
	Reg *registro = campo -> dados;

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
			for (i = 1; i < record && registro != NULL; i++)
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

		if(stricmp(c -> tipo, "NUMERIC") == 0)
			printf("%-7s", c -> nomeCampo);

		if(stricmp(c -> tipo, "CHARACTER") == 0)
			printf("%-20s", c -> nomeCampo);
		
		c = c -> prox;
	}
	printf("\n");

	Reg *busca = arq -> campos -> dados;

	if(stricmp(arq -> campos -> tipo, "NUMERIC") == 0)
		while(busca != NULL && busca != arq -> campos -> pAtual){
			record++;
			busca = busca -> prox;
		}
	
	if(stricmp(arq -> campos -> tipo, "CHARACTER") == 0)
		while(busca != NULL && stricmp(busca -> tipoDado.character, arq -> campos -> pAtual -> tipoDado.character) != 0){
			record++;
			busca = busca -> prox;
		}

	printf("      %-5d", record);

	c = arq -> campos;
	while(c != NULL){

		Reg *registro = c -> pAtual;
        
		if(stricmp(c -> tipo, "NUMERIC") == 0)
			printf("   %-4d", registro -> tipoDado.num);

		if(stricmp(c -> tipo, "CHARACTER") == 0)
			printf("%-20s", registro -> tipoDado.character);

        record++;
		c = c -> prox;
	}
}

void EDIT(DBF *arq){

	char novaString[50];
	int novoNum;

	Campo *c = arq -> campos;
	while(c != NULL){

		if(stricmp(c -> tipo, "CHARACTER") == 0)
			printf("%s     %s\n", toupper(c -> nomeCampo), c -> pAtual);
		
		else if(stricmp(c -> tipo, "NUMERIC") == 0)
			printf("%s   %d\n", toupper(c -> nomeCampo), c -> pAtual -> tipoDado);
		
		c = c -> prox;
	}

	c = arq -> campos;
	while(c != NULL){

		printf("\n%s: ", toupper(c -> nomeCampo));
		
		if(stricmp(c -> tipo, "CHARACTER") == 0){
			fflush(stdin);
			scanf("%s", novaString);
			strcpy(c -> pAtual -> tipoDado.character, novaString);
		}
		
		else if(stricmp(c -> tipo, "NUMERIC") == 0){
			fflush(stdin);
			scanf("%d", &novoNum);
			c -> pAtual -> tipoDado.num = novoNum;
		}
		c = c -> prox;
	}
}

void DELETE(DBF *arq){

	int pos = 0, i;
	
	Reg *auxReg = arq -> campos -> dados;
	while(auxReg != NULL && auxReg -> tipoDado.num != arq -> campos -> pAtual -> tipoDado.num){
		auxReg = auxReg -> prox;
		pos++;
	}

	Status *auxStatus = arq -> status;
	for(i = 0; i < pos; i++)
		auxStatus = auxStatus -> prox;
	
	auxStatus -> info = 'F';
}

void DELETE_ALL(DBF *arq){

	Status *auxStatus = arq -> status;

	while(auxStatus != NULL){
		auxStatus -> info = 'F';
		auxStatus = auxStatus -> prox;
	}
}

void RECALL(DBF *arq){

	int pos = 0, i;
	
	Reg *auxReg = arq -> campos -> dados;
	while(auxReg != NULL && auxReg -> tipoDado.num != arq -> campos -> pAtual -> tipoDado.num){
		auxReg = auxReg -> prox;
		pos++;
	}

	Status *auxStatus = arq -> status;
	for(i = 0; i < pos; i++)
		auxStatus = auxStatus -> prox;
	
	auxStatus -> info = 'T';
	printf("record recalled\n");
}

void RECALL_ALL(DBF *arq){

	if(arq -> DEL == 0){
		Status *auxStatus = arq -> status;

		while(auxStatus != NULL){
			auxStatus -> info = 'T';
			auxStatus = auxStatus -> prox;
		}
	}

	else
		printf("SET DELETED is ON, turn off to realize the operation!\n");
}

void SET_DELETED_OFF(DBF *arq){
	arq -> DEL = 0;
}

void SET_DELETED_ON(DBF *arq){
	arq -> DEL = 1;
}

void PACK(DBF *arq){

	int cont = 0, i;

	if (arq == NULL || arq->status == NULL || arq->campos == NULL) {
        printf("Nenhum arquivo encontrado!\n");
    }

	else{
		Status *auxStat = arq -> status; 
		Status *StatAnt = NULL;
	
		while(auxStat != NULL){
			if(auxStat -> info == 'F'){
				if(StatAnt == NULL)
					arq -> status = auxStat -> prox;
				else
					StatAnt -> prox = auxStat -> prox;

				Status *tempStat = auxStat;
				auxStat = auxStat -> prox;
				free(tempStat);

				Campo *c = arq -> campos;
				while(c != NULL){

					Reg *auxReg = c -> dados;
					Reg *RegAnt = NULL;

					if(cont == 0){
						c -> dados = auxReg -> prox;
						free(auxReg);
					}
					else{
						RegAnt = c -> dados;
						auxReg = RegAnt -> prox;
						for(i = 1; i < cont; i++){
							RegAnt = auxReg;
							auxReg = auxReg -> prox;
						}
						RegAnt -> prox = auxReg -> prox;
						free(auxReg);
					}
					c -> pAtual = c -> dados;
					c = c -> prox;
				}
				cont = 0;
			}
			else{
				StatAnt = auxStat;
				auxStat = auxStat -> prox;
				cont++;
			}
		}
	}
}

void ZAP(DBF *arq){

	Campo *c = arq -> campos;
	Reg *r = arq -> campos -> dados;
	Status *s = arq -> status;

	while(s != NULL){
		arq -> status = s -> prox;
		free(s);
		s = arq -> status;
	}

	while(c != NULL){
		while(r != NULL){
			c -> dados = r -> prox;
			free(r);
			r = c -> dados;
		}
		c -> dados = NULL;
		c -> pAtual =  NULL;
		c = c -> prox;
	}

	printf("Arquivo %s limpado com sucesso!\n", arq -> nomearq);
}

//INSERINDO NO INICIO
void MODIFY_STRUCTURE(DBF**dbf, char *arq, char *nomeCampo, char tipo[10], int tam, int dec){
	
    Campo *novoCampo = (Campo*)malloc(sizeof(Campo));
    
    strcpy(novoCampo -> nomeCampo, nomeCampo);
    strcpy(novoCampo -> tipo, tipo);
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
	else
		aux -> campos = novoCampo;
}

void TransformaMAIUSCULA(char str[]){

	int i;
	for(i = 0; i < strlen(str); i++)
		str[i] = toupper(str[i]);
}

///////////////////// INTERFACE /////////////////////////////

void Moldura(int CI, int LI, int CF, int LF, int Frente){
	
	textcolor(Frente);
	
	gotoxy(CI,LI);
	printf("%c", 201);
	gotoxy(CF,LI);
	printf("%c", 187);
	gotoxy(CI,LF);
	printf("%c", 200);
	gotoxy(CF,LF);
	printf("%c", 188);
	
	for(int a = CI + 1; a < CF; a++){
		gotoxy(a, LI);
		printf("%c", 205);
		gotoxy(a, LF);
		printf("%c", 205);
	}
	
	for(int a = LI + 1; a < LF; a++){
		gotoxy(CI, a);
		printf("%c", 186);
		gotoxy(CF, a);
		printf("%c", 186);
	}
	
	textcolor(7);
}

void TelaPrincipal(Unidade *unid){

	system("cls");

	gotoxy(25, 20);
	printf(". ");

	textbackground(15);
	Moldura(25, 21, 95, 21, 15);

	textcolor(0);
	gotoxy(25, 21);
	printf("Command Line    %c<%s>%c                       %c                   %c", 186, unid -> u, 186, 186, 186);
}

void TelaCREATE(Unidade *unid){
	
	system("cls");
	TelaPrincipal(unid);
	
	textbackground(15);
	textcolor(0);
	gotoxy(25, 21);
	printf("CREATE          %c<%s>%c%s", 186, unid -> u, 186, unid -> u -> arq -> nomearq);

	textbackground(0);

	//CAIXA 1
	Moldura(25, 5, 43, 10, 15);
	gotoxy(26, 6);
	printf(" CURSOR  <-- -->");
	gotoxy(26, 7);
	printf("  Char:    < >");
	gotoxy(26, 8);
	printf("  Word: Home End");
	gotoxy(26, 9);
	printf("  Pan:   ^< ^>");

	//CAIXA 2
	Moldura(43, 5, 57, 10, 15);
	gotoxy(44, 6);
	printf("    INSERT");
	gotoxy(44, 7);
	printf("  Char:  Ins");
	gotoxy(44, 8);
	printf("  Field: ^N");
	gotoxy(44, 9);
	printf("  Help:  F1");
	//CONECTORES CAIXA 2
	gotoxy(43, 5);
	printf("%c", 203);
	gotoxy(43, 10);
	printf("%c", 202);

	//CAIXA 3
	Moldura(57, 5, 72, 10, 15);
	gotoxy(58, 6);
	printf("     DELETE");
	gotoxy(58, 7);
	printf("  Char:");
	gotoxy(58, 8);
	printf("  Word:");
	gotoxy(58, 9);
	printf("  Field:");
	//CONECTORES CAIXA 3
	gotoxy(57, 5);
	printf("%c", 203);
	gotoxy(57, 10);
	printf("%c", 202);

	//CAIXA 4
	Moldura(72, 5, 95, 10, 15);
	gotoxy(73, 6);
	printf("  Up a field:    ^");
	gotoxy(73, 7);
	printf("  Down a field:  v");
	gotoxy(73, 8);
	printf("  Exit/Save:    ^End");
	gotoxy(73, 9);
	printf("  Abort:         Esc");
	//CONECTORES CAIXA 4
	gotoxy(72, 5);
	printf("%c", 203);
	gotoxy(72, 10);
	printf("%c", 202);


	//TEXTO DEBAIXO DO COMMAND LINE
	gotoxy(50, 22);
	printf("Enter the field name");
	gotoxy(21, 23);
	printf("Field names begin with a letter and may contain letter, digits and underscores");
}

void TelaCAMPO(int y, int j){
	//Cabeçalho dos campos
	textcolor(15);
	gotoxy(29, 11);
	printf("Field Name  Type     Width  Dec");
	gotoxy(29, 12);
	for(int i = 0; i <= 30; i++)
		printf("%c", 205);
	
	gotoxy(26, y);
	printf("%d", j);

	textcolor(0);
	textbackground(15);
	gotoxy(29, y);
	printf("          ");
	gotoxy(41, y);
	printf("         ");
	gotoxy(52, y);
	printf("   ");
	gotoxy(57, y);
	printf("   ");
}
