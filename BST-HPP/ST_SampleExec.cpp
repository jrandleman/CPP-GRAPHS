// Author: Jordan Randleman -- BST_SampleExec.cpp -- BST & AVL Classes Header Demo

#include <iostream>
#include <random> // for std::default_random_engine
#include <chrono> // for std::chrono::system_clock
#include <vector>
#include "BST.hpp"


/***
 * bst & avl Current Available Methods (suppose "Type elt;"):
 * (00) bst()/avl(), bst(elt)/avl(elt), bst(bst)/avl(bst), bst(intializer_list)/avl(intializer_list),
 *      bst(vector, BST::PREORDER [OR BST::POSTORDER])/avl(vector, BST::PREORDER [OR BST::POSTORDER]),
 *      ~bst()/~avl()
 * (01) insert(elt), insert(intializer_list), insert(bst/avl)
 * (02) remove(elt), remove(intializer_list), remove(bst/avl), remove_if(Unary_Predicate_function)
 * (03) clear()
 * (04) preorder_vector(), postorder_vector(), inorder_vector()
 * (05) min(), max()
 * (06) has(elt)
 * (07) size(), sizeO1(), height(), level_of(elt)
 * (08) show(), show_preorder(), show_inorder(), show_postorder()
 * (09) show_diagram()
 * (10) begin(), cbegin(), crbegin(), rbegin(), end(), cend(), crend(), rend()
 *
 * NOTES:
 *   => BST::bst && BST::avl => BOTH HAVE THE SAME PUBLIC INTERFACE!
 *   => Datatypes in BST's class objects must support '>', '<', & '=='
 */


int main() {
  using std::cout;
  using std::endl;


  // random number generator
  unsigned int random_number_generator_seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rando_num(random_number_generator_seed);


  // Binary Search Tree => namespace "BST" holds "bst" & "avl"
  BST::bst<int> tree;

  // inserting ints to binary tree
  for(int i = 0; i < 10; ++i)
    tree.insert(rando_num() % 100);

  // showing elts in 3 different orders
  cout << "\nPreorder  = ";
  tree.show_preorder();
  cout << endl << "Inorder   = ";
  tree.show_inorder();
  cout << endl << "Postorder = ";
  tree.show_postorder();
  cout << endl;

  // checking whether has an elt
  if(tree.has(8)) cout << "bst has 8!\n";
  if(tree.has(88)) cout << "bst has 88!\n";

  // checking min/max/size/height
  cout << "bst's min, max, size, & height = " << tree.min() << ", " 
                                              << tree.max() << ", " 
                                              << tree.size() << ", & " 
                                              << tree.height() << "\n";

  // removing a number 
  if(tree.remove(8)) {
    cout << "found and rmvd 8: ";
    tree.show(); // show() invokes "show_inorder()"
    cout << endl;
  } else {
    cout << "couldn't find 8 to remove: \n"
         << tree << endl; // overloaded 'ostream<<' invokes "show_inorder()"
  }

  // outputting size in O1 time ("size()" uses O(n) time) & clearing all elts
  cout << "bst's size prior \"clear()\" = " << tree.sizeO1() << endl;
  tree.clear();
  cout << "bst's size after \"clear()\" = " << tree.sizeO1() << endl;

  // using initializer brace notation to insert multiple values
  // => an "initializer brace" can also be passed to a CTOR!
  tree.insert({6, 1, 9, 0, 3, 7, 8, 2, 5, 4});

  // print the BST's diagram, remove all even elts, then show it's diagram again
  cout << "\nbst's \"show_diagram()\" after inserting \"6, 1, 9, 0, 3, 7, 8, 2, 5, 4\":";
  tree.show_diagram();
  tree.remove_if([](auto e){return e % 2 == 0;});
  cout << "\nbst's \"show_diagram()\" after removing all even elts:";
  tree.show_diagram();



  // Constructing an AVL from a bst => namespace "BST" holds "bst" & "avl"
  // avl's have an identical public interface to bst's -- the rotation differences
  // are autonomously applied privately
  BST::avl<int> avl_tree = tree;

  // inserting random elts to the AVL
  for(int i = 0; i < 10; ++i)
    avl_tree.insert((rando_num() % 100));

  // outputting an AVL's diagram
  cout << "\nAVL tree's diagram after initializing with the \"bst\" & adding random elts:";
  avl_tree.show_diagram();

  // remove all odd elts from AVL
  avl_tree.remove_if([](auto e){return e % 3 == 0;});
  cout << "\nAVL after removing all multiples of 3 (size = " 
       << avl_tree.size() << ") = " << avl_tree << endl
       << "Resulting AVL Diagram:";
  avl_tree.show_diagram();

  // outputting AVL's basic stats
  cout << "\nAVL'smin, max, size, & height = " 
       << avl_tree.min() << ", "
       << avl_tree.max() << ", "
       << avl_tree.size() << ", "
       << avl_tree.height() << endl;

  // outputting level of elt "5" in AVL
  cout << "level of elt \"5\" in AVL: " << avl_tree.level_of(5) << endl << endl;



  // BST::bst & BST::avl Iterators are ALWAYS CONST (__CANNOT__ CHANGE VALUE)!
  // iterators traverse "inorder"
  cout << "Using \033[1m\033[4mCONST\033[0m Iterators to traverse AVL \"inorder\":\n";
  for(auto it = avl_tree.begin(); it != avl_tree.end(); ++it) 
    cout << *it << ", ";
  cout << endl;



  // Constructing a BST from a std::vector of AVL preorder elts
  std::vector<int> avl_tree_preorder = avl_tree.preorder_vector();
  BST::bst<int> preord_ctord_bst(avl_tree_preorder, BST::PREORDER);
  cout << "\nBST::bst ctor'd via std::vector of AVL's \"preorder\" elts:";
  preord_ctord_bst.show_diagram();

  // Constructing an AVL from a std::vector of AVL's postorder elts
  std::vector<int> avl_tree_postorder = avl_tree.postorder_vector();
  BST::avl<int> postord_ctord_avl(avl_tree_postorder, BST::POSTORDER);
  cout << "\nBST::avl ctor'd via std::vector of AVL's \"postorder\" elts:";
  postord_ctord_avl.show_diagram();


  cout << endl << "Bye!\n\n";

  return 0;
}
