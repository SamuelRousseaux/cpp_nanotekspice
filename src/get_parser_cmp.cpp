//
// EPITECH PROJECT, 2018
// nanoTekSpice.hpp
// File description:
// get_parser_cmp.cpp
//

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "nanoTekSpice.hpp"

std::string &nts::Parser_cmp::get_name()
{
	return _name;
}
std::size_t &nts::Parser_cmp::get_nb_pins()
{
	return _nb_pins;
}
std::vector<nts::chipset_func_t> &nts::Parser_cmp::get_chipset_funcs()
{
	return _chipset_funcs;
}
