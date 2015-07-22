#include "node.hpp"
#include <exception>
#include <string.h>

using namespace aeon;

node::node( void *record ) : record_ptr(record)
{

	return ;
}

node::~node( void )
{

	return ;
}

/* getters */
void    node::get_field(sstr field_name, node **ret) const
{
	uint			index;
	uint			type_id;
	void			*rec;
	node			*fresh_node;
	std::vector<sstr>	fields_list; // store the names of the fields

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}

	rec = wg_decode_record(agdb->db_ptr, wg_get_field(agdb->db_ptr, record_ptr, index));

	fresh_node = new node(rec);
	fresh_node->agdb = this->agdb;
	type_id = wg_decode_int(agdb->db_ptr, wg_get_field(agdb->db_ptr, rec, 0)); // 0 is type field
	fields_list = agdb->get_type_fields[type_id];
	for (uint i = 0; i < fields_list.size(); i++)
	{
		fresh_node->get_field_index[fields_list[i]] = i;
	}

	*ret = fresh_node;
}

void    node::get_field(sstr field_name, sstr *ret) const
{
	uint			index;

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	*ret = wg_decode_str(agdb->db_ptr, wg_get_field(agdb->db_ptr, record_ptr, index));
}

void    node::get_field(sstr field_name, int *ret) const
{
	uint			index;

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	*ret = wg_decode_int(agdb->db_ptr, wg_get_field(agdb->db_ptr, record_ptr, index));
}

void    node::get_field(sstr field_name, double *ret) const
{
	uint			index;

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	*ret = wg_decode_double(agdb->db_ptr, wg_get_field(agdb->db_ptr, record_ptr, index));
}
/* end of getters */

/* setters */
void    node::set_field(sstr field_name, node *data)
{
	uint			index;
	wg_int			encoded_data;

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	
	encoded_data = wg_encode_record(agdb->db_ptr, data->record_ptr);
	if(encoded_data == WG_ILLEGAL)
	{
		std::cout << "/!\\ Shouldn't happen /!\\" << std::endl;
	}
	if (wg_set_field(agdb->db_ptr, record_ptr, index, encoded_data) < 0)
	{
		std::cout << "Impossible to write in the field" << std::endl;
	}
}

void    node::set_field(sstr field_name, sstr data)
{
	uint			index;
	wg_int			encoded_data;
        char			*str = new char[data.size() + 1];

        strcpy(str, data.c_str());


	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	
	encoded_data = wg_encode_str(agdb->db_ptr, str, 0);
	if(encoded_data == WG_ILLEGAL)
	{
		std::cout << "/!\\ Shouldn't happen /!\\" << std::endl;
	}
	if (wg_set_field(agdb->db_ptr, record_ptr, index, encoded_data) < 0)
	{
		std::cout << "Impossible to write in the field" << std::endl;
	}
	delete str;
}

void    node::set_field(sstr field_name, int data)
{
	uint			index;
	wg_int			encoded_data;

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	
	encoded_data = wg_encode_int(agdb->db_ptr, data);
	if(encoded_data == WG_ILLEGAL)
	{
		std::cout << "/!\\ Shouldn't happen /!\\" << std::endl;
	}
	if (wg_set_field(agdb->db_ptr, record_ptr, index, encoded_data) < 0)
	{
		std::cout << "Impossible to write in the field" << std::endl;
	}
}

void    node::set_field(sstr field_name, double data)
{
	uint			index;
	wg_int			encoded_data;

	try
	{
		index = this->get_field_index.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "Invalid field name: " << e.what() << std::endl;
		return ;
	}
	
	encoded_data = wg_encode_double(agdb->db_ptr, data);
	if(encoded_data == WG_ILLEGAL)
	{
		std::cout << "/!\\ Shouldn't happen /!\\" << std::endl;
	}
	if (wg_set_field(agdb->db_ptr, record_ptr, index, encoded_data) < 0)
	{
		std::cout << "Impossible to write in the field" << std::endl;
	}
}
/* end of setters */

/* list managment */

void	node::list_begining(sstr field_name)
{
	try
	{
		this->list_first_elem.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "[ERROR] Bad list initialisation: " << e.what() << std::endl;
		exit(-1) ;
	}
	list_current_elem[field_name] = list_first_elem[field_name];
}

int	node::list_next(sstr field_name)
{
	void	*rec;
	try
	{
		rec = this->list_current_elem.at(field_name);
	}
	catch (std::exception& e)
	{
		std::cout << "[ERROR] Bad list initialisation: " << e.what() << std::endl;
		exit(-1) ;
	}	
	list_current_elem[field_name] = wg_decode_record(agdb->db_ptr,\
								wg_get_field(agdb->db_ptr,\
								rec, AEON_LIST_NEXT)); 
	if (list_current_elem[field_name] == NULL)
		return (0);
	else
		return (1);

}

void	node::get_list_elem(sstr field_name, node **ret)
{
	void	*rec;
	std::vector<sstr>	fields_list;
	
	rec = list_current_elem[field_name];
	*ret = new node(wg_decode_record(agdb->db_ptr, wg_get_field(agdb->db_ptr, rec, AEON_LIST_VALUE)));
	(*ret)->agdb = this->agdb;
	fields_list = agdb->get_type_fields[ agdb->get_type_id[field_name] ];
	for (uint i = 0; i < fields_list.size(); i++)
	{
		(*ret)->get_field_index[fields_list[i]] = i;
	}
}
void	node::add_list_elem(sstr field_name, node *data)
{
	void	*rec;
	void	*prev;

	if (this->list_last_elem.count(field_name))
	{	
		prev = this->list_last_elem[field_name];
		rec = wg_create_record(agdb->db_ptr, 3);
		wg_set_field(agdb->db_ptr, rec, 0, wg_encode_int(agdb->db_ptr, agdb->get_type_id[field_name]));
		wg_set_field(agdb->db_ptr, rec, 1, wg_encode_record(agdb->db_ptr, NULL));
		wg_set_field(agdb->db_ptr, prev, 1, wg_encode_record(agdb->db_ptr, rec));
		wg_set_field(agdb->db_ptr, rec, 2, wg_encode_record(agdb->db_ptr, data->record_ptr));

		list_last_elem[field_name] = rec;
	}
	else
	{
		rec = wg_create_record(agdb->db_ptr, 3);
		wg_set_field(agdb->db_ptr, rec, 0, wg_encode_int(agdb->db_ptr, agdb->get_type_id[field_name]));
		wg_set_field(agdb->db_ptr, rec, 1, wg_encode_record(agdb->db_ptr, NULL));
		wg_set_field(agdb->db_ptr, rec, 2, wg_encode_record(agdb->db_ptr, data->record_ptr));

		list_first_elem[field_name] = rec;
		list_current_elem[field_name] = rec;
		list_last_elem[field_name] = rec;
	}	
}
/* end of list */
