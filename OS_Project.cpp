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


queue<int> Q;
set<int> s;
vector<int> dis;  		   	// distance
vector<int> order; 			// order
int cost; 					// sum cost
vector<int> listInput;		// print Input
int max_cld;

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

void ReadData(string filename)
{
	cout << "    Reading data from file...  ";
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
	
	
	s.insert( max_cld );
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
		
		s.insert( max_cld );
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

void Greeting()
{
	printf("De tai 17: Chuong trinh minh hoa cho cac giai thuat quan ly truy cap dia tu.");
	printf("\n\nInput:");
	printf("\n -danh sach cac cylinders can truy cap (tu keyboard hoac file)");
	printf("\n -gia tri lon nhat co the cua cylinder (max_cld)");
	printf("\nOutput:");
	printf("\n -thu tu cac cylinders duoc truy nhap (Order)");
	printf("\n -chi phi cua moi step (Cost)");
	printf("\n -tong chi phi theo thuat toan (Sum of cost)");
	printf("\n\nNote:");
	printf("\n -danh sach cylinders nhap tu keyboard hoac file can ket thuc boi -1");
	printf("\n -user co the insert them cylinders sau moi step");
	printf("\n -mot vai thuat toan co the se output them danh sach cac cylinder theo\n  thu tu \
tang dan de tien theo doi");
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
	
	printf("\n\nInput max_cld = ");
	cin >> max_cld;
}

void PrintMenu()
{
	
	int x; 
	char ch;
	
	do
	{
		printf("\n\n=== Lua chon thuat toan ===");
		printf("\n1.FCFS\n2.SSTF\n3.SCAN\n4.CSCAN\n5.LOOK\n6.CLOOK\n7.all");
		printf("\n your selection: "); cin >> x;
		
		switch ( x )
		{
			case 1: FCFS( Q ); break;
			case 2: SSTF( s ); break;
			case 3: SCAN( s ); break;
			case 4: CSCAN( s ); break;
			case 5: LOOK( s ); break;
			case 6: CLOOK( s ); break;
			case 7: { FCFS( Q ); SSTF( s ); SCAN( s ); CSCAN( s ); LOOK( s ); CLOOK( s ); } break;
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

