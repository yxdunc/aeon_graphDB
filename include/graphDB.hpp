#ifndef GRAPHDB_HPP
# define GRAPHDB_HPP

#include <iostream>
#include <whitedb/dbapi.h>
#include <map>
#include <vector>

/* internal field type id ( [0..1000] ) */

# define AEON_TYPE_FIELD 0
# define AEON_LIST_NEXT  1 
# define AEON_LIST_VALUE 2

typedef std::string sstr;
typedef unsigned int uint;
namespace aeon{
	class	node;
}

namespace aeon{
class graphDB
{
	friend class node;

	public:

		graphDB(sstr const& db_name, uint db_size);
		graphDB(sstr const& db_name);
		~graphDB( void );


		/* add a new node type */
		void	add_node_type(sstr const& name, uint size, std::vector<sstr> const& fields_name);

		/* create a new node of type "type_name" */
		node	*create_node(sstr const& type_name);

		/* get node */
		node	*search_node(sstr const& type_name, sstr const& field_name, sstr const& searched);
		node	*search_node(sstr const& type_name);

		/* return a vector of the fields names */
	//	std::map <uint, std::vector<sstr> >	get_type_fields; // do not use anymore
		std::map <uint, std::map<sstr, uint> >	get_type_fields_map;


	private:
		graphDB( void );

		/* connect to the db and store ptr in the class instance */
		void	create_db(sstr const& db_name, uint db_size);
		void	connect_db(sstr const& db_name);

		node	*type_data; // used to restore type_list
		void	*db_ptr;

		void	_add_node_type(sstr const& name, uint size, std::vector<sstr> const& fields_name);
		node	*_create_node(sstr const& type_name);

		std::map <sstr, uint>			get_type_id;
		std::map <sstr, uint>			get_type_size;
		std::map <uint, sstr>			get_type_name;

		uint	number_of_types;
};
}
#endif //!GRAPHDB_HPP

