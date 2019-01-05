//
// EPITECH PROJECT, 2018
// NanotekSpice
// File description:
// main.cpp
//

#include <iostream>
#include "nanoTekSpice.hpp"

int main(int argc, char **argv)
{
	nts::Parse_nts data_file;
	if (argc == 1) {
		std::cout << "No file on input" << std::endl;
		return 0;
	}
	data_file.parser_file_nts(argv[1]);
	data_file.instantiate_comp();
	data_file.get_all_argv(argv);
	data_file.prompt();
	return 0;
}
