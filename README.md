# belladewusa-SoalShiftSISOP20_modul3_T07-


## soal no 3
Membuat program untuk memindahkan file sesuai ekstensinya ke beberapa folder dengan nama sesuai ekstensi filenya. Menggunakan 3 argumen -f , * , dan -d.

### PENJELASAN CODINGAN

header and constant
list library 

        #include <stdlib.h>
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <unistd.h>
        #include <wait.h>
        #include <time.h>
        #include <stdbool.h>
        #include <dirent.h>
        #include <errno.h>
        #include <stdio.h>
        #include <string.h>
        #include <pthread.h>
        #include <ctype.h>
        #include <libgen.h>

mendefinisikan suatu nilai tertentu kepada suatu nama konstanta

        #define TRUE 1
        #define FALSE 0
        #define F 2
        #define D 3
        #define STAR 4


isDirectory cek apakah path yang dimasukkan adalah path ke directory atau file

        int isDirectory(const char *path) { //konstanta bertipe karakter
        struct stat statbuf; 
        if (stat(path, &statbuf) != 0)
            return FALSE;
        return S_ISDIR(statbuf.st_mode);
        }


checkAllFileExist cek apakah semua path file yang dimasukkan ada

        int checkAllFileExist(int argc, char *argv[]) {
            int i;
            char curr_dirr[1000];
            getcwd(curr_dirr, sizeof(curr_dirr));
            strcat(curr_dirr, "/");
            for ( i = 2; i < argc; i++)
            {
                char temp[1000];
                if(argv[i][0] != '/') {
                    strcpy(temp, curr_dirr);
                    strcat(temp, argv[i]);
                }else strcpy(temp, argv[i]);
                struct stat buffer;
                if(stat(argv[i], &buffer) != 0 || isDirectory(temp)) return FALSE;
            }
            return TRUE;
        }


 get_filename_ext untuk mendapat ekstensi dari suatu file

        const char *get_filename_ext(const char *filename) {
            const char *dot = strrchr(filename, '.');
            if(!dot || dot == filename) return "";

            return dot;
        }


checkMainArg checking argumen yang dimasukkan ke program

        int checkMainArg(int argc, char *argv[]) {
            if(argc < 2)
                return FALSE;
            if(argv[1][0] == '*' && strlen(argv[1]) == 1 && argc == 2)
                return STAR;   
            if(argv[1][0] == '-' && strlen(argv[1]) == 2)
                switch (argv[1][1])
                {
                case 'f':
                    return (argc < 3)? FALSE : (strlen(argv[1]) != 2)? FALSE : (!checkAllFileExist(argc, argv))? FALSE : F;
                case 'd':
                    return (argc != 3)? FALSE : (strlen(argv[1]) != 2)? FALSE : (!isDirectory(argv[2]))? FALSE : D;
                }

            return FALSE;
        }


moveFileToNewPath memindahkan file ke path baru

        void moveFileToNewPath(char *source, char *dest) {
            FILE *fp1, *fp2;
            int ch;
            
            fp1 = fopen(source, "r");
            fp2 = fopen(dest, "w");

            while ((ch = fgetc(fp1)) != EOF) fputc(ch, fp2);
            
            fclose(fp1);
            fclose(fp2);

            remove(source);
        }


moveFile fungsi untuk memindahkan ke suatu directory

        void *moveFile( void *ptr )
        {
            char *param = (char *)ptr;
            const char *extension_temp = get_filename_ext(param);
            int ch, i, length = strlen(extension_temp);
            char extension[100];
            char *filename = basename(param);
            char new_path[1000],temp[1000];

            memset(new_path, 0, sizeof(new_path));
            memset(extension, 0, sizeof(extension));

            for ( i = 1; i < length; i++)
            {
                extension[i - 1] = tolower(extension_temp[i]);
            }
            
            
            if(!strlen(extension_temp)) {
                if(!isDirectory("Unknown"))mkdir("Unknown", 0777);
                strcpy(new_path, "Unknown/");
                strcat(new_path, filename);
            }else{
                if(!isDirectory(extension))mkdir(extension, 0777);
                strcpy(new_path, extension);
                strcat(new_path, "/");
                strcat(new_path, filename);
            }

            moveFileToNewPath(param, new_path);
        }


