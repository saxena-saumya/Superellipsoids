/* Author: Saumya Saxena
CS536: Computer Graphics - Assignment 4
Date: November 26, 2018
Description: Superellipsoids
*/

/* Header Files */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <string.h>
#include <fstream>
#include <math.h>
#include <cmath>
#include <vector>

#define M_PI       3.141592653589793238463
using namespace std;

struct point
{
	double x, y, z;
};

/* Function to calculate sign */
double sgn(double x)
{
	if (x < 0)
		return -1;
	if (x == 0)
		return 0;
	if (x > 0)
		return 1;
}

/* Function to calculate the c auxillary function */
double c(double w, double m)
{
	double cosine = cos(w);
	double sign = sgn(cosine);

	cosine = abs(cosine);

	double powm = pow(cosine, m);
	double cc = sign*powm;

	return cc;
}

/* Function to calculate the s auxillary function */
double s(double w, double m)
{
	double sine = sin(w);
	double sign = sgn(sine);

	sine = abs(sine);

	double powm = pow(sine, m);
	double ss = sign*powm;

	return ss;
}

/* Function to calculate the superellipsoid */
struct point superellipsoid(int A, int B, int C, double s1, double s2, double u, double v)
{
	struct point p;
	p.x = A*c(v, s1)*c(u, s2);
	p.y = B*c(v, s1)*s(u, s2);
	p.z = C*s(v, s1);
	return p;
}

/* Function to calculate the normals */
struct point normal(int A, int B, int C, double s1, double s2, double u, double v)
{
	struct point n;

	n.x = (1.000 / A)*c(v, 2.000 - s1)*c(u, 2.000 - s2);
	n.y = (1.000 / B)*c(v, 2.000 - s1)*s(u, 2.000 - s2);
	n.z = (1.000 / C)*s(v, 2.000 - s1);

	return n;
}

