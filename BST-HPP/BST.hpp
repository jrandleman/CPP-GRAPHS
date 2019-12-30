// Author: Jordan Randleman -- BST.hpp -- Binary Search Tree & AVL Class Templates

#ifndef BST_HPP_
#define BST_HPP_

#include <iostream>
#include <sstream>
#include <assert.h>
#include <map>
#include <set>
#include <vector>
#include <initializer_list>

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

// Binary Search Tree class templates
// => BST::bst && BST::avl => BOTH HAVE THE SAME PUBLIC INTERFACE!
// => Set of unique nodes, left data < right data
// => Datatype "Type" for bst data must support '>', '<', & '=='
namespace BST {

  enum states {PREORDER, INORDER, POSTORDER};

  template <typename Type>
  class bst {
  protected:
    // bst Node Struct, Enum'd Printing Orders, & Data Set
    struct NODE { Type data; NODE *left = nullptr, *right = nullptr; };
    NODE *root = nullptr;
    std::set<Type> data_set; // (used w/ std::map to print tree's diagram)


    // Private Methods
    // copy 1 bst to another
    void copy_bst(const NODE *np) {
      if(np == nullptr) return;
      insert(np->data);
      data_set.insert(np->data);
      copy_bst(np->left), copy_bst(np->right);
    }
    // alloc a new node given data
    NODE *mknode(const Type e) {
      NODE *np = new NODE;
      np->data = e;
      return np;
    }
    // bst-wide node deletion (recursive)
    void destroy_bst(NODE *np) { 
      if(np == nullptr) return;
      destroy_bst(np->left), destroy_bst(np->right);
      data_set.erase(np->data);
      delete np;
    }


    // add a non-nullptr node to bst (recursive)
    virtual NODE *addnode(NODE *np, const Type e) {
      if(np == nullptr) {
        data_set.insert(e);
        return mknode(e);
      }
      if(e < np->data)
        np->left = addnode(np->left, e), data_set.insert(e);
      else if(e > np->data)
        np->right = addnode(np->right, e), data_set.insert(e);
      return np;
    }


    // rmv node from bst w/ given data 'e' (recursive).
    // replaces rmvd node w/ either the greatest val in its 
    // left subtree or least val in its right subtree
    virtual NODE *rmnode(NODE *np, const Type e, bool &found) {
      if(np == nullptr) return nullptr;
      if(e < np->data)
        np->left = rmnode(np->left, e, found);
      else if(e > np->data)
        np->right = rmnode(np->right, e, found);
      else {
        found = true;
        // if no left/right child, shift up right/left subtree (or nullptr)
        if(np->left == nullptr || np->right == nullptr) { 
          NODE *temp = (np->left == nullptr) ? np->right : np->left;
          delete np;
          return temp;
        }
        // "np" has both left & right subtrees, thus swap np's value w/ 
        // the least value in its right subtree & rmv said "least value" from the right subtree
        NODE *temp = np->right;
        while(temp->left != nullptr) temp = temp->left;
        np->data = temp->data;
        np->right = rmnode(np->right, temp->data, found);
      }
      return np;
    }


    // count nodes/height/level-of-elt of bst (recursive)
    long long count_nodes(const NODE *np) const {
      if(np == nullptr) return 0;
      return 1 + count_nodes(np->left) + count_nodes(np->right);
    }
    long long subtree_height(const NODE *np) const {
      if(np == nullptr) return 0;
      long long left_height  = subtree_height(np->left), 
                right_height = subtree_height(np->right);
      return 1 + ((left_height > right_height) ? left_height : right_height);
    }
    long long count_level_of(const Type e) const {
      long long bst_level = 0;
      NODE *np = root;
      while(np != nullptr) {
        if(np->data == e) return bst_level;
        np = (e < np->data) ? np->left : np->right;
        ++bst_level;
      }
      return -1; // not found
    }


