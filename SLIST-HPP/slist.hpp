// Author: Jordan Randleman -:- slist.hpp -:- Linked List Class Template -:- Compile >= C++17. -:- using SUPERLIST::Slist;

#ifndef SLIST_HPP_
#define SLIST_HPP_

#include <iostream>
#include <functional>
#include <string>

namespace SUPERLIST {
  // Doubly Linked List Class Template
  template <typename Type>
  class Slist {

  public:
    struct NODE { Type data; NODE *next, *prev; };

  private:
    long length;
    NODE *head, *tail;
    void AllocNewList(Slist &NewList, const Slist &OldList);
    void DeleteThisList();
    void AddNode(NODE *, NODE **, NODE **, NODE **, const Type);
    bool RmvNode(NODE *);
    void MergeLists(NODE *, Slist &, bool push_fronting = true) const;
    bool AssignPeekVal(Type &, NODE *) const; // front = head, back = tail
    template <typename iterator_type, typename iter_begin, typename iter_end>                   // Logic enabling 'slice' member
    Slist sliceLogic(iterator_type, iterator_type, iter_begin, iter_end, bool sliceFromThis = false);
    template <typename iterator_type, typename iter_begin, typename iter_end>                   // Logic enabling 'emplace' members
    void emplaceLogic(iterator_type, Slist &, iter_begin, iter_end);
    template <typename iterator_type, typename iter_begin, typename iter_end, typename compare> // Logic enabling 'splice' members
    void spliceLogic(iterator_type, iterator_type, Slist, iter_begin, iter_end, compare);


  public:
    // Iterator Class Template To Traverse Slist Objects.
    class Iterator {
    protected:
      NODE *it, **Slist_head, **Slist_tail;
      long *Slist_length;
      template<typename iterator_type> // reverse, ostream, or Iterator
      bool This_Is_Further_Than_Iter_In_List(iterator_type iter) const { // nullptr = tail -> next in comparison context
        if(it == iter.it) return false;
        NODE *This_node = it, *Iter_node = iter.it;
        while(This_node != nullptr && Iter_node != nullptr) This_node = This_node->next, Iter_node = Iter_node->next;
        return (This_node == nullptr); 
      }
      void mv_front() { (it != nullptr) ? it = it->next : it = *Slist_head; }
      void mv_back()  { (it != nullptr) ? it = it->prev : it = *Slist_tail; }
      void push_adjacent_to_iterator(Type e, bool pushing_back) {
        NODE *p = new NODE;
        p -> data = e;
        p -> next = p -> prev = nullptr;
        bool this_iter_was_nullptr = false;
        // if this == nullptr: ".push_front()" pushes prior head & ".push_back()" pushes after tail
        if(it == nullptr) {
          it = (pushing_back) ? *Slist_tail : *Slist_head;
          this_iter_was_nullptr = true, pushing_back = !pushing_back;
        }
        if(*Slist_head == nullptr) *Slist_head = *Slist_tail = p;
        else {
          // connect p to iterator's node
          if(pushing_back) p -> next = it, p -> prev = it -> prev;
          else             p -> prev = it, p -> next = it -> next;
          // connect iterator's adjecent nodes & iterator's node to p
          if(pushing_back && it -> prev != nullptr)       it -> prev -> next = p;
          else if(!pushing_back && it -> next != nullptr) it -> next -> prev = p;
          (pushing_back) ? (it -> prev = p) : (it -> next = p);
          // check whether pushing p to either end & shift head/tail accordingly
          if(pushing_back && it == *Slist_head)       *Slist_head = it -> prev;
          else if(!pushing_back && it == *Slist_tail) *Slist_tail = it -> next;
        }
        *Slist_length += 1;
        if(this_iter_was_nullptr) it = nullptr;
      }
      bool pop_adjacent_to_iterator(Type &e, bool popping_back) {
        NODE *del;
        if((popping_back && it == *Slist_head) || (!popping_back && it == *Slist_tail)) return false;
        if(it == nullptr) { // removes tail if popping_back && head if popping_front
          del = (popping_back) ? *Slist_tail : *Slist_head;
          if(*Slist_head == *Slist_tail) (popping_back) ? (*Slist_head = del->next) : (*Slist_tail = del->next); // if only 1 node
          else                           (popping_back) ? (del->prev->next = nullptr) : (del->next->prev = nullptr);
           // shift either end according to popping back or front
          (popping_back) ? (*Slist_tail = del -> prev) : (*Slist_head = del -> next);
        } else {
          del = (popping_back) ? it -> prev : it -> next;
          if(del -> next != nullptr) del -> next -> prev = del -> prev;
          if(del -> prev != nullptr) del -> prev -> next = del -> next;
          // check whether popping del at either end & shift head/tail accordingly
          if(popping_back && del == *Slist_head)       *Slist_head = del -> next;
          else if(!popping_back && del == *Slist_tail) *Slist_tail = del -> prev;
        }
        e = del -> data;
        delete del;
        *Slist_length -= 1;
        return true;
      }
    public: 
      // Constructors & Address Return
      Iterator() = default;
      Iterator(NODE *n, NODE **head_node, NODE **tail_node, long *len) { it = n, Slist_head = head_node, Slist_tail = tail_node; Slist_length = len; }
      Iterator(Iterator &iter)         { it = iter.it, Slist_head = iter.Slist_head, Slist_tail = iter.Slist_tail, Slist_length = iter.Slist_length; }
      NODE *NodeAddress()              { return it; } // Used in Slist class
      // Insertion/Deletion Of Adjacent Nodes
      void push_front(Type e)                  { push_adjacent_to_iterator(e, false); }
      void push_back(Type e)                   { push_adjacent_to_iterator(e, true); }
      bool pop_front(Type &e)                  { if(*Slist_length <= 0) return false; return pop_adjacent_to_iterator(e, false); }
      Type pop_front()                         { Type e; pop_adjacent_to_iterator(e, false); return e; }
      bool pop_back(Type &e)                   { if(*Slist_length <= 0) return false; return pop_adjacent_to_iterator(e, true); }
      Type pop_back()                          { Type e; pop_adjacent_to_iterator(e, true); return e; }
      // Move Iterator Immediately To head Or tail -> next
      void begin()                             { it = *Slist_head; }
      void end()                               { it = *Slist_tail; mv_front(); }
      // Output/Inupt Data Immediately To Slist Object's Beginning Or End, Keeping The Iterator Unmoved
      void push_begin(Type e)                  { Iterator tmp = *this; tmp.begin(); tmp.push_back(e); }
      void push_end(Type e)                    { Iterator tmp = *this; tmp.end();   tmp.push_back(e); }
      Type pop_begin()                         { Iterator tmp = *this; if(it == *Slist_head) mv_front(); tmp.end(); return tmp.pop_front(); }
      Type pop_end()                           { Iterator tmp = *this; if(it == *Slist_tail) mv_back();  tmp.end(); return tmp.pop_back();  }
      bool pop_begin(Type &e) { 
        if(*Slist_length <= 0) return false; Iterator tmp = *this; if(it == *Slist_head) mv_front(); tmp.end(); tmp.pop_front(e); return true;
      }
      bool pop_end(Type &e)   { 
        if(*Slist_length <= 0) return false; Iterator tmp = *this; if(it == *Slist_tail) mv_back(); tmp.end(); tmp.pop_back(e); return true;
      }
      // Overloaded Operators
      // assignment
      void operator=(const Iterator iter)      { it = iter.it; Slist_head = iter.Slist_head, Slist_tail = iter.Slist_tail; }
      // equality
      bool operator==(Iterator iter)     const { return iter.it == it; }
      bool operator==(std::nullptr_t NP) const { return it == nullptr; }
      bool operator!=(Iterator iter)     const { return iter.it != it; }
      bool operator!=(std::nullptr_t NP) const { return it != nullptr; }
      // location comparision in list
      bool operator>(Iterator iter)      const { return This_Is_Further_Than_Iter_In_List(iter); }
      bool operator<(Iterator iter)      const { return (!This_Is_Further_Than_Iter_In_List(iter) && *this != iter); }
      bool operator>=(Iterator iter)     const { return (operator>(iter) || *this == iter); }
      bool operator<=(Iterator iter)     const { return (operator<(iter) || *this == iter); }
      // dereference iterator for elt
      Type &operator*()                  const { return it -> data; } // Access data at Node
      // traverse list forwards
      Iterator operator+(const long long n)    { Iterator tp(*this); for(long long i = 0; i < n; ++i) tp.mv_front(); return tp; }
      void operator+=(const long long n)       { *this = *this + n; }
      Iterator &operator++()                   { mv_front(); return *this; }                     // prefix
      Iterator operator++(int)                 { Iterator tmp = *this; mv_front(); return tmp; } // postfix (dummy int variable)
      // traverse list backwards
      Iterator operator-(const long long n)    { Iterator tp(*this); for(long long i = 0; i < n; ++i) tp.mv_back(); return tp; }
      void operator-=(const long long n)       { *this = *this - n; }
      Iterator &operator--()                   { mv_back(); return *this; }                      // prefix
      Iterator operator--(int)                 { Iterator tmp = *this; mv_back(); return tmp; }  // postfix (dummy int variable)
      // I/O Friend Functions
      friend std::istream &operator>>(std::istream &is, Iterator &iter) { 
        Type e; 
        if(iter.it != nullptr) { 
          is >> e; 
          iter.it->data = e; 
        } else { // pushes post tail if iterator at '.end()'
          is >> e; 
          iter.push_back(e); 
        }
        return is; 
      }
      friend std::ostream &operator<<(std::ostream &os, const Iterator &iter) { os << iter.it; return os; }
    };


