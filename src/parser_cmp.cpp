//
// EPITECH PROJECT, 2018
// nanoTekSpice
// File description:
// paser_cmp.cpp
//

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "nanoTekSpice.hpp"



std::vector<std::string> nts::Parser_cmp::split(std::string &str,
						const char& c)
{
	std::string buff = "";
	std::vector<std::string> result;
	for (auto it : str)
	{
		if (it != c)
			buff += it;
		else if (it == c && buff != "") {
			result.push_back(buff);
			buff = "";
		}
	}
	if(buff != "")
		result.push_back(buff);
	return result;
}

int nts::Parser_cmp::is_nbr(std::string &str)
{
	for (auto it : str)
	{
		if (it < '0' || it > '9')
			return 1;
	}
	return 0;
}

std::vector<std::size_t> nts::Parser_cmp::to_str_int
(std::vector<std::string> v_str)
{
	std::vector<std::size_t> result;
	for (auto it : v_str) {
		if (is_nbr(it) == 0)
			result.push_back(std::stoi(it));
	}
	return result;
}

void nts::Parser_cmp::into_pin_t(std::vector<nts::pin_t> &pin,
				std::vector<std::size_t> &nb)
{
	nts::pin_t local;
	for (auto it : nb)
	{
		local._nb = it;
		local._state = UNDEFINED;
		pin.push_back(local);
	}
}

void nts::Parser_cmp::fill_pin_t(nts::chipset_func_t save,
				std::vector<std::string> tmp)
{
	std::vector<std::size_t> input_size = to_str_int(split(tmp[0], ','));
	into_pin_t(save.inputs, input_size);
	std::vector<std::size_t> out_size = to_str_int(split(tmp[1], ','));
	into_pin_t(save.outputs, out_size);
	if (tmp.size() == 3)
		save.func_name = tmp[2];
	else
		save.func_name = tmp[1];
	_chipset_funcs.push_back(save);
}

int nts::Parser_cmp::parse_file_cmp(nts::Parser_cmp &file_comp,
				    std::string name_file)
{
	size_t pos_cmp = name_file.find(".cmp");
	if (pos_cmp == std::string::npos)
		return FAIL;
	file_comp._name = (name_file.substr(0, pos_cmp));
	std::string line;
	std::ifstream file("./comps/" + name_file, std::ios::in);
	std::getline(file, line);
	if (!file)
		return FAIL;
	std::vector<std::string> tmp;
	if (line != "")
		file_comp._nb_pins = std::stoi(line);
	while (std::getline(file, line)) {
		nts::chipset_func_t save;
		tmp = split(line, ':');
		fill_pin_t(save, tmp);
	}
	file.close();
	return SUCCESS;
}
