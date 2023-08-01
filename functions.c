#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

/*//-------------------------------------------------------------------------------
//Parameter:        +head ... Erstes Node der Liste
//Rückgabewert:     -
//Funktion:         Ausgabe aller Nodes aus der Liste
*///-------------------------------------------------------------------------------
void printTerm(struct node* head) {
    struct node* temp = head;                       //temporäres Node um Liste durchzugehen
    while(temp != NULL) {                           //Schleife geht Liste durch (bis kein Knoten mehr vorhanden)
        if(temp->type!=number_type) {               //Wenn Knoten keine Zahl -> Rechenzeichen ausgeben (oder Klammer)
            printf("%c", temp->operator);
        } else {
            printf("%d", temp->number);             //Wenn Knoten Zahl -> Nummer ausgeben
        }
        temp = temp->next;
    }
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node *inputToNode(const char *input) {
    int number = atoi(input);
    if (number != 0) {
        return createNumberNode(number);
    }
    if (input[0] == '0') {
        return createNumberNode(0);
    }
    if (strcmp(input, "(") == 0 ||
            strcmp(input, ")") == 0) {
        return createParenthesisNode(input[0]);
    }

    if (strcmp(input, "+") == 0 ||
            strcmp(input, "-") == 0 ||
            strcmp(input, "*") == 0 ||
            strcmp(input, "/") == 0) {
        return createOperatorNode(input[0]);
    }
    return NULL;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* createParenthesisNode(char operator) {
    struct node* newNode =
    (struct node*)malloc(sizeof(struct node));      //Speicherver reservieren
    if (newNode == NULL) {                          //Fehlerüberprüfung (Speicher nicht reservierbar)
        printf("Speicherfehler!\n");
        exit(1);
    }
    newNode->operator=operator;                     //Initialisieren von Werten für Knoten
    newNode->type = parenthesis_type;
    newNode->next = NULL;
    return newNode;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* createOperatorNode(char operator) {
    struct node* newNode =
    (struct node*)malloc(sizeof(struct node));      //Speicherver reservieren
    if (newNode == NULL) {                          //Fehlerüberprüfung (Speicher nicht reservierbar)
        printf("Speicherfehler!\n");
        exit(1);
    }
    newNode->operator=operator;
    newNode->type = operator_type;
    newNode->next = NULL;
    return newNode;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* createNumberNode(int number) {
    struct node* newNode =
    (struct node*)malloc(sizeof(struct node));      //Speicherver reservieren
    if (newNode == NULL) {                          //Fehlerüberprüfung (Speicher nicht reservierbar)
        printf("Speicherfehler!\n");
        exit(1);
    }
    newNode->number=number;                         //Initialisieren von Werten für Knoten
    newNode->type = number_type;
    newNode->next = NULL;
    return newNode;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* addLast(struct node *head, struct node *newTail) {
    if (head == NULL) {                             //Spezialfall -> Keine Knoten in der Liste vorhanden -> head = newTail
        return newTail;
    }

    struct node* temp = head;                       //temporäres Node um Liste durchzugehen
    while (temp->next != NULL) {                    //Schleife die bis zum letzten Knoten geht -> wenn nächstes Node = NULL -> letzter Knoten
        temp = temp->next;
    }
    temp->next = newTail;                           //Neues Node wird angehängt an vorherigen letzten Knoten
    return head;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* findLastParenthesisClosed(struct node *head) {
    struct node* temp = head;                       //temporäres Node um Liste durchzugehen
    struct node* last = NULL;                       //Variable um die letzte Klammer einzuspeichern und zu retournieren

    while(temp != NULL) {                           //Schleife geht die Liste durch bis kein Knoten mehr vorhanden
        if(temp->type==parenthesis_type             //Wenn eine ')' erreicht wurde, dann wird diese gespeichert
           && temp->operator==')') {
            last = temp;
        }
        temp = temp->next;
    }
    return last;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* findFirstPointOperator(struct node *startNode) {
    struct node* temp = startNode;                  //temporäres Node um Liste durchzugehen

    while(temp != NULL) {                           //Schleife geht die Liste durch bis kein Knoten mehr vorhanden
        if(temp->type==operator_type&&
          (temp->operator=='*'
           || temp->operator=='/')) {               //Wenn ein '/' oder '*' erreicht wurde, dann wird dieser retourniert
            return temp;
        }else if(temp->type==parenthesis_type       //Wenn ')' vorkommt wird NULL retourniert (Grund-> dient als "Range" um Klammern auszurechnen)
                 &&temp->operator==')'){
        return NULL;
        }
        temp = temp->next;
    }
    return NULL;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* findFirstDashOperator(struct node *startNode) {
    struct node* temp = startNode;                  //temporäres Node um Liste durchzugehen

    while(temp != NULL) {                           //Schleife geht die Liste durch bis kein Knoten mehr vorhanden
        if(temp->type==operator_type&&
          (temp->operator=='+'
           || temp->operator=='-')) {               //Wenn ein '+' oder '-' erreicht wurde, dann wird dieser retourniert
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

//---------------------------------------------------------------------------------
//SIEHE ANGABE
//---------------------------------------------------------------------------------
struct node* findPrevious(struct node *head, struct node *node) {
    struct node* temp = head;                       //temporäres Node um Liste durchzugehen

    while(temp != NULL) {                           //Schleife geht die Liste durch bis kein Knoten mehr vorhanden
        if(temp->next==node) {                      //Wenn der nächste Knoten mit dem gesuchten übereinstimmt wird dieser retourniert
            return temp;
        }
        temp = temp->next;
    }
    return NULL;                                    //NULL wenn es keinen vorherigen Knoten gibt
}

/*//-------------------------------------------------------------------------------
//Parameter:        +head ... Erstes Node der Liste
//Rückgabewert:     -node ... Erstes (potentiell neues) Node der Liste
//Funktion:         Steuert Rechenaufgaben, Klammersetzung und auflösen von Knoten
*///-------------------------------------------------------------------------------
struct node *doMath(struct node *head) {
    struct node* temp=head;                         //temporäres Node um Liste durchzugehen
    struct node* help = NULL;                       //Hilfe-Node um die jeweils aufzulösenden Nodes nicht zu verlieren


//----------------------------------------------
//NOTE: Zur Verständnis habe ich folgendes Beispiel:
//Vor der Schleife: [(] -> [3] -> [)] -> [NULL oder Operator]

    while(temp != NULL) {                           //Schleife geht die Liste durch bis kein Knoten mehr vorhanden
        if(temp->operator=='(') {                   //temp = '(' für diesen Durchgang
            if(temp->next->next->operator==')') {   //temp->next->next (ist die ')')
                help=temp->next->next->next;        //Null(/)  gespeichert
                free(temp->next->next);             //')' vom Speicher freigeben
                temp->next->next = help;            //nach 3 -> nächster Knoten (NULL)

                if(findPrevious(head,temp)!=NULL) { //Wenn die erste Klammer einen Vorgänger-Knoten hat (also '(' = head)
                    temp=findPrevious(head,temp);   //temp ist der vorherige Knoten (also 3)
                    help=temp->next->next;          //3 gespeichert in help
                    free(temp->next);               //'(' vom Speicher freigeben
                    temp->next=help;                //temp -> 3
                } else {
                    temp=temp->next;                //temp -> 3
                    free(findPrevious(head,temp));  // '(' vom speicher freigeben
                    head=temp;                      //Neuer erster Knoten -> 3
                }
            }
        }
        temp = temp->next;
    }
//nach der Schleife: [3] -> [/]
//----------------------------------------------
    temp = findLastParenthesisClosed(head);         //Sucht/findet die letzte ')'
    if(temp!=NULL) {
        while(temp->operator!='(') {                //Falls es eine ')' gibt, geht die Liste (Richtung header) bis es eine '(' findet
            temp=findPrevious(head,temp);           //'(' wird als "Startpunkt" in temp für Rechnungen gespeichert
        }
    } else {
        temp=head;                                  //Wenn keine Klammern existieren wird der Startpunkt an den Anfang gesetzt
    }
//----------------------------------------------
    if(findFirstPointOperator(temp)!=NULL) {        //Überprüft auf Punktoperatoren ('*' und '/') in der Liste
        fixNodes(findPrevious(head,
        findFirstPointOperator(temp)),              //Übergibt die Zahl vor dem Operator
        findFirstPointOperator(temp));              //Übergibt den Operator
    } else if(findFirstDashOperator(temp)!=NULL){   //Überprüft auf Strichoperatoren ('+' und '-') in der Liste
        fixNodes(findPrevious(head,
        findFirstDashOperator(temp)),               //Übergibt die Zahl vor dem Operator
        findFirstDashOperator(temp));               //Übergibt den Operator
    }
    return head;
}

/*//-------------------------------------------------------------------------------
//Parameter:        +beg ... Zahlen-Node vor dem Operator
//                  +end ... Operator-Node
//Rückgabewert:     -node ... Erstes (potentiell neues) Node der Liste
//Funktion:         Setzt Rechenoperationen durch und löscht die überflüssigen Nodes
*///-------------------------------------------------------------------------------
void fixNodes(struct node *beg,struct node *end) {

    switch (end->operator) {                        //Switch zum auswählen der passenden Rechenoperation
    case '+':
        beg->number+=end->next->number;
        break;
    case '-':
        beg->number-=end->next->number;
        break;
    case '*':
        beg->number*=end->next->number;
        break;
    case '/':
        if(end->next->number!=0) {                  //Erlaubt keine Divisionen durch 0
            beg->number/=end->next->number;
        }
        break;
    default:
        printf("Fehler");
        break;
    }

    beg->next = end->next->next;                    //Verknüpft das Ergebnis (Zahl vor dem Operator) mit dem nächsten Operator/NULL
    free(end->next);                                //Gibt die Zahl nach dem Operator frei Speicher
    free(end);                                      //Gibt den Operator frei vom Speicher
}
