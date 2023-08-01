#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


//Main
int main(void) {
    char input[10]= {0}; //Eingabevariable
    struct node* head = NULL;                       //List-head

    do {                                            //Do-While f�r wiederholte Eingabe
        printf("\nInput: ");
        scanf("%s", &input[0]);                     //Eingabe -> [0] (f�r einen character)

        if(input[0]!='=') {                         //Wenn '=' eingegeben wird soll es keinen Node adden!
            head=addLast(head,inputToNode(input));  //neuen (neu erzeugten) Node in die Liste adden
            printf("\nTerm: ");
            printTerm(head);                        //Ausgabe von der Liste
        }
    } while(input[0]!='=');

    do {
        head=doMath(head);                          //Terme berechnen
        printf("\nResulting term: ");
        printTerm(head);
    } while(head->next!=NULL);                      //Wiederholt die Funktionen bis nurmehr ein Node �brig bleibt (= Ergebnis)

    free(head);                                     //L�st das letzte Node

    return 0;
}
