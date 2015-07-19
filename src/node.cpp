#include "node.hpp"
#include <exception>

using namespace aeon;

node::node( void *record ) : record_ptr(record)
{

	return ;
}

node::~node( void )
{

	return ;
}

void    node::get_field(sstr field_name, node **ret) const/* maybe a double dereferencement */
{
	uint			index;
	uint			type_id;
	void			*rec;
	node			*fresh_node;
	std::vector<sstr>	fields_list; /* store the names of the fields */

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
	type_id = wg_decode_int(agdb->db_ptr, wg_get_field(agdb->db_ptr, rec, 0)); /* 0 is type field */
	fields_list = agdb->get_type_fields[type_id];
	for (uint i = 0; i < fields_list.size(); i++)
	{
		fresh_node->get_field_index[fields_list[i]] = i+1;
	}

	*ret = fresh_node;
}
