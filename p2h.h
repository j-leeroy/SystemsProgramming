#ifndef HEADER
#define HEADER
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

class breaking_lines
{
public:
	char first_char;//stores the first char to check to see if its a blank or a label
	string label, comment, opcode, operand;
	bool error_duplicate_sym;
	bool error_invalid_op_code;
	bool error_invalid_byte;
	void to_intermediate( breaking_lines, int locctr, int errors, ofstream &output);//produces the intermediate file
	breaking_lines to_toke( breaking_lines &line, string temp);//tokenizer to break up the lines
	void reset_errors();
};

class OPCODE_TABLE
{
public:
	string mnemonics;
	string opcode;

};
//Array that hold the respected mnemonics and opcode value
OPCODE_TABLE opct[] = { {"ADD", "18"}, {"AND", "58"}, {"COMP","28"}, {"DIV","24"}, {"J","3C"}, {"JEQ","30"}, {"JGT","34"}, {"JLT","38"}, {"JSUB","48"}, {"LDA","00"}, {"LDCH","50"}, {"LDL","08"}, {"LDX", "04"}, {"MUL","20"}, {"OR","44"},{"RD","D8"}, {"RSUB","4C"}, {"STA","0C"}, {"STCH","54"}, {"STL","14"}, {"STX","10"}, {"SUB","1C"}, {"TD","E0"}, {"TIX","2C"}, {"WD","DC"} };

