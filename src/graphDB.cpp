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
	this->db_ptr = nullptr;
	this->create_db(db_name, db_size);
	this->number_of_types = 1000; // that let the first 1000 type id for internal purposes
	this->_add_node_type("_list_elem", 2, {"next", "value"}); // define AEON_LIST_NEXT 1/AEON_LIST_VALUE 2
	/* data type container */
	this->_add_node_type("_type_list", 1, {"list"});
	/* */
	this->_add_node_type("_type", 3, {"concat_fields_names", "name", "id"});
	this->type_data = search_node("_type_list");
	if (this->type_data == nullptr)
	{
		this->type_data = this->_create_node("_type_list"); //creat the type data container
	}
	else
	{
	/* init type data from db */
	node	*getted_node;
	sstr	str;
	sstr	name;
	char	*pch;
	char	*cstr;
	int	i = 0;
	int	id;

	this->type_data->list_begining("list");
	do
	{
		this->type_data->get_list_elem("list", &getted_node);
		if (getted_node == nullptr)
			break ;
		getted_node->get_field("concat_fields_names", &str);
		getted_node->get_field("name", &name);
		getted_node->get_field("id", &id);
		get_type_id[name] = id;
		get_type_name[id] = name;
		/**/
		cstr = new char[str.size()+1];
		strcpy(cstr, str.c_str());
		pch = strtok(cstr," ");
		i = 0;
		while (pch != nullptr)
		{
			std::cout << pch << std::endl;
			get_type_fields_map[get_type_id[name]][pch] = i + 1;
			pch = strtok(nullptr, " ");
			i++;
		}
		get_type_size[name] = i;
		delete cstr;
		/**/
		delete getted_node;
	}                                                                                            
	while(this->type_data->list_next("list"));
	}
	return ;
}

graphDB::graphDB( sstr db_name )
{
	this->db_ptr = nullptr;
	this->connect_db(db_name );
	this->number_of_types = 1000; // that let the first 1000 type id for internal purposes
	this->_add_node_type("_list_elem", 2, {"next", "value"}); 
	/* data type container */
	this->_add_node_type("_type_list", 1, {"list"});
	/* */
	this->_add_node_type("_type", 2, {"concat_fields_names", "name"});
	this->type_data = this->_create_node("_type_list"); //creat the type data container
	/* init */
	// type_data = find type
	//

	return ;
}

graphDB::~graphDB( void )
{

	return ;
}

void		graphDB::create_db(sstr db_name, uint db_size)
{
	char *name = new char[db_name.size()+1];

	if(this->db_ptr != nullptr)
	{
		std::cout << "ERROR: you can't connect twice to a database" << std::endl;
		exit(-1);
	}
	strcpy(name, db_name.c_str());
	db_ptr = wg_attach_database(name, db_size);
	if (!db_ptr)
	{
		std::cout << "Failed to attach database" << std::endl;
		exit(-1);
	}

	delete name;
}


void		graphDB::connect_db(sstr db_name)
{
	char *name = new char[db_name.size()+1];

	if(this->db_ptr != nullptr)
	{
		std::cout << "ERROR: you can't connect twice to a database" << std::endl;
		exit(-1);
	}
	strcpy(name, db_name.c_str());

	if (!(this->db_ptr = wg_attach_existing_database(name)))
	{
		std::cout << "Database named \"" << db_name << "failled to attach !" << std::endl;
		exit(-1);
	}

	delete name;
}

