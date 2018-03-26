#include<iostream>
#include<time.h>
#include<omp.h>
#include<windows.h>
#include<xmmintrin.h>
#include<emmintrin.h>
#include<pmmintrin.h>

using namespace std;
#define ROW 2000               //行
#define COL 1000               //列
#define TYPE double       
#define STEPSSE 2
#define STEPAVX 4
#define CACHE_LINE 16  
#define CACHE_LINE_AVX 32
#define CACHE_ALIGN __declspec(align(CACHE_LINE)) //对齐
#define CACHE_ALIGN_AVX __declspec(align(CACHE_LINE_AVX))

template <typename T>
T** new_Array2D(int row, int col)
{
	int size = sizeof(T);
	int point_size = sizeof(T*);
	//先申请内存，其中sizeof(T*) * row表示存放row个行指针  
	T **arr = (T **)malloc(point_size * row + size * row * col);
	if (arr != NULL)
	{
		T *head = (T*)((int)arr + point_size * row);//T* head = (T *) arr + point_size * row;  

		for (int i = 0; i < row; ++i)
		{
			arr[i] = (T*)((int)head + i * col * size);  //arr[i] =  head + size * i * col;  
			for (int j = 0; j < col; ++j)
				new (&arr[i][j]) T; //new (head + size * (i * col + j)) T;  

		}
	}
	return (T**)arr;
}

template <typename T>
T** mal_Array2D(int row, int col)
{
	int size = sizeof(T);
	int point_size = sizeof(T*);
	T** arr = (T**)malloc(point_size * row);
	if (arr != NULL)
	{
	  for (int i = 0; i < row; ++i)
		arr[i] = (T*)(size * col);
	}
	return (T**)arr;
} 

//释放二维数组  
template <typename T>
void delete_Array2D(T **arr, int row, int col)
{
	for (int i = 0; i < row; ++i)
	    for (int j = 0; j < col; ++j)
		   arr[i][j].~T();
	if (arr != NULL)
		free((void**)arr);
}

template <typename T>
void free_Array2D(T **arr, int row)
{
	if (arr != NULL)
	{
		for (int i = 0; i < row; ++i)
		{
			free(arr[i]);
		}
		free(arr);
	}
}