    // Reverse Iterator Class Template To Traverse Slist Objects In Reverse.
    class reverse_Iterator : public Iterator { // NOTE: can invoke base class members as either "this->member" OR "Iterator::member"
    private:
      bool passed_arg_iterator_pop(Type &e, bool popping_back) {
        if(*this->Slist_length <= 0) return false; 
        this->mv_back(); bool popped = this->pop_adjacent_to_iterator(e, popping_back); this->mv_front();
        return popped;
      }
      bool return_arg_iterator_pop(bool popping_back) { 
        Type e; 
        this->mv_back(); this->pop_adjacent_to_iterator(e, popping_back); this->mv_front(); 
        return e; 
      }
    public:
      // Constructors & Address Return
      reverse_Iterator() : Iterator() {};
      reverse_Iterator(NODE *n, NODE **head_node, NODE **tail_node, long *len) : Iterator(n, head_node, tail_node, len) {};
      reverse_Iterator(reverse_Iterator &iter) : Iterator(iter) {};
      NODE *NodeAddress()                           { return (this->it==nullptr) ? *this->Slist_tail : this->it->prev; } // Used in Slist class
      // Insertion/Deletion Of Adjacent Nodes
      void push_front(Type e)                       { this->mv_back(); this->push_adjacent_to_iterator(e, true);  this->mv_front(); }
      void push_back(Type e)                        { this->mv_back(); this->push_adjacent_to_iterator(e, false); this->mv_front(); }
      bool pop_front(Type &e)                       { return passed_arg_iterator_pop(e, true);  }
      Type pop_front()                              { return return_arg_iterator_pop(true);     }
      bool pop_back(Type &e)                        { return passed_arg_iterator_pop(e, false); }
      Type pop_back()                               { return return_arg_iterator_pop(false);    }
      // Move Iterator Immediately To Reverse Beginning Or Reverse End
      void begin()                                  { this->it = *this->Slist_tail; this->mv_front(); }
      void end()                                    { this->it = *this->Slist_head; }
      // Output/Inupt Data Immediately To Slist Object's Beginning Or End, Keeping The Iterator Unmoved
      void push_begin(Type e)                       { reverse_Iterator tmp = *this; tmp.begin(); tmp.push_back(e); }
      void push_end(Type e)                         { reverse_Iterator tmp = *this; tmp.end();   tmp.push_back(e); }
      Type pop_begin() { reverse_Iterator tmp = *this; if(this->it == *this->Slist_tail) this->mv_back(); tmp.end(); return tmp.pop_front(); }
      Type pop_end()   { reverse_Iterator tmp = *this; if(this->it == *this->Slist_head) this->mv_front(); tmp.end(); return tmp.pop_back();  }
      bool pop_begin(Type &e) { 
        if(*this->Slist_length <= 0) return false;
        reverse_Iterator tmp = *this; if(this->it == *this->Slist_tail) this->mv_back(); tmp.end(); tmp.pop_front(e);
        return true;
      }
      bool pop_end(Type &e)   { 
        if(*this->Slist_length <= 0) return false;
        reverse_Iterator tmp = *this; if(this->it == *this->Slist_head) this->mv_front(); tmp.end(); tmp.pop_back(e); 
        return true;
      }
      // Overloaded Operators
      // assigment
      void operator=(const reverse_Iterator &iter)  { this->it = iter.it; this->Slist_head = iter.Slist_head, this->Slist_tail = iter.Slist_tail; }
      // 1-node-back equality
      bool operator==(reverse_Iterator iter)  const { 
        if(this->it==nullptr) return (iter.it==nullptr) ? (*this->Slist_tail == *iter.Slist_tail) : (*this->Slist_tail == iter.it->prev);
        else                  return (iter.it==nullptr) ? (this->it->prev == *iter.Slist_tail)    : (this->it->prev == iter.it->prev); 
      }
      bool operator==(std::nullptr_t NP)      const { return (this->it==nullptr) ? (*this->Slist_tail == nullptr) : (this->it->prev == nullptr); }
      bool operator!=(reverse_Iterator iter)  const { return !operator==(iter); }
      bool operator!=(std::nullptr_t NP)      const { return (this->it==nullptr) ? (*this->Slist_tail != nullptr) : (this->it->prev != nullptr); }
      // 1-node-back reverse location comparision in list
      bool operator>(reverse_Iterator iter)   const { return (!this->This_Is_Further_Than_Iter_In_List(iter) && *this != iter); }
      bool operator<(reverse_Iterator iter)   const { return this->This_Is_Further_Than_Iter_In_List(iter); }
      bool operator>=(reverse_Iterator iter)  const { return (operator>(iter) || *this == iter); }
      bool operator<=(reverse_Iterator iter)  const { return (operator<(iter) || *this == iter); }
      // dereference reverse_iterator for 1-node-back elt
      Type &operator*() const                       { return(this->it==nullptr) ? (**this->Slist_tail).data : this->it->prev->data; } // Get Node data 
      // traverse list reverse forwards
      reverse_Iterator operator+(const long long n) { reverse_Iterator tp(*this); for(long long i = 0; i < n; ++i) tp.mv_back(); return tp; }
      void operator+=(const long long n)            { *this = *this + n; }
      reverse_Iterator &operator++()                { this->mv_back(); return *this; }                              // prefix
      reverse_Iterator operator++(int)              { reverse_Iterator tmp = *this; this->mv_back(); return tmp; }  // postfix (dummy int var)
      // traverse list reverse backwards
      reverse_Iterator operator-(const long long n) { reverse_Iterator tp(*this); for(long long i = 0; i < n; ++i) tp.mv_front(); return tp; }
      void operator-=(const long long n)            { *this = *this - n; }
      reverse_Iterator &operator--()                { this->mv_front(); return *this; }                             // prefix
      reverse_Iterator operator--(int)              { reverse_Iterator tmp = *this; this->mv_front(); return tmp; } // postfix (dummy int var)
      // I/O Friend Functions
      friend std::istream &operator>>(std::istream &is, reverse_Iterator &iter) { 
        Type e;
        if(iter != nullptr) { 
          is >> e; 
          *iter = e; 
        } else { // pushes prior head if iterator at '.rend()'
          is >> e; 
          iter.push_back(e);
        }
        return is; 
      }
      friend std::ostream &operator<<(std::ostream &os, const reverse_Iterator &iter) { os << iter.it; return os; }
    };


