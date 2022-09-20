#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct product {
  int code;
  int quantity;
  char drugname[30];
  float price;
  float price_ttc;
  char lasttimesold[100];
  float tatalsoldprice;
  int totaltimesold;
}product;
//    fonction pour ajouter un produit. 
void insert1() {
  FILE *fp;
  fp = fopen("products.bin", "ab");
  printf("Entrer le code du produit        :");
  scanf("%d", &product.code);
  printf("Entrer la quantité               :");
  scanf("%i", &product.quantity);
  printf("Entrer le Nom du produit         :");
  scanf("%s", product.drugname);
  printf("Entrer le prix du produit        :");
  scanf("%f", &product.price);
  product.price_ttc = product.price + product.price * 0.15;
  product.totaltimesold = 0;
  product.tatalsoldprice = 0;
  fwrite(&product, sizeof(product), 1, fp);
  fclose(fp);
  __fpurge(stdin);
}

//    fonction pour ajouter plusieur produits. 

void insertm() {
  int n;
  printf("entrez le nombre des produits que vous voulez ajouter :");
  scanf("%d", &n);
  for (int i = 1; i <= n; i++) {
    if (i == 1) {
      printf("\nentree les donnee de 1er produit :\n\n");
      insert1();
    } else {
      printf("\nentree les donnee de %deme produit :\n\n", i);
      insert1();
    }
  }
}

//    fonction mère pour ajouter un produit. 

void insert() {
  int option;
foo:
  printf("Combien des produit voulez vous ajouter? \n1.un seul produit. "
         "\n2.plusieur produits  \n");
  printf("Entrez votre choix:");
  scanf("%d", &option);
  switch (option) {
  case 1:
    insert1();
    break;
  case 2:
    insertm();
    break;
  default:
    __fpurge(stdin);
    goto foo;
    break;
  }
}

//    fonction pour verifier si le code de produit est dans le stock.

int drugcheck(int rno) {
  FILE *fp;
  int c = 0;
  fp = fopen("products.bin", "rb");
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

//    fonction pour verifier si le code de produit est dans le stock.

void searchcode() {
  FILE *fp2;
  int r, s, drog;
  printf("\nEntrez le code du produit que vous souhaitez rechercher  :");
  scanf("%d", &r);
  drog = drugcheck(r);
  if (drog == 0)
    printf("le médicament %d n'est pas disponible dans le stock\n", r);
  else {
    fp2 = fopen("products.bin", "rb");
    while (fread(&product, sizeof(product), 1, fp2)) {
      s = product.code;
      if (s == r) {
        printf("\nle code du produit      = %d", product.code);
        printf("\nle nom du produit       = %s", product.drugname);
        printf("\nla quantité du produit  = %d", product.quantity);
        printf("\nle prix TTC du produit  = %.2f DH\n", product.price_ttc);
      }
    }
    fclose(fp2);
  }
}

//    fonction pour verifier si la quantité de produit est dans le stock.

int drugcheckq(int rno) {
  FILE *fp;
  int c = 0;
  fp = fopen("products.bin", "rb");
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

//    fonction pour verifier si la quantité de produit est dans le stock.

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
    fp2 = fopen("products.bin", "rb");
    while (fread(&product, sizeof(product), 1, fp2)) {
      s = product.quantity;
      if (s == r) {
        printf("\nle code du produit      = %d", product.code);
        printf("\nle nom du produit       = %s", product.drugname);
        printf("\nla quantité du produit  = %d", product.quantity);
        printf("\nle prix TTC du produit  = %.2f DH\n", product.price_ttc);
      }
    }
    fclose(fp2);
  }
}

//    fonction mère pour chercher un produit.

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

//    fonction pour supprimer un produit.

