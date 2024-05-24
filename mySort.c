#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

struct parametreler {
    char **kelimeler;
    int baslangic;
    int bitis;
    int thread_sayisi;
};

void* quicksort(void *paramtrler) {
    struct parametreler *sp = (struct parametreler *)paramtrler;
    int baslangic = sp->baslangic;
    int bitis = sp->bitis;

    if (baslangic < bitis) {
        int pivot_index = baslangic + (rand() % (bitis - baslangic + 1));
        char *pivot_value = sp->kelimeler[pivot_index];
        char *temp;

        temp = sp->kelimeler[pivot_index];
        sp->kelimeler[pivot_index] = sp->kelimeler[bitis];
        sp->kelimeler[bitis] = temp;

        int store_index = baslangic;
        for (int i = baslangic; i < bitis; i++) {
            if (strcmp(sp->kelimeler[i], pivot_value) <= 0) {
                
                temp = sp->kelimeler[i];
                sp->kelimeler[i] = sp->kelimeler[store_index];
                sp->kelimeler[store_index] = temp;
                store_index++;
            }
        }

        sp->kelimeler[bitis] = sp->kelimeler[store_index];
        sp->kelimeler[store_index] = pivot_value;

        int sol_bitis = store_index - 1;
        int sag_baslangic = store_index + 1;

        struct parametreler sol_paramtrler = {sp->kelimeler, baslangic, sol_bitis, sp->thread_sayisi};
        struct parametreler sag_paramtrler = {sp->kelimeler, sag_baslangic, bitis, sp->thread_sayisi};

        pthread_t sol_thread, sag_thread;

        if (sp->thread_sayisi > 1) {
            pthread_create(&sol_thread, NULL, quicksort, &sol_paramtrler);
            pthread_create(&sag_thread, NULL, quicksort, &sag_paramtrler);

            pthread_join(sol_thread, NULL);
            pthread_join(sag_thread, NULL);
        } else {
            quicksort(&sol_paramtrler);
            quicksort(&sag_paramtrler);
        }
    }

    pthread_exit(NULL);
}

void merge(char **arr, int sol_baslangic, int orta, int sag_bitis) {
    int sol_dizi_boyut = orta - sol_baslangic + 1;
    int sag_dizi_boyut = sag_bitis - orta;
    
    char **solDizi = (char **)malloc(sol_dizi_boyut * sizeof(char *));
    char **sagDizi = (char **)malloc(sag_dizi_boyut * sizeof(char *));
    
    for (int i = 0; i < sol_dizi_boyut; i++)
        solDizi[i] = arr[sol_baslangic + i];
    
    for (int j = 0; j < sag_dizi_boyut; j++)
        sagDizi[j] = arr[orta + 1 + j];
    
    int i = 0, j = 0, birlestirme = sol_baslangic;
    while (i < sol_dizi_boyut && j < sag_dizi_boyut) {
        if (strcmp(solDizi[i], sagDizi[j]) <= 0) {
            arr[birlestirme] = solDizi[i];
            i++;
        } else {
            arr[birlestirme] = sagDizi[j];
            j++;
        }
        birlestirme++;
    }
    
   
    while (i < sol_dizi_boyut) {
        arr[birlestirme] = solDizi[i];
        i++;
        birlestirme++;
    }
    
    while (j < sag_dizi_boyut) {
        arr[birlestirme] = sagDizi[j];
        j++;
        birlestirme++;
    }
    
    free(solDizi);
    free(sagDizi);
}

void* mergesort(void *paramtrler) {
    struct parametreler *sp = (struct parametreler *)paramtrler;
    int baslangic = sp->baslangic;
    int bitis = sp->bitis;

    if (baslangic < bitis) {
        int orta = baslangic + (bitis - baslangic) / 2;
        
        struct parametreler sol_paramtrler = {sp->kelimeler, baslangic, orta, sp->thread_sayisi};
        struct parametreler sag_paramtrler = {sp->kelimeler, orta + 1, bitis, sp->thread_sayisi};

        pthread_t sol_thread, sag_thread;

        if (sp->thread_sayisi > 1) {
            pthread_create(&sol_thread, NULL, mergesort, &sol_paramtrler);
            pthread_create(&sag_thread, NULL, mergesort, &sag_paramtrler);
            
            pthread_join(sol_thread, NULL);
            pthread_join(sag_thread, NULL);
        } else {
            mergesort(&sol_paramtrler);
            mergesort(&sag_paramtrler);
        }
        
        
        merge(sp->kelimeler, baslangic, orta, bitis);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Çalıştırma şekli: %s <inputfile> <outputfile> <# of threads> <algoritma>\n", argv[0]);
        return 1;
    }
    
    const char *input_file = argv[1];
    const char *output_file = argv[2];
    int thread_sayisi = atoi(argv[3]);
    const char *algoritma = argv[4];
    
    FILE *input_fp = fopen(input_file, "r");
    if (input_fp == NULL) {
        printf("%s input dosyası açılamadı.\n", input_file);
        return 1;
    }
    
    char **kelimeler = NULL;
    char word[100]; 
    int kelime_sayisi = 0;
    while (fscanf(input_fp, "%s", word) != EOF) {
        kelimeler = (char **)realloc(kelimeler, (kelime_sayisi + 1) * sizeof(char *));
        kelimeler[kelime_sayisi] = strdup(word);
        kelime_sayisi++;
    }
    
    fclose(input_fp);

    struct timeval t_baslangic, t_bitis;
    gettimeofday(&t_baslangic, NULL);

   
    struct parametreler paramtrler = {kelimeler, 0, kelime_sayisi - 1, thread_sayisi};

    if (strcmp(algoritma, "quick") == 0) {
        pthread_t main_thread;
        pthread_create(&main_thread, NULL, quicksort, &paramtrler);
        pthread_join(main_thread, NULL);
    } else if (strcmp(algoritma, "merge") == 0) {
        pthread_t main_thread;
        pthread_create(&main_thread, NULL, mergesort, &paramtrler);
        pthread_join(main_thread, NULL);
    } else {
        printf("Lütfen 'quick' veya 'merge' algoritmalarından birini seçin.\n");
        return 1;
    }

    gettimeofday(&t_bitis, NULL);

    double t_toplam = (t_bitis.tv_sec - t_baslangic.tv_sec) + (t_bitis.tv_usec - t_baslangic.tv_usec) / 1000000.0;
    printf("Sıralama süresi: %.6f saniye\n", t_toplam);

    FILE *output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        printf("%s output dosyası açılamadı.\n", output_file);
        return 1;
    }

    for (int i = 0; i < kelime_sayisi; i++) {
        fprintf(output_fp, "%s\n", kelimeler[i]);
        free(kelimeler[i]);
    }
    free(kelimeler);
    
    fclose(output_fp);
    
    return 0;
}
   