    // Stream Iterator Class Template To Input/Output Elts (Via Overloaded '>>' & '<<') & Traverse Slist Objects.
    class stream_Iterator : public Iterator {
    private:
      void reverse_Slist_of_stream_Iterator() {
        NODE *p = *this->Slist_head, *tmp;
        while(p != nullptr) {
          tmp = p -> prev;
          p -> prev = p -> next;
          p -> next = tmp;
          p = p -> prev;
        }
        tmp = *this->Slist_head, *this->Slist_head = *this->Slist_tail, *this->Slist_tail = tmp;
      }
    public:
      // Constructors -- allows for 'Iterator's to initialize 'stream_Iterators'
      stream_Iterator() : Iterator() {};
      stream_Iterator(NODE *n, NODE **head_node, NODE **tail_node, long *len) : Iterator(n, head_node, tail_node, len) {};
      stream_Iterator(stream_Iterator &iter) : Iterator(iter) {};
      stream_Iterator(Iterator iter)         : Iterator(iter) {};
      // Overloaded Operators
      // assignment -- allows for 'Iterator's to initialize 'stream_Iterators'
      void operator=(const stream_Iterator &iter) {this->it = iter.it; this->Slist_head = iter.Slist_head, this->Slist_tail = iter.Slist_tail;};
      void operator=(const Iterator iter) {this->it = iter.it; this->Slist_head = iter.Slist_head, this->Slist_tail = iter.Slist_tail;};
      // equality
      bool operator==(stream_Iterator iter)  const { return iter.it == this->it; }
      bool operator==(std::nullptr_t NP)     const { return this->it == nullptr; }
      bool operator!=(stream_Iterator iter)  const { return iter.it != this->it; }
      bool operator!=(std::nullptr_t NP)     const { return this->it != nullptr; }
      // location comparision in list
      bool operator>(stream_Iterator iter)   const { return this->This_Is_Further_Than_Iter_In_List(iter); }
      bool operator<(stream_Iterator iter)   const { return (!this->This_Is_Further_Than_Iter_In_List(iter) && *this != iter); }
      bool operator>=(stream_Iterator iter)  const { return (operator>(iter) || *this == iter); }
      bool operator<=(stream_Iterator iter)  const { return (operator<(iter) || *this == iter); }
      // dereference either returns current node elt, or makes new node & returns its elt (if at nullptr)
      Type &operator*() { 
        if(this->it == nullptr) { // create a new node 
          Type e;
          this->push_back(e);
          this->mv_back();
          return this->it->data;
        }
        return this->it->data; 
      }
      // traverse list forwards
      stream_Iterator operator+(const long long n) { stream_Iterator tp(*this); for(long long i = 0; i < n; ++i) tp.mv_front(); return tp; }
      stream_Iterator &operator++()                { this->mv_front(); return *this; }                            // prefix
      // "if(this->it != nullptr)" enables appending via *s_iter++ = elt;
      stream_Iterator operator++(int)              { stream_Iterator tmp = *this; if(this->it != nullptr) this->mv_front(); return tmp; } // postfix (dummy int var)
      // traverse list backwards
      stream_Iterator operator-(const long long n) { stream_Iterator tp(*this); for(long long i = 0; i < n; ++i) tp.mv_back(); return tp; }
      stream_Iterator &operator--()                { this->mv_back(); return *this; }                             // prefix
      stream_Iterator operator--(int)              { stream_Iterator tmp = *this; this->mv_back(); return tmp; }  // postfix (dummy int variable)
      // output elt in back of, or input elt from the current, stream_Iterator
      stream_Iterator &operator<<(Type e)          { this->push_back(e); return *this; }                          // output in back of iterator
      stream_Iterator &operator>>(Type &e) {                                                                      // input from "this" iterator
        if(*this->Slist_length != 0) {
          if(this->it == nullptr) this->mv_back(); 
          e = this->it->data; this->mv_front(); this->pop_back();
        } 
        return *this; 
      } 
      // Reverse A List & Move stream_Iterator Immediately To ".send()" (nullptr) Or ".sbegin()" For Output To List
      void reverse_end()                           { reverse_Slist_of_stream_Iterator(); this->end(); }
      void reverse_begin()                         { reverse_Slist_of_stream_Iterator(); this->begin(); }
    };


