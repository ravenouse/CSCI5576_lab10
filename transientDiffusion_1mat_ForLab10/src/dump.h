//  ================================================================================
//  ||                                                                            ||
//  ||  M P I I O   D u m p   W r i t e r                                         ||
//  ||                                                                            ||
//  ================================================================================

void write_mpiio_dump(mpiInfo &myMPI)
{

  printf("************* Writing dump file *****************\n");


  // Using the hpsc2022/codes/mpiio_01/main.cpp sample code from class lecture as a 
  // guide, write phi to a single output file from any number of PEs using the
  // MPIIO library.  Consider two approaches:
  //
  // (1) Adapt the sample code to a 1D array
  // (2) Adapt phi to a 2D array and use the sample code as a start
  //
  // Either way, note that the sample code only works for a nPEx = nPEy = 2
  // situation. 
  //
  // Be sure to check the output using sb_py3.py
  //
  // Also, be sure to run a test case where you set phi equal to test values to
  // demonstrate that it is being written correctly to the binary output file.
  //
  // 

  int nTotalx = nRealx+2;
  int nTotaly = nRealy+2;
  int nPEx   = myMPI.nPEx;
  int nPEy   = myMPI.nPEy;
  int iPE    = myMPI.iPE;
  int jPE    = myMPI.jPE;

  // // 0.0 Initialize the test case
  double **A = Array2D_double(nTotalx, nTotaly);
  for (int i = 1; i <= nRealx; i++) 
    for (int j = 1; j <= nRealy; j++)
      A[i][j] = i + 10.*j + myPE/10.;

  // 0.1 Initialize the two D array for the phi
  // float **A = Array2D_float(nTotalx, nTotaly);
  // for (int i = 1; i <= nRealx; i++) 
  //   for (int j = 1; j <= nRealy; j++)
  //     {A[i][j] = phi[pid(i,j)];
  //     if (myMPI.myPE == 0)printf("A[%d][%d] = %f\n", i, j, A[i][j]);}

  // 1.0 Create the global array/picture
  MPI_Datatype myRealNodes;
  int idxStartThisPE  [2] = { 1        , 1        };  // Index coordinates of the sub-array inside this PE's array, A
  int AsizeThisPE     [2] = { nTotalx  , nTotaly  };  // Size of the A array on this PE    
  int sub_AsizeThisPE [2] = { nRealx   , nRealy   };  // Size of the A-sub-array on this PE
  MPI_Type_create_subarray(2, AsizeThisPE, sub_AsizeThisPE, idxStartThisPE, MPI_ORDER_C, MPI_DOUBLE, &myRealNodes);
  MPI_Type_commit(&myRealNodes);
  printf("PE %d: finished section1.0. myRealNodes = %d\n", myPE, myRealNodes);

  // 2.0 Create the local array/picture
  MPI_Datatype myPartOfGlobal;
  // adjust the boundaries
  // if ( myMPI.nei_e > 0 ) --nRealx;
  // if ( myMPI.nei_n > 0 ) --nRealy;
  int idxStartInGlobal [2] = { iPE * nRealx , jPE * nRealy };  // Index cordinates of the sub-arrayinside the global array
  int AsizeGlobal      [2] = { nPEx* nRealx , nPEy* nRealy };  // Size of the global array

  MPI_Type_create_subarray(2, AsizeGlobal, sub_AsizeThisPE, idxStartInGlobal, MPI_ORDER_FORTRAN, MPI_DOUBLE, &myPartOfGlobal);
  MPI_Type_commit(&myPartOfGlobal);

  // 3.0 Write the array
  MPI_File fh;
  MPI_File_open(MPI_COMM_WORLD, "output.bin", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
  MPI_File_set_view (fh, 0, MPI_FLOAT, myPartOfGlobal, "native", MPI_INFO_NULL);
  MPI_File_write_all(fh, &A[0][0], 1, myRealNodes, MPI_STATUS_IGNORE);
  MPI_File_close(&fh);

  // 4.0 Free the datatypes
  free(A[0]);
  free(A);
  
  MPI_Type_free(&myPartOfGlobal);
  MPI_Type_free(&myRealNodes);
  MPI_Finalize();
}

float  * * Array2D_float(int nRows,int nCols)
{
  float *myArray;
  myArray = new float [ nRows * nCols ];
    
  // Create a pointer that points to the beginning of each new row

  float * * myArray_ptr;
  myArray_ptr = new float * [nRows];

  int count = 0;

  for ( int row = 0 ; row < nRows ; ++ row )
    {
      myArray_ptr[row] = &myArray[ count*nCols ];
      ++count;
    }

  // Return that pointer
  
  return myArray_ptr;
}