class Symbol
{
public:
	string label;
	int location;
};
//class that will perform the operations invovled in pass1
class Operations
{
public:
	string get_byte_Hex_value(string byte_oper)
	{
		stringstream ss;
		int place_holder;
		if(byte_oper[0] == 'C')//checks format
		{
			for(unsigned int i=0;i<(byte_oper.size()-3);i++)//for loop that will check each char
			{
				place_holder = byte_oper.at(i+2);//get the int value for the current char
				ss << hex <<place_holder;//convert the int to hex and add it to the stringstream
			}
			return ss.str();
		}
		else if(byte_oper[0] == 'X')
		{
			unsigned first = byte_oper.find_first_of("'");
			unsigned last = byte_oper.rfind("'");
			byte_oper.erase(last);
			byte_oper = byte_oper.substr(first+1);
			return byte_oper;
		}
		else
		{
			stringstream s2(byte_oper);
			s2 >> place_holder;
			stringstream dummy;
			dummy << setfill('0') << setw(6) <<  hex << place_holder;
			return dummy.str();
		}
	}
	string get_op_hex(string opcode)
	{
		for(int i=0;i<25;i++)
		{
			if(opct[i].mnemonics == opcode)
				return opct[i].opcode;
		}
		return "This shouldnt return be here";//get_op_hex should never return this, if it does i messed up somewhere
	}
	string get_addr( vector<Symbol> &sym,string operand)
	{
		stringstream st;
		for(unsigned int i =0;i<sym.size();i++)
		{
			if(sym[i].label == operand)
			{
				st << hex << sym[i].location;
				return st.str();
			}
		}
		return "0000";
	}
	string get_index_addr( vector <Symbol> &sym, string operand)
	{
		stringstream ss;
		unsigned pos = operand.find(",");
		operand.erase(pos,operand.max_size());
		for(unsigned int i=0;i<sym.size();i++)
		{
			if(sym[i].label == operand)
			{
				sym[i].location |= 1 <<15;
				ss << hex << sym[i].location;
				return ss.str();
			}
		}
		return "0000";
	}
	bool search_sym( vector<Symbol> &sym, string label)//searches the symbol table and returns true if symbol is found
	{
		bool bad = false;
		for(unsigned int j=0;!bad && j<sym.size();j++)
		{
			if(sym[j].label == label)
				return true;
		}
		return bad;
	}
	bool search_opt( string op )//searches the opct and returns true if it is found
	{
		bool bad = false;
		for(int i=0;i<25 && !bad;i++)
		{
			if(opct[i].mnemonics == op)
			{
				return true;
			}
		}
		return false;
	}
	int op_value(string code )//returns an int value of the opcode
	{
		int opc;
		stringstream(code) >>  opc;
		return opc;
	}
	int get_byte( breaking_lines &f )//returns an int value of the requested byte
	{
		int value = 0;
		int size = 0;
		string t1,t2;
		stringstream T(f.operand);
		for(int i=0;getline(T,t1,'\'');i++)
		{
			if(t1 == "X" || t1 == "C")
				t2 = t1;
			else
			{
				if(t2 == "C")
				{
					for(unsigned int j=0; j < t1.size(); j++)
						size++;
					value = size;
				}
				else if(t2 == "X")
				{
					for(unsigned int j=0;j<t1.size();j++)
						size++;
					if(size%2 == 0)
					{
						return size/2;
					}
					else
					{
						f.error_invalid_byte = 1;
						return 0;
					}
				}
			}
		}
		return value;
	}
	bool set_error_flag(){return 1;}
	void PrintSymbolTable( vector<Symbol> &s)
	{
		cout << "************************SYMBOL-TABLE************************" << endl;
		for(unsigned int i=0; i < s.size(); i++)
			cout << s[i].label << " " << hex << s[i].location << endl;
	}
	void PrintOpcodeTable( )
	{
		cout << "************************OPCODE-TABLE************************" << endl;
		for(int i=0; i < 25; i++)
			cout << opct[i].mnemonics << " " << opct[i].opcode << endl;
	}
	
};
void reset_errors(breaking_lines &l)
{
	l.error_duplicate_sym = 0;
	l.error_invalid_byte = 0;
	l.error_invalid_op_code = 0;
}
string get_opcode( breaking_lines f,vector<Symbol> s )
{
	
	for(int i=0;i<25;i++)
	{
		if(f.opcode == opct[i].mnemonics)
		{
			return opct[i].opcode;
		}

	}
	if(f.opcode == "WORD")
		return f.operand;
	return f.opcode;
}
void to_intermediate( breaking_lines line, int locctr, fstream &output,vector<Symbol> s)//Produces the intermediate file
{
	if(line.opcode == "")
	{
		output << line.comment;
	}
	else if(line.opcode == "END")
	{
		output << line.opcode << " " << line.operand << " ";
		output << "Errors:";
		if(line.error_duplicate_sym || line.error_invalid_op_code || line.error_invalid_byte)
		{
			if(line.error_duplicate_sym)
				output << "00xS";//00xS = duplicate symbol
			if(line.error_invalid_op_code)
				output << "00xO";//00xO = invalid opcode
			if(line.error_invalid_byte)
				output << "00xB";//00xB = invalid byte
			output << ";" << line.comment;
		}
		else
			output << "0" << " ;" << line.comment;
	}
	else
	{
		output << hex << locctr << " "  << line.label << " " <<  line.opcode << " "  << line.operand << " ";
		output << "Errors:";
		if(line.error_duplicate_sym || line.error_invalid_op_code || line.error_invalid_byte)
		{
			if(line.error_duplicate_sym)
				output << "00xS";//00xS = duplicate symbol
			if(line.error_invalid_op_code)
				output << "00xO";//00xO = invalid opcode
			if(line.error_invalid_byte)
				output << "00xB";//00xB = invalid byte
			output << " ;" << line.comment;

		}
		else
			output << "0 " << " ;" << line.comment;
	}
	output << endl;
}
breaking_lines to_toke(breaking_lines &line, string temp)
{
	stringstream converter(temp);
	converter >> noskipws >> line.first_char;//Reads into first_char to check including white spaces
	if( !ispunct(line.first_char))//checks to see if it is a comment
	{
		if( isalpha(line.first_char))//checks to see if it is a label
		{
			converter >> line.label >> skipws >> line.opcode >> skipws >> line. operand;
			line.label = line.first_char + line.label;//Appends first_char to label to correct itself
			getline(converter,line.comment);
			
		}
		else if( isspace(line.first_char))//No label, then place empty into label 
		{
			line.label = "";
			converter >> std::skipws >> line.opcode >> line.operand;
			getline(converter,line.comment);
			if(line.opcode == "RSUB")//Had trouble with RSUB because it would take the comment and place it in operand field
			{
				line.comment = line.operand + line.comment;
				line.operand.clear();
			}
			
		}
	}
	else
	{
		line.label = "";
		line.opcode = "";
		line.operand = "";
		getline(converter,line.comment);
		line.comment = line.first_char + line.comment;
	}
	return line;
}