    // Constructors & Destructor
    Slist(const Type e);
    Slist()                            { length = 0, head = tail = nullptr; }
    Slist(const Slist &sLL)            { AllocNewList(*this, sLL); }
    ~Slist()                           { DeleteThisList(); }

    // Basic Insertion/Deletion Methods
    // peek head value
    bool front(Type &e) const          { return AssignPeekVal(e, head); }          // assign head value to arg 'e'
    Type front()        const          { return (length > 0) ? head -> data : 0; } // return head value
    // peek tail value
    bool back(Type &e)  const          { return AssignPeekVal(e, tail); }          // assign tail value to arg 'e'
    Type back()         const          { return (length > 0) ? tail -> data : 0; } // return tail value
    // add to head
    void push_front(const Type);                                                   // push_front arg
    void push_front(const Slist &sLL)  { MergeLists(sLL.tail, *this); }            // Merge/push_front 2 lists
    // add to tail
    void push_back(const Type);                                                    // push_back arg
    void push_back(const Slist &sLL)   { MergeLists(sLL.head, *this, false); }     // Merge/push_back 2 lists - 'false' to push_back
    // rmv head value
    bool pop_front(Type &e)            { if(length <= 0) return false; e = head -> data; return RmvNode(head); } // rmv & assign head value to arg 'e'
    Type pop_front()                   { Type e; pop_front(e); return e; }                                       // rmv & return head value
    // rmv tail value
    bool pop_back(Type &e)             { if(length <= 0) return false; e = tail -> data; return RmvNode(tail); } // rmv & assign tail value to arg 'e'
    Type pop_back()                    { Type e; pop_back(e); return e; }                                        // rmv & return tail value

