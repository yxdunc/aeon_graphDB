#include "graphDB.hpp"
#include <string.h>

using namespace aeon;

graphDB::graphDB( void )
{

	return ;
}

graphDB::graphDB( sstr db_name, uint db_size )
{
	this->connect_db(db_name, db_size);
	return ;
}

graphDB::~graphDB( void )
{

	return ;
}

void		graphDB::connect_db(sstr db_name, uint db_size)
{
	char *name = new char[db_name.size()+1];

	strcpy(name, db_name.c_str());

	if (!(this->db_ptr = wg_attach_database(name, db_size)))
		std::cout << "Failed to attach database" << std::endl;

	delete name;
}

/*

void		graphDB::add_node_type(sstr name, uint size, uint *fields_type, sstr *fields_name)
{

}
aeon::node	*create_node(sstr type_name)
{

}
*/
