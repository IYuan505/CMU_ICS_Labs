/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

void transpose_32(int M, int N, int A[N][M], int B[M][N]);
void transpose_64(int M, int N, int A[N][M], int B[M][N]);



/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    int tmp_0, tmp_1, tmp_2, tmp_3;
    int tmp_4, tmp_5, tmp_6, tmp_7;
    if (M%8==0){
	// if the matrix size is the multiple of 8, using block size as 8 will perfectly suit the case, each 8 by 8 block of A will have a corresponding block in B.
        for (i = 0; i < N; i+=8) {
            for (j = 0; j < M; j+=8) {
		// Here the two variables are to check whether they are diagonal blocks or not. As each block has 8 bytes and there are 32 sets in total.
                tmp_0 = ((i*M + j) / 8) % 32;
                tmp_1 = ((j*N + i) / 8) % 32;
		
		// In order to reduce the cache miss rate, we store the diagonal blocks first into a nearby block. As for B, as long as it is a block in another column, we are able to bypass the conflicts causing by the same set nubmers in the diagonal entries.
		// So here, we handle the two blocks together, the first one is the diagonal block, the other is the nearby (in terms of column of B) block.
                tmp_2 = (((i-8)*M + j) / 8) % 32;
                tmp_3 = ((j*N + i - 8) / 8) % 32;

                if ((tmp_0==tmp_1 && i+15<N) || (tmp_2==tmp_3 && i+7<N)){
                    // check whether they are the two blocks we want.
		    if(tmp_0==tmp_1){
			// if they are the two blocks we want, we handle the two blocks all in once, thus, we have to filter out the tmp_2==tmp_3 case.
                        // This special handling is for the 64 by 64 case. Besides, it does not hurt the performance of 32 by 32 case.
			// For 64 by 64 case, each 8 by 8 block will have 4 different set nubmers. The 1st row and 5th row will conflict. The same situation holds for 2nd & 6th rows, 3rd & 7th rows and 4th & 8th rows.
			// Therefore, to reduce the conflicts that caused by writing to different rows in matrix B, we first write the first 4 elements.
			// In this case, only the first 4 rows of B are written. No conflict eviction will occur. 
			for (k = i; k < i+8; ++k)
                            for (l = j; l < j+4; ++l)
                                B[l][k+8] = A[k][l];
			// Here I use i+7 to i, because after the above operation, A[i+7], A[i+6], A[i+5], A[i+4] are still in the cache. While the other 4 rows has already be evicted. So, using i+7 to i will reduce 4 misses per block.
                        for (k = i+7; k >= i; --k)
                            for (l = j+4; l < j+8; ++l)
                                B[l][k+8] = A[k][l];
			// Here I copy the element from the nearby block to the location where it supposed to be. Because we are iterating matrix B, so row-wise loop is preferred. In this case, the outer loop becomes "l".
                        for (l = j+7; l >= j; --l)
                            for (k = i; k < 8+i; ++k)
                                B[l][k] = B[l][k+8];
			
			// After fix the problem of diagonal blocks, some of the nearby blocks are still in the cache, thus, we continue to utilize the advantage by handle them in a row. The ideas are similar.
                        for (k = i; k < i+8; ++k)
                            for (l = j; l < j+4; ++l)
                                B[l][k+8] = A[k+8][l];
                        for (k = i+7 ; k >= i; --k)
                            for (l = j+4; l < j+8; ++l)
                                B[l][k+8] = A[k+8][l];
                    }
                }
                else{
		    // If it is not diagonal blocks nor the nearby block we choose, in other words, it is a normal block.
		    // We handle they quite efficiently by considering the conflict eviction that might be caused by the different row visiting of matrix B.
                    for (k = i; k < i+8; ++k){
                        for (l = j; l < j+4; ++l)
                            B[l][k] = A[k][l];
			// Still, because we only use the first 4 elements, the 1st row of A will be reloaded then to continue reading. Thus we could utilize the 12 local variables by storing some of the informations in them. Later, we no longer have to visit A[i] to fetch the information.
                        if (k==i){
                            tmp_0 = A[k][j+4];
                            tmp_1 = A[k][j+5];
                            tmp_2 = A[k][j+6];
                            tmp_3 = A[k][j+7];
                        }
                        if (k==i+1){
                            tmp_4 = A[k][j+4];
                            tmp_5 = A[k][j+5];
                            tmp_6 = A[k][j+6];
                            tmp_7 = A[k][j+7];
                        }
                    }

                    for (k = i+7; k >= i; --k){
                        if (k==i){
                            B[j+4][k] = tmp_0;
                            B[j+5][k] = tmp_1;
                            B[j+6][k] = tmp_2;
                            B[j+7][k] = tmp_3;
                        }
                        else if (k==i+1 && j+7<M){
                            B[j+4][k] = tmp_4;
                            B[j+5][k] = tmp_5;
                            B[j+6][k] = tmp_6;
                            B[j+7][k] = tmp_7;
                        }
                        else
                            for (l = j+4; l < j+8 && l < M; ++l)
                                B[l][k] = A[k][l];
                    }
                }
            }
        }
    }
    else{
	// For 61 by 67 case, using the above solution will have a slightly higher miss count arround 2060. In order to get the full credit, we have to consider another solution.
	// For 61 by 67, they are not multiple of 8, thus, there is almost no diagonal cases that we have to handle, i.e. in a block of 8 by 8, one row will cause confict with another row.
	// However, it will have another problem, for each 8 numbers we choose, they are highly likely to belong to two data blocks. Therefore, using a block size of 8 will reduce the temporal locality.Instead, using a block size such as 16, will increase the temporal locality a bit by utilizing the two different data blocks of each row of the previously 8 by 8 data block.
        for (i = 0; i < N; i+=16) {
            for (j = 0; j < M; j+=16) {
                for (k = i; k < i+16 && k < N; ++k){
                    for (l = j; l < j+16 && l < M; ++l)
                        B[l][k] = A[k][l];
                }
            }
        }   
    }

}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char transpose_block_desc[] = "Transpose block";
void transpose_block(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    //int tmp_0, tmp_1, tmp_2, tmp_3;
    //int tmp_4, tmp_5, tmp_6, tmp_7;
    int block_size = 16;
    for(i=0; i<N; i+=block_size){
	for(j=0; j<M; j+=block_size){
	    for(k=i;k<block_size+i && k<N;k++){
		for(l=j;l<block_size+j && l<M;l++){
		    B[l][k] = A[k][l];
		}
	    }	
	}
    }
}