    // traverse bst adding elts to "order_vector" in either pre-or-post order (recursive)
    void get_bst_ordered_elts(std::vector<Type> &order_vector, const NODE *np, const int pushing_order) const {
      if(np == nullptr) return;
      if(pushing_order == PREORDER) order_vector.push_back(np->data);  // SLR
      get_bst_ordered_elts(order_vector, np->left, pushing_order);
      get_bst_ordered_elts(order_vector, np->right, pushing_order);
      if(pushing_order == POSTORDER) order_vector.push_back(np->data); // LRS
    }
    // returns a vector either in pre-or-post order (as per "pushing_order")
    std::vector<Type> prepost_ordered_vector(const int pushing_order) const {
      std::vector<Type> order_vector;
      order_vector.reserve(data_set.size());
      get_bst_ordered_elts(order_vector, root, pushing_order);
      return order_vector;
    }


    // output bst nodes in the given order (recursive)
    void show_nodes(const NODE *np, const int printing_order, const std::string seperator, std::ostream &os) const {
      if(np == nullptr) return;
      if(printing_order == PREORDER) os << np->data << seperator;  // SLR
      show_nodes(np->left, printing_order, seperator, os);
      if(printing_order == INORDER) os << np->data << seperator;   // LSR
      show_nodes(np->right, printing_order, seperator, os);
      if(printing_order == POSTORDER) os << np->data << seperator; // LRS
    }


    // "show_diagram()" Method Helper Functions
    // # of underscores on either side of the diagram's data points for the bst level
    long long underscore_padding_for_level_data(const long long level, const long long data_width, 
                                                             long long uscores = 0, long long current_level = 0) {
      if(current_level == level) return uscores;
      long long new_uscores = (2 * uscores) + ((data_width + 1) * (current_level & 1));
      return underscore_padding_for_level_data(level, data_width, new_uscores, current_level + 1);
    }
    // # of initial spaces to pad the left side of the diagram w/
    long long initial_space_padding_for_level_data(const long long level, const long long data_width) {
      return underscore_padding_for_level_data(level + 1, data_width);
    }
    // # of spaces between each data point for the bst level in the diagram
    long long level_data_spacing(const long long level, const long long bst_height,
                                          const long long data_width, long long spacing = -1, long long current_level = 0) {
      if(spacing == -1) spacing = data_width + 2;
      if(current_level == level) return spacing;
      long long new_spacing = spacing + (2 * data_width) + 2 + (4 * underscore_padding_for_level_data(current_level, data_width));
      return level_data_spacing(level, bst_height, data_width, new_spacing, current_level + 1);
    }
    // find max data point output width in bst (for show_diagram())
    long long max_data_OP_width() {
      long long max_width = 0, width;
      std::ostringstream outstr;
      for(auto it = data_set.begin(); it != data_set.end(); ++it) {
        outstr << *it; // ouput data as std::string
        if((width = outstr.str().size()) > max_width) // save widest data O/P length 
          max_width = width;
        outstr.str(""); // clear std::string output stream
      }
      return max_width;
    }
    // returns std::string of data point, prefixing data widths < "max_data_width" w/ '_'s
    std::string data_string(Type data, long long max_data_width) {
      std::ostringstream outstr, data_ostr;
      data_ostr << data;
      std::string data_str = data_ostr.str();
      outstr << std::string(max_data_width - data_str.size(), '_') << data_str;
      return outstr.str();
    }


    // Returns Level Of A Given Pre-Or-Post Ordered Elt At Index "n" W/in "ordered_vect"
    long long map_prepost_order_levels(long long idx, const long long n, const std::vector<Type> &ordered_vect, const long long increment) const {
      if(idx == n) return 0;
      Type last_seen = ordered_vect[idx];
      if(last_seen < ordered_vect[n])
        while(idx != n && ordered_vect[(idx += increment)] < last_seen);
      else
        while(idx != n && ordered_vect[(idx += increment)] > last_seen);
      return 1 + map_prepost_order_levels(idx, n, ordered_vect, increment);
    }
    // Returns std::multimap Of std::pair(level,elt) From Either Pre-Or-Post 
    // Ordered std::vector "ordered_vect"'s elts (W/ Order Denoted By "order_type")
    std::multimap<long long,Type> sort_ordered_levels(const std::vector<Type> ordered_vect, const int order_type) const {
      std::multimap<long long,Type> leveled_map;
      long long start = 0, increment = 1, end = ordered_vect.size(); // as if order_type == PREORDER
      if(order_type == POSTORDER)
        start = ordered_vect.size()-1, increment = -1, end = -1;
      for(long long i = start; i != end; i += increment)
        leveled_map.insert(std::make_pair(map_prepost_order_levels(start, i, ordered_vect, increment), ordered_vect[i]));
      return leveled_map;
    }



