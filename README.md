# belladewusa-SoalShiftSISOP20_modul3_T07-



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


## kendala :
1 dan 2 : tidak sanggup membayangkan harus menggunakan fungsi apa saja dan bagaimana pengerjaannya.
out of skill
tidak seimbang antara pengajaran di lab yang cuman sekali dan hanya basic saja . sementara soal latihan apalagi shift yang sangat jauh tingkat kesulitannya untuk dikerjakan dalam 5 hari.
sistem penilaian yang tidak adil, padahal background IT berbeda jauh denga TC yang sudah besar dan mempunyai banyak kating yang dapat dimintai pengajaran jika ada hal yang dibingungkan.
setiap kelompok belum tentu ada yang bisa, kadang ada yang lebih pro didalamnya, tapi kadang juga ada yang memang basic saja baru mau belajar. sehingga sistem pembagian kelompok 2 orang tidak adil.

