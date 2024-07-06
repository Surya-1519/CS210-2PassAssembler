/*****************************************************************************
TITLE: Assembler                                                                                                                             
AUTHOR: Sripati Surya Teja
ROLL NUMBER: 2201CS71
Declaration of Authorship
This C++ file, asm.cpp, is part of the assignment of CS210 at the 
department of Computer Science and Engineering, IIT Patna. 
*****************************************************************************/

#include <bits/stdc++.h>
#define pb push_back
#define mk make_pair
#define fr first 
#define sec second 
#define f(i, a, b) for (int i=a; i<=b; i++)

using namespace std;

//To store the instructions, we define a table of format: instruction name, Operand exists or not and OpCode
map<string, pair<bool, int> > availop;

//Class containing all the available Op codes
void allcom(){

	availop["ldc"] = mk(true, 0),availop["adc"] = mk(true, 1),availop["ldl"] = mk(true, 2),	
	
	availop["stl"] = mk(true, 3),availop["ldnl"] = mk(true, 4),availop["stnl"] = mk(true, 5),
		
	availop["add"] = mk(false, 6),availop["sub"] = mk(false, 7),availop["shl"] = mk(false, 8),
	
	availop["shr"] = mk(false, 9),availop["adj"] = mk(true, 10),availop["a2sp"] = mk(false, 11),
	
	availop["sp2a"] = mk(false, 12),availop["call"] = mk(true, 13),availop["return"] = mk(false, 14),
	    
	availop["brz"] = mk(true, 15),availop["brlz"] = mk(true ,16),availop["br"] = mk(true, 17),
		
	availop["HALT"] = mk(false, 18),availop["data"] = mk(true, -1),availop["SET"] = mk(true, -2);

};

//List of all instructions
vector<pair<string, string> > commands; 

//List of instructions after trimming; format is opCode operand
vector<pair<int, int> > cleanedcommands; 

//table to store PC count and line no.
map<int, int> index; 

//table to store trimmed code extracted from the test file
map<int, vector<string> > TrimmedCode; 

//List of all errors that popup in the code, if exists
vector<pair<int, string> > errata; 

//Table to store labels and line no.
map<string, int> lineRef; 

//Prototyping of all functions used

string decitohex(int num, int req);
string clean(string text);
int Refcheck(string ref);
void ReferenceInsert(string ref, int PC, int lineNo);
bool InvalidNumber(string s);
int decode(string opr, int lineNo);
void firstPass(string inFile);
void secondPass(ofstream &logFile, ofstream &outFile, ofstream &objFile);

//Main function

int main(int argc, char* argv[]){

//Check if required no.of arguments are given by the user or not
	if(argc < 2){                                                          
        cout<<"Usage: ./asm file.asm"<<endl;                            //If not then print the error
        cout<<"Where file.asm is the file to be compiled."<<endl;
        return 0;
    }

//initialize all opcodes
	allcom();         

//Extract input file name
	string inFile = string(argv[1]);     

//Initiate first pass         
	firstPass(inFile);

//Create output files
	inFile = inFile.substr(0, inFile.find('.'));
	ofstream outFile(inFile + ".l"); 
	ofstream logFile(inFile + ".log");
	ofstream objFile(inFile + ".o", ios::out|ios::binary);

//Initiate second pass
	secondPass(logFile, outFile, objFile);

	return 0;
}

int charLocate(string s,char target){
      for(int i=0;i<s.size();++i){
          if(s[i]==target)
             return i;
      }

      return -1;
}

//Function to convert decimal numbers to hexadecimal numbers

string decitohex(int num, int req){
        
        string res="";

         if(num==0)
          res=res+'0';

	 unsigned int n= num;                            //Defined as unsigned int so that bit representation remains the same
        
        string digits= "0123456789abcdef";
        while(n!=0){
            int rem= n%16;
            res=res+digits[rem];
            
            n=n/16;
              
        }
        reverse(res.begin(),res.end());               //Decimal converted to hexadecimal

	if (req < res.size()){
		res = res.substr(res.size()-req, req);        //We get 8 characters so if we require less characters then we take only them
	} 
	if(res.size()<req){
		for(int i=0;i<(req-res.size());++i)           //If we have less characters than required(mainly for positive numbers) then we append additional zeroes at the beginning
			res='0'+res;
	}

	return res;
}

