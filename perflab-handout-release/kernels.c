/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Spencer C. Bonds",     /* Full name */
  "u0885366@utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;


  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}

char complex_1_descr[] = "complex_1: basic revision";
void complex_1(int dim, pixel *src, pixel *dest)
{
  int i, j, ii, jj, i32, j32;
  int i_dim, src_gray;
  pixel source, destination;
  pixel *src_ptr;
  
  src_ptr = src;
  for(i = 0; i < dim; i+=32){
     i32 = i+32;
   
     for(j = 0; j < dim; j+=32){
        j32 = j+32; 
        for(ii=i; ii<i32; ii++){
          i_dim = ii*dim;
          src_ptr = src + i_dim + j;
       
           for(jj=j; jj<j32; jj++){

              source = *src_ptr++;
             
              src_gray = ((int)source.red + (int)source.green +(int)source.blue) / 3;

              destination.red = src_gray;
              destination.green = src_gray;   
              destination.blue = src_gray;
             
              dest[RIDX(dim - jj - 1, dim - ii - 1, dim)] = destination;   
	     
	          }
	        }     
      }
    }
}

/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  //naive_complex(dim, src, dest);
  complex_1(dim,src,dest);
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
  add_complex_function(&complex_1, complex_1_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/

static pixel weighted_2i(int dim, int i, int j, pixel *src)
{
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i,j+1,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green;

  current_pixel.red = (unsigned short) (red / 2);
  current_pixel.green = (unsigned short) (green / 2);
  current_pixel.blue = (unsigned short) (blue / 2);

  return current_pixel;
}

static pixel weighted_2j(int dim, int i, int j, pixel *src)
{
   pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i+1,j,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green;

  current_pixel.red = (unsigned short) (red / 2);
  current_pixel.green = (unsigned short) (green / 2);
  current_pixel.blue = (unsigned short) (blue / 2);

  return current_pixel;
}

static pixel weighted_3i(int dim, int i, int j, pixel *src)
{
   pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int j1 = j+1;
  int j2 = j+2;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i,j1,dim)];
  pixel neighbor_2 = src[RIDX(i,j2,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red +
        (int) neighbor_2.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue +
        (int) neighbor_2.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green +
        (int) neighbor_2.green;

  current_pixel.red = (unsigned short) (red / 3);
  current_pixel.green = (unsigned short) (green / 3);
  current_pixel.blue = (unsigned short) (blue / 3);

  return current_pixel;
}

static pixel weighted_3j(int dim, int i, int j, pixel *src)
{
   pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int i1 = i+1;
  int i2 = i+2;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i1,j,dim)];
  pixel neighbor_2 = src[RIDX(i2,j,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red +
        (int) neighbor_2.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue +
        (int) neighbor_2.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green +
        (int) neighbor_2.green;

  current_pixel.red = (unsigned short) (red / 3);
  current_pixel.green = (unsigned short) (green / 3);
  current_pixel.blue = (unsigned short) (blue / 3);

  return current_pixel;
}

static pixel weighted_4(int dim, int i, int j, pixel *src)
{
   pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int i1 = i+1;
  int j1 = j+1;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i1,j,dim)];
  pixel neighbor_2 = src[RIDX(i,j1,dim)];
  pixel neighbor_3 = src[RIDX(i1,j1,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red +
        (int) neighbor_2.red +
        (int) neighbor_3.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue +
        (int) neighbor_2.blue +
        (int) neighbor_3.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green +
        (int) neighbor_2.green +
        (int) neighbor_3.green;

  current_pixel.red = (unsigned short) (red / 4);
  current_pixel.green = (unsigned short) (green / 4);
  current_pixel.blue = (unsigned short) (blue / 4);

  return current_pixel;
}

static pixel weighted_6i(int dim, int i, int j, pixel *src)
{
   pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int i1 = i+1;
  int j1 = j+1;
  int j2 = j+2;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i1,j,dim)];
  pixel neighbor_2 = src[RIDX(i,j1,dim)];
  pixel neighbor_3 = src[RIDX(i1,j1,dim)];
  pixel neighbor_4 = src[RIDX(i,j2,dim)];
  pixel neighbor_5 = src[RIDX(i1,j2,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red +
        (int) neighbor_2.red +
        (int) neighbor_3.red +
        (int) neighbor_4.red +
        (int) neighbor_5.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue +
        (int) neighbor_2.blue +
        (int) neighbor_3.blue +
        (int) neighbor_4.blue +
        (int) neighbor_5.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green +
        (int) neighbor_2.green +
        (int) neighbor_3.green +
        (int) neighbor_4.green +
        (int) neighbor_5.green;

  current_pixel.red = (unsigned short) (red / 6);
  current_pixel.green = (unsigned short) (green / 6);
  current_pixel.blue = (unsigned short) (blue / 6);

  return current_pixel;
}

static pixel weighted_6j(int dim, int i, int j, pixel *src)
{
   pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int i1 = i+1;
  int i2 = i+2;
  int j1 = j+1;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i1,j,dim)];
  pixel neighbor_2 = src[RIDX(i2,j,dim)];
  pixel neighbor_3 = src[RIDX(i,j1,dim)];
  pixel neighbor_4 = src[RIDX(i1,j1,dim)];
  pixel neighbor_5 = src[RIDX(i2,j1,dim)];

  red=(int) neighbor_0.red +
        (int) neighbor_1.red +
        (int) neighbor_2.red +
        (int) neighbor_3.red +
        (int) neighbor_4.red +
        (int) neighbor_5.red;
  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue +
        (int) neighbor_2.blue +
        (int) neighbor_3.blue +
        (int) neighbor_4.blue +
        (int) neighbor_5.blue;
  green=(int) neighbor_0.green +
        (int) neighbor_1.green +
        (int) neighbor_2.green +
        (int) neighbor_3.green +
        (int) neighbor_4.green +
        (int) neighbor_5.green;

  current_pixel.red = (unsigned short) (red / 6);
  current_pixel.green = (unsigned short) (green / 6);
  current_pixel.blue = (unsigned short) (blue / 6);

  return current_pixel;
}

