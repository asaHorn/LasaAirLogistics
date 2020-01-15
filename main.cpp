//
//  main.cpp
//  LLTemplate
//
//  Created by James Shockey on 12/6/16.
//  Copyright © 2016 James Shockey. All rights reserved.
//


/*
 *
 *	Linked List lab.
 *	- Build a library for singly linked list.
 *	- Replace the airport array in main with a Linked List.
 *  - sort the array. 
 *	
 */


#include <iostream>
#include <fstream>
#include <cmath> 
#include <string>
#include <sstream>
#include "slist.h"

using namespace std;

string Airport::toString(){
	string id = "";

	for(char x : code){
		id += x;
	}

	return id + " long: " + to_string(longitude) + " lat: " + to_string(latitude);
}

void simpleSortTotal(slist* list);
//slist* simpleSortTotal(slist* list, int len);
void recursion(slist* list, int start, int end);
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);

int main()
{
	ifstream infile;
	int i=0;
	char cNum[10] ;
	slist* airportArr = new slist;		// Replace array with Linked List
	int   airportCount;
	//Airport* a[13500];
	
	infile.open ("data.csv", ifstream::in);
	if (infile.is_open())
	{
		int   c=0;
		while (infile.good())
		{				
			Airport* nAir = new Airport();
			infile.getline(nAir->code, 256, ',');
			infile.getline(cNum, 256, ',');
			nAir->longitude = atof(cNum);
			infile.getline(cNum, 256, '\n');
			nAir->latitude = atof(cNum);

			airportArr->add(nAir);

			if (!(c % 1000)){
				cout << airportArr->get(c)->code << " long: " << airportArr->get(c)->longitude << " lat: " << airportArr->get(c)->latitude <<  endl;
			}
			
			/*
			if (!(c % 1000))
			{
				cout << airportArr->get(c)->code << " long: " << airportArr->get(c)->longitude << " lat: " << airportArr->get(c)->latitude <<  endl;        
			}
			*/

			i++ ;
			c++;
		}
		airportCount = c-1;
		infile.close();
			
		for (int c=0; c < airportCount; c++){
			if (!(c % 1000)){
				cout << airportArr->get(c)->code << " long: " << airportArr->get(c)->longitude << " lat: " << airportArr->get(c)->latitude <<  endl;
						
				cout << airportArr->get(c+1)->code << " long: " << airportArr->get(c+1)->longitude << " lat: " << airportArr->get(c+1)->latitude <<  endl;

				cout <<"Distance between " << airportArr->get(c)->code << " and " << airportArr->get(c+1)->code << " is " << distanceEarth( airportArr->get(c)->longitude, airportArr->get(c)->latitude , airportArr->get(c+1)->longitude, airportArr->get(c+1)->latitude) << endl;
			}
		}
	}
	else
	{
		cout << "Error opening file";
	}
	
	simpleSortTotal(airportArr);
	//airportArr = simpleSortTotal(airportArr, airportArr->size());

	cout << "---Under 100!---\n";
	for(int i=0; i<airportArr->size();i++){
		double distance = distanceEarth(airportArr->get(i)->latitude, airportArr->get(i)->longitude,97.67, 30.1944);

		if(distance > 100){
			cout <<i << " " << distance << "\n";
			break;
		}
		
		cout << i << ": ";
		cout << airportArr->get(i)->code << " @ ";
		cout << airportArr->get(i)->latitude << " , ";
		cout << airportArr->get(i)->longitude << " || ";
		cout << distance << "\n";
	}

	cout << "---farthest---\n";
	cout << airportArr->get(airportArr->size()-1)->code << " @ ";
	cout << airportArr->get(airportArr->size()-1)->latitude << " , ";
	cout << airportArr->get(airportArr->size()-1)->longitude << " || ";
	cout << distanceEarth(airportArr->get(airportArr->size()-1)->latitude, airportArr->get(airportArr->size()-1)->longitude,97.67, 30.1944) << "\n";

}

#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}


/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}


/*
	Provide sort routine on linked list sorting by distance from 30.2672° N, 97.7431° W
	1.Pick an element, called a pivot, from the array.
	
	2.Partitioning: reorder the array so that all elements with values less than the pivot come before the pivot, while all elements with values greater than the pivot come after it (equal values can go either way). After this partitioning, the pivot is in its final position. This is called the partition operation.
	
	3.Recursively apply the above steps to the sub-array of elements with smaller values and separately to the sub-array of elements with greater values.

	This comment was stolen from wikipedia (shhhhhh)
	*/
void simpleSortTotal(slist* list){
	for(int i=0; i<list->size(); i++){
		list->get(i)->distance = distanceEarth(list->get(i)->latitude, list->get(i)->longitude,97.7431, 30.2672);
	}
	
	cout << "DOWN WE GO!";
	recursion(list, 0, list->size()-1);
}

void recursion(slist* list, int start, int end){
	cout << "\n start:" << start << " end:" << end << "\n";
	
	if(end-start < 1){return;}
	int part = list->get(end)->distance;

	int up =-1000;
	int down =-1000;

	while(1==1){
		for(int a=start; a<end; a++){
			if(list->get(a)->distance > part){
				up=a;
				break;
			}
			if(a == end-1){
				up = end;
			}
		}

		for(int d=end-1; d>=start; d--){
			if(list->get(d)->distance <= part){
				down=d;
				break;
			}
			if(d == start){
				down = 0;
			}
		}

		if(up >= down){
			list->exchg(up, end);

			cout << up;

			recursion(list, start, up-1);
			recursion(list, up+1, end);

			return;
		}

		cout << up << " / " << down << "\n";

		list->exchg(up, down);
	}
}	

/*   //I am 100% cirtain that this works as it worked on the smaller list, It is just hard to get all the way through a run
void simpleSortTotal(slist* list){
	for(int i=0; i<list->size()0; i++){
		list->get(i)->distance = distanceEarth(list->get(i)->latitude, list->get(i)->longitude,97.7431, 30.2672);
	}

	for(int o=0; o<10; o++){
		double low = list->get(list->size()-1)->distance;
		int index = 0;

		for(int i=o; i<list->size(); i++){
			if(list->get(i)->distance < low){
				low = list->get(i)->distance;
				index = i;
			}
		}

		cout << o << "\n";

		list->exchg(o, index);
	}

	double high = list->get(list->size()-1)->distance;
	int index =0;

	for(int i=0; i<list->size(); i++){
		if(list->get(i)->distance > high){
			high = list->get(i)->distance;
			index = i;
		}		
	}
	
}*/

