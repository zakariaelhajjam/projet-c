#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
typedef struct fraction
{
    int num;
    int denom;
} fraction;
typedef struct polynome
{
    fraction coefficient;
    int exposant;
    struct polynome *next;
} polynome;

// fonction permet de renvoyer un pointeur sur un monome
polynome *monome(fraction coeff, int exp)
{
    polynome *temp = (polynome *)malloc(sizeof(polynome));
    temp->coefficient.num = coeff.num;
    temp->coefficient.denom = coeff.denom;
    temp->exposant = exp;
    temp->next = NULL;
    return temp;
}

// Renvoie une sous chaine determiner par un debut et nombre de caractere données en arguments
char *cutString(char *source, int startIndex, int length)
{
    char *substring = (char *)malloc((length + 1) * sizeof(char));
    int i;
    for (int i = 0; i < length; i++) // on parcours la chaine source et on copie les caractères dans la
    {
        substring[i] = source[startIndex + i];
    }
    substring[length] = '\0'; // on met le caractère de fin de chaine
    return substring;
}

// faire une operation d'addition ou soustraction entre deux fraction , cela depend du troisième arguments s'il est 0 : somme
// sinon soustraction
fraction somme(fraction f1, fraction f2, int op)
{
    fraction result;
    if (op == 0)
    {
        result.num = (f1.num * f2.denom) + (f1.denom * f2.num);
    }
    else
    {
        result.num = (f1.num * f2.denom) - (f1.denom * f2.num);
    }
    result.denom = f1.denom * f2.denom;
    return result;
}

// cette fonction faire la division d'une fraction par un entier
fraction division(fraction f1, int exp)
{
    fraction result;
    result.num = f1.num;           // on garde le numérateur
    result.denom = f1.denom * exp; // on multiplie le dénominateur par l'entier
    return result;
}

// permet de faire le produit de deux fractions
fraction produit(fraction f1, fraction f2)
{
    fraction result;
    result.num = f1.num * f2.num;
    result.denom = f1.denom * f2.denom;
    return result;
}

polynome *LET(const char *input)
{
    polynome *result = NULL; // on crée un polynome vide
    polynome *current = NULL;
    int length = strlen(input); // on calcule la longueur de la chaine de caractère
    int i = 0;
    while (i < length) // on parcours la chaine de caractère
    {
        int sign = 1;
        if (input[i] == '-') // si le premier caractère est un moins on met le signe à -1
        {
            sign = -1;
            i++;
        }
        else if (input[i] == '+') // si le premier caractère est un plus on met le signe à 1
        {
            sign = 1;
            i++;
        }

        int coeff_num = 0;
        int coeff_denom = 1;
        while (isdigit(input[i])) // on parcours la chaine de caractère et on récupère le numérateur et le dénominateur
        {
            coeff_num = coeff_num * 10 + (input[i] - '0'); // on calcule le numérateur
            i++;
        }
        if (input[i] == '/') // si on trouve un slash on calcule le dénominateur
        {
            i++;
            coeff_denom = 0;
            while (isdigit(input[i])) // on parcours la chaine de caractère et on récupère le dénominateur
            {
                coeff_denom = coeff_denom * 10 + (input[i] - '0');
                i++;
            }
        }

        int exponent = 0;
        if (input[i] == 'X' || input[i] == 'x') // si on trouve un X ou x on calcule l'exposant
        {
            i++;
            if (input[i] == '^') // si on trouve un ^ on calcule l'exposant
            {
                i++;
                while (isdigit(input[i])) // on parcours la chaine de caractère et on récupère l'exposant
                {
                    exponent = exponent * 10 + (input[i] - '0'); // on calcule l'exposant
                    i++;
                }
            }
            else // sinon l'exposant est égale à 1
            {
                exponent = 1;
            }
        }

        fraction coeff;
        coeff.num = sign * coeff_num;
        coeff.denom = coeff_denom;
        polynome *monomee = monome(coeff, exponent);

        if (result == NULL) // si le polynome est vide on ajoute le monome au début
        {
            result = monomee;
            current = monomee;
        }
        else
        {
            polynome *prev = NULL;
            polynome *temp = result;
            while (temp != NULL && temp->exposant > exponent) // on parcours le polynome et on cherche la position du monome
            {
                prev = temp;
                temp = temp->next;
            }

            if (temp != NULL && temp->exposant == exponent) // si on trouve un monome avec le mÃªme exposant on fait la somme
            {
                temp->coefficient = somme(temp->coefficient, monomee->coefficient, 0); // on fait la somme des deux monomes
                free(monomee);
            }
            else // sinon on ajoute le monome à la bonne position
            {
                if (prev == NULL) // si le monome est ajouté au début
                {
                    monomee->next = result;
                    result = monomee;
                }
                else // sinon on ajoute le monome à la bonne position
                {
                    monomee->next = prev->next;
                    prev->next = monomee;
                }
            }
        }

        while (isspace(input[i])) // on parcours la chaine de caractère et on saute les espaces
        {
            i++;
        }
    }
    return result;
}

