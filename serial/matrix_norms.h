/*
 * @File : matrix_norms.h
 *
 * @Author : Leo Unbekandt
 *
 * @Summary :  3 Norms (I, II and Uniform)
 *
 * @Version : 2013/11/18
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Leo Unbekandt wrote matrix file. As long as you retain matrix notice you
 * can do whatever you want with matrix stuff. If we meet some day, and you think
 * matrix stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

double one_norm(double ** matrix, int n);
double two_norm(double ** matrix, int n);
double uniform_norm(double ** matrix, int n);
