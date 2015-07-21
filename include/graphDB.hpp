#ifndef GRAPHDB_HPP
# define GRAPHDB_HPP

#include <iostream>
#include <whitedb/dbapi.h>
#include <map>
#include <vector>

/* internal field type id ( [0..1000] ) */
//# define _LIST_TYPE 0

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

		graphDB( void );
		graphDB(sstr db_name, uint db_size);
		~graphDB( void );

		/* connect to the db and store ptr in the class instance */
		void	connect_db(sstr db_name, uint db_size);

		/* add a new node type */
		void	add_node_type(sstr name, uint size, std::vector<sstr> fields_name);

		/* create a new node of type "type_name" */
		node	*create_node(sstr type_name);

		/* return a vector of the fields names */
		std::map <uint, std::vector<sstr> >	get_type_fields;


	private:
		void	*db_ptr;

		void	_add_node_type(sstr name, uint size, std::vector<sstr> fields_name);
		node	*_create_node(sstr type_name);

		std::map <sstr, uint>			get_type_id;
		std::map <sstr, uint>			get_type_size;
		std::map <uint, sstr>			get_type_name;

		uint	number_of_types;
};
}
#endif //!GRAPHDB_HPP

