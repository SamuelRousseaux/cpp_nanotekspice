//
// EPITECH PROJECT, 2018
// nanoTekSpice
// File description:
// Parse_nts.cpp
//

#include <cstring>
#include <csignal>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <algorithm>
#include "nanoTekSpice.hpp"

bool nts::Parse_nts::Looping = false;

void sighandle(int sig)
{
	if (sig == SIGINT) {
		std::cout << std::endl;
		nts::Parse_nts::Looping = false;
	} else if (sig == SIGTERM)
		std::exit(0);
}

void do_nothing(int sig)
{
	(void)sig;
}

nts::Parse_nts::Parse_nts()
{
	std::signal(SIGTERM, sighandle);
	std::signal(SIGINT, do_nothing);
	cmd_map["exit"] = std::bind(&nts::Parse_nts::exit, this);
	cmd_map["display"] = std::bind(&nts::Parse_nts::display, this);
	cmd_map["simulate"] = std::bind(&nts::Parse_nts::simulate, this);
	cmd_map["loop"] = std::bind(&nts::Parse_nts::loop, this);
	cmd_map["dump"] = std::bind(&nts::Parse_nts::dump, this);
}

void nts::Parse_nts::fill_map_input()
{
	for (auto it : tab.input) {
		std::string str_save = "";
		std::string type_comp = "";
		std::string name_comp ="";
		str_save = it;
		int token = 0;
		for (int count = 0 ; str_save[count] != '\0' ; count++)
		{
			if (isalnum(str_save[count]) && token == 0)
				type_comp += str_save[count];
			else
				token = 1;
			if (isalnum(str_save[count]) && token == 1)
				name_comp += str_save[count];
		}
		comp_map[name_comp] = createComponent(type_comp, name_comp);
	}
}

void nts::Parse_nts::fill_map_output()
{
	for (auto it : tab.outputs) {
		std::string str_save = "";
		std::string type_comp = "";
		std::string name_comp ="";
		str_save = it;
		int token = 0;
		for (int count = 0 ; str_save[count] != '\0' ; count++)
		{
			if (isalnum(str_save[count]) && token == 0)
				type_comp += str_save[count];
			else
				token = 1;
			if (isalnum(str_save[count]) && token == 1)
				name_comp += str_save[count];
		}
		comp_map[name_comp] = createComponent(type_comp, name_comp);
	}
}

void nts::Parse_nts::fill_map_clock()
{
	for (auto it : tab.clocks) {
		std::string str_save = "";
		std::string type_comp = "";
		std::string name_comp ="";
		str_save = it;
		int token = 0;
		for (int count = 0 ; str_save[count] != '\0' ; count++)
		{
			if (isalnum(str_save[count]) && token == 0)
				type_comp += str_save[count];
			else
				token = 1;
			if (isalnum(str_save[count]) && token == 1)
				name_comp += str_save[count];
		}
		comp_map[name_comp] = createComponent(type_comp, name_comp);
	}
}


int nts::Parse_nts::instantiate_comp()
{
	if (tab.input.empty()|| tab.outputs.empty()
	|| tab.links.empty())
		return FAIL;
	fill_map_input();
	fill_map_output();
	fill_map_clock();
	if (set_all_link() == FAIL)
		return FAIL;
	return SUCCESS;
}

std::string get_pin1(std::string input)
{
	std::string str_pin1 = "";
	for (int i = (input.find(":") + 1) ; std::isalnum(input[i]) ; i++)
		str_pin1 += input[i];
	return str_pin1;
}

std::string get_name2(std::string input)
{
	std::string str_name2 = "";
	for (int i = (input.rfind(":") -1) ; std::isalnum(input[i]) ; i--)
		str_name2 += input[i];
	std::reverse(str_name2.begin(), str_name2.end());
	return str_name2;
}

int nts::Parse_nts::set_all_link()
{
	for (auto it : tab.links)
	{
		std::size_t pin1 = 0;
		std::size_t pin2 = 0;
		std::string name1 = "";
		std::string name2 = "";
		if (it.find(".links:") == std::string::npos) {
			name1 = it.substr(0, it.find(":"));
			if (it != "")
				pin1 = std::stoi(get_pin1(it));
			name2 = get_name2(it);
			if (it.substr((it.rfind(":") + 1)) != "")
			pin2 = std::stoi(it.substr((it.rfind(":") + 1),
						it.size()));
			comp_map[name1]->setLink(pin1, comp_map[name2], pin2);
			comp_map[name2]->setLink(pin2, comp_map[name1], pin1);
		}
	}
	return 0;
}

int nts::Parse_nts::get_data(std::vector<std::string> &tab,
			const std::string name_file)
{
	int flag = 0;
	std::string line;
	std::ifstream file(name_file, std::ios::in);
	if (name_file.find(".nts") == std::string::npos
	|| !file)
		return flag;
	while (std::getline(file, line)) {
		if (line.find(".chipsets:") != std::string::npos)
			flag += INPUT_CHIPSET;
		if (line.find(".links:") != std::string::npos)
			flag += INPUT_LINKS;
		if (line.find("#") == std::string::npos) {
			tab.push_back(line);
		} else if (line.find("#") != 0) {
			tab.push_back(line.substr(0, line.find("#")));
		}
	}
	file.close();
	return flag;
}

int nts::Parse_nts::get_data_line(std::string &str)
{
	int ret = str.size();

	while (ret != 0)
	{
		if (str[ret] != ' ' || str[ret] != '\t')
			ret -= 1;
		else
			return ret;
	}
	return ret;
}