    // Sweeping List Alterations
    // For '.slice()' & '.splice()': if iterator1 B4 iterator2: [iterator1, iterator2) else: (iterator1, iterator2]
    // splice in a list or value
    void splice(Iterator iter1, Iterator iter2, Slist &emplaced) { 
      spliceLogic(iter1, iter2, emplaced , &Slist::begin, &Slist::end, &Slist::Iterator::operator>); 
    }
    void splice(reverse_Iterator iter1, reverse_Iterator iter2, Slist &emplaced) { 
      spliceLogic(iter1,iter2,emplaced,&Slist::rbegin,&Slist::rend,&Slist::reverse_Iterator::operator<); 
    }
    template <typename iterator_type> // Iterator or reverse_Iterator
    void splice(iterator_type iter1, iterator_type iter2, const Type e) { Slist<Type> spliceInto(e); splice(iter1, iter2, spliceInto); }
    // slice out a list
    Slist slice(reverse_Iterator iter1, reverse_Iterator iter2, bool sliceFromThis = false) {
      return sliceLogic(iter1, iter2, &Slist::rbegin, &Slist::rend, sliceFromThis);
    };
    Slist slice(Iterator iter1, Iterator iter2, bool sliceFromThis = false) {
      return sliceLogic(iter1, iter2, &Slist::begin, &Slist::end, sliceFromThis);
    };
    // emplace in a list or value prior iterator
    void emplace(Iterator iter, Slist &emplaced)                                     { emplaceLogic(iter, emplaced, &Slist::begin, &Slist::end); }
    void emplace(reverse_Iterator iter, Slist &emplaced)                             { emplaceLogic(iter, emplaced, &Slist::rbegin, &Slist::rend); }
    template <typename iterator_type> void emplace(iterator_type iter, const Type e) { Slist<Type> emplaced(e); emplace(iter, emplaced); }
    
    // remove duplicate, a particular, or all elt instances in list.
    void unique();
    void remove(Type rmv) { filterList([=](auto elt){return elt != rmv;}); }
    void clear()          { DeleteThisList(); length = 0; head = tail = nullptr; }
    
    // General Iterative Functions -:- NOTE: All Use 'e' As NODE Variable In Writing 'operation's & 'condition's.
    // 'mapList' called by 'map' macro, executing 'operation' on each node
    #define map(operation)    mapList([=](auto e){return operation;})
    template <typename Function> void mapList(Function fcn) { for(auto i=begin(); i != end(); i++) *i=fcn(*i); }
    // 'filterList' called by 'filter' macro, removing nodes that don't satisfy 'condition'. 
    #define filter(condition) filterList([=](auto e){return condition;})
    template <typename Function> void filterList(Function);
    // 'funnelList' called by 'funnel' macro, 2 elts compared w/ their superior passed on to "funnel" through a value.
    #define funnel(condition) funnelList([=](auto e, auto e2){return condition;}) // 'e' elt precedes 'e2'
    template<typename Function> Type funnelList(Function);
    // 'unfoldList' called by 'unfold' macro, invokes 'operation' recursively on 'start_value' 'numElts' times, unfolds to list.
    #define unfold(...) UnfoldOverload(__VA_ARGS__, unfoldMultiElts, unfoldSetLength, unfoldVarLength)(__VA_ARGS__) // becomes 'UnfoldInstance'(__VA_ARGS__)
    template <typename Function> void unfoldList(Type, Function, long long numElts = 10);
    // 'foldList' called by 'fold' macro, where Operator passed as arg is iterated across elts, returning a value.
    #define fold(Operator)    foldList([=](auto total, auto node){return total Operator node;})
    template<typename Function> Type foldList(Function);

    // Overloaded Operators
    // invoke push_front
    Slist operator+(const Slist &sLL) const; // combine lists in operand order
    void operator+=(const Slist &sLL)  { *this = *this + sLL; }
    // invoke search & delete from head
    Slist operator-(const Slist &sLL) const;
    bool operator-=(const Slist &sLL);
    // assignment & equality operators
    Slist &operator=(const Slist &);
    bool operator==(const Slist &)    const;
    bool operator!=(const Slist &sLL) const { return !operator==(sLL); };
    // numerical map operations (any number)
    void operator+=(const Type n)      { map(e + n); }
    void operator-=(const Type n)      { map(e - n); }
    void operator*=(const Type n)      { map(e * n); }
    void operator/=(const Type n)      { map(e / n); }
    // binary map operations (no floating point)
    void operator%=(const Type n)      { map(e % n); }
    void operator&=(const Type n)      { map(e & n); }
    void operator|=(const Type n)      { map(e | n); }
    void operator^=(const Type n)      { map(e ^ n); }
    void operator<<=(const Type n)     { map(e << n); }
    void operator>>=(const Type n)     { map(e >> n); }
    
    // Numerical Analysis
    Type sum()                         { return fold(+); }
    Type max()                         { return funnel(e > e2 ? e : e2); }
    Type min()                         { return funnel(e < e2 ? e : e2); }

    // Sort/Merge/Reverse/Swap Functions
    void sort(bool ascending = true);          // ascending or descending
    void sort(std::function<bool(Type,Type)>); // given a binary predicate function/functor/lambda
    void sortAscending()                                            { sort(); };
    void sortDescending()                                           { sort(false); };
    void merge(Slist sortedList, bool ascending = true);
    void merge(Slist sortedList, std::function<bool(Type,Type)>fcn) { push_back(sortedList); sort(fcn); }
    void mergeAscending(Slist sortedList)                           { merge(sortedList); };
    void mergeDescending(Slist sortedList)                          { merge(sortedList, false); };
    void reverse();
    void swap(Slist &sLL)                                           { Slist tmp = sLL; sLL = *this; *this = tmp; }

    // Else - Find If List Has Elt, Get Size, Check If List Empty, & Show List
    bool has(Type)   const;
    long long size() const       { return length; }
    bool empty()     const       { return length == 0; }
    void show(std::string ch = " ", std::ostream &os = std::cout, bool newl = true);
    void show(char ch, std::ostream &os = std::cout);

