//
// EPITECH PROJECT, 2018
// nanoTekSpice
// File description:
// Return_nts.cpp
//

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "nanoTekSpice.hpp"

const std::vector<std::string> &nts::Parse_nts::get_clocks()
{
	return tab.clocks;
}

const std::vector<std::string> &nts::Parse_nts::get_input()
{
	return tab.input;
}

const std::vector<std::string> &nts::Parse_nts::get_outputs()
{
	return tab.outputs;
}

const std::vector<std::string> &nts::Parse_nts::get_links()
{
	return tab.links;
}
