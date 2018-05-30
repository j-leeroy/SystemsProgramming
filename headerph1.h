#ifndef PHASE1
#define PHASE1
#include "p2h.h"
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

void load_file( string f)
{
	cout << f << " now loading.." << endl;
}
void execute( string e)
{
	load_file(e);//execute calls the load_file function
	cout << "executing.." << endl;
}
void debug( string d )
{
	cout << d << " preparing debugging.." << endl;
}
void dump(string s, string e)
{
	cout << "Start: " << s << endl;
	cout << "End: " << e << endl;
}
void help()
{
	cout << "*******:Commands avaliable to you:*******" << endl;
	cout << "load" << endl << "execute" << endl << "debug" << endl << "dump" << endl << "help" << endl << "assemble" << endl << "directory" << endl << "exit" << endl;
}
void assemble( string a,fstream &i )
{
	cout << a << " Preparing to assemble.." << endl;
	pass1(a,i);	
	//Pass2();
}
void directory()
{
	cout <<  system("ls") << endl;
}
void exit()
{
	cout << "************************Exiting Command Line************************" << endl;
	cout << "                         ******Goodbye******        " << endl;
}
#endif