Lalu di main function

        int main(int argc, char *argv[]) {
            int check = checkMainArg(argc, argv), i = 0, iret[10000], let = 1;

            if(!check) {
                printf("Argumen Salah!\n");
                exit(EXIT_FAILURE);
            }

            char curr_dirr[1000];
            pthread_t threads[10000];
            struct dirent *ep;     
            DIR *dp;

case F

            switch (check)
            {
            case F:
                for (i = 2; i < argc; i++)
                {
                    char *arr = argv[i];
                    iret[i - 2] = pthread_create(&threads[i - 2], NULL, moveFile, (void*) arr);
                    
                    if(iret[i - 2])
                    {
                        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret[i - 2]);
                        exit(EXIT_FAILURE);
                    }

                    pthread_join(threads[i - 2], NULL); 
                }
                break;

case D

            case D:
                getcwd(curr_dirr, sizeof(curr_dirr));
                if(argv[2][0] != '/'){
                    strcat(curr_dirr, "/");
                    strcat(curr_dirr, argv[2]);
                    if(argv[2][strlen(argv[2]) - 1] != '/')
                        strcat(curr_dirr, "/");
                }
                dp = opendir (argv[2]);
                if(dp != NULL) {
                    while (ep = readdir (dp)){
                        char temp[1000];
                        strcpy(temp, curr_dirr);
                        strcat(temp, ep->d_name);
                        if(strcmp(".",ep->d_name) !=0 && strcmp("..",ep->d_name) != 0 && !isDirectory(temp)) {
                            strcpy(temp, argv[2]);
                            if(argv[2][strlen(argv[2]) - 1] != '/') strcat(temp, "/");
                            strcat(temp, ep->d_name);
                            char *arr = temp;
                            iret[i] = pthread_create(&threads[i], NULL, moveFile, (void*) arr);
                    
                            if(iret[i])
                            {
                                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret[i]);
                                exit(EXIT_FAILURE);
                            }

                            pthread_join(threads[i], NULL); 
                        }
                        i++;
                    }
                }
                break;

star case

            case STAR:
                if(getcwd(curr_dirr, sizeof(curr_dirr)) != NULL) {
                    dp = opendir (curr_dirr);
                    if(dp != NULL) {
                        while (ep = readdir (dp)){
                            char temp[1000];
                            strcpy(temp, curr_dirr);
                            strcat(temp, "/");
                            strcat(temp, ep->d_name);
                            if(strcmp(".",ep->d_name) !=0 && strcmp("..",ep->d_name) != 0 && !isDirectory(temp)) {
                                char *arr = temp;
                                iret[i] = pthread_creae(&threads[i], NULL, moveFile, (void*) arr);
                        
                                if(iret[i])
                                {
                                    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret[i]);
                                    exit(EXIT_FAILURE);
                                }

                                pthread_join(threads[i], NULL); 
                            }
                            i++;
                        }
                    }
                }
                break;
            }
        }


### DOKUMETASI

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/1.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/2.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/3.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/4.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/5.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/6.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/7.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/8.png)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/9.png)



