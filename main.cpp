#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstdlib>
#include <cstdio>

using namespace std;
class Memory {
public:
	Memory() {
		for (int i = 0; i < 1000; i++) {
			memory[i] = "nop";
		}
	}
	string getMem(int x) {
		return memory[x];
	}

	void setMem(int x, string data) {
		memory[x] = data;
	}
private:
	string memory[1000];
};

Memory newMem;

int programCounter = 0;
string instructionRegister;
string memoryBufferRegister;
int memoryAddressRegister;
string accumulatorRegister = "0";
string multiplierQuotientRegister = "0";

void decode(string instruction, match_results<string::const_iterator> parse);
void execute(int select, int location);
int stringToInt(string convert);

void decode(string instruction, match_results<string::const_iterator> parse) {

	regex comment("(.*)(\\.)(.*)");
	regex loadMQ("cargar\\s?MQ");
	regex loadMQMX("cargar\\s?MQ,M\\((\\d+)\\)");
	regex storMX("almacena\\s?M\\((\\d+)\\)");
	regex loadMX("cargar\\s?M\\((\\d+)\\)");
	regex loadNegativeMX("cargar\\s?-M\\((\\d+)\\)");
	regex loadAbsoluteMX("cargar\\s?\\|M\\((\\d+)\\)\\|");
	regex loadNegativeAbsoluteMX("cargar\\s?-\\|M\\((\\d+)\\)\\|");
	regex jump("salto\\s?M\\((\\d+)\\)");
	regex jumpPlus("salto\\+\\s?M\\((\\d+)\\)");
	regex addMX("sum\\s?M\\((\\d+)\\)");
	regex addAbsoluteMX("sum\\s?\\|M\\((\\d+)\\)\\|");
	regex subMX("sub\\s?M\\((\\d+)\\)");
	regex subAbsoluteMX("sub\\s?\\|M\\((\\d+)\\)\\|");
	regex mulMX("mul\\s?M\\((\\d+)\\)");
	regex divMX("div\\s?M\\((\\d+)\\)");
	

	if (regex_match(instruction, parse, comment)) {
	}

	else if ((instruction == "inicio") || (instruction == "fin") || (instruction == "nop")) {
	}

	else if (regex_match(instruction, parse, loadMQ)) {
		int mem = stringToInt(parse[1]);
		execute(1, mem);
	}
	else if (regex_match(instruction, parse, loadMQMX)) {
		int mem = stringToInt(parse[1]);
		execute(2, mem);
	}
	else if (regex_match(instruction, parse, storMX)) {
		int mem = stringToInt(parse[1]);
		execute(3, mem);
	}
	else if (regex_match(instruction, parse, loadMX)) {
		int mem = stringToInt(parse[1]);
		execute(4, mem);
	}
	else if (regex_match(instruction, parse, loadNegativeMX)) {
		int mem = stringToInt(parse[1]);
		execute(5, mem);
	}
	else if (regex_match(instruction, parse, loadAbsoluteMX)) {
		int mem = stringToInt(parse[1]);
		execute(6, mem);
	}
	else if (regex_match(instruction, parse, loadNegativeAbsoluteMX)) {
		int mem = stringToInt(parse[1]);
		execute(7, mem);
	}
	else if (regex_match(instruction, parse, jump)) {
		int mem = stringToInt(parse[1]);
		execute(8, mem);
	}
	else if (regex_match(instruction, parse, jumpPlus)) {
		int mem = stringToInt(parse[1]);
		execute(9, mem);
	}
	else if (regex_match(instruction, parse, addMX)) {
		int mem = stringToInt(parse[1]);
		execute(10, mem);
	}
	else if (regex_match(instruction, parse, addAbsoluteMX)) {
		int mem = stringToInt(parse[1]);
		execute(11, mem);
	}
	else if (regex_match(instruction, parse, subMX)) {
		int mem = stringToInt(parse[1]);
		execute(12, mem);
	}
	else if (regex_match(instruction, parse, subAbsoluteMX)) {
		int mem = stringToInt(parse[1]);
		execute(13, mem);
	}
	else if (regex_match(instruction, parse, mulMX)) {
		int mem = stringToInt(parse[1]);
		execute(14, mem);
	}
	else if (regex_match(instruction, parse, divMX)) {
		int mem = stringToInt(parse[1]);
		execute(15, mem);
	}
	
	else {
		cout << "ERROR: No se ha podido leer una instrucción válida.";
		exit(1);
	}
	return;
}