    // Iterator Functions
    // return iterator
    Iterator begin()                      { return Iterator(head, &head, &tail, &length); }               // iterator at head
    Iterator end()                        { return Iterator(tail->next, &head, &tail, &length); }         // iterator at tail->next: 1PastTheEnd STL compatability
    Iterator iterator(Type);                                                                              // iterator at first elt instance from head
    Iterator lastIterator(Type);                                                                          // iterator at first elt instance from tail
    reverse_Iterator rbegin()             { return reverse_Iterator(tail->next, &head, &tail, &length); } // reverse iterator at tail -> next
    reverse_Iterator rend()               { return reverse_Iterator(head, &head, &tail, &length); }       // reverse iterator at next: 1PastTheEnd STL compatability
    reverse_Iterator riterator(Type);                                                                     // iterator at first elt instance from tail
    reverse_Iterator rlastIterator(Type);                                                                 // iterator at first elt instance from head
    stream_Iterator sbegin()              { return stream_Iterator(head, &head, &tail, &length); }        // stream iterator at head
    stream_Iterator send()                { return stream_Iterator(tail->next, &head, &tail, &length); }  // stream iterator at tail -> next
    stream_Iterator siterator(Type e)     { return iterator(e); }                                         // stream iterator at first elt instance from head
    stream_Iterator slastIterator(Type e) { return lastIterator(e); }                                     // stream iterator at first elt instance from tail
    // remove iterator's node from list
    bool remove(Iterator iter)            { return (iter != nullptr) ? RmvNode(iter.NodeAddress()) : false; } // also works for stream_Iterator's
    bool remove(reverse_Iterator iter)    { return (iter != nullptr) ? RmvNode(iter.NodeAddress()) : false; } 

    // I/O Buddies
    friend std::ostream &operator<<(std::ostream &os, Slist &sLL)       { sLL.show(' ', os); return os; }
    friend std::istream &operator>>(std::istream &is, Slist &sLL)       { Type e; is >> e; is.get(); sLL.push_front(e); return is; }         // Any Type
    friend std::istream &getline(std::istream &is, Slist &sLL)          { std::string s; getline(is, s); sLL.push_front(s); return is; }     // Type: std::string
    friend std::istream &getline(std::istream &is, Slist &sLL, char ch) { std::string s; getline(is, s, ch); sLL.push_front(s); return is; } // Type: std::string
  };


  // Slist Class Methods:
  // Constructors
  template <typename Type> 
  Slist<Type>::Slist(Type e) { 
    NODE *p = new NODE;
    p -> data = e;
    p -> next = p -> prev = nullptr;
    head = tail = p; 
    length = 1;
  }

  // Basic Insertion/Deletion Methods
  // add to head
  template <typename Type> 
  void Slist<Type>::push_front(const Type e) {
    NODE *p = new NODE; 
    AddNode(p, &(p)->next, &head, &(head)->prev, e); 
  }
  // add to tail
  template <typename Type> 
  void Slist<Type>::push_back(const Type e) {
    NODE *p = new NODE; 
    AddNode(p, &(p)->prev, &tail, &(tail)->next, e); 
  } 

  // Sweeping List Alterations
  // splice in a list 
  template <typename Type>
    template <typename iterator_type, typename iter_begin, typename iter_end, typename compare>
    void Slist<Type>::spliceLogic(iterator_type iter1, iterator_type iter2, Slist<Type> emplaced, iter_begin iterator_begin, iter_end iterator_end, compare cmp) {
      if(iter1 == iter2) return; // [iter1, iter1) => exclusive RHS of range supersedes inclusive LHS, thus no splice (emplace instead)
      if((iter1.*cmp)(iter2)) {
        emplaced.reverse(); // splice in reverse
        iterator_type tmp = iter1;
        iter1 = iter2, iter2 = tmp;
      }
      if((iter1 == (this->*iterator_end)() || iter2 == (this->*iterator_end)()) && (iter1 == (this->*iterator_begin)() || iter2 == (this->*iterator_begin)())) {
        *this = emplaced; // splice entire list
      } else {            // splice portion of list
        emplace(iter1, emplaced); 
        slice(iter1, iter2, true); 
      }
    }
  // slice out list
  template <typename Type>
    template <typename iterator_type, typename iter_begin, typename iter_end>
    Slist<Type> Slist<Type>::sliceLogic(iterator_type iter1, iterator_type iter2, iter_begin iterator_begin, iter_end iterator_end, bool sliceFromThis) { 
      Slist<Type> sliced;
      bool reverse = false;
      if(iter1.operator>(iter2)) { // whether slicing in reverse
        iterator_type tmp = iter1;
        iter1 = iter2, iter2 = tmp;
        reverse = true;
      }
      iterator_type scout = (this->*iterator_begin)();
      while(scout != (this->*iterator_end)()) {
        if(scout == iter1) // in slice range
          while(scout != (this->*iterator_end)() && scout != iter2) {
            (reverse) ? sliced.push_front(*scout) : sliced.push_back(*scout);
            if(sliceFromThis) remove(scout);
            ++scout;
          }
        if(scout != (this->*iterator_end)()) ++scout; // out slice range
      }
      return sliced;
    }
  // emplace in a list prior iterator
  template <typename Type> // emplace a list
    template <typename iterator_type, typename iter_begin, typename iter_end>
    void Slist<Type>::emplaceLogic(iterator_type iter, Slist<Type> &emplaced, iter_begin iterator_begin, iter_end iterator_end) {
      iterator_type scout1 = (this->*iterator_begin)();
      while(scout1 != (this->*iterator_end)()) {
        if(scout1 == iter) {
          iterator_type scout2 = (emplaced.*iterator_begin)();
          for(int i = 0; i++ < emplaced.length; ++scout2) scout1.push_back(*scout2);
          for(int i = 0; scout1 != (this->*iterator_end)() && i < emplaced.length; ++i) ++scout1;
        }
        if(scout1 != (this->*iterator_end)()) ++scout1;
      }
      if(scout1 == iter) {
        iterator_type scout2 = (emplaced.*iterator_begin)();
        for(int i = 0; i++ < emplaced.length; ++scout2) scout1.push_back(*scout2);
        for(int i = 0; scout1 != (this->*iterator_end)() && i < emplaced.length; ++i) ++scout1;
      }
    }
  // remove all duplicate elts in list
  template <typename Type>
  void Slist<Type>::unique() {
    NODE *p = head, *q, *tmp;
    while(p != nullptr) {
      q = p -> next;
      while(q != nullptr) {
        tmp = q -> next;
        if(p -> data == q -> data) RmvNode(q);
        q = tmp;
      }
      p = p -> next;
    }
  }