  public:
    // Constructors/Assignment/Destructor 
    bst() = default;
    bst(const Type e)    { root = mknode(e), data_set.insert(e); }
    bst(const bst &tree) { copy_bst(tree.root); }
    bst(const std::initializer_list<Type>&e) { insert(e); }
    bst(const std::vector<Type> ordered_vect, int order_type) { 
      if(order_type != PREORDER && order_type != POSTORDER) {
        std::cerr << "\n\033[1m" << __FILE__ << ":" << __func__ << ":" << __LINE__ 
          << ":\033[31mERROR:\033[0m\033[1m namespace::BST::bst CTOR'S ORDERED"
          << " VECTOR DENOTED NEITHER PRE NOR POST\033[0m\n    –> DEFAULTING TO PRE.\n\n";
        order_type = PREORDER;
      }
      auto leveled_map = sort_ordered_levels(ordered_vect, order_type);
      for(auto e : leveled_map)
        insert(e.second);
    }
    virtual ~bst() { destroy_bst(root), root = nullptr; }
    bst &operator=(const bst &tree) {
      destroy_bst(root), root = nullptr, copy_bst(tree.root);
      return *this;
    }


    // Iterators To The In-Order std::set Of Data
    typename std::set<Type>::iterator               begin()   const {return data_set.begin();}
    typename std::set<Type>::iterator               end()     const {return data_set.end();}
    typename std::set<Type>::const_iterator         cbegin()  const {return data_set.cbegin();}
    typename std::set<Type>::const_iterator         cend()    const {return data_set.cend();}
    typename std::set<Type>::reverse_iterator       rbegin()  const {return data_set.rbegin();}
    typename std::set<Type>::reverse_iterator       rend()    const {return data_set.rend();}
    typename std::set<Type>::const_reverse_iterator crbegin() const {return data_set.crbegin();}
    typename std::set<Type>::const_reverse_iterator crend()   const {return data_set.crend();}


    // Insert/Remove A Particular Elt, Clear Entire bst
    virtual void insert(const Type e) { root = addnode(root, e); }
    virtual void insert(const std::initializer_list<Type>&e){for(auto it=e.begin();it!=e.end();++it)insert(*it);}
    virtual void insert(const bst &tree){for(auto it=tree.begin();it!=tree.end();++it)insert(*it);}
    virtual bool remove(const Type e) {
      bool found = false;
      root = rmnode(root, e, found);
      if(found) data_set.erase(e);
      if(data_set.size() == 0) root = nullptr;
      return found;
    }
    virtual void remove(const std::initializer_list<Type>&e){for(auto it=e.begin();it!=e.end();++it)remove(*it);}
    virtual void remove(const bst &tree){for(auto it=tree.begin();it!=tree.end();++it)remove(*it);}
    void clear() { destroy_bst(root), root = nullptr; }

    
    // Remove Data Satisfying A Particular Condition
    template<typename Unary_Predicate>
    void remove_if(Unary_Predicate condition) {
      for(auto it = data_set.begin(); it != data_set.end();)
        condition(*it) ? remove(*it),data_set.erase(*it),it=data_set.begin() : ++it;
    }


    // Return Preorder Elt Vector 
    std::vector<Type> preorder_vector()  const {return prepost_ordered_vector(PREORDER);}
    // Return Postorder Elt Vector
    std::vector<Type> postorder_vector() const {return prepost_ordered_vector(POSTORDER);}
    // Return Inorder Elt Vector
    std::vector<Type> inorder_vector()   const {
      std::vector<Type> inord_vect;
      inord_vect.reserve(data_set.size());
      for(auto e : data_set)
        inord_vect.push_back(e);
      return inord_vect;
    }


