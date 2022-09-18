#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

struct Product {
  int code;
  int quantity;
  char drugname[30];
  float price;
  float price_ttc;
};
struct Product product;
//    function to add records
void insert() {
  FILE *fp;
  fp = fopen("products", "a");
  printf("Entrer le code du produit        :");
  scanf("%d", &product.code);
  printf("Entrer la quantité               :");
  scanf("%i", &product.quantity);
  printf("Entrer le Nom du produit         :");
  scanf("%s", product.drugname);
  printf("Entrer le prix du produit        :");
  scanf("%f", &product.price);
  product.price_ttc = product.price + product.price * 0.15;
  fwrite(&product, sizeof(product), 1, fp);
  fclose(fp);
  __fpurge(stdin);
}

//    function o check if the drug is there //
int drugcheck(int rno) {
  FILE *fp;
  int c = 0;
  fp = fopen("products", "r");
  while (!feof(fp)) {
    fread(&product, sizeof(product), 1, fp);

    if (rno == product.code) {
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}

void searchcode() {
  FILE *fp2;
  int r, s, drog;
  printf("\nEntrez le code du produit que vous souhaitez rechercher  :");
  scanf("%d", &r);
  drog = drugcheck(r);
  if (drog == 0)
    printf("le médicament %d n'est pas disponible dans le stock\n", r);
  else {
    fp2 = fopen("products", "r");
    while (fread(&product, sizeof(product), 1, fp2)) {
      s = product.code;
      if (s == r) {
        printf("\nle code du produit      = %d", product.code);
        printf("\nle nom du produit       = %s", product.drugname);
        printf("\nla quantité du produit  = %d", product.quantity);
        printf("\nle prix TTC du produit  = %.2f\n", product.price_ttc);
      }
    }
    fclose(fp2);
  }
}

int drugcheckq(int rno) {
  FILE *fp;
  int c = 0;
  fp = fopen("products", "r");
  while (!feof(fp)) {
    fread(&product, sizeof(product), 1, fp);

    if (rno == product.quantity) {
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}

void searchq() {
  FILE *fp2;
  int r, s, drog;
  printf("\nEntrez la quantité du produit que vous souhaitez rechercher  :");
  scanf("%d", &r);
  drog = drugcheckq(r);
  if (drog == 0)
    printf("le médicament du quantité %d n'est pas disponible dans le stock\n",
           r);
  else {
    fp2 = fopen("products", "r");
    while (fread(&product, sizeof(product), 1, fp2)) {
      s = product.quantity;
      if (s == r) {
        printf("\nle code du produit      = %d", product.code);
        printf("\nle nom du produit       = %s", product.drugname);
        printf("\nla quantité du produit  = %d", product.quantity);
        printf("\nle prix TTC du produit  = %.2f\n", product.price_ttc);
      }
    }
    fclose(fp2);
  }
}

void search() {
  int option;
foo:
  printf("choisissez comment rechercher :\n1.par code \n2.par quantite  \n");
  printf("Entrez votre choix:");
  scanf("%d", &option);
  switch (option) {
  case 1:
    searchcode();
    break;
  case 2:
    searchq();
    break;
  default:
    __fpurge(stdin);
    goto foo;
    break;
  }
}

void delete () {
  FILE *fpo;
  FILE *fpt;
  int r, s;
  printf("Entrez le code du produit que vous souhaitez supprimer :");
  scanf("%d", &r);
  if (drugcheck(r) == 0)
    printf("le médicament %d n'est pas disponible dans le stock\n", r);
  else {
    fpo = fopen("products", "r");
    fpt = fopen("TempFile", "w");
    while (fread(&product, sizeof(product), 1, fpo)) {
      s = product.code;
      if (s != r)
        fwrite(&product, sizeof(product), 1, fpt);
    }
    fclose(fpo);
    fclose(fpt);
    fpo = fopen("products", "w");
    fpt = fopen("TempFile", "r");
    while (fread(&product, sizeof(product), 1, fpt))
      fwrite(&product, sizeof(product), 1, fpo);
    printf("\nPRODUIT SUPPRIMÉ\n");
    fclose(fpo);
    fclose(fpt);
  }
}

void astock() {
  int drug1;
  FILE *fpt;
  FILE *fpo;
  int s, r, ch;
  printf("Entrez le code du produit à ajouter dans le stock :");
  scanf("%d", &r);
  drug1 = drugcheck(r);
  if (drug1 == 0) {
    printf("le médicament %d n'est pas disponible dans le stock", r);
  } else {
    fpo = fopen("products", "r");
    fpt = fopen("TempFile", "w");
    while (fread(&product, sizeof(product), 1, fpo)) {
      s = product.code;
      if (s != r)
        fwrite(&product, sizeof(product), 1, fpt);
      else {
        int t;
        printf("Entrez le nombre que vous voulez ajouter :");
        scanf("%d", &t);
        product.quantity = product.quantity + t;

        fwrite(&product, sizeof(product), 1, fpt);
      }
    }
    fclose(fpo);
    fclose(fpt);
    fpo = fopen("products", "w");
    fpt = fopen("TempFile", "r");
    while (fread(&product, sizeof(product), 1, fpt)) {
      fwrite(&product, sizeof(product), 1, fpo);
    }
    fclose(fpo);
    fclose(fpt);
    printf("LE STOCK EST ALIMENTE");
  }
}

void vendre() {
  int drug1;
  FILE *fpt;
  FILE *fpo;
  int s, r, ch;
  printf("Entrez le code du produit que vous voulez vendre :");
  scanf("%d", &r);
  drug1 = drugcheck(r);
  if (drug1 == 0) {
    printf("le médicament %d n'est pas disponible dans le stock", r);
  } else {
    fpo = fopen("products", "r");
    fpt = fopen("TempFile", "w");
    while (fread(&product, sizeof(product), 1, fpo)) {
      s = product.code;
      if (s != r)
        fwrite(&product, sizeof(product), 1, fpt);
      else {
        int t;
        printf("Entrez la quantité que vous voulez vendre :");
        scanf("%d", &t);
        product.quantity = product.quantity - t;

        fwrite(&product, sizeof(product), 1, fpt);
      }
    }
    fclose(fpo);
    fclose(fpt);
    fpo = fopen("products", "w");
    fpt = fopen("TempFile", "r");
    while (fread(&product, sizeof(product), 1, fpt)) {
      fwrite(&product, sizeof(product), 1, fpo);
    }
    fclose(fpo);
    fclose(fpt);
    printf("produit vendu");
  }
}

void sort() {
  int a[200], count = 0, i, j, t, c;
  FILE *fpo;
  fpo = fopen("products", "r");
  while (fread(&product, sizeof(product), 1, fpo)) {
    a[count] = product.price;
    count++;
  }
  c = count;
  for (i = 0; i < count - 1; i++) {
    for (j = i + 1; j < count; j++) {
      if (a[i] < a[j]) {
        t = a[i];
        a[i] = a[j];
        a[j] = t;
      }
    }
  }
  printf("\nCODE  \t\tQUANTITE \t\t NOM \t\tPRIX \t\t PRIX TTC\n\n");
  count = c;
  for (i = 0; i < count; i++) {
    rewind(fpo);
    while (fread(&product, sizeof(product), 1, fpo)) {
      if (a[i] == product.price)
        printf("%d\t\t\t%i\t\t\t%s\t\t\t%.2f\t\t\t%.2f\n", product.code,
               product.quantity, product.drugname, product.price,
               product.price_ttc);
    }
  }
}

int main(void) {
  int c, emp;
  do {
    printf("\n\t---------bienvenue dans votre pharmacie---------\n");
    printf("\n\t1. Ajouter un nouveau produit");
    printf("\n\t2. Lister tous les produits");
    printf("\n\t3. Acheter un produit");
    printf("\n\t4. Rechercher un produits");
    printf("\n\t5. Etat du stock");
    printf("\n\t6. Alimenter le stock");
    printf("\n\t7. Supprimer les produits");
    printf("\n\t8. Exit");
    printf("\n\n------------------------------------------\n");
    printf("\nEntrez votre choix:");
    scanf("%d", &c);
    printf("\n");
    switch (c) {
    case 1:
      insert();
      break;
    case 2:
      sort();
      break;
    case 3:
      vendre();
      break;
    case 4:
      search();
      break;
    case 5:
      // estock();
      break;
    case 6:
      astock();
      break;
    case 7:
      delete ();
      break;
    case 8:
      exit(1);
      break;
    default:
      printf("\nYour choice is wrong\nPlease try again...\n");
      __fpurge(stdin);
      break;
    }
  } while (c != 8);
}