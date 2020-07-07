/*
 * EasyGAL.cpp
 *
 *  Created on: May 28, 2020
 *      Author: elias
 */

#include <iostream>
#include <vector>

#include "Shared/TableData.h"
#include "Parser/Parser.h"
#include "Parser/Error.h"
#include "Translator/Translator.hpp"

using namespace std;

// printTableData is for debuging
void printTableData(TableData tableData)
{
	cout << "output pin: " << tableData.m_OutputPin << endl;

	cout << "input pins:";
	for (uint32_t pin : tableData.m_InputPins)
		cout << " " << pin;
	cout << endl;

	cout << "table:\t    ";
	for (bool b : tableData.m_Table)
		cout << b;
	cout << endl;

	cout << "is dff:\t    " << tableData.m_EnableFlipFlop << endl;

	cout << endl;
}

void printError(string msg)
{
	cerr << msg << endl;
	exit(1);
}

bool inVec(vector<uint32_t> vec, uint32_t value)
{
	for (uint32_t temp : vec)
		if (value == temp)
			return true;
	return false;
}

void validate(vector<TableData> tables, vector<uint32_t> inputPins, vector<uint32_t> outputPins)
{
	for (uint32_t i = 0; i < tables.size(); i++)
	{
		for (uint32_t pin : tables.at(i).m_InputPins)
			if (!inVec(inputPins, pin))
				printError("pin " + to_string(pin) + " is not an input pin");

		if (!inVec(outputPins, tables.at(i).m_OutputPin))
			printError("pin " + to_string(tables.at(i).m_OutputPin) + " is not an output pin");

		if (pow(2, tables.at(i).m_InputPins.size()) != tables.at(i).m_Table.size())
			printError("table at output pin " + to_string(tables.at(i).m_OutputPin) + " has the wrong shape");

		for (uint32_t j = i + 1; j < tables.size(); j++)
		{
			if (tables.at(i).m_OutputPin == tables.at(j).m_OutputPin)
				printError("table at output pin " + to_string(tables.at(i).m_OutputPin) + " was defined twice");
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 4)
		Error::printError("You have to supply a path to your EasyGAL code and the name for the output file");

	Parser parser = Parser(argv[1]);
	vector<TableData> tableData = parser.parse();

	vector<uint32_t> inputPins = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
	vector<uint32_t> outputPins = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23};

	validate(tableData, inputPins, outputPins);

	Translator::Process(tableData, argv[3], argv[2]);
}