    // Min/Max Values In bst (iterative).
    Type min() const {
      assert(root != nullptr);
      NODE *np = root;
      while(np->left != nullptr) np = np->left;
      return np->data;
    }
    Type max() const {
      assert(root != nullptr);
      NODE *np = root;
      while(np->right != nullptr) np = np->right;
      return np->data;
    }


    // Check Whether bst Contains Given 'e' Data (iterative)
    bool has(const Type e) const { return (count_level_of(e) != -1); }


    // Count Nodes/Height/Level-Of-Elt Of bst
    long long size()                 const { return count_nodes(root);    }
    long long sizeO1()               const { return data_set.size();      } // O(1), no algorithm
    long long height()               const { return subtree_height(root); }
    long long level_of(const Type e) const { return count_level_of(e);    } // -1 if DNE


    // Output Each Node In bst
    void show_preorder(const std::string seperator = " ", std::ostream &os = std::cout)  const {
      show_nodes(root, PREORDER, seperator, os);
    }
    void show_inorder(const std::string seperator = " ", std::ostream &os = std::cout)   const {
      show_nodes(root, INORDER, seperator, os);
    }
    void show_postorder(const std::string seperator = " ", std::ostream &os = std::cout) const {
      show_nodes(root, POSTORDER, seperator, os);
    }
    void show(const std::string seperator = " ", std::ostream &os = std::cout)           const {
      show_nodes(root, INORDER, seperator, os); // inorder, LSR
    }


    // Overloaded I/O Stream Friends
    friend std::ostream &operator<<(std::ostream &os, bst &tree) { tree.show(" ", os); return os; }
    friend std::istream &operator>>(std::istream &is, bst &tree) { 
      Type e; is >> e; is.get(); tree.insert(e); return is; 
    }


    // Output ASCII Diagram Of bst
    void show_diagram(std::ostream &os = std::cout) {
      os << std::endl;
      if(data_set.size() == 0) return;
      const long long max_width = max_data_OP_width();
      long long underscores, initial_padding, data_spacing;
      std::string data_str, uscore_str, space_str, 
                  branch_str, branch_space_str, blank_node_str;

      // create a multimap sorted by elt-level keys w/ elt-data values
      std::multimap<long long, Type>data_multimap;
      for(auto it = data_set.begin(); it != data_set.end(); ++it)
        data_multimap.insert({level_of(*it), *it}); 

      // print the bst tree's diagram via ASCII art
      for(long long level = 0, bst_height = height(); level < bst_height; ++level) { // for each level
        auto range = data_multimap.equal_range(level); // iterator range of level's node data
        // the recursive padding fcns go bottom-up (rather than top-down like 
        // here) thus "bst_height-1-level" gives the inverse bst level position
        underscores     = underscore_padding_for_level_data(bst_height-1-level, max_width);
        initial_padding = initial_space_padding_for_level_data(bst_height-1-level, max_width);
        data_spacing    = level_data_spacing(bst_height-level-1, bst_height, max_width);
        uscore_str = std::string(underscores, '_'), space_str = std::string(data_spacing, ' ');
        blank_node_str  = uscore_str + std::string(max_width, '_') + uscore_str;
        long long filled_level_nodes = 0, total_level_nodes = 1 << level; // 2^(level)
        os << std::string(initial_padding, ' ');        // pad the front of the diagram

        for(auto it = range.first; it != range.second;) { // for each node data in the level
          NODE *np = root;
          long long skipped_subtree_leaves, np_level = 0, skipped_nodes = 0;
          data_str =  data_string(it->second, max_width);
          // count # of nodes on the level to the left of the current node's elt
          while(np->data != it->second) { // "it->second" == elt data
            if(it->second < np->data)
              np = np->left;
            else { // going right skips over level's leaves from the left subtree
              skipped_subtree_leaves = 1 << (level - np_level - 1); // 2^(level - np_level - 1)
              skipped_nodes += skipped_subtree_leaves;
              np = np->right;
            }
            ++np_level;
          }
          // output '_' where nodes could've been in bst if assigned data
          for(; filled_level_nodes < skipped_nodes; ++filled_level_nodes)
            os << blank_node_str << space_str; // EMPTY NODE VALUE WHERE DATA COULD'VE BEEN
          // output data point
          os << uscore_str << data_str << uscore_str;
          if(++it != range.second) os << space_str;
          ++filled_level_nodes;
        }

        // output any empty data point slots left tailing the bst level
        for(; filled_level_nodes < total_level_nodes; ++filled_level_nodes)
          os << space_str << blank_node_str; // EMPTY NODE VALUE WHERE DATA COULD'VE BEEN
        os << std::endl;

        // output branches below the data point if not the lowest level
        if(level + 1 != bst_height) {
          branch_str       = "/" + std::string(2 * underscores + max_width, ' ') + "\\";
          branch_space_str = std::string(data_spacing - 2, ' ');
          os << std::string(initial_padding - 1, ' '); // pad front of diagram's branches
          for(long long idx = 0; idx < filled_level_nodes; ++idx) {
            os << branch_str;
            if(idx + 1 != filled_level_nodes) os << branch_space_str;
          }
          os << std::endl;
        }
      }
    }
  };




