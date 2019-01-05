//
// EPITECH PROJECT, 2018
// nanotekspice
// File description:
// funcs
//

#include <iostream>
#include "nanoTekSpice.hpp"

int nts::Component::check_if_undefined(nts::chipset_func_t func)
{
	if (func.inputs[0]._state == nts::UNDEFINED ||
		func.inputs[1]._state == nts::UNDEFINED)
		return 1;
	return 0;
}

nts::Tristate nts::Component::find_ret(nts::link_t link)
{
	for (std::size_t i = 0 ; i < parse_nts->comp_map[link.comp_name]->
			_chipset_funcs.size() ; i++) {
		nts::chipset_func_t tmp_cf = parse_nts->comp_map[link.comp_name]->
			_chipset_funcs[i];
		for (std::size_t j = 0 ; j < tmp_cf.outputs.size() ; j++) {
			if (tmp_cf.outputs[j]._nb == link.pin) {
				auto ret = parse_nts->comp_map
					[link.comp_name]->
					_functions[tmp_cf.func_name](tmp_cf);
				return tmp_cf.outputs[j]._state;
			}
		}
	}
	return nts::UNDEFINED;
}

nts::chipset_func_t nts::Component::and_func(nts::chipset_func_t &func)
{
	if (check_if_undefined(func)) {
		if (!func.inputs[0]._link.comp_name.empty() &&
			!func.inputs[1]._link.comp_name.empty()) {
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
			func.inputs[1]._state = find_ret(func.inputs[1]._link);
		}
		if (check_if_undefined(func)) {
			func.outputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	func.outputs[0]._state = (func.inputs[0]._state == nts::TRUE &&
				func.inputs[1]._state == nts::TRUE)
		? nts::TRUE : nts::FALSE;
	return func;
}

nts::chipset_func_t nts::Component::or_func(nts::chipset_func_t &func)
{
	if (check_if_undefined(func)) {
		if (!func.inputs[0]._link.comp_name.empty() &&
		!func.inputs[1]._link.comp_name.empty()) {
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
			func.inputs[1]._state = find_ret(func.inputs[1]._link);
		}
		if (check_if_undefined(func)) {
			func.outputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	func.outputs[0]._state = (func.inputs[0]._state == nts::FALSE &&
				func.inputs[1]._state == nts::FALSE)
		? nts::FALSE : nts::TRUE;
	return func;
}

nts::chipset_func_t nts::Component::not_func(nts::chipset_func_t &func)
{
	if (func.inputs[0]._state == nts::UNDEFINED) {
		if (!func.inputs[0]._link.comp_name.empty())
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
		if (func.inputs[0]._state == nts::UNDEFINED) {
			func.outputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	func.outputs[0]._state = (func.inputs[0]._state == nts::TRUE)
		? nts::FALSE : nts::TRUE;
	return func;
}

nts::chipset_func_t nts::Component::nand_func(nts::chipset_func_t &func)
{
	if (check_if_undefined(func)) {
		if (!func.inputs[0]._link.comp_name.empty() &&
			 !func.inputs[1]._link.comp_name.empty()) {
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
			func.inputs[1]._state = find_ret(func.inputs[1]._link);
		}
		if (check_if_undefined(func)) {
			func.outputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	func.outputs[0]._state = (func.inputs[0]._state == nts::TRUE &&
				  func.inputs[1]._state == nts::TRUE)
		? nts::FALSE : nts::TRUE;
	return func;
}

nts::chipset_func_t nts::Component::nor_func(nts::chipset_func_t &func)
{
	if (check_if_undefined(func)) {
		if (!func.inputs[0]._link.comp_name.empty() &&
			!func.inputs[1]._link.comp_name.empty()) {
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
			func.inputs[1]._state = find_ret(func.inputs[1]._link);
		}
		if (check_if_undefined(func)) {
			func.outputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	func.outputs[0]._state = (func.inputs[0]._state == nts::FALSE &&
					func.inputs[1]._state == nts::FALSE)
		? nts::TRUE : nts::FALSE;
	return func;
}

nts::chipset_func_t nts::Component::xor_func(nts::chipset_func_t &func)
{
	if (check_if_undefined(func)) {
		if (!func.inputs[0]._link.comp_name.empty() &&
		    !func.inputs[1]._link.comp_name.empty()) {
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
			func.inputs[1]._state = find_ret(func.inputs[1]._link);
		}
		if (check_if_undefined(func)) {
			func.outputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	if ((func.inputs[0]._state == nts::FALSE &&
		func.inputs[1]._state == nts::FALSE) ||
		(func.inputs[0]._state == nts::TRUE &&
		func.inputs[1]._state == nts::TRUE))
		func.outputs[0]._state = nts::FALSE;
	else
		func.outputs[0]._state = nts::TRUE;
	return func;
}

nts::chipset_func_t nts::Component::in_func(nts::chipset_func_t &func)
{
	return func;
}

nts::chipset_func_t nts::Component::out_func(nts::chipset_func_t &func)
{
	if (func.inputs[0]._state == nts::UNDEFINED) {
		if (!func.inputs[0]._link.comp_name.empty())
			func.inputs[0]._state = find_ret(func.inputs[0]._link);
		if (func.inputs[0]._state == nts::UNDEFINED) {
			func.inputs[0]._state = nts::UNDEFINED;
			return func;
		}
	}
	func.outputs[0]._state = func.inputs[0]._state;
	return func;
}

nts::chipset_func_t nts::Component::true_func(nts::chipset_func_t &func)
{
	func.outputs[0]._state = nts::TRUE;
	return func;
}

nts::chipset_func_t nts::Component::false_func(nts::chipset_func_t &func)
{
	func.outputs[0]._state = nts::FALSE;
	return func;
}
