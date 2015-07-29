#include "graphDB.hpp"
#include "node.hpp"

void		create_tree(aeon::node *node, int i)
{
	std::cout << "new node" << std::endl;
	aeon::node	*next1 = node->agdb->create_node("node_b");
	aeon::node	*next2 = node->agdb->create_node("node_b");
	node->set_field("val1", "hello");	
	node->set_field("level", i);	
	node->set_field("next1", next1);	
	node->set_field("next2", next2);
	if (i > 0)
	{
		i--;
		create_tree(next1, i);
		create_tree(next2, i);
	}
	delete next1;
	delete next2;
}

int		main(void)
{
	aeon::node *node = NULL;
	aeon::node *getter = NULL;

	aeon::graphDB	*db = new aeon::graphDB("aeonDB", 100000);

	/* creat a new type of nodes */
	db->add_node_type("node_b", 4, {"next1", "next2", "val1", "level"}); // size is useless
	db->add_node_type("unique_to_display", 1, {"nodes"}); // size is useless
	/* */

	/* create node of type "node_b" */
	node = db->create_node("node_b");
	getter = db->create_node("unique_to_display");
	/* */

	// set
	create_tree(node, 1);
	getter->set_field("nodes", node);	

	delete node;
	delete getter;
	delete db;
	return 0;
}