void SET(polynome **poly, char *miseajour) // SET permet de mettre à jour le polynome en lui affectant un autre polynome comme chaines de caractères
{
    polynome *temp = *poly;
    polynome *prev = NULL;
    while (temp != NULL) // on libère la mémoire du polynome
    {
        prev = temp;
        temp = temp->next; // on passe au monome suivant
        free(prev);
    }
    *poly = LET(miseajour); // on affecte le polynome à partir de la chaine de caractère
}

float EVAL(polynome *poly, int a) // EVAL permet de calculer la valeur du polynome pour une valeur donnée
{
    if (poly == NULL)
    {
        return 0.0; // si le polynome est vide on retourne 0
    }
    float terme_actuel = pow(a, poly->exposant) * (float)(poly->coefficient.num) / poly->coefficient.denom; // on calcule le terme actuel en utilisant la fonction pow de la bibliothèque math.h
    return terme_actuel + EVAL(poly->next, a);                                                              // on fait la somme du terme actuel et le terme suivant récursivement
}
polynome *copierPolynome(polynome *monPoly)// copierPolynome permet de copier un polynome
{

    if (monPoly == NULL)// si le polynome est vide on retourne NULL
    {

        return NULL;
    }
    else// sinon on copie le polynome
    {
        polynome *copiedPoly = monome(monPoly->coefficient, monPoly->exposant);// on crée un polynome temporaire contenant le premier monome du polynome à copier

        polynome *R = monPoly->next;

        polynome *current = copiedPoly;

        while (R != NULL)// on parcours le polynome
        {

            current->next = monome(R->coefficient, R->exposant);// on crée un monome temporaire contenant le monome à copier

            current = current->next;

            R = R->next;
        }
        return copiedPoly;
    }
}
polynome *INT(polynome *monPoly) // INT permet de calculer l'integrale du polynome
{
    polynome *integrale = copierPolynome(monPoly);
    polynome *R = integrale; // on crée un polynome temporaire

    while (R != NULL) // on parcours le polynome
    {
        R->exposant++;                                          // on incrémente l'exposant
        R->coefficient = division(R->coefficient, R->exposant); // on divise le coefficient par l'exposant
        R = R->next;                                            // on passe au monome suivant
    }
    return integrale;
}
polynome *ajouterMonome(polynome *monPoly, fraction coef, int exp, int op)
{
    if (monPoly == NULL) // si le polynome est vide on crée un monome
    {
        return monome(coef, exp);
    }
    else
    {
        if (exp > monPoly->exposant ) // si l'exposant du monome est supérieur à l'exposant du premier monome du polynome on ajoute le monome au début du polynome
        {
            if (op == 0)
            {
                polynome *P = monome(coef, exp); // on crée le monome
                P->next = monPoly;
                return P; // on le chaine avec le polynome
            }
            else
            {
                coef.num = (-1) * coef.num;
                polynome *P = monome(coef, exp); // on crée le monome
                P->next = monPoly;
                return P; // on le chaine avec le polynome
            }
        }
        if (op == 0) // si on fait une somme
        {
            if (exp == monPoly->exposant) // si l'exposant du monome est égale à l'exposant du premier monome du polynome on fait la somme des deux monomes
            {
                monPoly->coefficient = somme(monPoly->coefficient, coef, 0); // on fait la somme des deux monomes
                return monPoly;
            }
            else
            {
                monPoly->next = ajouterMonome(monPoly->next, coef, exp, 0); // sinon on passe au monome suivant récursivement
                return monPoly;
            }
        }
        else // si on fait une soustraction
        {
            if (exp == monPoly->exposant) // si l'exposant du monome est égale à l'exposant du premier monome du polynome on fait la soustraction des deux monomes
            {
                monPoly->coefficient = somme(monPoly->coefficient, coef, 1); // on fait la soustraction des deux monomes
                return monPoly;
            }
            else
            {
                if(monPoly->next==NULL){
                    coef.num = (-1) * coef.num;
                    monPoly->next = ajouterMonome(monPoly->next, coef, exp, 0); // sinon on passe au monome suivant récursivement
                    return monPoly;
                }
                monPoly->next = ajouterMonome(monPoly->next, coef, exp, 1); // sinon on passe au monome suivant récursivement
                return monPoly;
            }
        }
    }
}
polynome *MUL(polynome *monPoly1, polynome *monPoly2) // MUL permet de faire le produit de deux polynomes
{
    polynome *mul = NULL;
    polynome *R1 = monPoly1;

    while (R1 != NULL) // on parcours le premier polynome
    {
        polynome *R2 = monPoly2; // on parcours le deuxième polynome
        while (R2 != NULL)
        {
            fraction coef = produit(R1->coefficient, R2->coefficient); // on fait le produit des deux coefficients
            int exp = R1->exposant + R2->exposant;                     // on fait la somme des deux exposants
            mul = ajouterMonome(mul, coef, exp, 0);                    // on ajoute le monome dans le polynome
            R2 = R2->next;                                             // on passe au monome suivant du deuxième polynome
        }
        R1 = R1->next; // on passe au monome suivant du premier polynome
    }
    return mul;
}
polynome *DER(polynome *monPoly) // DER permet de calculer la dérivée du polynome
{
    if (monPoly == NULL) // si le polynome est vide on retourne NULL
    {
        return NULL;
    }
    polynome *derive = monPoly; // on crée un polynome temporaire contenant le polynome à dériver

    derive->coefficient.num *= derive->exposant; // on multiplie le coefficient par l'exposant
    derive->exposant--;                          // on décrimente l'exposant

    if (derive->next != NULL && derive->next->exposant == 0) // si le monome suivant est null ou de degré 0 on le supprime
    {
        free(derive->next);
        derive->next = NULL;
    }
    else
    {
        derive->next = DER(derive->next); // sinon on passe au monome suivant récursivement
    }
    return derive;
}
polynome *ADD(polynome *P, polynome *Q, int op) // ADD permet de faire la somme ou la soustraction de deux polynomes
{
    polynome *somme = copierPolynome(P); // on crée un polynome temporaire contenant le premier polynome
    polynome *R = copierPolynome(Q);     // on crée un polynome temporaire contenant le deuxième polynome
    while (R != NULL)                    // on parcours le deuxième polynome
    {
        somme = ajouterMonome(somme, R->coefficient, R->exposant, op); // on ajoute le monome dans le polynome
        R = R->next;                                                   // on passe au monome suivant
    }
    return somme;
}
polynome *POW(polynome *P, int n) // POW permet de calculer la puissance n du polynome
{
    if (n == 0) // si la puissance est égale à  0 on retourne le polynome 1
    {
        return monome((fraction){1, 1}, 0); // on crée un monome de coefficient 1 et d'exposant 0
    }
    else if (n == 1) // si la puissance est égale à 1 on retourne le polynome lui mÃªme
    {
        return P;
    }
    else
    {
        // Multiplie le polynome par lui-meme (n-1) fois récursivement.
        return MUL(P, POW(P, n - 1));
    }
}
void AFFECT(polynome **dest, polynome **source)
{
    polynome *temp = *dest;
    polynome *prev = NULL;
    while (temp != NULL)
    {
        prev = temp;
        temp = temp->next;
        free(prev);
    }
    *dest = NULL;
    *dest=copierPolynome(*source);
}
void DISPLAY(polynome *poly, int test) // DISPLAY permet d'afficher le polynome
{
    polynome *temp = poly;
    if (temp == NULL) // si le polynome est vide on retourne rien
    {
        printf("\n");
        return;
    }
    if (test != 0) // si c'est pas le premier monome on affiche le signe
    {
        if (temp->coefficient.num > 0) // si le coefficient est positif on affiche le signe +
            printf(" +");
        else // sinon on affiche rien car le signe moins et déjà  chainée dans le monome
            printf(" ");
    }
    if (temp->exposant == 0) // si l'exposant est égale à 0 on affiche le coefficient
    {
        if (temp->coefficient.num == temp->coefficient.denom) // si le coefficient est égale à 1 on affiche 1
        {
            printf("1");
        }
        else if (temp->coefficient.denom == 1) // si le dénominateur est égale à  1 on affiche le numérateur
        {
            printf("%d", temp->coefficient.num);
        }
        else // sinon on affiche le fraction
        {
            printf("%d/%d", temp->coefficient.num, temp->coefficient.denom);
        }
    }
    else // sinon on affiche le monome
    {
        if (temp->coefficient.num == temp->coefficient.denom) // si le coefficient est égale à  1 on affiche x^k
        {
            if (temp->exposant == 1) // si l'exposant est égale à  1 on affiche x
            {
                printf("x");
            }
            else // sinon on affiche x^k
            {
                printf("x^%d", temp->exposant);
            }
        }
        else if (temp->coefficient.denom == 1) // si le dénominateur est égale à 1 on affiche le numérateur
        {
            if (temp->exposant == 1)
            {
                printf("%dx", temp->coefficient.num);
            }
            else
            {
                printf("%dx^%d", temp->coefficient.num, temp->exposant);
            }
        }
        else // sinon on affiche le monome complet Ã§a dépend ainsi du cas de l'exposant
        {
            if (temp->exposant == 1)
            {
                printf("%d/%dx", temp->coefficient.num, temp->coefficient.denom);
            }
            else
            {
                printf("%d/%dx^%d", temp->coefficient.num, temp->coefficient.denom, temp->exposant);
            }
        }
    }
    test++;              // on incrémente le test pour dire que c'est pas le premier monome
    temp = temp->next;   // on passe au monome suivant
    DISPLAY(temp, test); // on affiche le monome suivant récursivement
}