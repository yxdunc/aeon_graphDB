#include "graphDB.hpp"
#include <string.h>
#include <cstdlib>
#include "node.hpp"

using namespace aeon;

graphDB::graphDB( void )
{

	return ;
}

graphDB::graphDB( sstr db_name, uint db_size )
{
	this->db_ptr = NULL;
	this->connect_db(db_name, db_size);
	this->number_of_types = 1000; // that let the first 1000 type id for internal purposes
	this->_add_node_type("_list", 2, {"next", "value"}); // define AEON_LIST_NEXT 1 define AEON_LIST_VALUE 2
	return ;
}

graphDB::~graphDB( void )
{

	return ;
}

void		graphDB::connect_db(sstr db_name, uint db_size)
{
	char *name = new char[db_name.size()+1];

	if(this->db_ptr != NULL)
	{
		std::cout << "ERROR: you can't connect twice to a database" << std::endl;
		exit(-1);
	}
	strcpy(name, db_name.c_str());

	if (!(this->db_ptr = wg_attach_database(name, db_size)))
	{
		std::cout << "Failed to attach database" << std::endl;
		exit(-1);
	}

	delete name;
}


void		graphDB::add_node_type(sstr name, uint size, std::vector<sstr> fields_name)
{
	if(name[0] == '_')
	{
		std::cout << "/!\\ Type name begining by '_' are reserved." << std::endl;
		return ;
	}
	this->get_type_id[name] = this->number_of_types;
	this->get_type_size[name] = size;
	this->get_type_name[this->number_of_types] = name;
	this->get_type_fields[this->number_of_types] = fields_name;

	this->number_of_types += 1; 
}

void		graphDB::_add_node_type(sstr name, uint size, std::vector<sstr> fields_name)
{
	this->get_type_id[name] = this->number_of_types;
	this->get_type_size[name] = size;
	this->get_type_name[this->number_of_types] = name;
	this->get_type_fields[this->number_of_types] = fields_name;

	this->number_of_types += 1; 
}

node		*graphDB::create_node(sstr type_name)
{
	node	*nnode = NULL;
	wg_int	encoded_data;
	void	*rec;

	if(type_name[0] == '_')
	{
		std::cout << "/!\\ Type name begining by '_' are reserved." << std::endl;
		exit(0);
	}
	rec = wg_create_record(this->db_ptr, get_type_size[type_name] + 1); // remember +1 for type field
	if(rec == NULL)
	{
		std::cout << "ERROR: couln't add a new record to the database." << std::endl;
		exit(-1);
	}
	/* set the type field */
        encoded_data = wg_encode_int(db_ptr, get_type_id[type_name]);
        if(encoded_data == WG_ILLEGAL)
        {
                std::cout << "/!\\ Shouldn't happen /!\\" << std::endl;
        }
        if (wg_set_field(db_ptr, rec, 0, encoded_data) < 0)
        {
                std::cout << "Impossible to write in the field" << std::endl;
        }
	/* */
	nnode = new node(rec);
	for (uint i = 0; i < get_type_size[type_name]; i++)
		nnode->get_field_index[(get_type_fields[get_type_id[type_name]])[i]] = i + 1;
	
	nnode->agdb = this;
	
	/* don't forget to add type field !!!!! */
	return ( nnode );
}

node		*graphDB::_create_node(sstr type_name)
{
	node	*nnode = NULL;
	wg_int	encoded_data;
	void	*rec;

	rec = wg_create_record(this->db_ptr, get_type_size[type_name] + 1); // remember +1 for type field
	if(rec == NULL)
	{
		std::cout << "ERROR: couln't add a new record to the database." << std::endl;
		exit(-1);
	}
	/* set the type field */
        encoded_data = wg_encode_int(db_ptr, get_type_id[type_name]);
        if(encoded_data == WG_ILLEGAL)
        {
                std::cout << "/!\\ Shouldn't happen /!\\" << std::endl;
        }
        if (wg_set_field(db_ptr, rec, 0, encoded_data) < 0)
        {
                std::cout << "Impossible to write in the field" << std::endl;
        }
	/* */
	nnode = new node(rec);
	for (uint i = 0; i < get_type_size[type_name]; i++)
		nnode->get_field_index[(get_type_fields[get_type_id[type_name]])[i]] = i + 1;
	nnode->agdb = this;
	
	/* don't forget to add type field !!!!! */
	return ( nnode );
}
