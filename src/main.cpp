#include "error_handling.h"
#include "graphDB.hpp"
#include "node.hpp"

int		main(void)
{
	sstr	str;
	aeon::graphDB	*db = new aeon::graphDB("aeonDB", 100000);
	/* */
	std::vector<sstr> nodeV = {"field1", "field2", "field3", "field4", "field5"};
	db->add_node_type("nodeV", 5, nodeV);
	aeon::node *node = db->create_node("nodeV");
	node->set_field("field1", "hello boys and girls");
	node->get_field("field1", &str);
	std::cout << str << std::endl;
	delete db;
	return 0;
}