void nts::Parse_nts::fill_struct_chip(std::vector<std::string> &data_unclean,
			parser_file_t &tab)
{
	std::vector<std::string>::iterator it;
	for (it = data_unclean.begin();
		it->find(".chipsets:") == std::string::npos
		&& it != data_unclean.end(); it++);

	for (it = it; it->find(".links:") == std::string::npos
		&& it != data_unclean.end(); it++)
	{
		if (it->find("output") != std::string::npos)
			tab.outputs.push_back(*it);
		else if (it->find("input") != std::string::npos)
			tab.input.push_back(*it);
		else if (it->find("clock") != std::string::npos)
			tab.clocks.push_back(*it);
		else if (*it != "")
			tab.input.push_back(*it);
	}
}

void nts::Parse_nts::fill_struct_links(std::vector<std::string> &data_unclean,
			parser_file_t &tab)
{
	std::vector<std::string>::iterator it;

	for (it = data_unclean.begin();
		it->find(".links:") == std::string::npos
		&& it != data_unclean.end(); it++);
	for (it = it; it != data_unclean.end(); it++)
	{
		if (*it != "")
			tab.links.push_back(*it);
	}
}

int nts::Parse_nts::parser_file_nts(std::string name_file)
{
	std::vector<std::string> data_unclean;

	if (get_data(data_unclean, name_file) != INPUT_ALLOK)
		return FAIL;
	fill_struct_chip(data_unclean, tab);
	fill_struct_links(data_unclean, tab);
	return SUCCESS;
}

std::vector<std::string> nts::Parse_nts::convert_char_vec(char **argv)
{
	std::vector<std::string> vec;
	int count = 0;
	while(argv[count] != NULL)
	{
		vec.push_back(argv[count]);
		count += 1;
	}
	return (vec);
}

int nts::Parse_nts::is_nbr(std::string &str)
{
	for (auto it : str)
	{
		if (it < '0' || it > '9')
			return 1;
	}
	return 0;
}


void nts::Parse_nts::get_all_argv(char **argv)
{
	std::vector<std::string> tmp_vec = convert_char_vec(argv);
	if (tmp_vec.size() <= 2)
		return ;
	for(auto it : tmp_vec)
	{
		if (it != argv[0] && it != argv[1]) {
			std::string name = it.substr(0, it.find("="));
			std::string str_tmp = it.substr(it.find("=") + 1,
							it.size());
			if (str_tmp != "" && is_nbr(str_tmp) == SUCCESS) {
				if (comp_map.count(name) > 0) {
					comp_map[name]->
						_chipset_funcs[0].outputs[0].
						_state =
						static_cast<nts::Tristate>
						(std::stoi(str_tmp)); }
			}
		}
	}
}

std::string nts::Parse_nts::get_name(std::string line)
{
	std::string ret;
	for (auto i = line.size() - 1 ; std::isalnum(line[i]) ; i--)
		ret += line[i];
	std::reverse(ret.begin(), ret.end());
	return ret;
}

void nts::Parse_nts::reverse_clock()
{
	for (auto line : tab.clocks) {
		std::string name = get_name(line);
		char tmp = comp_map[name]->_chipset_funcs[0].outputs[0]._state;
		if (tmp == nts::TRUE)
			comp_map[name]->_chipset_funcs[0].
				outputs[0]._state = nts::FALSE;
		else if (tmp == nts::FALSE)
			comp_map[name]->_chipset_funcs[0].
				outputs[0]._state = nts::TRUE;
	}
}

void nts::Parse_nts::simulate()
{
	for (auto line : tab.outputs) {
		std::string name = get_name(line);
		comp_map[name]->_chipset_funcs[0].inputs[0]._state =
			comp_map[name]->compute(1);
	}
	reverse_clock();
}

void nts::Parse_nts::display()
{
	for (auto line : tab.outputs) {
		std::string name = get_name(line);
		std::cout << name << "=";
		nts::Tristate tmp =
			comp_map[name]->_chipset_funcs[0].inputs[0]._state;
		if (tmp == -1)
			std::cout << 'U' << std::endl;
		else
			std::cout << tmp << std::endl;
	}
}

void nts::Parse_nts::dump()
{
	for (auto line : tab.input) {
		std::string name = get_name(line);
		if (!name.empty())
			comp_map[name]->dump();
	}
	for (auto line : tab.outputs) {
		std::string name = get_name(line);
		comp_map[name]->dump();
	}
	for (auto line : tab.clocks) {
		std::string name = get_name(line);
		comp_map[name]->dump();
	}
}

void nts::Parse_nts::loop()
{
	nts::Parse_nts::Looping = true;
	std::signal(SIGINT, sighandle);
	while (nts::Parse_nts::Looping)
		simulate();
	std::signal(SIGINT, do_nothing);
}

void nts::Parse_nts::exit()
{
	std::exit(0);
}

void nts::Parse_nts::set_input(std::string line)
{
	std::string name = line.substr(0, line.find("="));
	std::string str_tmp = line.substr(line.find("=") + 1, line.size());
	if (str_tmp != "" && is_nbr(str_tmp) == SUCCESS)
		if (comp_map.count(name) > 0) {
			comp_map[name]->_chipset_funcs[0].outputs[0]._state =
				static_cast<nts::Tristate>(std::stoi(str_tmp));
			return;
		}
	std::cout << "Unknown command" << std::endl;
}

void nts::Parse_nts::prompt()
{
	std::string input;
	simulate();
	display();
	std::cout << "> ";
	while (std::cin >> input) {
		if (cmd_map.count(input) > 0) {
			cmd_map[input]();
			std::cout << "> ";
			continue;
		} else if (input.empty()) {
			std::cout << "> ";
			continue;
		}
		set_input(input);
		std::cout << "> ";
	}
}
