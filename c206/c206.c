
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->Last = NULL;
    L->First = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    tDLElemPtr elemPtr = L->First;
    tDLElemPtr nextElemPtr;

    while (elemPtr != NULL)
    {
        nextElemPtr = elemPtr->rptr;
        free(elemPtr);
        elemPtr = nextElemPtr;
    }

    L->Act = NULL;
    L->Last = NULL;
    L->First = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr newElemPtr = malloc(sizeof(struct tDLElem));
    if (newElemPtr == NULL)
    {
        DLError();
        return;
    }
    
    newElemPtr->data = val;
    newElemPtr->rptr = L->First; //pravý nového na prvního
    newElemPtr->lptr = NULL; //levý nového ukazuje na NULL
    if (L->First != NULL)
    {   //seznam už měl prvního
        L->First->lptr = newElemPtr; //první bude doleva
        //ukazovat na nový prvek
    }
    else
    { //vložení do prázdného seznamu
        L->Last = newElemPtr;
    }
    L->First = newElemPtr; //korekce ukazatele začátku
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr newElemPtr = malloc(sizeof(struct tDLElem));
    if (newElemPtr == NULL)
    {
        DLError();
        return;
    }
    
    newElemPtr->data = val;
    newElemPtr->rptr = NULL; //pravý nového na NULL
    newElemPtr->lptr = L->Last; //levý nového ukazuje na přechozího posledního
    if (L->Last != NULL)
    {   //seznam už měl posledního
        L->Last->rptr = newElemPtr;
        //ukazovat na nový prvek
    }
    else
    { //vložení do prázdného seznamu
        L->First = newElemPtr;
    }
    L->Last = newElemPtr; //korekce ukazatele začátku
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL)
    {
        DLError();
    }
    else
    {
        *val = L->First->data;
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    tDLElemPtr elemPtr;
    if (L->First != NULL)
    {
        elemPtr = L->First;
        if (L->Act == L->First)
        { //první byl aktivní
            L->Act = NULL; //ruší se aktivita
        }
    
        if (L->First == L->Last)
        {//seznam měl jediný prvek–zruší se
            L->First = NULL;
            L->Last = NULL;
        }
        else 
        {
            L->First = L->First->rptr; //aktualizace začátku seznamu
            L->First->lptr = NULL; //ukazatel prvního doleva na NULL
        }
        free(elemPtr);
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if (L->Act != NULL)
    {
        if (L->Act->rptr != NULL)
        { //Je co rušit?
            tDLElemPtr elemPtr;
            elemPtr = L->Act->rptr; //ukazatel na rušený
            L->Act->rptr = elemPtr->rptr;//překlenutí rušeného
            
            if (elemPtr == L->Last)
            { //rušený poslední
                L->Last = L->Act; //posledním bude aktivní
            }
            else
            { //prvek za zrušeným ukazuje doleva na Act
                elemPtr->rptr->lptr = L->Act;
            }
            free(elemPtr);
        } //je co rušit
    } //aktivní		
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if (L->Act != NULL)
    {
        if (L->Act->lptr != NULL)
        { //Je co rušit?
            tDLElemPtr elemPtr;
            elemPtr = L->Act->lptr; //ukazatel na rušený
            L->Act->lptr = elemPtr->lptr; //překlenutí rušeného
            if (elemPtr == L->First)
            { //rušený první
                L->First = L->Act; //prvním bude aktivní
            }
            else
            { //prvek před zrušeným ukazuje doprava na Act
                elemPtr->lptr->rptr = L->Act;
            }
            free(elemPtr);
        } //je co rušit
    } //aktivní
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL)
    {   //je kam vkládat
        tDLElemPtr newElemPtr = malloc(sizeof(struct tDLElem));
        if (newElemPtr == NULL)
        {
            DLError();
            return;
        }


        newElemPtr->data = val;
        newElemPtr->rptr = L->Act->rptr;
        newElemPtr->lptr = L->Act;
        L->Act->rptr = newElemPtr;
        //navázání levého souseda na nový
        if (L->Act == L->Last)
        { //vkládá za posledního
            L->Last = newElemPtr; //korekce ukazatele na konec
        }
        else
        { //navázání pravého souseda na vložený prvek
            newElemPtr->rptr->lptr = newElemPtr;
        }
    } //aktivní
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
	
 solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

/* Konec c206.c*/