void delete () {
  FILE *fpo;
  FILE *fpt;
  int r, s;
  printf("Entrez le code du produit que vous souhaitez supprimer :");
  scanf("%d", &r);
  if (drugcheck(r) == 0)
    printf("le médicament %d n'est pas disponible dans le stock\n", r);
  else {
    fpo = fopen("products.bin", "rb");
    fpt = fopen("TempFile.bin", "wb");
    while (fread(&product, sizeof(product), 1, fpo)) {
      s = product.code;
      if (s != r)
        fwrite(&product, sizeof(product), 1, fpt);
    }
    fclose(fpo);
    fclose(fpt);
    fpo = fopen("products.bin", "wb");
    fpt = fopen("TempFile.bin", "rb");
    while (fread(&product, sizeof(product), 1, fpt))
      fwrite(&product, sizeof(product), 1, fpo);
    printf("\nPRODUIT SUPPRIMÉ\n");
    fclose(fpo);
    fclose(fpt);
  }
}

//    fonction pour alimenter le stock d'un produit.

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
    fpo = fopen("products.bin", "rb");
    fpt = fopen("TempFile.bin", "wb");
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
    fpo = fopen("products.bin", "wb");
    fpt = fopen("TempFile.bin", "rb");
    while (fread(&product, sizeof(product), 1, fpt)) {
      fwrite(&product, sizeof(product), 1, fpo);
    }
    fclose(fpo);
    fclose(fpt);
    printf("LE STOCK EST ALIMENTE");
  }
}

//    fonction pour effectuer une commande.

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
    fpo = fopen("products.bin", "rb");
    fpt = fopen("TempFile.bin", "wb");
    while (fread(&product, sizeof(product), 1, fpo)) {
      s = product.code;
      if (s != r)
        fwrite(&product, sizeof(product), 1, fpt);
      else {
        int t;
        printf("Entrez la quantité que vous voulez vendre :");
        scanf("%d", &t);
        product.quantity = product.quantity - t;
        long ti = time(NULL);
        strcpy(product.lasttimesold, ctime(&ti));
        product.totaltimesold += t;
        product.tatalsoldprice = product.totaltimesold * product.price_ttc;
        fwrite(&product, sizeof(product), 1, fpt);
      }
    }
    fclose(fpo);
    fclose(fpt);
    fpo = fopen("products.bin", "wb");
    fpt = fopen("TempFile.bin", "rb");
    while (fread(&product, sizeof(product), 1, fpt)) {
      fwrite(&product, sizeof(product), 1, fpo);
    }
    fclose(fpo);
    fclose(fpt);
    printf("produit vendu");
  }
}

//    fonction pour verifier l'état du stock d'un produit.

void estock() {
  FILE *fpo;
  fpo = fopen("products.bin", "rb");
  printf("\nCODE  \t\tQUANTITE \t\t NOM \t\tPRIX \t\t PRIX TTC\n\n");
  rewind(fpo);
  while (fread(&product, sizeof(product), 1, fpo)) {
    if (product.quantity <= 3)
      printf("%d\t\t\t%i\t\t\t\t%s\t\t\t%.2f DH\t\t\t%.2f DH\n", product.code,
             product.quantity, product.drugname, product.price,
             product.price_ttc);
  }
}

//    fonction pour trier tous les produits selon l’ordre  décroissant du prix.

void sort() {
  int a[200], count = 0, i, j, t, c;
  FILE *fpo;
  fpo = fopen("products.bin", "rb");
  while (fread(&product, sizeof(product), 1, fpo)) {
    a[count] = product.price;
    count++;
  }
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
  for (i = 0; i < count; i++) {
    rewind(fpo);
    while (fread(&product, sizeof(product), 1, fpo)) {
      if (a[i] == product.price)
        printf("%d\t\t\t%i\t\t\t\t%s\t\t\t%.2f DH\t\t\t%.2f DH\n", product.code,
               product.quantity, product.drugname, product.price,
               product.price_ttc);
    }
  }
}

//    fonction pour trier tous les produits selon l’ordre alphabétique  croissant du nom.