void Pass2(vector<Symbol> s, int pl);
void pass1( string file, fstream &intermediate )
{

	breaking_lines line;
	reset_errors(line);
	Symbol symbol;
	vector<Symbol> s;//using a vector to store the symbols
	Operations operation;
	int LOCCTR, PL;
	
	ifstream inFile(file.c_str());
	if(inFile == NULL)
	{
		cout << "Failed to open" << endl; 
		return;
	}

	inFile >> line.first_char >> line.label >> line.opcode >> line.operand;//reads the first line
	line.label = line.first_char + line.label;//Appends the first_char to label to correct for placement

	getline(inFile,line.comment);
	if(line.opcode == "START")
		stringstream(line.operand) >> hex >> LOCCTR; 
	else
		stringstream("0") >> hex >> LOCCTR;
	
	PL = LOCCTR;
	to_intermediate(line, LOCCTR, intermediate,s);
	getline(inFile, file);
	to_toke( line, file);
	do
	{
		if(operation.search_sym(s,line.label))//checks to see if label already exists
			line.error_duplicate_sym = operation.set_error_flag();
		else if(line.label=="")
		{
			//so it wont place a "" into the symbol table
		}
		else
		{
			symbol.label = line.label;
			symbol.location = LOCCTR;
			s.push_back(symbol);
			
		}
		int old_locctr = LOCCTR;
		if(operation.search_opt(line.opcode))
			LOCCTR += 3;
		else if(line.opcode == "WORD")
			LOCCTR += 3;
		else if(line.opcode == "RESW")
			LOCCTR += 3*operation.op_value(line.operand);
		else if(line.opcode == "RESB")
			LOCCTR += operation.op_value(line.operand);
		else if(line.opcode == "BYTE")
			LOCCTR += operation.get_byte(line);
		else
			line.error_invalid_op_code = operation.set_error_flag();

		to_intermediate(line,old_locctr, intermediate,s);
		line.comment.clear(); 
		getline(inFile, file);
		to_toke( line, file);
		reset_errors(line);
	}while(line.opcode != "END");
	//do_while loop ends when "END" is found
	getline(inFile,file);
	to_toke(line,file);
	
	to_intermediate(line,LOCCTR,intermediate,s);
	
	//calcluate program length
	PL = LOCCTR - PL;
	cout << "PROGRAM LENGTH:" << hex << PL << endl;
	Pass2(s,PL);

}

ifstream interfile("intermediate.txt");
ofstream ObjectFile("Object.txt");
ofstream ListingFile("listing.txt");

class READ
{
public:
	string label,opcode,operand,errors,locctr,comment;
	bool undefine_sym;
	bool dummy_flag;
	bool perfect;//perfect stays true till it hits an error in the intermediate
	READ(){undefine_sym=0;dummy_flag=0;perfect=1;}
};

