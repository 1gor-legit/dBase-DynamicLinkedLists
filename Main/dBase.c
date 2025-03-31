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

                else if (strcmp(command, "CLEAR") == 0){
                    CLEAR(unid, arqAberto);
                }

                else if (strcmp(command, "LOCATE") == 0){}

                else if (strcmp(command, "GOTO") == 0){}

                else if (strcmp(command, "DISPLAY") == 0){}

                else if (strcmp(command, "EDIT") == 0){}

                else if (strcmp(command, "DELETE") == 0){}

                else if (strcmp(command, "DELETE ALL") == 0){}

                else if (strcmp(command, "RECALL") == 0){}

                else if (strcmp(command, "SET DELETED") == 0){}

                else if (strcmp(command, "PACK") == 0){}

                else if (strcmp(command, "ZAP") == 0){}

            } while(strcmp(command, "BACK") != 0);
        }

        else if (strcmp(command, "CLEAR") == 0){}

        else if (strcmp(command, "SET DELETED") == 0){}

        textbackground(0);
        textcolor(15);
		
	}while(strcmp(command, "EXIT") != 0);

    return 0;
}