#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Enum
enum node_type {
    number_type = 1, operator_type, parenthesis_type
};
//Node
struct node {
    char operator;
    int number;
    enum node_type type;
    struct node* next;
};
//Deklaration von eigenen Funktionen
void printTerm(struct node* temp);
struct node* doMath(struct node *head);
void fixNodes(struct node *beg,struct node *end);
//Deklaration von Funktionen aus der Angabe
struct node* inputToNode(const char *input);
struct node* createParenthesisNode(char operator);
struct node* createOperatorNode(char operator);
struct node* createNumberNode(int number);
struct node* addLast(struct node *head, struct node *newNode);
struct node* findLastParenthesisOpen(struct node *head);
struct node* findFirstPointOperator(struct node *startNode);
struct node* findFirstDashOperator(struct node *startNode);
struct node* findPrevious(struct node *head, struct node *node);
