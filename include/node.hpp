#ifndef NODE_HPP
# define NODE_HPP

# include "graphDB.hpp"

namespace aeon{
/* field 0 of a node is always the type hence we start at 1 */
class node
{
	friend class graphDB;

	public:

		/* can only be construct with a valid record */
		node(void *record);

		~node( void );

		/* getters/setters */
		void	get_field(sstr field_name, node **ret) const; // dynamically allocate the reutned node
		void	get_field(sstr field_name, sstr *ret) const;
		void	get_field(sstr field_name, int	*ret) const;
		void	get_field(sstr field_name, double *ret) const;

		void	set_field(sstr field_name, node *data);
		void	set_field(sstr field_name, sstr data);
		void	set_field(sstr field_name, int data);
		void	set_field(sstr field_name, double data);

		/* list getting/setting */
//		void	get_list_elem(sstr field_name, node **ret); // ret allocated
//		void	add_list_elem(sstr field_name, node *data);
		/* list offset controle */
//		void	list_next(sstr field_name);
		void	list_begining(sstr field_name);

	private:
		node( void ) {};

		/* store the graphdb ptr of the instance that created it	*/
		/* (allow access to up to date type list)			*/
		graphDB	*agdb;

		void	*record_ptr;

		std::map <sstr, uint>	get_field_index; // reminder field 0 is the node type

		/* lists offset maps */
		std::map<sstr, void*>	list_current_elem;
		std::map<sstr, void*>	list_first_elem;
		std::map<sstr, void*>	list_last_elem;

};
}
#endif //!NODE_HPP

