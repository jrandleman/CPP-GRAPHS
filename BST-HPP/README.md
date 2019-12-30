# BST-AVL
## Binary Search Tree &amp; AVL Class Templates/Visualization!
--------
## Notes:
* _Compile as C++11 or higher!_</br>
* _Datatypes stored in BST's/AVL's must support `>`, `<`, & `==` operators!_</br></br>
* _`namespace BST` contains `bst` & `avl` class templates (`BST::bst` & `BST::avl`)!_</br>
* _Iterators are constant (can't change values) & traverse `inorder`!_</br>
* _`BST::avl` & `BST::bst` have the same public interface!_</br>
* _`BST::PREORDER` & `BST::POSTORDER` enums used with vector-of-ordered-elts ctors!_
--------
## Methods
### Constructors (`bst()` == `avl()`)
`bst()` => _empty bst_</br>
`bst(elt)` => _bst with an element `elt`_</br>
`bst(bst)` => _bst copy of another bst_</br>
`bst(std::intializer_list)` => _bst with elts from an `initializer_list`_</br>
`bst(std::vector, int order_type)` => _bst copy given vector of preorder or postorder elts_</br>
=> **Note: "`int order_type`" _should either be_ `BST::PREORDER` _or_ `BST::POSTORDER`_!_**
### Destructor
`~bst()` => _frees nodes_
### Insertion
`.insert(elt)` => _insert an element `elt`_</br>
`.insert(std::intializer_list)` => _insert elts from an `initializer_list`_</br>
`.insert(bst)` => _insert all `elts` from given bst_</br>
`.insert(avl)` => _insert all `elts` from given avl_
### Removal
`.remove(elt)` => _remove an element `elt` from a bst_</br>
`.remove(std::intializer_list)` => _remove elts from an `initializer_list` from a bst_</br>
`.remove(bst)` => _remove all `elts` from given bst_</br>
`.remove(avl)` => _remove all `elts` from given avl_</br>
`.remove_if(Unary_Predicate)` => _remove elts from a bst if they satisfy a particular condition_</br>
`.clear()` => _remove all elts from bst_
### Element "std::vector"s
`.preorder_vector()` => _return vector of bst elts preorder_</br>
`.inorder_vector()` => _return vector of bst elts inorder_</br>
`.postorder_vector()` => _return vector of bst elts postorder_
### Min/Max
`.min()` => _returns min elt_</br>
`.max()` => _returns max elt_
### Search
`.has(elt)` => _returns bool as to whether bst has `elt`_
### Sizing
`.size()` => _# of nodes in bst, `O(n)` time_</br>
`.sizeO1()` => _# of nodes in bst, `O(1)` time_</br>
`.height()` => _height of bst_</br>
`.level_of(elt)` => _level of `elt` in bst_
### Show
=> _Next 4 methods below take: `(const std::string seperator=" ",std::ostream&os=std::cout)` by default_ </br>
`.show_preorder()` => _print bst elts in `preorder`_</br>
`.show_inorder()` => _print bst elts in `inorder`_</br>
`.show_postorder()` => _print bst elts in `postorder`_</br>
`.show()` => _print bst elts in `inorder`_</br>
`.show_diagram()` => _Output ASCII diagram of bst's node structure_
### Iterators (all w/ inorder traversal & _CONSTANT!_):
`.begin()` => _iterator at the beginning_</br>
`.cbegin()` => _iterator at the beginning (same effect as the above method)_</br>
`.crbegin()` => _reverse iterator at the beginning_</br>
`.rbegin()` => _reverse iterator at the beginning_</br>
`.end()` => _iterator at 1 past the end_</br>
`.cend()` => _iterator at 1 past the end_</br>
`.crend()` =>_reverse iterator at 1 past the end_</br>
`.rend()` => _reverse iterator at 1 past the end_

--------
## Visualization
### Sample `.show_diagram()` Output of AVL w/ `1,5,7,11,17,32,80,86,98`:
 ```
                ______32______
               /              \
       ____7___                ___86___
      /        \              /        \
    _5          11          80          98
   /  \        /  \        /  \        /  \
_1    __    __    17    __    __    __    __
 ```
