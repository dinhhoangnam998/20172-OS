#include <stdio.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <queue>
#include <set>

#include <math.h>

#define MAX 199 // max value of cylinder

using namespace std;


queue<int> Q;
set<int> s;
vector<int> dis;  		   	// distance
vector<int> order; 			// order
int cost; 					// sum cost
vector<int> listInput;		// print Input

void ReadData()
{
	cout << "\nInput cylinders want to access (Enter -1 to end): "; 
	int cld; cin >> cld;
	while( cld != -1 )
	{
		Q.push(cld);
		s.insert(cld);
		listInput.push_back(cld);
		cin >> cld;
	} 

}

void ReadData(char* filename)
{
	cout << "Reading Data from file...  ";
	ifstream f (filename);

	int cld; f >> cld;
	while( cld != -1 )
	{
		Q.push(cld);
		s.insert(cld);
		listInput.push_back(cld);
		f >> cld;
	}
	
	cout << "done!";
}



void init()
{
	cost = 0;
	dis.clear();
	order.clear();
	
}

void PrintRezult ( vector<int> listInput, vector<int> order, vector<int> dis, int cost  )
{
	
	cout << " Input: ";
	for(int i = 0; i <= listInput.size() -1 ; i++ )
		cout << listInput[i] << "   ";
	
	cout << "\n Order: ";
	for( int i = 0; i <= order.size() - 2; i++)
		printf("%3d --> ",order[i]);
	cout << order[order.size() - 1];
	
	cout << "\n Cost :    ";
	for( int i = 0; i <= dis.size() -1; i++)
		printf(" %3d    ",dis[i]);
		
	cout << "\n Sum of cost: " << cost;
}

void FCFS( queue<int> Q )
{
	init();
	cout << "\n\nFCFS:\n";
	int cur = Q.front(); Q.pop();
	order.push_back(cur);
	
	bool flag = false;
	char ch; 

	while( !Q.empty() )
	{
		int cld = Q.front(); Q.pop();
		order.push_back( cld );
		int d = abs( cld - cur );
		dis.push_back( d );
		cur = cld;
		cost += d;		
	}
	PrintRezult( listInput, order, dis, cost );
	

}


void SSTF( set<int> s )
{
	init();
	cout << "\n\nSSTF:\n";
	set<int>::iterator cur = s.find( listInput[0] ); order.push_back(*cur);
	int d, d_pre, d_post;
	
	while( s.size() != 1 )
	{
		
		if( cur == s.begin() && cur != --s.end() )
		{
			set<int>::iterator post = next(cur,1);
			order.push_back( *post );
			d = *post - *cur;
			dis.push_back( d );
			cost += d;
			
			s.erase( cur );
			cur = post;
		}
		else if( cur == --s.end() && cur != s.begin() )
		{
			set<int>::iterator pre = prev(cur,1);
			order.push_back( * pre );
			d = * cur - *pre;
			dis.push_back( d );
			cost += d;
			
			s.erase( cur );
			cur = pre;
		}
		else
		{
			set<int>::iterator pre = prev(cur,1);
			set<int>::iterator post = next(cur,1);
			
			d_pre = *cur - *pre;
			d_post = *post - *cur;
			
			if( d_pre < d_post )
			{
				order.push_back( *pre );
				dis.push_back( d_pre );
				cost += d_pre;
				
				s.erase ( cur );
				cur = pre;
			}
			else
			{
				order.push_back( *post );
				dis.push_back( d_post );
				cost+= d_post;
				
				s.erase( cur );
				cur = post;
			}
		}	
				
	}
	
	PrintRezult( listInput, order, dis, cost );
}


void SCAN ( set<int> s  ) //version update
{
	cout << "\n\nSCAN:\n";
	init();
	
	set<int>::iterator cur = s.find( listInput[0] );
	order.push_back(*cur);
	bool flag = false;
	
 while(1)
 {
 	s.insert(0);
	set<int>::iterator initial = prev(cur,1);
	for(set<int>::iterator it = initial; it != --s.begin(); it--)
	{
		if( s.size() == 2)
		{
			flag = true;
			break;
		}
		
		int now = *it;
		int d = *cur - now;
		order.push_back(now);
		dis.push_back(d);
		cost+=d;
		
		s.erase(cur);
		cur = it;
	}
	if( flag == true)
		break;
	
	
	s.insert(MAX);
	initial = next( cur, 1);
	for(set<int>::iterator it = initial; it != s.end(); it++)
	{
		if( s.size() == 2)
		{
			flag = true;
			break;
		}
			
		int now = *it;
		int d = now - *cur;
		order.push_back(now);
		dis.push_back(d);
		cost+=d;
		
		s.erase(cur);
		cur = it;
	
	}
	if( flag == true)
		break;
		
 }
	
	PrintRezult( listInput, order, dis, cost);
	
	
}



void CSCAN(set<int> s) //?????
{
	cout <<"\n\nCSCAN:\n";
	init();
	
	set<int>::iterator cur = s.find( listInput[0] );
	order.push_back( *cur );
	set<int>::iterator initial = prev(cur,1);
	bool flag = false;
	s.insert(0);
	while(1)
	{
		for(set<int>::iterator it = initial; it != --s.begin(); it--)
		{
			if( s.size() == 2 && *(prev( cur, 1)) == 0 )
			{
				flag = true;
				break;
			}
			
			int now = *it;
			int d = abs(*cur - now);
			order.push_back(now);
			dis.push_back(d);
			cost+=d;
			
			s.erase( cur );
			cur = it;
			s.insert(0);			
		}
		
		if( flag == true)
			break;
		
		s.insert(MAX);
		initial = --s.end();
	}
	
	PrintRezult( listInput, order, dis, cost);
}


void LOOK( set<int> s )
{
	cout << "\n\nLOOK:\n";
	init();
	
	set<int>::iterator cur = s.find( listInput[0] );
	order.push_back(*cur);
	
	while ( s.size() != 1 )
	{
		set<int>::iterator initial = prev(cur, 1);
		for(set<int>::iterator it = initial; it != --s.begin(); it--)
		{
			int now = *it;
			int d = *cur - now;
			order.push_back(now);
			dis.push_back(d);
			cost+=d;
		
			s.erase(cur);
			cur = it;	
		}
		if( s.size() == 1 )
			break;
		initial = next( cur, 1 );
		for( set<int>::iterator it = initial; it != s.end(); it++)
		{
			int now = *it;
			int d = now - *cur;
			order.push_back(now);
			dis.push_back(d);
			cost+=d;
		
			s.erase(cur);
			cur = it;
		}
		
	}
 
	
	PrintRezult( listInput, order, dis, cost);
	
	
}

void CLOOK( set<int> s )
{
	cout << "\n\nCLOOK:\n";
	init();
		
	set<int>::iterator cur = s.find( listInput[0] );
	order.push_back( *cur );
	set<int>::iterator initial = prev( cur, 1 );
	
	while( s.size() != 1)
	{
		for(set<int>::iterator it = initial; it != --s.begin(); it --)
		{
			int now = *it;
			int d = abs(now - *cur);
			order.push_back(now);
			dis.push_back(d);
			cost+=d;
		
			s.erase(cur);
			cur = it;
		}
		if( s.size() == 1)
			break;
		initial = prev(s.end(),1);
	}
	
	PrintRezult( listInput, order, dis, cost );
	
}







int main()
{
//	ReadData();
	ReadData( "Input.txt" );
	FCFS( Q );
	SSTF( s );
	SCAN( s );
	CSCAN( s );
	LOOK ( s );
	CLOOK (	s );

 printf("\n\nHello World\n");
 return 0;
}