  template <typename Type>
  class avl : public bst<Type> {
  private:
    // NODE Structure
    using NODE = typename bst<Type>::NODE;


    // Right & Left Rotations
    NODE *rotate_right(NODE *np) {
      NODE *subtree_head  = np->left;
      NODE *subtree_child = subtree_head->right;
      subtree_head->right = np;
      np->left = subtree_child;
      return subtree_head;
    }
    NODE *rotate_left(NODE *np) {
      NODE *subtree_head  = np->right;
      NODE *subtree_child = subtree_head->left;
      subtree_head->left = np;
      np->right = subtree_child;
      return subtree_head;
    }


    // Balance Factor For A Node
    long long balance_factor(NODE *np) {
      if(np == nullptr) return 0;
      return this->subtree_height(np->left) - this->subtree_height(np->right);
    }


    // Add a Given Elt & Re-Balance As Needed
    NODE *addnode(NODE *np, const Type e) {
      // New Elt
      if(np == nullptr) {
        this->data_set.insert(e);
        return this->mknode(e);
      }

      // Traverse Tree
      if(e < np->data)
        np->left = addnode(np->left, e);
      else if(e > np->data)
        np->right = addnode(np->right, e);
      else
        return np; // AVL already has elt

      // Rotate AVL As Needed
      const long long np_balance_factor = balance_factor(np);
      if(np_balance_factor > 1 && e < np->left->data)     // LEFT LEFT
        return rotate_right(np);
      if(np_balance_factor < -1 && e > np->right->data)   // RIGHT RIGHT
        return rotate_left(np);
      if(np_balance_factor > 1 && e > np->left->data) {   // LEFT RIGHT
        np->left = rotate_left(np->left);
        return rotate_right(np);
      } 
      if(np_balance_factor < -1 && e < np->right->data) { // RIGHT LEFT
        np->right = rotate_right(np->right);
        return rotate_left(np);
      }
      return np; // Already Balanced - No Rotations Needed
    }


