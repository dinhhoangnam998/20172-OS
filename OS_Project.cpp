#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <queue>
#include <set>
#include <string>

#include <math.h>

using namespace std;


queue<int> myQ;
set<int> mys;
vector<int> mylistInput;		// print Input
int max_cld;

vector<int> order; 			// order
vector<int> dis;  		   	// distance
int cost; 					// sum cost

bool flag_insert = false;
int count;

void ReadData()
{
	cout << "\nInput cylinders want to access (Enter -1 to end): "; 
	int cld; cin >> cld;
	while( cld != -1 )
	{
		myQ.push(cld);
		mys.insert(cld);
		mylistInput.push_back(cld);
		cin >> cld;
	} 

}

void ReadData(string filename)
{
	cout << "    Reading data from file...  ";
	ifstream f (filename);

	int cld; f >> cld;
	while( cld != -1 )
	{
		myQ.push(cld);
		mys.insert(cld);
		mylistInput.push_back(cld);
		f >> cld;
	}
	
	cout << "done!";
}



void init()
{
	cost = 0;
	dis.clear();
	order.clear();
	flag_insert = false;
	count = 0;
	
	
}

void PrintResult ( vector<int> dsInput )
{
	cout << "\n\n*** Output ***\n";
	cout << " Input: ";
	for(int i = 0; i <= dsInput.size() -1 ; i++ )
		cout << dsInput[i] << "   ";
	
	cout << "\n Order: ";
	for( int i = 0; i <= order.size() - 2; i++)
		printf("%3d --> ",order[i]);
	cout << order[order.size() - 1];
	
	cout << "\n Cost :    ";
	for( int i = 0; i <= dis.size() -1; i++)
		printf(" %3d    ",dis[i]);
		
	cout << "\n Sum of cost: " << cost;
}


void PrintStepWithQueue( queue<int> Q )
{

	cout << "\n--Step " << ++count << "--" << endl;
	
	cout << "  Order:";
	for( int i = 0; i <= order.size() - 1; i++)
	{
	cout <<"  " << order[i] << " --> ";
	}
	

	cout << "\n  Queue:";
	while( !Q.empty() )
	{
		cout << "  " << Q.front(); 
		Q.pop();
	}
	
}

void PrintStepWithSet( set<int> s )
{
	cout << "\n--Step " << ++count << "--"<< endl;
	cout << "  Order:";
	for( int i = 0; i <= order.size() - 1; i++)
	{
	cout <<"  " << order[i] << " --> ";
	}
	cout << "\n  Set:";
	
	for(set<int>::iterator it = s.begin(); it != s.end(); it++)
		cout << "  " << *it;
	
}

void InsertToQueue( queue<int>& Q, vector<int>& dsInput )
{
	
	if( flag_insert == false)
	{		
		PrintStepWithQueue( Q );
		
		cout << "\nDo you want to insert cylinders? (y/n)  (c to cancle, no more insert) ";
		char ch; cin >> ch;
		
		if( ch == 'y' )
		{		
			cout << "\n --> Input cylinders want to access (Enter -1 to end): "; 
			int cld; cin >> cld;
			while( cld != -1 )
			{
				Q.push(cld);
				dsInput.push_back(cld);
				cin >> cld;
			}					
		}
		
		else if( ch == 'c')
			flag_insert = true;
	}
}

void InsertToSet( set<int>& s, vector<int>& dsInput )
{	
	
	if( flag_insert == false)
	{
		PrintStepWithSet( s );
		
		cout << "\nDo you want to insert cylinders? (y/n)  (c to cancle, no more insert) ";
		char ch; cin >> ch;
		
		if( ch == 'y' )
		{
			cout << "\n --> Input cylinders want to access (Enter -1 to end): "; 
			int cld; cin >> cld;
			while( cld != -1 )
			{
				s.insert(cld);
				dsInput.push_back(cld);
				cin >> cld;
			}			

		}
				
		else if( ch == 'c')
			flag_insert = true;
	}
}


void FCFS( queue<int> Q, vector<int> dsInput )
{
	init();
	cout << "\n\nFCFS:\n";
	int cur = Q.front(); Q.pop();
	order.push_back(cur);
	
	while( !Q.empty() )
	{
		int cld = Q.front(); Q.pop();
		order.push_back( cld );
		int d = abs( cld - cur );
		dis.push_back( d );
		cur = cld;
		cost += d;
		
		InsertToQueue(Q, dsInput);		
	}
	
	PrintResult( dsInput );
}


