#ifndef RANDOM
#define RANDOM
#include "headerph1.h"
#include "p2h.h"

int main()
{
	fstream intermediate("intermediate.txt", ios::out);
	string command;//user command will be stored here after breaking up the input
	string temp;//temporary variable for storage
	string parameter,p2;//Variables that will hold the parameters

	cout << "************:Now entering Justin's command line interpreter:************" << endl;
	
	while(1)//First loop that will run until user enters exit
	{
		cout << "Command->";//Request user input
		getline(cin,temp);//store the line in temp variable
		int count=0;// counter that will keep track of the temp variable

		for(unsigned int i=0;i<temp.size();i++)//This for loop will store the first word into command till it reaches the first delimiter
		{
			if(temp[i]==' '){break;}
			command.push_back(temp[i]);
			count++;
		}
		count++;//increment count by 1 to account for the space

		for(unsigned int j=count;j<temp.size();j++)//This for loop will store the second word into parameter after the first space and stops when it reaches a delimiter
		{
			if(temp[j]==' '){break;}
			parameter.push_back(temp[count]);
			count++;
		}
		count++;//increment count by 1 to account for the space

		for(unsigned int k=count;k<temp.size();k++)//Repeat above for loops for p2, the second parameter
		{
			if(temp[k]==' '){break;}
			p2.push_back(temp[count]);
			count++;
		}
		while(command != "exit")//Enter second loop only if command is not exit
		{
			// if else tree that checks for commands
			if(command == "load")
			{
				//Inside each if else branch are if statements that check for the presence or absence of parameters
				if(parameter.empty() == 1)//check for parameter
				{
					cout << "load requires a parameter, retype command WITH parameter" << endl;
					command.clear();
					break;
				}
				if(p2.empty() == 0)//double check no extra parameters
				{
					cout << "load only requires ONE parameter, retype command WITH ONE parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				load_file(parameter);
				command.clear();//Empty the string to work again in the while loop
				parameter.clear();//same thing
				break;
				
			}
			else if(command == "execute")
			{
				if(parameter.empty() == 1)
				{
					cout << "execute requires a parameter, retype command WITH paramter" << endl;
					command.clear();
					parameter.clear();
					break;
				}
				if(p2.empty() == 0)
				{
					cout << "execute only requires ONE parameter, retype command WITH ONE parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				execute(parameter);
				command.clear();
				parameter.clear();
				break;
			}
			else if(command == "debug")
			{
				if(parameter.empty() == 1 )
				{
					cout << "debug requires a parameter, retype command WITH paramter" << endl;
					command.clear();
					parameter.clear();
					break;
				}
				if(p2.empty() == 0)
				{
					cout << "debug only requires ONE parameter, retype command WITH ONE parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				debug(parameter);
				command.clear();
				parameter.clear();
				break;
			}
			else if(command == "dump")
			{
				if(parameter.empty() || p2.empty() == 1)
				{
					cout << "dump requires TWO parameters, retype command WITH TWO parameters" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				dump(parameter,p2);
				command.clear();
				parameter.clear();
				p2.clear();
				break;
			}
			else if(command == "assemble")
			{
				if(parameter.empty() == 1 )
				{
					cout << "assemble requires a parameter, retype command WITH parameter" << endl;
					command.clear();
					break;
				}
				if(p2.empty() == 0)
				{
					cout << "assemble only requires ONE parameter, retype command WITH ONE parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				assemble(parameter,intermediate);
				command.clear();
				parameter.clear();
				break;
			}
			else if(command == "directory")
			{
				if(parameter.empty() == 0)
				{
					cout << "directory does not require a parameter, retype command WITHOUT  parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				directory();
				break;
			}
			else if(command == "help")
			{
				if(parameter.empty() == 0)
				{
					cout << "help does not require a parameter, retype command WITHOUT  parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
				help();
				command.clear();
				break;
			}
			else//This last statement only occurs if the command was not recgonized
			{	
				cout << "Command was not recgonized, type help for avaiable commands" << endl;
				command.clear();
				parameter.clear();
				p2.clear();
				break;
			}
		}
		//coming out of the second loop, still in the first loop, checks to see if command is exit 
		if(command == "exit")
		{ 
			if(parameter.empty() == 0)//checks to make sure no parameters are passed
				{
					cout << "exit does not require a parameter, retype command WITHOUT  parameter" << endl;
					command.clear();
					parameter.clear();
					p2.clear();
					break;
				}
			exit(); break;
		}
				
	}




	return 0;
}
#endif