    // Rmv a Given Elt & Re-Balance As Needed
    NODE *rmnode(NODE *np, const Type e, bool &found) {
      // Elt Not Found
      if(np == nullptr) return np;

      // Traverse Tree
      if(e < np->data)
        np->left = rmnode(np->left, e, found);
      else if(e > np->data)
        np->right = rmnode(np->right, e, found);
      else {
        found = true;
        // if missing either child (or both) shift up other side (or nullptr)
        if(np->left == nullptr || np->right == nullptr) { 
          NODE *temp = (np->left != nullptr) ? np->left : np->right;
          if(temp == nullptr) // no children, nothing to shift up
            temp = np, np = nullptr;
          else
            *np = *temp; // shift up non-nullptr child
          delete temp;
        } else { // "np" has both children
          // swap np's value w/ least value in right subtree & rmv said value
          NODE *temp = np->right;
          while(temp->left != nullptr) temp = temp->left;
          np->data = temp->data;
          np->right = rmnode(np->right, temp->data, found);
        }
      }

      // If No Children, No Rotations
      if(np == nullptr) return np; 

      // Rotate Tree As Needed
      const long long np_balance_factor = balance_factor(np);
      const long long left_child_balance_factor = balance_factor(np->left);
      const long long right_child_balance_factor = balance_factor(np->right);
      if(np_balance_factor > 1 && left_child_balance_factor >= 0)    // LEFT LEFT
        return rotate_right(np);
      if(np_balance_factor < -1 && right_child_balance_factor <= 0)  // RIGHT RIGHT
        return rotate_left(np);
      if(np_balance_factor > 1 && left_child_balance_factor < 0) {   // LEFT RIGHT
        np->left = rotate_left(np->left);
        return rotate_right(np);
      }
      if(np_balance_factor < -1 && right_child_balance_factor > 0) { // RIGHT LEFT
        np->right = rotate_right(np->right);
        return rotate_left(np);
      }
      return np;
    }


    // Copy subtree to AVL, Balancing tree as needed
    void copy_avl(const NODE *np) {
      if(np == nullptr) return;
      insert(np->data);
      copy_avl(np->left), copy_avl(np->right);
    }



  public:
    // Constructors/Assignment/Destructor 
    avl() = default;
    avl(const Type e)                        { insert(e); }
    avl(const std::initializer_list<Type>&e) { insert(e); }
    avl(const bst<Type> &tree)               { insert(tree); }
    avl(const avl &tree)                     { insert(tree); }
    avl(const std::vector<Type> ordered_vect, int order_type) { 
      if(order_type != PREORDER && order_type != POSTORDER) {
        std::cerr << "\n\033[1m" << __FILE__ << ":" << __func__ << ":" << __LINE__ 
          << ":\033[31mERROR:\033[0m\033[1m namespace::BST::avl CTOR'S ORDERED"
          << " VECTOR DENOTED NEITHER PRE NOR POST\033[0m\n    –> DEFAULTING TO PRE.\n\n";
        order_type = PREORDER;
      }
      auto leveled_map = this->sort_ordered_levels(ordered_vect, order_type);
      for(auto e : leveled_map)
        insert(e.second);
    }
    virtual ~avl() = default;
    avl &operator=(const avl &tree) {
      this->destroy_bst(this->root), this->root = nullptr;
      copy_avl(tree.root);
      return *this;
    }
    avl &operator=(const bst<Type> &tree) {
      avl avl_tree(tree); 
      *this = avl_tree; 
      return *this;
    }

    // Insert/Remove A Particular Elt, Clear Entire avl
    void insert(const Type e) { this->root = addnode(this->root, e); }
    void insert(const std::initializer_list<Type>&e){for(auto it=e.begin();it!=e.end();++it)insert(*it);}
    void insert(const bst<Type> &tree){for(auto it=tree.begin();it!=tree.end();++it)insert(*it);}
    void insert(const avl &tree){copy_avl(tree.root);}
    bool remove(const Type e){bool found=false; this->root = rmnode(this->root,e,found); if(found)this->data_set.erase(e); return found;}
    void remove(const std::initializer_list<Type>&e){for(auto it=e.begin();it!=e.end();++it)remove(*it);}
    void remove(const bst<Type> &tree){for(auto it=tree.begin();it!=tree.end();++it)remove(*it);}
    void remove(const avl &tree){for(auto it=tree.begin();it!=tree.end();++it)remove(*it);}

    // Remove Data Satisfying A Particular Condition
    template<typename Unary_Predicate>
    void remove_if(Unary_Predicate condition) {
      for(auto it = this->data_set.begin(); it != this->data_set.end();)
        (condition(*it)) ? (remove(*it), this->data_set.erase(*it), it=this->data_set.begin()) : ++it;
    }
  };
}
#endif