## soal no 4
### nomor 4a
Batu mulia pertama. Emerald. Batu mulia yang berwarna hijau mengkilat. Pada batu itu Ia menemukan sebuah kalimat petunjuk. Ada sebuah teka-teki yang berisi: 1. Buatlah program C dengan nama "4a.c", yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka). 2. Tampilkan matriks hasil perkalian tadi ke layar.

    pthread_t thr1, thr2, thr3;
  
  deklarasi thread

    int row1=4;
    int column1=2;
    int row2=2;
    int column2=5;
    int matrix01[20][20];
    int matrix02[20][20];
    int (*hasil)[20];
    int status;
    
  deklarasi variabel

    void *perkalian(void *);
    void *matrix_1(void *);
    void *matrix_2(void *);
  membuat void
  
      void *matrix_1(void *arg)
    {
	  status=0;
        matrix01[0][0]=1;
	matrix01[0][1]=1;
        matrix01[1][0]=1;
	matrix01[1][1]=1;
        matrix01[2][0]=1;
	matrix01[2][1]=1;
        matrix01[3][0]=1;
	matrix01[3][1]=1;

	status=1;
	return NULL;
}


     void *matrix_2(void *arg)
    {
	 while(status!=1)
	{

	}
        matrix02[0][0]=2;
	matrix02[1][0]=2;
        matrix02[0][1]=2;
	matrix02[1][1]=2;
        matrix02[0][2]=2;
	matrix02[1][2]=2;
        matrix02[0][3]=2;
	matrix02[1][3]=2;
 	matrix02[0][4]=2;
	matrix02[1][4]=2;
       
	status=2;
	return NULL;
  
 memasukkan isi matrik 1 dan 2 kedalam void
 
    void *perkalian(void *arg)
    {
	  while(status!=2)
	{

	}
	int i, j, k;
	for(i=0;i<row1;i++)
	{
	for(j=0;j<column2;j++)
		{
	hasil[i][j] = 0;
		for(k=0;k<2;k++)
			{
		hasil[i][j] +=matrix01[i][k] * matrix02[k][j];
			}
		}
	}
	return NULL;
}


membuat perkalian matrik dalam void 

    int main()
     {
	  int i, j;
    	key_t key = 1234;
        int shmid = shmget(key, sizeof(int[20][20]), IPC_CREAT | 0666);
        hasil = shmat(shmid, 0, 0);

	pthread_create(&thr1, NULL, matrix_1, NULL);
	pthread_join(thr1,NULL);

	pthread_create(&thr2, NULL, matrix_2, NULL);
	pthread_join(thr2,NULL);

melakukan shared memory (ShmKey = SharedMemory Key , ShmID = SharedMemory ID)
dan melakukan join (fungsi wait) pada tiap tiap thread

    printf("Hasil Perkalian\n");
	for(i=0;i<row1;i++)
	{
		for(j=0;j<column2;j++)
		{
			printf("%d\t", hasil[i][j]);
		}
		printf("\n");
	}
  
  melakukan print pada hasil perkalian
  
## soal no 4c

Batu ketiga adalah Onyx. Batu mulia berwarna hitam mengkilat. Pecahkan
teka-teki berikut!

1. Buatlah program C ketiga dengan nama "4c.c". Program ini tidak
memiliki hubungan terhadap program yang lalu.
2. Pada program ini, Norland diminta mengetahui jumlah file dan
folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar
IPC, Norland mengerjakannya dengan semangat.
(Catatan! : Harus menggunakan IPC Pipes)


    #include<stdio.h>
    #include<stdlib.h>
    #include<unistd.h>
    #include<sys/types.h>
    #include<string.h>
    #include<sys/wait.h>
    
 library yang digunakan 
 

	int main() {
  	int fd[4];
  	pid_t pid;
 	pipe(fd);

membuat file deskriptor (tiap pipe membutuhkan 2 file deskriptor)


  	pid = fork();
 	 if (pid == 0) {
    dup2(fd[1], 1);
    close(fd[0]);
    close(fd[1]);
    close(fd[2]);
    close(fd[3]);
    char *argv[] = {"ls", NULL};
    execv("/bin/ls", argv);
  }
  while(wait(NULL) > 0);

fungsi untuk melakukan listing pada direktori tempat program berada


  	pid = fork();
  	if (pid == 0) {
    dup2(fd[0], 0);
    //dup2(fd[3], 1);
    close(fd[0]);
    close(fd[1]);
    close(fd[2]);
    close(fd[3]);
    char *argv[] = {"wc", "-l", NULL};
    execv("/usr/bin/wc", argv);
  }
 
 fungsi wc yang digunakan untuk Menampilkan jumlah baris, kata, dan besar memori suatu file.
 lalu menggunakan -l untuk mengambil jumlah direktori yang ada

### DOKUMENTASI

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/1586609247597.jpg)

**Contoh Output:**\
![Output Soal ](https://github.com/belladewusa/SoalShiftSISOP20_modul3_T07/blob/master/modul3/1586609324106.jpg)



## kendala :
1 dan 2 : tidak sanggup membayangkan harus menggunakan fungsi apa saja dan bagaimana pengerjaannya.
out of skill
tidak seimbang antara pengajaran di lab yang cuman sekali dan hanya basic saja . sementara soal latihan apalagi shift yang sangat jauh tingkat kesulitannya untuk dikerjakan dalam 5 hari.
sistem penilaian yang tidak adil, padahal background IT berbeda jauh denga TC yang sudah besar dan mempunyai banyak kating yang dapat dimintai pengajaran jika ada hal yang dibingungkan.
setiap kelompok belum tentu ada yang bisa, kadang ada yang lebih pro didalamnya, tapi kadang juga ada yang memang basic saja baru mau belajar. sehingga sistem pembagian kelompok 2 orang tidak adil.

