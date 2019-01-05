//
// EPITECH PROJECT, 2018
// nts
// File description:
// comp class
//

#include <iostream>
#include "nanoTekSpice.hpp"

nts::Component::Component(nts::Parse_nts *ps) : parse_nts(ps)
{
	_functions["AND"] = std::bind(&nts::Component::and_func,
					this, std::placeholders::_1);
	_functions["OR"] = std::bind(&nts::Component::or_func, this,
				     std::placeholders::_1);
	_functions["NOT"] = std::bind(&nts::Component::not_func,
					this, std::placeholders::_1);
	_functions["NAND"] = std::bind(&nts::Component::nand_func,
					this, std::placeholders::_1);
	_functions["NOR"] = std::bind(&nts::Component::nor_func,
					this, std::placeholders::_1);
	_functions["XOR"] = std::bind(&nts::Component::xor_func,
					this, std::placeholders::_1);
	_functions["IN"] = std::bind(&nts::Component::in_func,
					this, std::placeholders::_1);
	_functions["OUT"] = std::bind(&nts::Component::out_func,
					this, std::placeholders::_1);
	_functions["TRUE"] = std::bind(&nts::Component::true_func,
					this, std::placeholders::_1);
	_functions["FALSE"] = std::bind(&nts::Component::false_func,
					this, std::placeholders::_1);
}

void nts::Component::set_nb_pins(std::size_t &_nb_pins_input)
{
	_nb_pins = _nb_pins_input;
}

void nts::Component::set_name(const std::string &name)
{
	_name = name;
}

std::string nts::Component::get_name()
{
	return _name;
}

void nts::Component::set_chipset_funcs(std::vector<chipset_func_t>
					&chipset_funcs)
{
	_chipset_funcs = chipset_funcs;
}

std::unique_ptr<nts::Component> nts::Parse_nts::createComponent(
	const std::string &type, const std::string &value)
{
	std::unique_ptr<nts::Component> new_compt(new nts::Component(this));
	nts::Parser_cmp parse_class;
	parse_class.parse_file_cmp(parse_class, (type + ".cmp"));
	new_compt->set_nb_pins(parse_class.get_nb_pins());
	new_compt->set_name(value);
	new_compt->set_chipset_funcs(parse_class.get_chipset_funcs());
	return new_compt;
}

nts::Tristate nts::Component::compute(std::size_t pin)
{
	for (std::size_t i = 0 ; i <_chipset_funcs.size() ; i++) {
		auto tmp_cf = _chipset_funcs[i];
		for (std::size_t j = 0 ; j < tmp_cf.outputs.size() ; j++) {
			if (tmp_cf.outputs[j]._nb == pin) {
				auto rt = _functions[tmp_cf.func_name](tmp_cf);
				return rt.outputs[j]._state;
			}
		}
		for (std::size_t j = 0 ; j < tmp_cf.inputs.size() ; j++) {
			if (tmp_cf.inputs[j]._nb == pin &&
			!tmp_cf.inputs[j]._link.comp_name.empty())
				return find_ret(tmp_cf.inputs[j]._link);
			else if (tmp_cf.inputs[j]._nb == pin)
				return tmp_cf.inputs[j]._state;
		}
	}
	return UNDEFINED;
}


void nts::Component::setLink(std::size_t pin,
				std::unique_ptr<nts::Component> &other,
			std::size_t otherPin)
{
	for (std::size_t i = 0 ; i < _chipset_funcs.size() ; i++) {
		nts::chipset_func_t cf = _chipset_funcs[i];
		for (std::size_t j = 0 ; j < cf.outputs.size() ; j++) {
			if (cf.outputs[j]._nb == pin) {
				cf.outputs[j]._link.pin = otherPin;
				cf.outputs[j]._link.comp_name = other->_name;
				_chipset_funcs[i] = cf;
				return;
			}
		}
		for (std::size_t j = 0 ; j < cf.inputs.size() ; j++) {
			if (cf.inputs[j]._nb == pin) {
				cf.inputs[j]._link.pin = otherPin;
				cf.inputs[j]._link.comp_name = other->_name;
				_chipset_funcs[i] = cf;
				return;
			}
		}
	}
	std::cout << "can't find right pin\n" << std::endl;
}
void nts::Component::setLink(std::size_t pin, nts::IComponent &other,
				std::size_t otherPin)
{
	(void)pin;
	(void)other;
	(void)otherPin;
}

char nts::Component::retstate(nts::Tristate state)
{
	if (state == -1)
		return 'U';
	else {
		if (state == nts::TRUE)
			return '1';
		else if (state == nts::FALSE)
			return '0';
	}
	return 'U';
}

void nts::Component::dump()
{
	std::cout << "Component name: " << _name << std::endl;
	std::cout << "Number of pins: " << _nb_pins << std::endl;
	for (std::size_t i = 1 ; i <= _nb_pins ; i++) {
		for (std::size_t j = 0 ; j < _chipset_funcs.size() ; j++) {
			auto cf = _chipset_funcs[j];
			for (std::size_t x = 0 ; x < cf.inputs.size() ; x++)
				if (cf.inputs[x]._nb == i)
					std::cout << "\tpin #" << i << " = "
						<< retstate(compute(i))
						<< std::endl;
			for (std::size_t x = 0 ; x < cf.outputs.size() ; x++)
				if (cf.outputs[x]._nb == i)
					std::cout << "\tpin #" << i << " = "
						<< retstate(compute(i))
						<< std::endl;
		}
	}
	std::cout << std::endl;
}