static pixel weighted_9(int dim, int i, int j, pixel *src)
{
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int i1 = i+1;
  int i2 = i+2;
  int j1 = j+1;
  int j2 = j+2;

  pixel neighbor_0 = src[RIDX(i,j,dim)];
  pixel neighbor_1 = src[RIDX(i1,j,dim)];
  pixel neighbor_2 = src[RIDX(i2,j,dim)];
  pixel neighbor_3 = src[RIDX(i,j1,dim)];
  pixel neighbor_4 = src[RIDX(i1,j1,dim)];
  pixel neighbor_5 = src[RIDX(i2,j1,dim)];
  pixel neighbor_6 = src[RIDX(i,j2,dim)];
  pixel neighbor_7 = src[RIDX(i1,j2,dim)];
  pixel neighbor_8 = src[RIDX(i2,j2,dim)];

  red=(int) neighbor_0.red +
	      (int) neighbor_1.red +
	      (int) neighbor_2.red +
	      (int) neighbor_3.red +
	      (int) neighbor_4.red +
	      (int) neighbor_5.red +
	      (int) neighbor_6.red +
	      (int) neighbor_7.red +
	      (int) neighbor_8.red;

  blue=(int) neighbor_0.blue +
        (int) neighbor_1.blue +
        (int) neighbor_2.blue +
        (int) neighbor_3.blue +
        (int) neighbor_4.blue +
        (int) neighbor_5.blue +
        (int) neighbor_6.blue +
        (int) neighbor_7.blue +
        (int) neighbor_8.blue;

  green=(int) neighbor_0.green +
        (int) neighbor_1.green +
        (int) neighbor_2.green +
        (int) neighbor_3.green +
        (int) neighbor_4.green +
        (int) neighbor_5.green +
        (int) neighbor_6.green +
        (int) neighbor_7.green +
        (int) neighbor_8.green;

  current_pixel.red = (unsigned short) (red / 9);
  current_pixel.green = (unsigned short) (green / 9);
  current_pixel.blue = (unsigned short) (blue / 9);

  return current_pixel;
}


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
	num_neighbors++;
	red += (int) src[RIDX(i+ii,j+jj,dim)].red;
	green += (int) src[RIDX(i+ii,j+jj,dim)].green;
	blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}





/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


// seperates the calls to weighted combo to handle specific points within the grid
// and more optimally handle the rest
char complex_motion_descr[] = "weighted_motion: Optimized implementation";
void complex_motion(int dim, pixel *src, pixel *dst)
{
  int i, j, dim1, dim2;
  dim1 = dim-1;
  dim2 = dim1-1;

  for (i = 0; i < dim2; i++){
    for (j = 0; j < dim2; j++){
      // middle calls
      dst[RIDX(i, j, dim)] = weighted_9(dim, i, j, src);
    }
    dst[RIDX(i, dim2, dim)] = weighted_6j(dim, i, dim2, src);
    dst[RIDX(i, dim1, dim)] = weighted_3j(dim, i, dim1, src);
  }
  for (j = 0; j < dim2; j++){
     dst[RIDX(dim2, j, dim)] = weighted_6i(dim, dim2, j, src);
     dst[RIDX(dim1, j, dim)] = weighted_3i(dim, dim1, j, src);
  }
  dst[RIDX(dim2, dim2, dim)] = weighted_4(dim, dim2, dim2, src);
  dst[RIDX(dim2, dim1, dim)] = weighted_2j(dim, dim2, dim1, src); 
  dst[RIDX(dim1, dim2, dim)] = weighted_2i(dim, dim1, dim2, src);
  dst[RIDX(dim1, dim1, dim)] = src[RIDX(dim1,dim1,dim)];
}

/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}


/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  //naive_motion(dim, src, dst);
  complex_motion(dim, src, dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
  add_motion_function(&complex_motion, complex_motion_descr);
}