int main(int argc, char** argv)
{
	double num_u = 19, num_v = 9;
	double du, dv;
	double s1 = 1, s2 = 1;
	int A = 1, B = 1, C = 1;
	int flat_shaded = 0;
	int smooth_shaded = 0;

	// loop to take arguments from command line else take up default values
	for (int i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "-u") == 0)
		{
			i++;
			num_u = atof(argv[i]);
		}
		else if (strcmp(argv[i], "-v") == 0)
		{
			i++;
			num_v = atof(argv[i]);
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			i++;
			s1 = atof(argv[i]);
			//cout<<argv[i]<<" ";
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			i++;
			s2 = atof(argv[i]);
			//cout<<argv[i]<<" ";
		}
		else if (strcmp(argv[i], "-A") == 0)
		{
			i++;
			A = atof(argv[i]);
			//cout<<argv[i]<<" ";
		}
		else if (strcmp(argv[i], "-B") == 0)
		{
			i++;
			B = atof(argv[i]);
			//cout<<argv[i]<<" ";
		}
		else if (strcmp(argv[i], "-C") == 0)
		{
			i++;
			C = atof(argv[i]);
			//cout<<argv[i]<<" ";
		}
		else if (strcmp(argv[i], "-S") == 0)
		{
			smooth_shaded = 1;
			//cout<<argv[i]<<" ";
		}
		else if (strcmp(argv[i], "-F") == 0)
		{
			flat_shaded = 1;
			//cout<<argv[i]<<" ";
		}
	}

	if (smooth_shaded == 0 && flat_shaded == 0)
		flat_shaded = 1;

	du = 2.00*M_PI / (num_u - 1.00);
	dv = M_PI / (num_v - 1.00);

	vector<vector<struct point> > vertices;
	vector<vector<struct point> > normals;

	int numberOfPoints = 0;
	int m = 0, n = 0;

	cout << "#Inventor V2.0 ascii\n";

	cout << "ShapeHints {";
	cout << " vertexOrdering" << "        " << "COUNTERCLOCKWISE\n";
	cout << "}\n";

	cout << "Separator {\n";
	cout << " Coordinate3 {\n";
	cout << "   point [\n";
	int i = 0, j = 0;

	cout << "0.0 0.0 1.0,\n";

	for (double v = -M_PI_2; v <= M_PI_2; v += dv)
	{		
		for (double u = -M_PI; u <= M_PI; u += du)
		{
			m++;
		}
		n++;
	}
	vertices.resize(n+1,vector<struct point>(m+1));

	n = 0;
	m = 0;

	//Loop to calculate the superellipsoid points
	for (double v = -M_PI / 2; v <= M_PI / 2; v += dv)
	{
		m = 0;
		for (double u = -M_PI; u <= M_PI; u += du)
		{
			struct point p = superellipsoid(A, B, C, s1, s2, u, v);
			vertices[i][j].x = -1 * p.x;
			vertices[i][j].y = -1 * p.y;
			vertices[i][j].z = -1 * p.z;
			numberOfPoints++;
			if(i != 0)
			{
				cout << vertices[i][j].x << " " << vertices[i][j].y << " " << vertices[i][j].z << ",\n";
			}
			j++;
			m++;
		}
		i++;
		n++;
		j = 0;
		
	}
	i = 0;
	normals.resize(100,vector<struct point>(100));

	cout << "]\n";
	cout << "}\n";
	
	//Check if want to print smooth-shaded
	if (smooth_shaded == 1)
	{
		cout << "NormalBinding {\n";
		cout << "value        PER_VERTEX_INDEXED\n";
		cout << "}\n\n";
		cout << "Normal {\n";
		cout << "vector [\n";
		cout << "0.0 0.0 1.0, \n";
		//Loop to calculate the normals
		for (double v = -M_PI / 2; v <= M_PI / 2; v += dv)
		{
			for (double u = -M_PI; u <= M_PI; u += du)
			{
				struct point n = normal(A, B, C, s1, s2, u, v);
				normals[i][j].x = -1 * n.x;
				normals[i][j].y = -1 * n.y;
				normals[i][j].z = -1 * n.z;
				if(i != 0)
				{
					cout << normals[i][j].x << " " << normals[i][j].y << " " << normals[i][j].z << ",\n";
				}
				j++;
			}
			i++;
			j = 0;
		}
		cout << "]\n";
		cout << "}\n";
	}

	cout << "IndexedFaceSet {\n";
	cout << "coordIndex [\n";

	int count1 = n + 2;
	int count2 = num_u - 1;
	int count = 1;

	vector<vector<int> > index;
	
	index.resize(count1,vector<int>(count2));

	for (int m = 0; m<count1; m++){
		for (int n = 0; n<count2; n++){
			if (m == 0){
				index[m][n] = 0;
			}
			else{
				index[m][n] = count;
				count++;
			}
		}
	}

	int x = 0;
	for (m = 1; m<count1; m++){
		for (n = 0; n<count2 - 1; n++){
			if (m == 1){
				cout << "\n" << index[m][n] << ", " << index[m][n + 1] << ", " << index[0][0] << ", -1,";
				if (n == count2 - 2){
					cout << "\n" << index[m][n + 1] << ", " << index[m][0] << ", " << index[m - 1][0] << ", -1,";
				}
			}
		}
	}

	for (m = 1; m<count1; m++){
		for (n = 0; n<count2 - 1; n++){
			if (m == count1 - 3){  
				cout << "\n" << index[m][0] << ", " << index[m - 1][n + 1] << ", " << index[m - 1][n] << ", -1,";
				if (n == count2 - 2){
					cout << "\n" << index[m][0] << ", " << index[m - 1][0] << ", " << index[m - 1][n + 1] << ", -1,";
				}
			}
		}
	}

	for (m = 1; m<count1; m++){
		for (n = 0; n<count2 - 1; n++){
			if (m == 1){
			}
			else {
				if (m <= count1 - 4){  
					cout << "\n" << index[m][n] << ", " << index[m][n + 1] << ", " << index[m - 1][n + 1] << ", -1,";
					cout << "\n" << index[m][n] << ", " << index[m - 1][n + 1] << ", " << index[m - 1][n] << ", -1,";
					if (n == count2 - 2){
						cout << "\n" << index[m][n + 1] << ", " << index[m][0] << ", " << index[m - 1][0] << ", -1,";
						cout << "\n" << index[m][n + 1] << ", " << index[m - 1][0] << ", " << index[m - 1][n + 1] << ", -1,";
					}
				}
			}
		}
	}

	cout << "     ]\n";
	cout << " }\n";
	cout << "}\n";
	return 0;
}