void execute(int select, int location) {
	
	// cargar transfer contents of MQ to AC
	if (select == 1) {
		accumulatorRegister = multiplierQuotientRegister;
	}

	//cargar MQMX transfer contents of memory location to MQ
	else if (select == 2) {
		multiplierQuotientRegister = newMem.getMem(location);
	}

	// almacenar MX store contents of AC to memory location
	else if (select == 3) {
		newMem.setMem(location, accumulatorRegister);
	}

	// cargarMX which transfers the contents of M(X) to AC
	else if (select == 4) {
		accumulatorRegister = newMem.getMem(location);
	}

	// cargarnegativoMX, which transfers the negative M(X) to AC
	else if (select == 5) {
		int x = stringToInt(newMem.getMem(location));
		x = -x;
		
		accumulatorRegister = to_string(x);
	}

	// cargarAbsoluteMX, which transfers the absolute value of MX to AC
	else if (select == 6) {
		int x = stringToInt(newMem.getMem(location));
		if (x < 0) {
			x = -x;
		}
		
		accumulatorRegister = to_string(x);
	}

	//cargarNegativeAbsoluteMX, which transfers the negative absolute value of MX to AC
	
	else if (select == 7) {
		int x = stringToInt(newMem.getMem(location));
		if (x < 0) {
			x = -x;
		}
		
		x = -x;
		accumulatorRegister = to_string(x);
	}

	//salto which sets the program counter equal to the memory location
	else if (select == 8) {
		programCounter = location;
	}

	//salto+ if the AC is nonnegative, then PC = location
	else if (select == 9) {
		if (stringToInt(accumulatorRegister) >= 0) {
			programCounter = location;
		}
	}

	//sumMX, which adds the contents of the memory location to AC
	else if (select == 10) {
		int aReg = stringToInt(accumulatorRegister);
		int value = stringToInt(newMem.getMem(location));
		
		aReg = aReg + value;
		accumulatorRegister = to_string(aReg);
	}

	//sumAbsoluteMX, which adds the absolute value of location to AC
	else if (select == 11) {
		int aReg = stringToInt(accumulatorRegister);
		int value = stringToInt(newMem.getMem(location));
		if (value < 0) {
			value = -value;
		}
		
		aReg = aReg + value;
		accumulatorRegister = to_string(aReg);
	}

	//subMX, which subtracts contents of memory location from AC, same manner as add
	else if (select == 12) {
		int aReg = stringToInt(accumulatorRegister);
		int value = stringToInt(newMem.getMem(location));
		
		aReg = aReg - value;
		accumulatorRegister = to_string(aReg);
	}

	//subAbsoluteMX, which subtracts the absolute value of location from AC
	else if (select == 13) {
		int aReg = stringToInt(accumulatorRegister);
		int value = stringToInt(newMem.getMem(location));
		if (value < 0) {
			value = -value;
		}
		
		aReg = aReg - value;
		accumulatorRegister = to_string(aReg);
	}

	//mulMX, multiply location by MQ and put the result in AC
	else if (select == 14) {
		int mQuot = stringToInt(multiplierQuotientRegister);
		int value = stringToInt(newMem.getMem(location));
		
		mQuot = mQuot * value;
		accumulatorRegister = to_string(mQuot);
	}

	//divMX, divide AC by M(X), put result in MQ and remainder in AC
	else if (select == 15) {
		int aReg = stringToInt(accumulatorRegister);
		int aReg2 = aReg;
		int value = stringToInt(newMem.getMem(location));
		
		aReg = aReg / value;
		aReg2 = aReg2 % value;
		
		multiplierQuotientRegister = to_string(aReg);
		accumulatorRegister = to_string(aReg2);
	}


	return;
}

// convert a string to an integer
int stringToInt(string convert) {
	int intReturn = 0;
	sscanf(convert.c_str(), "%d", &intReturn);
	return (intReturn);
}

int main() {
	
	ifstream inFile;
	string inString;
	match_results<string::const_iterator> matchResults;

	string fileName;
	cout << "introduzca el nombre del archivo a leer. \n";
	getline(cin, fileName);
	inFile.open(fileName);
	if (!inFile.is_open()) {
		cout << "ERROR: No se pudo abrir el archivo";
		exit(1);
	}

	cout << "-------- Verificando Codigo --------" << endl;
	while (!inFile.eof()) {
		getline(inFile, inString);
		cout << inString << endl;
	}
	inFile.close();

	cout << "------- Cargar en Memoria --------" << endl;
	string instruction;
	int lineNo;


	inFile.open(fileName);
	while (!inFile.eof()) {
		regex memLine("(\\d+) (.*)");
		getline(inFile, inString);
		if (regex_match(inString, matchResults, memLine)) {
			instruction = matchResults[2];
			lineNo = stringToInt(matchResults[1]);
			if (instruction == "inicio") {
				programCounter = lineNo;
			}
			newMem.setMem(lineNo, instruction);
		}
	}
	inFile.close();
	cout << endl << " carga completa " << endl;
	
	cout << endl << "------- instrucciones --------" << endl;

	while (instructionRegister != "fin"){
		memoryAddressRegister = programCounter;
		memoryBufferRegister = newMem.getMem(programCounter);
		instructionRegister = memoryBufferRegister;
		
		cout << "PC = " << programCounter << "  |  IR = " << instructionRegister << endl;
		
		programCounter++;
		decode(instructionRegister, matchResults);
		
		cout << "PC = " << programCounter << "  |  AC = " << accumulatorRegister <<
			"  |  MQ = " << multiplierQuotientRegister << endl << endl << endl;
	}

	cout << endl << "------- Impresion Memoria -------" << endl;
	for (int i = 0; i < 1000; i++) {
		inString = newMem.getMem(i);
		cout << i << " " << inString << endl;
		if (inString == "fin") {
			break;
		}
	}

	return 0;
}