void sortname() {
  int count = 0, i, j, c;
  char a[30][30], t[30];
  FILE *fpo;
  fpo = fopen("products.bin", "rb");
  while (fread(&product, sizeof(product), 1, fpo)) {
    strcpy(a[count], product.drugname);
    count++;
  }
  for (i = 0; i < count - 1; i++) {
    for (j = i + 1; j < count; j++) {
      int index = strcmp(a[i], a[j]);
      if (index > 0) {
        strcpy(t, a[i]);
        strcpy(a[i], a[j]);
        strcpy(a[j], t);
      }
    }
  }
  printf("\nCODE  \t\tQUANTITE \t\t NOM \t\tPRIX \t\t PRIX TTC\n\n");
  for (i = 0; i < count; i++) {
    rewind(fpo);
    while (fread(&product, sizeof(product), 1, fpo)) {
      if (strcmp(a[i], product.drugname) == 0)
        printf("%d\t\t\t%i\t\t\t\t%s\t\t\t%.2f DH\t\t\t%.2f DH\n", product.code,
               product.quantity, product.drugname, product.price,
               product.price_ttc);
    }
  }
}

//    fonction mère pour lister les produits.

void lister() {
  int option;
foo:
  printf(
      "choisissez comment voulez vous lister les produits :\n1.Selon l’ordre "
      "alphabétique  croissant du nom \n2.L’ordre  décroissant du prix.  \n");
  printf("Entrez votre choix:");
  scanf("%d", &option);
  switch (option) {
  case 1:
    sortname();
    break;
  case 2:
    sort();
    break;
  default:
    __fpurge(stdin);
    goto foo;
    break;
  }
}

//    fonction affiche les statistique d'un produit.

void statistique() {
  FILE *fp2;
  int r, s, drog;
  printf("\nEntrez le code du produit que vous souhaitez rechercher  :");
  scanf("%d", &r);
  drog = drugcheck(r);
  if (drog == 0)
    printf("le médicament %d n'est pas disponible dans le stock\n", r);
  else {
    fp2 = fopen("products.bin", "rb");
    while (fread(&product, sizeof(product), 1, fp2)) {
      s = product.code;
      if (s == r) {
        printf("\nle code du produit      = %d", product.code);
        printf("\nle nom du produit       = %s", product.drugname);
        printf("\nla dernier fois vendu  = %s", product.lasttimesold);
        printf("\ncombien de fois ce produit est vendu  = %d",
               product.totaltimesold);
        printf("\ntotal d'argent fais par ce produit  = %.2f DH",
               product.tatalsoldprice);
      }
    }
    fclose(fp2);
  }
}

//    fontion principale.

int main(void) {
  int c;
  do {
    printf("\n\n\t---------bienvenue dans votre pharmacie---------\n");
    printf("\n\t1. Ajouter un nouveau produit");
    printf("\n\t2. Lister tous les produits");
    printf("\n\t3. Acheter un produit");
    printf("\n\t4. Rechercher un produits");
    printf("\n\t5. Etat du stock");
    printf("\n\t6. Alimenter le stock");
    printf("\n\t7. Supprimer un produit");
    printf("\n\t8. statistique");
    printf("\n\t8. exit");
    printf("\n\n------------------------------------------\n");
    printf("\nEntrez votre choix:");
    scanf("%d", &c);
    printf("\n");
    switch (c) {
    case 1:
      insert();
      break;
    case 2:
      lister();
      break;
    case 3:
      vendre();
      break;
    case 4:
      search();
      break;
    case 5:
      estock();
      break;
    case 6:
      astock();
      break;
    case 7:
      delete ();
      break;
    case 8:
      statistique();
      break;
    case 9:
      exit(1);
      break;
    default:
      printf("\nchoix invalide !\nveuillez entrez un choix entre 1 et 8\n");
      __fpurge(stdin);
      break;
    }
  } while (1);
}