//Function to trim(clean) text(instructions,operand,labels etc)

string clean(string text){
	int len = text.size(),strt = 0, last = len-1;
	string cleaned = "";

 //Checking where the text starts after space
	f(i,0,len-1){                                     
		if(text[i]!=' ' && text[i]!='\t'){
			strt = i;
			break;
		}
	}
//checking where the text ends after space
	for (int i=len-1; i>=0; i--){                      
		if (text[i]!=' ' && text[i]!='\t'){
			last = i;
			break;
		}
	}
//String that contains only text
	f(i,strt,last)                                    
		cleaned= cleaned+text[i];
	
	return cleaned;
}

 //Function to check validity of label
int Refcheck(string ref){

//Checking if first character is any other character other than a-z and A-Z in the label                     
	if (!((ref[0] <= 90 && ref[0] >= 65) || (ref[0] <= 122 && ref[0] >= 97))){
		   return 0;                                       
	}

//Checking if remaining characters are any other character other than 0-9,a-z and A-Z in the label
f(i,1,ref.size()-1){                                       
	if (!((ref[i] <= 90 && ref[i] >= 65) || (ref[i] <= 122 && ref[i] >= 97) || (ref[i] <= 57 && ref[i] >= 48))){
		return 0;
	}
		
 }

//return 1 if the label is valid
        return 1;                                                
}

//Function to put(insert) the label into the object code

void ReferenceInsert(string ref, int PC, int lineNo){

//To check if name of label is valid or not
	if (Refcheck(ref)==0){ 

//If not valid we enter that into the list of errors(errata)                           
		errata.pb({lineNo, "Bogus labelname: "+ ref});         
		return;
	}
// If the label is unique then we store it along with the PC count 
	if (lineRef.find(ref)==lineRef.end()){              
		lineRef[ref]=PC;
}else{
		errata.pb({lineNo,"Duplicate label definition: " +ref});   
    }
//If the label is previously in use then we have multiple definition of label and we enter it into the errata

}

//Check if the digit is valid or not
int NotvalidNum(string num){

//
	f(i,0,num.size()-1){                  
		if (num[i] < 48 || num[i] > 57){
			return 1;
		}
	}
	return 0;
}

//Check the format of operand 
int decode(string opr, int lno){
    
    int num,errordetect = 0;
    int sep = 0;
	const char* first_point = opr.c_str();
	char* last_point;
	string sym;
	
if (Refcheck(opr)!=0){
		if (lineRef.find(opr)==lineRef.end()){
			errata.pb({lno,"No such label: " +opr});
		}
		
		return lineRef[opr];
	}
	
	if (opr.size() > 1 && opr[0] == 48){
 //If first character is 0 then it can be octal,hexadecimal or binary based on its second character
		switch (opr[1]){

		case 'x':          
		    sym= opr.substr(2, opr.size()-2);                               //If second character is 'x' (Hexadecimal is of the form 0x)
        	errordetect = NotvalidNum(sym);
        	num = strtol(first_point+2,&last_point,16);
        	break;
		case 'b':                                         ////If second character is 'b' (binary is of the form 0)
			sym= opr.substr(2, opr.size()-2);
			errordetect = NotvalidNum(opr.substr(2, opr.size()-2));
			num = strtol(first_point+2,&last_point,2);
			break;
        
        default:                                          //If not both then it is octal
        	errordetect = NotvalidNum(opr);
        	num = strtol(first_point,&last_point,8);
        }
    } else{                                               //If first character is not 0 then it is decimal
    	
    	if (opr[0] == '+' || opr[0] == '-') sep++;
    	errordetect = NotvalidNum(opr.substr(sep, opr.size()-sep));
    	num = strtol(first_point, &last_point, 10);
    } 
    if (errordetect!=0){                                  //If any error detected in the operand it is added into the errata
    	errata.pb({lno,"Invalid expression: " +opr});
    	return 0;
    }
    return num;
}