  // General Iterative Function
  // only keep nodes satisfying fcn's condition
  template <typename Type>
    template <typename Function> 
    void Slist<Type>::filterList(Function fcn) {
      Slist<Type> filterListed;
      for(auto i=begin(); i!=end(); i++) if(fcn(*i)) filterListed.push_back(*i);
      *this = filterListed;
    }
  // called by 'funnel' macro, 2 elts compared w/ their superior passed on to "funnel" through a value.
  template <typename Type>
    template<typename Function> 
    Type Slist<Type>::funnelList(Function fcn) { // 'e' elt precedes 'e2'
      Type funneledValue = *begin();
      for(auto i=begin()+1; i!=end(); i++) funneledValue = fcn(funneledValue, *i);
      return funneledValue;
    }
  // Simulate macro overloading for 'unfold'. 2 'unfold' args makes 'unfoldSetLength' = 'arg3' & 'unfoldVarLength' = 'UnfoldInstance',
  // 3 'unfold' args makes 'unfoldSetLength' = 'UnfoldInstance' & passes 'unfoldVarLength' to the variadic '...' argument.
  #define UnfoldOverload(arg1, arg2, arg3, arg4, UnfoldInstance, ...) UnfoldInstance 
  #define unfoldVarLength(start_value, operation) SUPERLIST::unfoldList(start_value, [](auto e) {return operation;}, 10)
  #define unfoldSetLength(start_value, operation, numElts) SUPERLIST::unfoldList(start_value, [](auto e) {return operation;}, numElts)
  #define unfoldMultiElts(first_value,second_value,operation,numElts) SUPERLIST::unfoldList(first_value,second_value,[](auto e,auto e2){return operation;},numElts)
  // operate fcn recursively on 'e' numElts times.
  template <typename Type>
    template <typename Function> // changes current list
    void Slist<Type>::unfoldList(Type e, Function fcn, long long numElts) { 
      Slist<Type> newThis(e);
      for(int i = 1; i < numElts; ++i) newThis.push_back(fcn(newThis.back()));
      *this = newThis;
    }
  template <typename Type, typename Function> // used in intialization - returns a new object
  Slist<Type> unfoldList(Type e, Function fcn, long long numElts = 10) {
    Slist<Type> newThis(e);
    for(int i = 1; i < numElts; ++i) newThis.push_back(fcn(newThis.back()));
    return newThis;
  }
  // Takes 2 list elts for recursion - enables lucas numbers, fibonacci, etc.
  template <typename Type, typename Function> 
  Slist<Type> unfoldList(Type e, Type e2, Function fcn, long long numElts) { // 'e' elt precedes 'e2'
    Slist<Type> newThis(e);
    newThis.push_back(e2);
    for(int i = 2; i < numElts; ++i) newThis.push_back(fcn(*(newThis.end() - 2), newThis.back()));
    return newThis;
  }
  // called by 'fold' macro, where an operator passed as an argument is iterated across list elts, returning a value.
  template <typename Type>
    template<typename Function>
    Type Slist<Type>::foldList(Function fcn) { 
      long double total = *begin(); 
      for(auto i = begin() + 1; i != end(); i++) total = fcn(total, *i);
      return (Type)total;
    }

  // Overloaded Operators
  // invoke push_front
  template <typename Type> // combine lists in operand order
  Slist<Type> Slist<Type>::operator+(const Slist<Type> &sLL) const { 
    Slist<Type> sum = sLL; 
    sum.push_front(*this);
    return sum;
  }
  // invoke search & delete from head
  template <typename Type> 
  Slist<Type> Slist<Type>::operator-(const Slist<Type> &sLL) const {
    Slist<Type> difference = *this;
    difference -= sLL;
    return difference;
  }
  template <typename Type>
  bool Slist<Type>::operator-=(const Slist<Type> &sLL) {
    NODE *p = sLL.head, *q;
    const long long originalLength = length;
    while(p != nullptr && length > 0) {
      q = head; 
      while(q != nullptr && q -> data != p -> data) q = q -> next; 
      if(q != nullptr) RmvNode(q);
      p = p -> next;
    }
    return !(originalLength == length);
  }
  // assignment & equality operators
  template <typename Type>
  Slist<Type> &Slist<Type>::operator=(const Slist<Type> &sLL) {
    if(this == &sLL) return *this;
    DeleteThisList();
    AllocNewList(*this, sLL);
    return *this;
  }
  template <typename Type>
  bool Slist<Type>::operator==(const Slist<Type> &sLL) const {
    if(this == &sLL) return true;
    NODE *p = head, *q = sLL.head;
    while(p != nullptr && q != nullptr && p -> data == q -> data) p = p -> next, q = q -> next;
    return (p == q && p == nullptr);
  }

