#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>
#include <conio2.h>
#include "commands.h"

int main(){
	
    DBF *arqAberto = NULL;
    Unidade *unid = NULL;
    char command[45] = {0};

	Init(&unid);

    do{
        TelaPrincipal(unid);
        textbackground(0);
        textcolor(15);
        gotoxy(27, 20);
        fflush(stdin);
        gets(command);
        TransformaMAIUSCULA(command);

        if (strncmp(command, "SET DEFAULT TO ", 15) == 0)
            SET_DEFAULT_TO(&unid, command);
        
        else if (strncmp(command, "CREATE ", 7) == 0)
            CREATE(&unid, command);

        else if (strcmp(command, "DIR") == 0)
            DIR(unid -> u);

        else if (strcmp(command, "QUIT") == 0){
            QUIT();
        }

        else if (strncmp(command, "USE ", 4) == 0){

            USE(unid, command, &arqAberto);

            do{
                if(arqAberto != NULL){
                    TelaPrincipal(unid);
                    gotoxy(25, 21);
                    printf("USE             %c<%s>%c%s                  %c                %c ", 186, unid -> u, 186, arqAberto -> nomearq, 186, 186);
                    textbackground(0);
                    textcolor(15);
                    gotoxy(45, 22);
                    printf("Type BACK to exit from file");
                    gotoxy(27, 20);
                    fflush(stdin);
                    gets(command);
                    TransformaMAIUSCULA(command);

                    if(strcmp(command, "LIST STRUCTURE") == 0)
                        LIST_STRUCTURE(unid -> u, arqAberto);

                    else if (strcmp(command, "APPEND") == 0){
                        APPEND(unid -> u -> arq);
                    }

                    else if (strcmp(command, "LIST") == 0){
                        LIST(unid -> u -> arq);
                    }

                    else if(strncmp(command, "LIST FOR ", 9) == 0){
                        LIST_FOR(unid -> u -> arq, command);
                    }

                    else if (strcmp(command, "CLEAR") == 0){
                        CLEAR(unid, arqAberto);
                    }

                    else if (strncmp(command, "LOCATE FOR ", 11) == 0){
                        LOCATE(arqAberto, command);
                    }

                    else if (strncmp(command, "GOTO ", 5) == 0){
                        GOTO(arqAberto, command);
                    }

                    else if (strcmp(command, "DISPLAY") == 0){
                        DISPLAY(arqAberto);
                    }

                    else if (strcmp(command, "EDIT") == 0){
                        EDIT(arqAberto);
                    }

                    else if (strcmp(command, "DELETE") == 0){
                        DELETE(arqAberto);
                    }

                    else if (strcmp(command, "DELETE ALL") == 0){
                        DELETE_ALL(arqAberto);
                    }

                    else if (strcmp(command, "RECALL") == 0){
                        RECALL(arqAberto);
                    }

                    else if (strcmp(command, "RECALL ALL") == 0){
                        RECALL_ALL(arqAberto);
                    }

                    else if (strcmp(command, "SET DELETED OFF") == 0){
                        SET_DELETED_OFF(arqAberto);
                    }

                    else if(strcmp(command, "SET DELETED ON") == 0){
                        SET_DELETED_ON(arqAberto);
                    }

                    else if (strcmp(command, "PACK") == 0){
                        PACK(arqAberto);
                    }

                    else if (strcmp(command, "ZAP") == 0){
                        ZAP(arqAberto);
                    }
                }
                else{
                    strcpy(command, "BACK");
                }

            } while(strcmp(command, "BACK") != 0);
        }

        else if (strcmp(command, "CLEAR") == 0){
            TelaPrincipal(unid);
        }

        else if (strcmp(command, "SET DELETED OFF") == 0){
            SET_DELETED_OFF(arqAberto);
        }

        else if(strcmp(command, "SET DELETED ON") == 0){
            SET_DELETED_ON(arqAberto);
        }

        textbackground(0);
        textcolor(15);
		
	}while(strcmp(command, "EXIT") != 0);

    return 0;
}