void firstPass(string inFile){
	ifstream input(inFile);
	int PC = 0;
	string text;
	int sep,delimit;
	int lineNo = 0;
	
    pair<int, string> checkLabel;

 //Reading code from the input file
	while (getline(input, text)){                      
		lineNo++;
 //Cleaning the code that is extracted from the input file
		text = clean(text);                            
		TrimmedCode[PC].pb(text);
//Extracting code till comment and ignoring the comment,if exists
		text = text.substr(0, charLocate(text,';'));          
		text = clean(text);
		string label = "", mnemonic = "", oprd = "";

//Checking if there is a colon,i.e, a label in the code extracted
		if (charLocate(text,':') != -1){                 
		     sep = charLocate(text,':');
			label = text.substr(0, sep);   
 //If there is a colon, then label is extracted and trimmed     
			label = clean(label);
			if (label.size()!=0){
				ReferenceInsert(label, PC, lineNo);         //The validity of label is checked
				checkLabel=mk(PC, label);
			}
 //After extraction of label remaining code is extracted
			text = text.substr(sep+1, text.size()-sep-1);   
			text = clean(text);
		}
//After label,instruction and operand and divided by a space, so we look for the space
		if (text.find(' ') != -1){                          
			delimit = text.find(' ');
//Now we extract instruction
			mnemonic = text.substr(0, delimit);
			mnemonic = clean(mnemonic);
			oprd = text.substr(delimit+1, text.size()-delimit-1); 
			oprd = clean(oprd);
		} else
			mnemonic = clean(text);
		
		
		if (text.size()==0) continue;
		index[PC] = lineNo;
//If instruction is not available then it is bogus and added into errors
		if (availop.find(mnemonic)==availop.end())
			errata.pb({lineNo, "Bogus Mnemonic: " + mnemonic});
		
		if (mnemonic == "SET"){
			if (checkLabel.fr != PC)
				errata.pb({lineNo, "Label doesn't exist: " + mnemonic});
			 else
				lineRef[label] = decode(oprd, lineNo);
			
		}

//If instruction is available and it requires operand but operand is not given by the user then we add it into errors	
		if (oprd.size()==0 && availop[mnemonic].fr==true){
			errata.pb({lineNo, "Missing operand: " + mnemonic});
		} else if (oprd.size()>0 && availop[mnemonic].fr==false){
			errata.pb({lineNo, "Unexpected operand: " + mnemonic + " " + oprd});
		}
//If instruction is available and it does not require operand but operand is given by the user then we add it into errors
		pair<string,string>in = make_pair(mnemonic,oprd);
		commands.pb(in);
		PC++;
	}
}

void secondPass(ofstream &logFile, ofstream &outFile, ofstream &objFile){

	int PC = 0;
	// Those who take offsets, here have to find the distance
	set<int> PCoffset={13,15,16,17};
	

	f(i,0,commands.size()-1){
        
        string instr = commands[i].fr;
		int opCode = availop[instr].sec;
		string opr = commands[i].sec; 
		int line = index[i];
		
        // if coder haven't provided operand, take it as zero and continue on the function to add in errors
        // in case of error the code stops later
		int operand;
		if(availop[instr].fr==0){
			operand=0;
		}
		else{
			   operand=decode(opr, line);
		}
		if(instr == "data"){
            // Since data doesn't have any opcode
            operand = operand >> 8;
            // in the pdf they tell to use data as operand entirely
			opCode = operand & 0xff;
			
		}
		pair<int,int>inp = mk(opCode,operand);
		cleanedcommands.pb(inp);
	}
	if (errata.size()!=0){
		cout<<"Code contains errors"<<endl;
		sort(errata.begin(), errata.end());
		for (auto &it : errata){
			logFile<<"Line: "<<it.fr<<" "<<it.sec<<endl;
		}
		return;
	} else{
		cout<<"Compiled successfully"<<endl;
	}
    
	f(PC,0,cleanedcommands.size()-1){
		outFile << decitohex(PC, 8) << " ";
		for (int i=0; i<TrimmedCode[PC].size()-1; i++){
			string s = TrimmedCode[PC][i];
			outFile << "\t\t " << " " << s <<"\n";
			outFile << decitohex(PC, 8) << " ";
		}
        
        int opr = cleanedcommands[PC].sec;

		int opCode = cleanedcommands[PC].fr;
		
		if (PCoffset.find(opCode)!=PCoffset.end()){
			opr -= PC + 1;
		} 
		outFile << decitohex(opr, 6) << decitohex(opCode, 2) << " " << TrimmedCode[PC][TrimmedCode[PC].size()-1] << endl;

		objFile << decitohex(opr, 6) << decitohex(opCode, 2);
	}
}

