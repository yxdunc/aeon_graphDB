#include "graphDB.hpp"
#include "node.hpp"

int		main(void)
{
	sstr	str;
	int	num;
	double	dbl;
	aeon::node *node = NULL;
	aeon::node *next_node = NULL;
	aeon::node *getted_node = NULL;

	aeon::graphDB	*db = new aeon::graphDB("aeonDB", 100000);

	/* creat a new type of nodes */

	// list of the node fields names
	std::vector<sstr> nodeV = {"field1", "field2", "field3", "field4", "field5"}; 
	// add the node type to db
	db->add_node_type("nodeX", 5, nodeV);
	/* */

	/* create nodes of type "nodeX" */
	node = db->create_node("nodeX");
	next_node = db->create_node("nodeX");
	/* */

	/* set and get fields data of node */

	// str
	node->set_field("field1", "hello world");
	node->get_field("field1", &str);
	std::cout << str << std::endl;
	
	// int
	node->set_field("field2", 4242);
	node->get_field("field2", &num);
	std::cout << num << std::endl;

	// double
	node->set_field("field3", 42.42);
	node->get_field("field3", &dbl);
	std::cout << dbl << std::endl;

	// node (link)
	node->set_field("field4", next_node);
	node->get_field("field4", &getted_node);

	// str
	node->set_field("field5", "bye world");
	node->get_field("field5", &str);
	std::cout << str << std::endl;

/* to do
	// list
	node->add_list_elem("field6", next_node);
	node->add_list_elem("field6", getted_node);
	node->list_begining("field6");
	do
	{
		node->get_list_elem("field6", getted_node);
		node->list_next("field6");
	}
	while(getted_node != NULL)
*/
	delete db;
	return 0;
}