void SSTF( set<int> s, vector<int> dsInput )
{
	init();
	cout << "\n\nSSTF:\n";
	set<int>::iterator cur = s.find( dsInput[0] ); order.push_back(*cur);
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
		
		InsertToSet( s, dsInput);	
				
	}
	
	PrintResult( dsInput );
}


void SCAN ( set<int> s, vector<int> dsInput  ) 
{
	cout << "\n\nSCAN:\n";
	init();
	
	set<int>::iterator cur = s.find( dsInput[0] );
	order.push_back(*cur);
	bool flag = false;
	
 while(1)
 {
	
	
 	s.insert(0);
	set<int>::iterator initial = prev(cur,1);
	
	for(set<int>::iterator it = initial; it != --s.begin(); it--)
	{	
		InsertToSet( s, dsInput);
		
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
	
	
	s.insert( max_cld );
	initial = next( cur, 1);
	for(set<int>::iterator it = initial; it != s.end(); it++)
	{
		InsertToSet( s, dsInput);
	
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
 
 PrintResult( dsInput );	
}



void CSCAN(set<int> s, vector<int> dsInput) 
{
	cout <<"\n\nCSCAN:\n";
	init();
	
	set<int>::iterator cur = s.find( dsInput[0] );
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
		
		s.insert( max_cld );
		initial = --s.end();
	}
	
	PrintResult( dsInput );
}


void LOOK( set<int> s, vector<int> dsInput )
{
	cout << "\n\nLOOK:\n";
	init();
	
	set<int>::iterator cur = s.find( dsInput[0] );
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
 	
	PrintResult( dsInput );
}

void CLOOK( set<int> s, vector<int> dsInput )
{
	cout << "\n\nCLOOK:\n";
	init();
		
	set<int>::iterator cur = s.find( dsInput[0] );
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
	
	PrintResult( dsInput );	
}


void Greeting()
{
	printf("De tai 17: Chuong trinh minh hoa cho cac giai thuat quan ly truy cap dia tu.");
	printf("\n\nInput:");
	printf("\n -danh sach cac cylinders can truy cap (tu keyboard hoac file)");
	printf("\n -gia tri max cua cylinder (max_cld) (thuat toan SCAN, CSCAN need input max_cld)");
	printf("\nOutput:");
	printf("\n -thu tu cac cylinders duoc truy nhap (Order)");
	printf("\n -chi phi cua moi step (Cost)");
	printf("\n -tong chi phi theo thuat toan (Sum of cost)");
	printf("\n\nNote:");
	printf("\n -danh sach cylinders nhap tu keyboard hoac file can ket thuc boi -1");
	printf("\n -user co the insert them cylinders sau moi step");
	printf("\n -mot vai thuat toan se output them danh sach cac cylinder da sap xep (Sort)");
}

void SelectInput()
{
	printf("\n\nInput danh sach cylinders:\n 1. keyboard\n 2. file\n");
	printf("  your selection: ");
	
	int x; cin >> x;
	switch( x )
	{
		case 1: 
		{
			ReadData();
		} break;
		
		case 2: 
		{
			cout << "    input file name: ";
			string s; cin >> s; 
			ReadData( s );
		} break;
	}
	
	printf("Input max_cld = ");
	cin >> max_cld;

}

void PrintMenu()
{
	
	int x; 
	char ch;
	
	do
	{
		printf("\n=== Menu ===");
		printf("\n1. FCFS\n2. SSTF\n3. SCAN\n4. CSCAN\n5. LOOK\n6. CLOOK\n7. all");
		printf("\n your selection: "); cin >> x;
		
		switch ( x )
		{
			case 1: FCFS( myQ, mylistInput ); break;
			case 2: SSTF( mys, mylistInput ); break;
			case 3: SCAN( mys, mylistInput ); break;
			case 4: CSCAN( mys, mylistInput ); break;
			case 5: LOOK( mys, mylistInput ); break;
			case 6: CLOOK( mys, mylistInput ); break;
			case 7: { FCFS( myQ, mylistInput ); SSTF( mys, mylistInput ); SCAN( mys, mylistInput ); CSCAN( mys, mylistInput ); LOOK( mys, mylistInput ); CLOOK( mys, mylistInput ); } break;
		}
		
		printf("\n\nDo you want to try more? (y/n) ");
		 cin >> ch;
	}while( ch == 'y');
		
		
		printf("\nPress any keys to exit..."); getch();
	
}



int main()
{
	Greeting();
	SelectInput();
	PrintMenu();

 return 0;
}

