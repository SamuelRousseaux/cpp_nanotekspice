//
// EPITECH PROJECT, 2018
// nanotekSpice
// File description:
// nanotekSpice.hpp
//

#ifndef NANOTEKSPICE_HPP_
# define NANOTEKSPICE_HPP_

#include <map>
#include <memory>
#include <vector>
#include <functional>

namespace nts
{
	enum Tristate {
		UNDEFINED = (-true),
		TRUE = true,
		FALSE = false
	};

	class IComponent
	{
	public:
		virtual ~IComponent() = default;
		virtual nts::Tristate compute(std::size_t pin = 1) = 0;
		virtual void setLink(std::size_t pin, nts::IComponent &other,
				     std::size_t otherPin) = 0;
		virtual void dump() = 0;
	};

	typedef struct link_s {
		std::size_t pin;
		std::string comp_name;
	} link_t;

	typedef struct pin_s {
		std::size_t _nb;
		Tristate _state;
		link_t _link;
	} pin_t;

	typedef struct chipset_func_s {
		std::vector<nts::pin_t> inputs;
		std::vector<nts::pin_t> outputs;
		std::string func_name;
	} chipset_func_t;

	class Parse_nts;
	class Component : public nts::IComponent
	{
	public:
		Component(Parse_nts *);
		nts::Tristate compute(std::size_t pin = 1);
		void set_nb_pins(std::size_t &_nb_pins_input);
		void set_name(const std::string &name);
		void set_chipset_funcs(std::vector<chipset_func_t>
					&chipset_funcs);
		void setLink(std::size_t pin, nts::IComponent &other,
				std::size_t otherPin);
		void setLink(std::size_t pin, std::unique_ptr<nts::Component> &other,
				std::size_t otherPin);
		std::size_t get_nb_pins();
		std::string get_name();
		std::vector<chipset_func_t> get_chipset_funcs();
		void dump();
		char retstate(nts::Tristate);

		int check_if_undefined(nts::chipset_func_t);
		nts::Tristate find_ret(nts::link_t);
		nts::chipset_func_t and_func(nts::chipset_func_t &);
		nts::chipset_func_t or_func(nts::chipset_func_t &);
		nts::chipset_func_t not_func(nts::chipset_func_t &);
		nts::chipset_func_t nand_func(nts::chipset_func_t &);
		nts::chipset_func_t nor_func(nts::chipset_func_t &);
		nts::chipset_func_t xor_func(nts::chipset_func_t &);
		nts::chipset_func_t in_func(nts::chipset_func_t &);
		nts::chipset_func_t out_func(nts::chipset_func_t &);
		nts::chipset_func_t true_func(nts::chipset_func_t &);
		nts::chipset_func_t false_func(nts::chipset_func_t &);
		std::map<std::string, std::function<
			nts::chipset_func_t(nts::chipset_func_t &)>> _functions;
		std::vector<chipset_func_t> _chipset_funcs;
	private:
		std::size_t _nb_pins;
		std::string _name;
		Parse_nts *parse_nts;
	};

	class Parser_cmp
	{
	public :
		int parse_file_cmp(nts::Parser_cmp &file_comp,
					std::string name_file);
		std::string &get_name();
		std::size_t &get_nb_pins();
		std::vector<chipset_func_t> &get_chipset_funcs();
	private:
		int is_nbr(std::string &str);
		std::vector<std::size_t> to_str_int(std::vector<std::string>
						    v_str);
		void fill_pin_t(nts::chipset_func_t save,
				std::vector<std::string> tmp);
		void into_pin_t(std::vector<nts::pin_t> &pin,
				std::vector<std::size_t> &nb);
		std::vector<std::string> split(std::string &str,
					       const char& c);
		std::string _name;
		std::size_t _nb_pins;
		std::vector<chipset_func_t> _chipset_funcs;
	};

	typedef struct parser_file_s {
		std::vector<std::string> clocks;
		std::vector<std::string> input;
		std::vector<std::string> outputs;
		std::vector<std::string> links;
	} parser_file_t;

	class Parse_nts
	{
	public:
		Parse_nts();
		std::unique_ptr<nts::Component>
		createComponent(const std::string &type,
				const std::string &value);
		int parser_file_nts(std::string name_file);
		int instantiate_comp();
		const std::vector<std::string> &get_clocks();
		const std::vector<std::string> &get_input();
		const std::vector<std::string> &get_outputs();
		const std::vector<std::string> &get_links();
		void get_all_argv(char **argv);
		std::string get_name(std::string);
		void reverse_clock();
		void simulate();
		void display();
		void dump();
		void loop();
		void exit();
		void prompt();
		void set_input(std::string);
		static bool Looping;
		std::map<std::string,
			 std::unique_ptr<nts::Component>> comp_map;
	private:
		int set_all_link();
		void fill_map_input();
		void fill_map_output();
		void fill_map_clock();
		int get_data(std::vector<std::string> &tab,
			     const std::string name_file);
		int get_data_line(std::string &str);
		void fill_struct_links(std::vector<std::string> &data_unclean,
				       parser_file_t &tab);
		void fill_struct_chip(std::vector<std::string> &data_unclean,
				      parser_file_t &tab);
		std::vector<std::string> convert_char_vec(char **argv);
		int is_nbr(std::string &str);
		parser_file_t tab;
		std::vector<std::string> input_name;
		std::vector<std::size_t> input_val;
		std::map<std::string, std::function<void(void)>> cmd_map;
	};
}

# define INPUT_CHIPSET 2
# define INPUT_LINKS 1
# define INPUT_ALLOK 3
# define FAIL 1
# define SUCCESS 0
#endif /*!NANOTEKSPICE_HPP_*/