char transpose_32_desc[] = "Transpose 32";
void transpose_32(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k;
    int tmp_0, tmp_1, tmp_2, tmp_3;
    int tmp_4, tmp_5, tmp_6, tmp_7;

    for (i = 0; i < N; i+=8) {
        for (j = 0; j < M; j+=8) {
            for (k = i; k < 8+i; ++k){
                tmp_0 = A[k][j+0];
                tmp_1 = A[k][j+1];
                tmp_2 = A[k][j+2];
                tmp_3 = A[k][j+3];
                tmp_4 = A[k][j+4];
                tmp_5 = A[k][j+5];
                tmp_6 = A[k][j+6];
                tmp_7 = A[k][j+7];
                B[j+0][k] = tmp_0;
                B[j+1][k] = tmp_1;
                B[j+2][k] = tmp_2;
                B[j+3][k] = tmp_3;
                B[j+4][k] = tmp_4;
                B[j+5][k] = tmp_5;
                B[j+6][k] = tmp_6;
                B[j+7][k] = tmp_7;
            }
        }
    }
}

char transpose_64_desc[] = "Transpose 64";
void transpose_64(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    int tmp_0, tmp_1, tmp_2, tmp_3;
    int tmp_4, tmp_5, tmp_6, tmp_7;
    for (i = 0; i < N; i+=8) {
        for (j = 0; j < M; j+=8) {
            if (i+16>=N && j+8>=M){
                if (i+8<N){
                    // first compute the last block
                    for (k = i; k < 8+i; ++k){
                        for (l = j; l < j+4; ++l)
                            B[l][k] = A[k+8][l];
                        if(k==i){
                            tmp_0 = A[k+8][j+4];
                            tmp_1 = A[k+8][j+5];
                            tmp_2 = A[k+8][j+6];
                            tmp_3 = A[k+8][j+7];
                        }
                        else if ((k-1)==i){
                            tmp_4 = A[k+8][j+4];
                            tmp_5 = A[k+8][j+5];
                            tmp_6 = A[k+8][j+6];
                            tmp_7 = A[k+8][j+7];
                        }
                    }
                    for (k = 7+i; k >= i; --k){
                        if(k>i+1){
                            for (l = j+4; l < j+8; ++l)
                                B[l][k] = A[k+8][l];
                        }
                        else{
                            B[j+4][i] = tmp_0;
                            B[j+5][i] = tmp_1;
                            B[j+6][i] = tmp_2;
                            B[j+7][i] = tmp_3;
                            B[j+4][i+1] = tmp_4;
                            B[j+5][i+1] = tmp_5;
                            B[j+6][i+1] = tmp_6;
                            B[j+7][i+1] = tmp_7;
                        }
                    }
                    for (l = j+7; l >= j; --l){
                        for (k = i; k < 8+i; ++k){
                            B[l][k+8] = B[l][k];
                        }
                    }

                    // next compute the second last block
                    for (k = i; k < 8+i; ++k){
                        for (l = j; l < j+4; ++l)
                            B[l][k] = A[k][l];
                        if(k==i){
                            tmp_0 = A[k][j+4];
                            tmp_1 = A[k][j+5];
                            tmp_2 = A[k][j+6];
                            tmp_3 = A[k][j+7];
                        }
                        else if ((k-1)==i){
                            tmp_4 = A[k][j+4];
                            tmp_5 = A[k][j+5];
                            tmp_6 = A[k][j+6];
                            tmp_7 = A[k][j+7];
                        }
                    }
                    for (k = 7+i; k >= i; --k){
                        if(k>i+1){
                            for (l = j+4; l < j+8; ++l)
                                B[l][k] = A[k][l];
                        }
                        else{
                            B[j+4][i] = tmp_0;
                            B[j+5][i] = tmp_1;
                            B[j+6][i] = tmp_2;
                            B[j+7][i] = tmp_3;
                            B[j+4][i+1] = tmp_4;
                            B[j+5][i+1] = tmp_5;
                            B[j+6][i+1] = tmp_6;
                            B[j+7][i+1] = tmp_7;
                        }
                    }
                }
            }
            else if((i==j||(i-8)==j) && i+8 < N){
                if(i==j){
                    for (k = i; k < 8+i; ++k){
                        for (l = j; l < j+4; ++l)
                            B[l][k+8] = A[k][l];
                        if(k==i){
                            tmp_0 = A[k][j+4];
                            tmp_1 = A[k][j+5];
                            tmp_2 = A[k][j+6];
                            tmp_3 = A[k][j+7];
                        }
                        else if ((k-1)==i){
                            tmp_4 = A[k][j+4];
                            tmp_5 = A[k][j+5];
                            tmp_6 = A[k][j+6];
                            tmp_7 = A[k][j+7];
                        }
                    }
                    for (k = 7+i; k >= i; --k){
                        if(k>i+1){
                            for (l = j+4; l < j+8; ++l)
                                B[l][k+8] = A[k][l];
                        }
                        else{
                            B[j+4][i+8] = tmp_0;
                            B[j+5][i+8] = tmp_1;
                            B[j+6][i+8] = tmp_2;
                            B[j+7][i+8] = tmp_3;
                            B[j+4][i+1+8] = tmp_4;
                            B[j+5][i+1+8] = tmp_5;
                            B[j+6][i+1+8] = tmp_6;
                            B[j+7][i+1+8] = tmp_7;
                        }
                    }
                    for (l = j+7; l >= j; --l){
                        for (k = i; k < 8+i; ++k){
                            B[l][k] = B[l][k+8];
                        }
                    }

                    for (k = i; k < 8+i; ++k){
                        for (l = j; l < j+4; ++l)
                            B[l][k+8] = A[k+8][l];
                        if(k==i){
                            tmp_0 = A[k+8][j+4];
                            tmp_1 = A[k+8][j+5];
                            tmp_2 = A[k+8][j+6];
                            tmp_3 = A[k+8][j+7];
                        }
                        else if ((k-1)==i){
                            tmp_4 = A[k+8][j+4];
                            tmp_5 = A[k+8][j+5];
                            tmp_6 = A[k+8][j+6];
                            tmp_7 = A[k+8][j+7];
                        }
                    }
                    for (k = 7+i; k >= i; --k){
                        if(k>i+1){
                            for (l = j+4; l < j+8; ++l)
                                B[l][k+8] = A[k+8][l];
                        }
                        else{
                            B[j+4][i+8] = tmp_0;
                            B[j+5][i+8] = tmp_1;
                            B[j+6][i+8] = tmp_2;
                            B[j+7][i+8] = tmp_3;
                            B[j+4][i+1+8] = tmp_4;
                            B[j+5][i+1+8] = tmp_5;
                            B[j+6][i+1+8] = tmp_6;
                            B[j+7][i+1+8] = tmp_7;
                        }
                    }
                }
            }
            else{
                for (k = i; k < 8+i; ++k){
                    for (l = j; l < j+4; ++l)
                        B[l][k] = A[k][l];
                    if(k==i){
                        tmp_0 = A[k][j+4];
                        tmp_1 = A[k][j+5];
                        tmp_2 = A[k][j+6];
                        tmp_3 = A[k][j+7];
                    }
                    else if ((k-1)==i){
                        tmp_4 = A[k][j+4];
                        tmp_5 = A[k][j+5];
                        tmp_6 = A[k][j+6];
                        tmp_7 = A[k][j+7];
                    }
                }
                for (k = 7+i; k >= i; --k){
                    if(k>i+1){
                        for (l = j+4; l < j+8; ++l)
                                B[l][k] = A[k][l];
                        }
                        else{
                            B[j+4][i] = tmp_0;
                            B[j+5][i] = tmp_1;
                            B[j+6][i] = tmp_2;
                            B[j+7][i] = tmp_3;
                            B[j+4][i+1] = tmp_4;
                            B[j+5][i+1] = tmp_5;
                            B[j+6][i+1] = tmp_6;
                            B[j+7][i+1] = tmp_7;
                        }
                }
            }
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

    registerTransFunction(transpose_32, transpose_32_desc);

    registerTransFunction(transpose_64, transpose_64_desc);

    registerTransFunction(transpose_block, transpose_block_desc);
}


/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

