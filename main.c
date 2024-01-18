#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "function.c"
int main()
{
    struct polynome *P = NULL; // polynome P
    struct polynome *Q = NULL; // polynome Q
    char command[100];         // commande qui va contenir la commande saisie par l'utilisateur
    while (1)                  // boucle infinie garante que le programme ne s'arrete pas
    {
        printf(">> ");
        fgets(command, 100, stdin);                        // saisie de la commande en utilisant fgets qui permet de saisir une chaine de caractere sans depasser la taille de 100 et stdin pour lire a partir du clavier
        if (strcmp(cutString(command, 0, 4), "EXIT") == 0) // si la commande est EXIT on sort du calculatrice en liberant la memoire
        {
            if (P != NULL)
            {
                polynome *temp = P;
                polynome *prev = NULL;
                while (temp != NULL)
                {
                    prev = temp;
                    temp = temp->next;
                    free(prev);
                }
            }
            if (Q != NULL)
            {
                polynome *temp = P;
                polynome *prev = NULL;
                while (temp != NULL)
                {
                    prev = temp;
                    temp = temp->next;
                    free(prev);
                }
            }

            break;
        }

        if (strcmp(cutString(command, 0, 3), "LET") == 0) // si la commande est LET on cree un polynome ça dépend est ce que c'est P ou Q
        {
            if (strcmp(cutString(command, 4, 2), "P=") == 0) // en utilisat la fonction cutString pour couper la commande et l'analyser
            {
                if (P == NULL)
                {
                    P = LET(command + 6);
                }
                else
                {
                    printf("POLYNOME N'EST PAS VIDE\n");
                }
            }
            else if (strcmp(cutString(command, 4, 1), "Q") == 0)
            {
                if (Q == NULL)
                {
                    Q = LET(command + 6);
                }
                else
                {
                    printf("POLYNOME N'EST PAS VIDE\n");
                }
            }
        }
        else if (strcmp(cutString(command, 0, 3), "SET") == 0) // si la commande est SET on modifie un polynome ça dépend est ce que c'est P ou Q
        {
            if (strcmp(cutString(command, 4, 2), "P=") == 0)
            {
                if (P == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    SET(&P, command + 6);
                }
            }
            else if (strcmp(cutString(command, 4, 2), "Q=") == 0)
            {
                if (Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    SET(&Q, command + 6);
                }
            }
        }
        else if (strcmp(cutString(command, 0, 3), "INT") == 0) // si la commande est INT on calcule l'integrale d'un polynome ça dépend est ce que c'est P ou Q
        {
            if (strcmp(cutString(command, 4, 1), "P") == 0)
            {
                if (P == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    polynome *integrale = INT(P);
                    DISPLAY(integrale, 0);
                }
            }
            else if (strcmp(cutString(command, 4, 1), "Q") == 0)
            {
                if (Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    polynome *integrale = INT(Q);
                    DISPLAY(integrale, 0);
                }
            }
        }
       else if (strcmp(cutString(command, 0, 6), "AFFECT") == 0)
        {
            if (strcmp(cutString(command, 7, 3), "P,Q") == 0)
            {
                if (P == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    AFFECT(&P, &Q);
                    DISPLAY(P,0);
                    DISPLAY(Q,0);
                }
            }
            else if (strcmp(cutString(command, 7, 3), "Q,P") == 0)
            {
                if (Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    AFFECT(&Q,&P);
                    DISPLAY(P,0);
                    DISPLAY(Q,0);
                }
            }
        }
        else if (strcmp(cutString(command, 0, 3), "DER") == 0) // si la commande est DER on calcule la derive du polynome choisi
        {
            if (strcmp(cutString(command, 4, 1), "P") == 0)
            {
                if (P == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    polynome *derive = DER(P); // appel de la fonction DER et on stocke le resultat dans un polynome derive
                    DISPLAY(derive, 0);        // affichage du polynome derive
                }
            }
            else if (strcmp(cutString(command, 4, 1), "Q") == 0)
            {
                if (Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    polynome *derive = DER(Q);
                    DISPLAY(derive, 0);
                }
            }
        }
        else if (strcmp(cutString(command, 0, 3), "POW") == 0) // si la commande est POW on calcule la puissance du polynome
        {
            if (strcmp(cutString(command, 4, 2), "P,") == 0)
            {
                int n;
                if (sscanf(cutString(command, 6, 1), " %d", &n) == 1)
                {
                    if (P == NULL)
                    {
                        printf("POLYNOME EST VIDE\n");
                    }
                    else
                    {
                        polynome *puissance = POW(P, n); // appel de la fonction POW et on stocke le resultat dans un polynome puissance
                        DISPLAY(puissance, 0);           // affichage du polynome puissance
                    }
                }
            }
            else if (strcmp(cutString(command, 4, 1), "Q") == 0)
            {
                int n;
                if (sscanf(cutString(command, 6, 1), " %d", &n) == 1)
                {
                    if (Q == NULL)
                    {
                        printf("POLYNOME EST VIDE\n");
                    }
                    else
                    {
                        polynome *puissance = POW(Q, n);
                        DISPLAY(puissance, 0);
                    }
                }
            }
        }
        else if (strcmp(cutString(command, 0, 3), "MUL") == 0) // si la commande est MUL on calcule la multiplication de deux polynomes exists
        {
            if (strcmp(cutString(command, 4, 3), "P,Q") == 0 || strcmp(cutString(command, 4, 3), "Q,P") == 0)
            {
                if (P == NULL || Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    polynome *multiplication = MUL(P, Q); // appel de la fonction MUL et on stocke le resultat dans un polynome multiplication
                    DISPLAY(multiplication, 0);           // affichage du polynome multiplication
                }
            }
        }
        else if (strcmp(cutString(command, 0, 3), "ADD") == 0) // si la commande est ADD on calcule l'addition ou la soustraction de deux polynomes
        {
            if (strcmp(cutString(command, 4, 3), "P,Q") == 0 || strcmp(cutString(command, 4, 3), "Q,P") == 0)
            {
                if (P == NULL)
                {
                    printf("P EST VIDE\n");
                }
                else if (Q == NULL)
                {
                    printf("Q EST VIDE\n");
                }
                else
                {
                    polynome *addition = ADD(P, Q, 0); // appel de la fonction ADD et on stocke le resultat dans un polynome addition
                    printf("P + Q = ");
                    DISPLAY(addition, 0); // affichage du polynome addition
                }
            }
        }
        else if (strcmp(cutString(command, 0, 4), "SOUS") == 0) // si la commande est SOUS on calcule la soustraction de deux polynomes
        {
            if (strcmp(cutString(command, 5, 3), "P,Q") == 0)
            {
                if (P == NULL)
                {
                    printf("P EST VIDE\n");
                }
                else if (Q == NULL)
                {
                    printf("Q EST VIDE\n");
                }
                else
                {
                    polynome *soustraction = ADD(P, Q, 1); // appel de la fonction ADD avec op=1 c'est à dire la soustraction et on stocke le resultat dans un polynome soustraction
                    printf("P - Q = ");
                    DISPLAY(soustraction, 0); // affichage du polynome résultat de la soustraction
                }
            }
            else if (strcmp(cutString(command, 5, 3), "Q,P") == 0)
            {
                if (P == NULL)
                {
                    printf("P EST VIDE\n");
                }
                else if (Q == NULL)
                {
                    printf("Q EST VIDE\n");
                }
                else
                {
                    polynome *soustraction = ADD(Q, P, 1); // appel de la fonction ADD avec op=1 c'est à dire la soustraction et on stocke le resultat dans un polynome soustraction
                    printf("Q - P = ");
                    DISPLAY(soustraction, 0); // affichage du polynome résultat de la soustraction
                }
            }
        }
        else if (strcmp(cutString(command, 0, 7), "DISPLAY") == 0) // si la commande est DISPLAY on affiche le polynome choisi
        {
            if (strcmp(cutString(command, 8, 1), "P") == 0)
            {
                if (P == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    printf("P=");
                    DISPLAY(P, 0);
                }
            }
            else if (strcmp(cutString(command, 8, 1), "Q") == 0)
            {
                if (Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    printf("Q=");
                    DISPLAY(Q, 0);
                }
            }
            else
            {
                printf("command incorrect\n");
            }
        }
        else if (strcmp(cutString(command, 0, 4), "EVAL") == 0) // si la commande est EVAL on calcule la valeur du polynome choisi pour une valeur de X
        {
            if (strcmp(cutString(command, 5, 8), "P for X=") == 0)
            {
                if (P == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    printf("%.2f\n", EVAL(P, atoi(command + 13))); // atoi pour convertir la chaine de caractere en entier
                }
            }
            else if (strcmp(cutString(command, 5, 8), "Q for X=") == 0)
            {
                if (Q == NULL)
                {
                    printf("POLYNOME EST VIDE\n");
                }
                else
                {
                    printf("%.2f\n", EVAL(Q, atoi(command + 13)));
                }
            }
            else
            {
                printf("command incorrect\n");
            }
        }
        else
        {
            printf("Invalid command.\n");
        }
    }
    return 0;
}