class OBJC
{
public:
	string middle,assembled;
	int size;
};
void toke2(READ &r, string temp)
{
	Operations op;
	stringstream converter(temp);
	converter >> r.locctr;
	if(r.locctr == ".")
	{
		temp.erase(0,1);
		r.label = temp;
	    r.opcode.clear(); r.operand.clear(); r.errors.clear(); r.comment.clear();
		return;
	}
	else if(r.locctr == "END")
	{
		r.opcode = r.locctr;
		converter >> r.operand >> r.errors;
		r.locctr.clear();
		return;
	}
	converter >> r.label;
	if(op.search_opt(r.label))
	{
		if(r.label == "RSUB")
		{
			r.opcode = r.label; r.label.clear(); r.operand.clear();
			converter >> r.errors;
			getline(converter,r.comment);
		}
		else
		{
			r.opcode = r.label; r.label.clear();
			converter >> r.operand >> r.errors;
			getline(converter,r.comment);
		}
	}
	else
		converter >> r.opcode >> r.operand >> r.errors; getline(converter,r.comment);
	if(r.errors == "Errors:00xB" || r.errors == "Errors:00xO" || r.errors == "Errors:00xS")
		r.perfect = 0;

}
void Pass2(vector<Symbol> s, int pl)
{
	string line;
	READ Read;
	OBJC Objc;
	Objc.size = 0;
	Operations op;
	ostringstream ss;
	getline(interfile,line);
	toke2(Read,line);
	if(Read.opcode == "START")
	{
		ListingFile << Read.locctr << " " << Read.label << " " << Read.opcode << " " << Read.operand  << " " << Read.comment << ";" << endl;
	}
	ObjectFile << "H"  << Read.label << "  "  << setfill('0') << setw(6) << Read.locctr << setfill('0') << setw(6) << hex << pl << endl;
	getline(interfile,line);
	toke2(Read,line);
	ObjectFile << "T" << setfill('0') << setw(6)  << Read.locctr;
	while(Read.opcode != "END")
	{
		if(Read.locctr != ".")
		{
			if(op.search_opt(Read.opcode))
			{
				if(Read.operand != "")
				{
					if(op.search_sym(s,Read.operand))
					{
						Objc.middle = op.get_addr(s,Read.operand);
					}
					else if(Read.operand.at(Read.operand.size()-2)==',' && Read.operand.at(Read.operand.size()-1) =='X' )
					{
						Objc.middle = op.get_index_addr(s,Read.operand);
					}
					else
					{
						Objc.middle = "0";
						Read.undefine_sym = 1;
					}
				}
				else
					Objc.middle = "0000";
				Objc.middle = op.get_op_hex(Read.opcode)+Objc.middle;
				Objc.size = Objc.size + Objc.middle.size();
			}
			else if(Read.opcode == "BYTE" || Read.opcode == "WORD")
			{
				Objc.middle = op.get_byte_Hex_value(Read.operand);
				Objc.size = Objc.size + Objc.middle.size();
			}
			else if(Read.opcode == "RESW" || Read.opcode == "RESB")
			{
				Objc.middle = "      ";
				Objc.size = Objc.size + 6;
				
			}
			if(Objc.size >= 60)
			{
					unsigned found = Objc.assembled.find(" ");
					if(found != std::string::npos)
					{
						unsigned first = Objc.assembled.find_first_of(" ");
						unsigned last = Objc.assembled.rfind(" ");
						Objc.assembled.erase(first,last);
						if(Objc.size > 60)
						{
							ObjectFile << hex << (Objc.assembled.size()/2) << Objc.assembled << endl;
							Objc.assembled.clear();
							Objc.size = 0;
							ObjectFile << "T"; Read.dummy_flag = 1;
						}
						else
						{
							ObjectFile << hex << (Objc.assembled.size()/2) << Objc.assembled << endl;
							Objc.assembled.clear();
							Objc.size = 0;
							ObjectFile << "T"; Read.dummy_flag = 1;
							//<< setfill('0') << setw(6) << Read.locctr;
						}
					}
					else if(Objc.size > 60)
					{
						ObjectFile << hex << (Objc.assembled.size()/2) << Objc.assembled << endl;
						Objc.assembled.clear();
						Objc.size = Objc.middle.size();
						ObjectFile << "T" << setfill('0') << setw(6) << Read.locctr;
						Objc.assembled = Objc.middle; Objc.middle.clear();
					}
					else
					{
						Objc.assembled = Objc.assembled + Objc.middle;
						ObjectFile << hex << (Objc.assembled.size()/2) << Objc.assembled << endl;
						Objc.assembled.clear();
						Objc.size = 0;
						ObjectFile << "T"; Read.dummy_flag = 1;
						//<< setfill('0') << setw(6) << Read.locctr;
						//system("pause>nul");
					}
			}
			else
				Objc.assembled = Objc.assembled + Objc.middle;
		}
		ListingFile << Read.locctr << " " << Read.label << " " << Read.opcode << " " << Read.operand << " " << Read.errors << setw(Read.comment.size()) << Read.comment <<";" << " " << Objc.middle << endl; 
		Objc.middle.clear();
		getline(interfile,line);
		toke2(Read,line);
		if(Read.dummy_flag == 1 && Read.locctr != ".")
		{
			ObjectFile << setfill('0') << setw(6) << Read.locctr;
			Read.dummy_flag = 0;
		}
	}
	Objc.assembled = Objc.assembled + Objc.middle;
	ObjectFile << hex << setw(2) << (Objc.assembled.size()/2) << Objc.assembled << endl;
	ObjectFile << "E" << setfill('0') << setw(6) <<  s[0].location;
	ListingFile << Read.opcode << " " << Read.operand << endl;
	ListingFile << "************************SYMBOL-TABLE************************" << endl;
	for(unsigned int i=0; i < s.size(); i++)
		ListingFile << s[i].label << " " << hex << s[i].location << endl;
	ObjectFile.close();
	ListingFile.close();
	if(Read.undefine_sym == 1 || Read.perfect == 0)
	{
		remove("Object.txt");
		cout << "Errors in source, no object file created" << endl;
	}
	else
		cout << "Assembly successfully completed" << endl;

}

#endif