  // Sort & Merge Methods
  // sort ascending or descending
  template <typename Type>
  void Slist<Type>::sort(bool ascending) {
    if(ascending) sort([](auto x, auto y){return x < y;});
    else          sort([](auto x, auto y){return x > y;});
  }
  // sort given a binary predicate function/functor/lambda
  template <typename Type> // O(n^2)
  void Slist<Type>::sort(std::function<bool(Type,Type)>fcn) {
    NODE *wall = head, *min, *scout;
    Slist<Type> SortedList;
    while(wall != nullptr) {
      min = wall;
      scout = wall -> next;
      while(scout != nullptr) {
        if(fcn(scout->data, min->data)) min = scout;
        scout = scout -> next;
      }
      SortedList.push_back(min -> data);
      RmvNode(min);
      wall = head;
    }
    *this = SortedList;
  }
  // merge ascending or descending
  template <typename Type>
  void Slist<Type>::merge(Slist<Type> sortedList, bool ascending) {
    if(ascending) merge(sortedList, [](auto x, auto y){return x < y;});
    else          merge(sortedList, [](auto x, auto y){return x > y;});
  }
  // reverse a list preserving iterator positions
  template <typename Type>
  void Slist<Type>::reverse() { 
    NODE *p = head, *tmp;
    while(p != nullptr) {
      tmp = p -> prev;
      p -> prev = p -> next;
      p -> next = tmp;
      p = p -> prev;
    }
    tmp = head, head = tail, tail = tmp;
  }

  // Else - Show List, Find If List Has Elt
  template <typename Type>
  void Slist<Type>::show(std::string divider, std::ostream &os, bool newl) {
    NODE *p = head;
    while(p != nullptr) {
      if(p -> next == nullptr) divider = "";
      os << p -> data << divider;
      p = p -> next;
    }
    if(newl) os << '\n'; // if .show() was execute as method, not by '<<' operator overload fcn.
  }
  template <typename Type> 
  void Slist<Type>::show(char ch, std::ostream &os) { 
    char str[2] = {ch,'\0'}; 
    show(str, os, false); 
  };
  template <typename Type> 
  bool Slist<Type>::has(Type e) const { 
    NODE *p = head; 
    while(p != nullptr && p -> data != e) p = p->next; 
    return (p != nullptr); 
  }

  // Iterator Functions
  template <typename Type>
  typename Slist<Type>::Iterator Slist<Type>::iterator(Type e) {
    NODE *p = head;
    while(p != nullptr && p -> data != e) p = p -> next;
    return Iterator(p, &head, &tail, &length);
  }
  template <typename Type>
  typename Slist<Type>::Iterator Slist<Type>::lastIterator(Type e) {
    NODE *p = tail;
    while(p != nullptr && p -> data != e) p = p -> prev;
    return Iterator(p, &head, &tail, &length);
  }
  template <typename Type>
  typename Slist<Type>::reverse_Iterator Slist<Type>::riterator(Type e) {
    NODE *p = head;
    while(p != nullptr && p -> data != e) p = p -> next;
    return reverse_Iterator(p -> next, &head, &tail, &length);
  }
  template <typename Type>
  typename Slist<Type>::reverse_Iterator Slist<Type>::rlastIterator(Type e) {
    NODE *p = tail;
    while(p != nullptr && p -> data != e) p = p -> prev;
    return reverse_Iterator(p -> next, &head, &tail, &length);
  }

  // Private:
  template <typename Type> 
  void Slist<Type>::AllocNewList(Slist<Type> &NewList, const Slist<Type> &OldList) {
    NewList.length = OldList.length;
    if(length == 0) {
      NewList.head = NewList.tail = nullptr;
    } else {
      NODE *root = OldList.head;
      for(int i = 0; i < length; ++i) {
        NODE *p = new NODE;
        p -> data = root -> data;
        p -> next = nullptr;
        if(i == 0) {
          NewList.head = p;
          p -> prev = nullptr;
        } else {
          NewList.tail -> next = p;
          p -> prev = NewList.tail;
        }
        NewList.tail = p;
        root = root -> next;
      }
    }
  }
  template <typename Type>
  void Slist<Type>::DeleteThisList() {
    NODE *p = head, *tmp; 
    while(p != nullptr) { 
      tmp = p->next; 
      delete p; 
      p = tmp; 
    }
  }
  template <typename Type> 
  void Slist<Type>::AddNode(NODE *p, NODE **pConnector, NODE **root, NODE **rootConnector, const Type e) {
    p -> data = e;
    p -> next = p -> prev = nullptr;
    if(head == nullptr) head = tail = p;
    else *pConnector = *root, *rootConnector = p, *root = p;
    ++length;
  }
  template <typename Type> 
  bool Slist<Type>::RmvNode(NODE *delNode) {
    if(delNode == head) {
      head = head -> next; 
      if(head != nullptr) head -> prev = nullptr;
    } else if(delNode == tail) {
      tail = tail -> prev; 
      if(tail != nullptr) tail -> next = nullptr;
    } else {
      NODE *p = head -> next;
      while(p != nullptr && p != delNode) p = p -> next;
      if(p == nullptr) return false;
      delNode -> prev -> next = delNode -> next;
      delNode -> next -> prev = delNode -> prev;
    }
    delete delNode; 
    --length;
    return true;
  }
  template <typename Type>
  void Slist<Type>::MergeLists(NODE *p, Slist<Type> &sum, bool push_fronting) const {
    while(p != nullptr) {
      sum.push_front(p -> data);
      p = (push_fronting) ? p -> prev : p -> next; // 'push_front' or 'push_back' merge
    }
  }
  template <typename Type> 
  bool Slist<Type>::AssignPeekVal(Type &e, NODE *p) const {
    if(length > 0) e = p -> data;
    return (length > 0);
  }
}

#endif