int main(int argc, char* argv[])
{
	time_t t;
	TYPE m, n;
	//动态申请连续的二维数组  
	TYPE CACHE_ALIGN **A = new_Array2D<TYPE>(ROW, COL);
	TYPE CACHE_ALIGN **B = new_Array2D<TYPE>(COL, ROW);

	//LARGE_INTEGER BegainTime ; 
	//LARGE_INTEGER EndTime ; 
	//LARGE_INTEGER Frequency ;
	clock_t start, end, total;
	for (int i = 0; i<ROW; ++i)
	{
		for (int j = 0; j<COL; ++j)
		{
			srand((unsigned int)time(&t));
			n = (rand() % 100);
			m = n / 59.0000000;
			A[i][j] = B[j][i] = m;
		}
	}
	/*  ----------------------------------------------- 单线程 -----------------------------------------------  */
	// QueryPerformanceFrequency(&Frequency);
	// QueryPerformanceCounter(&BegainTime) ;
	TYPE CACHE_ALIGN **C0 = new_Array2D<TYPE>(ROW, ROW);
	start = clock();
	//for(z=0;z<20;z++){
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<ROW; j++){
			C0[i][j] = 0;
			for (int k = 0; k<COL; k++){
				C0[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	/*}*/
	end = clock();
	total = end - start;
	//QueryPerformanceCounter(&EndTime) ;
	cout << C0[99][9] << endl;
	//cout << "Time="<<( EndTime.QuadPart - BegainTime.QuadPart )*1000 / Frequency.QuadPart<<endl ;
	cout << "Time=" << total << endl;
	delete_Array2D<TYPE>(C0, ROW, ROW);
	/*  ----------------------------------------------- ***** -----------------------------------------------  */

	/*  ----------------------------------------------- 2线程 -----------------------------------------------  */
	//QueryPerformanceFrequency(&Frequency);
	//QueryPerformanceCounter(&BegainTime) ;
	// //omp_set_num_threads(2);
	TYPE CACHE_ALIGN **C1 = new_Array2D<TYPE>(ROW, ROW);
	start = clock();
#pragma omp parallel for num_threads(2)
	//for(z=0;z<20;z++){
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<ROW; j++){
			C1[i][j] = 0;
			for (int k = 0; k<COL; k++){
				C1[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	/*}*/
	end = clock();
	total = end - start;
	//QueryPerformanceCounter(&EndTime) ;
	cout << C1[99][9] << endl;
	//cout << "Time="<<( EndTime.QuadPart - BegainTime.QuadPart )*1000 / Frequency.QuadPart<<endl ;
	cout << "Time=" << total << endl;
	delete_Array2D<TYPE>(C1, ROW, ROW);
	/*  ----------------------------------------------- ***** -----------------------------------------------  */


	/*  ----------------------------------------------- 4线程 -----------------------------------------------  */
	//QueryPerformanceFrequency(&Frequency);
	//QueryPerformanceCounter(&BegainTime) ;
	//omp_set_num_threads(3);
	TYPE CACHE_ALIGN **C2 = new_Array2D<TYPE>(ROW, ROW);
	start = clock();
#pragma omp parallel for num_threads(4)
	//for(z=0;z<20;z++){
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<ROW; j++){
			C2[i][j] = 0;
			for (int k = 0; k<COL; k++){
				C2[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	/*}*/
	end = clock();
	total = end - start;
	//QueryPerformanceCounter(&EndTime) ;
	cout << C2[99][9] << endl;
	//cout << "Time="<<( EndTime.QuadPart - BegainTime.QuadPart )*1000 / Frequency.QuadPart<<endl ;
	cout << "Time=" << total << endl;
	delete_Array2D<TYPE>(C2, ROW, ROW);
	/*  ----------------------------------------------- ***** -----------------------------------------------  */


	/*  ----------------------------------------------- 8线程 -----------------------------------------------  */
	//QueryPerformanceFrequency(&Frequency);
	//QueryPerformanceCounter(&BegainTime) ;
	// omp_set_num_threads(4);
	TYPE CACHE_ALIGN **C3 = new_Array2D<TYPE>(ROW, ROW);
	start = clock();
#pragma omp parallel for num_threads(8)
	//for(z=0;z<20;z++){
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<ROW; j++){
			C3[i][j] = 0;
			for (int k = 0; k<COL; k++){
				C3[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	/*}*/
	end = clock();
	total = end - start;
	//QueryPerformanceCounter(&EndTime) ;
	cout << C3[99][9] << endl;
	//cout << "Time="<<( EndTime.QuadPart - BegainTime.QuadPart )*1000 / Frequency.QuadPart<<endl ;
	cout << "Time=" << total << endl;
	delete_Array2D<TYPE>(C3, ROW, ROW);
	/*  ----------------------------------------------- ***** -----------------------------------------------  */

	/*  -----------------------------------------------  SSE  -----------------------------------------------  */
	TYPE CACHE_ALIGN **C4 = new_Array2D<TYPE>(ROW, ROW);
	start = clock();
	TYPE CACHE_ALIGN E[2] = { 0, 0 };
	TYPE temp;
	__m128d a, b, c;
	c = _mm_set_pd(0, 0);

	TYPE CACHE_ALIGN **D = new_Array2D<TYPE>(ROW, COL);
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<COL; j++){
			D[i][j] = B[j][i];
		}
	}
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<ROW; j++){
			temp = 0.0;
			for (int k = 0; k<COL; k += STEPSSE){
				a = _mm_load_pd(&A[i][k]);
				b = _mm_load_pd(&D[j][k]);
				c = _mm_mul_pd(a, b);
				_mm_store_pd(E, c);
				temp += E[0] + E[1];
				C4[i][j] = temp;
			}
		}
	}
	end = clock();
	total = end - start;
	//cout << "Time="<<( EndTime.QuadPart - BegainTime.QuadPart )*1000 / Frequency.QuadPart<<endl ;
	cout << C4[89][9] << endl;
	cout << "Time=" << total << endl;
	delete_Array2D<TYPE>(C4, ROW, ROW);
	delete_Array2D<TYPE>(D, ROW, COL);
	/*  ----------------------------------------------- ***** -----------------------------------------------  */

	/*  ------------------------------------------------ AVX ------------------------------------------------  */
	TYPE CACHE_ALIGN **C5 = new_Array2D<TYPE>(ROW, ROW);
	start = clock();
	TYPE CACHE_ALIGN F[4] = { 0, 0 ,0,0};
	__m256d a1, b1, c1;
	c1 = _mm256_set_pd(0,0,0,0);
	TYPE CACHE_ALIGN **D1 = new_Array2D<TYPE>(ROW, COL);
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<COL; j++){
			D1[i][j] = B[j][i];
		}
	}
	for (int i = 0; i<ROW; i++){
		for (int j = 0; j<ROW; j++){
			temp = 0.0;
			for (int k = 0; k<COL; k += STEPAVX){
				a1 = _mm256_load_pd(&A[i][k]);
				b1 = _mm256_load_pd(&D1[j][k]);
				c1 = _mm256_mul_pd(a1, b1);
				_mm256_store_pd(F, c1);
				temp += F[0] +F[1]+F[2]+F[3];
				C5[i][j] = temp;
			}
		}
	}
	end = clock();
	total = end - start;
	//cout << "Time="<<( EndTime.QuadPart - BegainTime.QuadPart )*1000 / Frequency.QuadPart<<endl ;
	cout << C5[89][9] << endl;
	cout << "Time=" << total << endl;
	delete_Array2D<TYPE>(C5, ROW, ROW);
	delete_Array2D<TYPE>(D1, ROW, COL);
	/*  ----------------------------------------------- ***** -----------------------------------------------  */

	delete_Array2D<TYPE>(A, ROW, COL);
	delete_Array2D<TYPE>(B, COL, ROW);

	system("pause");
	return 0;
}