void		graphDB::add_node_type(sstr name, uint size, std::vector<sstr> fields_name)
{
	std::map<sstr, uint>	map_fields_name;
	sstr	concated;
	node *node2;

	if(name[0] == '_')
	{
		std::cout << "/!\\ Type name begining by '_' are reserved." << std::endl;
		return ;
	}
	if(get_type_id.count(name))
	{
		std::cout << "/!\\ This node type is already defined." << std::endl;
		return ;
	}
	this->get_type_id[name] = this->number_of_types;
	this->get_type_size[name] = size;
	this->get_type_name[this->number_of_types] = name;

	for (uint i = 0; i < size; i++)
		map_fields_name[fields_name[i]] = i + 1;

//	this->get_type_fields[this->number_of_types] = fields_name;
	this->get_type_fields_map[this->number_of_types] = map_fields_name;


	/* add type to shared mem */
	for (uint i = 0; i < fields_name.size(); i++)
		concated = concated + ' ' + fields_name[i];
	node2 = this->_create_node("_type"); //  create a new
	node2->set_field("concat_fields_names", concated);
	node2->set_field("name", name);
	node2->set_field("id", (int)get_type_id[name]);
	type_data->add_list_elem( "list", node2 ); // add a node in the unique fied
	delete node2;
	this->number_of_types += 1; 
}

void		graphDB::_add_node_type(sstr name, uint size, std::vector<sstr> fields_name)
{
	std::map<sstr, uint>	map_fields_name;
	/*	sstr	concated;
		node *node2;
	 */

	if(get_type_id.count(name))
	{
		std::cout << "/!\\ This node type is already defined." << std::endl;
		return ;
	}
	this->get_type_id[name] = this->number_of_types;
	this->get_type_size[name] = size;
	this->get_type_name[this->number_of_types] = name;

	for (uint i = 0; i < size; i++)
		map_fields_name[fields_name[i]] = i + 1;

//	this->get_type_fields[this->number_of_types] = fields_name;
	this->get_type_fields_map[this->number_of_types] = map_fields_name;
	/* add type to shared mem */
	/*
	   for (uint i = 0; i < fields_name.size(); i++)
	   concated = concated + ' ' + fields_name[i];
	   node2 = this->_create_node("_type"); //  create a new
	   std::cout << "log" << std::endl;
	   node2->set_field("concat_fields_name", concated);
	   node2->set_field("name", name);
	   std::cout << "log" << std::endl;
	   type_data->add_list_elem( "list", node2 ); // add a node in the unique fied
	   delete node2;
	 */
	/*
	 ** ok the matter is that we whant to add each type to the mem but we do
	 ** that using types that should have been previously added to the mem... infinit recu !
	 */


	this->number_of_types += 1; 
}


node		*graphDB::create_node(sstr type_name)
{
	node	*nnode = nullptr;
	wg_int	encoded_data;
	void	*rec;

	if(type_name[0] == '_')
	{
		std::cout << "/!\\ Type name begining by '_' are reserved." << std::endl;
		exit(0);
	}
	rec = wg_create_record(this->db_ptr, get_type_size[type_name] + 1); // remember +1 for type field
	if(rec == nullptr)
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
	nnode = new node(rec, this);
	nnode->_type_id = get_type_id[type_name];

	return ( nnode );
}

node		*graphDB::_create_node(sstr type_name)
{
	node	*nnode = nullptr;
	wg_int	encoded_data;
	void	*rec;

	rec = wg_create_record(this->db_ptr, get_type_size[type_name] + 1); // remember +1 for type field
	if(rec == nullptr)
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
	nnode = new node(rec, this);
	//	for (uint i = 0; i < get_type_size[type_name]; i++)
	//		nnode->get_field_index[(get_type_fields[get_type_id[type_name]])[i]] = i + 1;

	/* don't forget to add type field !!!!! */
	return ( nnode );
}
node	*graphDB::search_node(sstr type_name, sstr field_name, sstr searched)
{
	void	*rec;
	char *searched_cstr = new char[searched.size()+1];

	strcpy(searched_cstr, searched.c_str());
	wg_int fieldnr = (get_type_fields_map[get_type_id[type_name]])[field_name];
	rec = wg_find_record_str(db_ptr, fieldnr, WG_COND_EQUAL, searched_cstr, nullptr);
	delete searched_cstr;
	if (rec == nullptr)
		return (nullptr);
	else
		return (new node(rec, this));
}

node	*graphDB::search_node(sstr type_name)
{
	void	*rec;

	rec = wg_find_record_int(db_ptr, 0, WG_COND_EQUAL, get_type_id[type_name], nullptr);
	if (rec == nullptr)
		return (nullptr);
	else
		return (new node(rec, this));
}
