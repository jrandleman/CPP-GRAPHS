// Author: Jordan Randleman -:- slist_SampleExec.cpp -:- Sample Linked List Class Template Implemenation -:- Compile >= C++17.

#include <iostream>
#include <string>
#include <algorithm>
#include "slist.hpp"

/***
 * Slist was designed to improve upon STL's list library with methods
 * from <valarray> added, support for linked-list specific mapping functions, 
 * and more versatile iterator capabilities. 
 * 
 * Notes: (1) Avoid C-Style strings, use <string> class instead.
 *        (2) Use 'e' as variable in writing .map(), .filter(), .unfold().
 *            Use 'e' & 'e2' ('e' precedes 'e2') for .unfold() overload & .funnel().
 *
 * Basic Iterator Properties (more below):
 * (1) Iterator: list traversal, alter existing nodes via '*' (NOT "end()"
 *               though) & make new/rmv nodes via 'push'/'pop' variants
 * (2) reverse_Iterator: same as "Iterator", BUT: traverses in reverse
 *                       & dereferencing gives value of prior node
 * (3) stream_Iterator: same as "Iterator", BUT '*' can ALSO create new
 *                      nodes if dereferenced at "end()", and can quickly
 *                      reverse a list & jump to either end for rapid I/O
 */

int main() {
  using std::cout; using std::cin; using std::endl; using std::string;
  
  using SUPERLIST::Slist; // The 'Slist' Class

  const char *line_break = "\n--------------------------------------------------------------------------------\n";

  /******************************************************************************
  * BASIC INSERTION AND REMOVAL: PUSH, POP, & FRONT/BACK METHODS
  ******************************************************************************/

  // push_back(), push_front(), pop_back(), pop_front(), back(), front()

  // NOTE: Slist Objects can be ctor'd w/o args, w/ an elt, or w/ another list

  cout << line_break << "(1) BASIC INSERTION AND REMOVAL: PUSH, POP, & FRONT/BACK METHODS:\n";
  Slist<int> list1;
  for(int i = 0; i < 10; ++i) // push back even numbers & push front odds
    ((i & 1) == 0) ? list1.push_back(i) : list1.push_front(i);

  // overloaded '<<' outputs list contents seperated by " "
  cout << "list1:                      " << list1 << endl; 
  int a1 = list1.front(); // see (but don't rmv) head value
  int a2 = list1.back();  // see (but don't rmv) tail value
  cout << "Peeked front & back Values: " << a1 << " & " << a2 << endl;

  a1 = list1.pop_front(), a2 = list1.pop_back(); // pop head & tail values
  cout << "Popped front & back Values: " << a1 << " & " << a2 << endl;
  cout << "list1:                      " << list1 << endl << line_break;

  /******************************************************************************
  * CHECKING FOR AN ELT, GETTING # OF ELTS, CHECKING IF LIST EMPTY, SHOWING LIST
  ******************************************************************************/

  // has(), size(), empty(), show()

  cout << "(2) CHECKING FOR AN ELT, GETTING # OF ELTS, CHECKING IF LIST EMPTY, SHOWING LIST:\n";
  cout << "list1 has 4 & 22: " << std::boolalpha << list1.has(4) 
       << " & " << list1.has(22) << endl;
  cout << "list1 size:       " << list1.size() << endl;
  cout << "list1 is empty:   " << std::boolalpha << list1.empty() << endl;

  cout << "Using Slist's \".show()\" Methods:\n";
  cout << "\t";
  list1.show();       // display elts seperated by a " "
  cout << "\t";
  list1.show('-');    // display elts seperated by the character argument
  cout << "\n\t";
  list1.show(" -> "); // display elts seperated by the std::string argument
  cout << line_break;

  /******************************************************************************
  * REMOVING DUPLICATE ELTS, INSTANCES OF A PARTICULAR ELT, & ALL ELTS
  ******************************************************************************/

  // remove(), unique(), clear()

  cout << "(3) REMOVING DUPLICATE ELTS, INSTANCES OF A PARTICULAR ELT, & ALL ELTS:\n";
  for(int i = 5; i < 15; ++i) list1.push_back(i);
  cout << "list1 with duplicate elts:    " << list1 << endl;

  list1.remove(5);
  list1.remove(6);
  cout << "list1 with \"5\" & \"6\" removed: " << list1 << endl;

  list1.unique();
  cout << "list1 w/o any duplicate elts: " << list1 << endl;
  list1.clear();
  cout << "list1 cleared of all elts:    " << list1 << endl << line_break;

  /******************************************************************************
  * UNFOLDING, MAPPING, FILTERING, FUNNELING, & FOLDING METHODS
  ******************************************************************************/

  // unfold(), map(), filter(), funnel(), fold()

  cout << "(4) UNFOLDING, MAPPING, FILTERING, FUNNELING, & FOLDING METHODS:\n";
  // unfold (unary) list from 1, adding 1 to the prior elt, 20 times
  list1 = unfold(1, e + 1, 20);
  cout << "Unfolded list1:           " << list1 << endl;

  // unfold (binary) list from 2 & 1, adding the 2 prior elts together, 20 times
  Slist<int>lucasNumbers = unfold(2, 1, e + e2, 20); // 'e' precedes 'e2'
  cout << "Lucas Numbers:            " << lucasNumbers << endl;

  // map (unary) '* 2' operation across each elt in linked list
  list1.map(e * 2);
  cout << "Mapped list1 * 2:         " << list1 << endl;

  // filter (unary predicate) out elts not satifying below condition
  list1.filter(e % 3 != 0 && e <= 20);
  cout << "Filtered list1:           " << list1 << " (No Multiples of 3 & <= 20)\n";

  // push_back 9, 6, & 3
  for(int i = 9; i > 0; i -= 3)
    list1.push_back(i);
  cout << "push_back() 9, 6, & 3:    " << list1 << endl;

  // funnel (binary predicate) out a value by comparing every 2 elts & passing on the superior
  int funneledValue = list1.funnel(e > e2 ? e : e2); // 'e' precedes 'e2'
  cout << "Funneled list1 Max Value: " << funneledValue << endl;

  // 'fold' (1 operator) each elt on top of each other, combining their values w/ the operator argument
  cout << "list1 fold(+) & fold(*):  " << list1.fold(+) << " & " 
       << list1.fold(*) << endl << line_break;

  /******************************************************************************
  * SPLICE, SLICE, & EMPLACE METHODS
  ******************************************************************************/

  // splice() [iter1, iter2), slice() [iter1, iter2), & emplace() (prior iter)

  // Previews: begin(), end(), rbegin(), rend() => See Below For More On Iterators
  // NOTE: splice() & emplace() can also take elts as args instead of entire lists

  cout << "(5) SPLICE, SLICE, & EMPLACE METHODS:\n";
  Slist<int> list2;
  // see more on "begin" & "end" iterators below!
  list2 = list1.slice(list1.begin() + 1, list1.end() - 1); 
  cout << "list2 slice of list1 w/o head & tail:    " << list2 << endl;

  // see more on "rbegin" & "rend" reverse iterators below!
  list2 = list2.slice(list2.rbegin(), list2.rend());
  cout << "list2 reverse slice of itself:           " << list2 << endl;

  list1.splice(list1.begin() + 1, list1.end() - 1, list2);
  cout << "list1 w/ list2 spliced btwn head & tail: " << list1 << endl;

  list2.emplace(list2.begin(), list1);
  cout << "list2 w/ list1 emplaced prior head:      " << list2 << endl << line_break;

  /******************************************************************************
  * OVERLOADED ARITHMETIC OPERATOR MAPPING, SUM, MAX, & MIN
  ******************************************************************************/

  // sum() max(), min(), MAPPING: +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=

  cout << "(6) OVERLOADED ARITHMETIC OPERATOR MAPPING, SUM, MAX, & MIN:\n"
       << "\t=> ALL NUMBERS: +=, -=, *=, /=\n"
       << "\t=> NON-FLOATS:  %=, &=, |=, ^=, <<=, >>=\n";
  list2 /= 2;
  list2 *= 3;
  cout << "list2 /= 2 *= 3: " << list2 << endl;
  cout << "list2 sum: " << list2.sum() << endl;
  cout << "list2 max: " << list2.max() << endl;
  cout << "list2 min: " << list2.min() << endl << line_break;

  /******************************************************************************
  * OVERLOADED ARITHMETIC LIST CONCATENATION, REMOVAL, & EQUALITY OPERATORS
  ******************************************************************************/

  // JOINING: +, +=, -, -= , =, ==, !=

  cout << "(7) OVERLOADED ARITHMETIC LIST CONCATENATION, REMOVAL, & EQUALITY OPERATORS:\n";
  // "list1 + list2" creates a list, which is then assigned to list3
  Slist<int> list3 = list1 + list2; 
  cout << "list3 = list1 + list2: " << list3 << endl;
  list3 -= list1;
  cout << "list3 -= list1: " << list3 << " (list3 w/ all list1 elts removed)\n";
  cout << "list3 == list1: " << std::boolalpha << (list3 == list1) << " (if list1 & list3 have the same elts)\n";
  cout << "list3 != list1: " << std::boolalpha << (list3 != list1) << endl << line_break;

  /******************************************************************************
  * SORT, MERGE, REVERSE, & SWAP
  ******************************************************************************/

  // sort(), sortAscending(), sortDescending(), merge(), mergeAscending(), 
  // mergeDescending(), reverse(), swap()

  cout << "(8) SORT, MERGE, REVERSE, & SWAP:\n";
  list1.sortDescending();
  cout << "list1 sorted in descending order:  " << list1 << endl;

  list1.sort(); // default 'ascending' boolean argument defaults to true 
  cout << "list1 sorted in ascending order:   " << list1 << endl;

  // overloaded "sort()" can also take a binary predicate argument
  list2.unique();
  list2.sort([](auto a1, auto a2){return a1 < a2;}); 
  cout << "list2.unique() & sorted ascending: " << list2 << endl;

  // merge() takes a binary predicate to merge the lists w/
  list2.merge(list1, [](auto a1, auto a2){return a1 < a2;});
  cout << "list2 & list1 merged in ascending: " << list2 << endl << endl;

  list2.reverse();
  cout << "list2 reversed:  " << list2 << endl;
  cout << "list2 pre-swap:  " << list2 << endl
       << "list1 pre-swap:  " << list1 << endl;
  list2.swap(list1);
  cout << "list2 post-swap: " << list2 << endl
       << "list1 post-swap: " << list1 << endl << line_break;

  /******************************************************************************
  * OVERLOADED 'OSTREAM <<', 'ISTREAM >>', & GETLINE
  ******************************************************************************/

  // '<<', '>>', getline()

  cout <<  "(9) OVERLOADED 'OSTREAM <<', 'ISTREAM >>', & GETLINE:\n";
  Slist<string> stringList;
  cout << "Enter 3 words (pressing <enter> between each) to be added to the \"stringList\" object:\n";
  cout << ">> ";
  cin >> stringList;
  cout << ">> ";
  cin >> stringList;
  cout << ">> ";
  cin >> stringList;
  cout << "stringList, output via '<<': " << stringList << endl << endl;

  cout << "Enter a line of words to be retrieved via getline():\n";
  cout << ">> ";
  getline(cin, stringList);
  cout << "stringList, output via '<<': " << stringList << endl << endl;

  cout << "Enter words to be retrieved via getline(), with '@' as the delimiting char:\n";
  cout << ">> ";
  getline(cin, stringList, '@');
  cout << "stringList, output via '<<': " << stringList << endl << line_break;

  /******************************************************************************
  * ITERATOR METHODS: BEGIN, END, ITERATOR, LASTITERATOR, REMOVE, REVERSE/STREAM
  ******************************************************************************/

  // "Iterator":         begin(),  end(),  iterator(),  lastIterator()
  // "reverse_Iterator": rbegin(), rend(), riterator(), rlastIterator()
  // "stream_Iterator":  sbegin(), send(), siterator(), slastIterator()
  // ALL: remove()

  /* 
   * NOTE: get iterator value via '*' dereferencing operator
   * NOTE: .end() = 1-past-the-end value for compatability w/ STL algorithms, as does .rbegin()
   *       (though dereferencing reverse_Iterator's gives the value of the node before them)
   * NOTE: stream_Iterator-related methods = identical to Iterator methods & prefixed w/ 's'
   * NOTE: 'iterator', 'riterator', & 'siterator' provide identical functionality (as do 
   *       'lastIterator', 'rlastIterator', & 'slastIterator') for the 3 different iterators
   */     

  cout << "(10) ITERATOR METHODS: BEGIN, END, ITERATOR, LASTITERATOR, REMOVE, REVERSE/STREAM:\n";
  Slist<int>::Iterator it1 = list1.begin(); // regular iterator
  auto it2 = list1.end() - 1; // last node -- .end() returns 1-past-the-end value

  Slist<int>::reverse_Iterator r_it1 = list1.rbegin(); // reverse iterator
  auto r_it2 = list1.rend() - 1;
  cout << "list1 begin(), end()-1, rbegin(), rend()-1: " << *it1 << ", " << *it2 << ", " << *r_it1 << ", " << *r_it2 << endl;
  
  it1 = list1.iterator(14); // returns an iterator at the first instance of 14 from begin()
  it2 = list1.lastIterator(6); // returns an iterator at the last instance of 6 from tail()

  cout << "list1 Iterators at \"14\" & \"6\":              " << *it1 << ", " << *it2 << endl;
  list1.remove(it1);
  list1.remove(it2);
  cout << "list1 w/ Iterators at 14 & 6 removed:       " << list1 << endl << line_break;

  /******************************************************************************
  * WORKING WITH ITERATORS
  ******************************************************************************/

  // push_front(), push_back(), push_begin(), push_end(), pop_front(), pop_back(), 
  // pop_begin(), pop_end(), begin(), end()
  // +, -, +=, -=, ++, --, *, =, ==, !=, >, <, <=, >=, ostream <<

  /*
   * NOTE: push_begin() & push_end() do NOT move the Iterator itself, only creating
   *       a new node & pushing a it w/ the given value to either end.
   * COMPARISON: ">, <, <=, >=" operators test for which iterator = closer to iterator's ".begin()"
   * EQUALITY:   '==' operators compare node address' of iterators
   * OSTREAM:    'ostream <<' ouputs the iterator's node address
   * ISTREAM:    'istream >>' assigns input to iterator value, or pushes a new node
   *              after "tail" if at ".end()" & prior "head" if at ".rend()"
   */

  cout << "(11) WORKING WITH ITERATORS:\n";
  cout << "\t=> NOTE: \".begin()\" = \"head\" & \".end()\" = \"tail + 1\"\n";
  cout << "Outputting list1 elts via an Iterator:                  ";
  for(auto it = list1.begin(); it != list1.end();)
    cout << *it++ << " ";
  cout << endl;

  it1 = list1.begin();
  *it1 = 888;
  cout << "list1 w/ head changed to \"888\" via an Iterator:         " << list1 << endl;

  it1.push_front(999); // push a value as a new node in front of the Iterator
  it1.push_back(777);  // push a value as a new node in back of the Iterator
  cout << "Via Iterator, pushed \"999\" in front & \"777\" in back:    " 
       << list1 << endl;

  it1 += 5;            // move Iterator 5 positions up
  it1.push_begin(111); // push a value as a new node at the beginning of the list
  it1.push_end(222);   // push a value as a new node at the end of the list

  cout << "list1 w/ \"111\" pushed to head & \"222\" pushed to tail:   " << list1 << endl;
  cout << "Iterator value moved 5 past its position at \"888\":      " << *it1 << endl;

  it1.begin();
  cout << "Iterator moved to head via \".begin()\":                  " << *it1 << endl;
  it1.end();
  --it1;
  cout << "Iterator moved to tail via \".end()\" then \"- 1\":         " << *it1 << endl;

  it1.pop_back();
  it1.pop_front();
  it1.pop_begin();
  it1.pop_end();
  cout << "list1 popped head, tail, & values adjacent to iterator: " << list1 <<endl << endl;

  cout << "list1.begin() < list1.end():                 " << std::boolalpha << (list1.begin() < list1.end()) << endl; 
  cout << "list1.begin() > list1.end():                 " << std::boolalpha << (list1.begin() > list1.end()) << endl; 
  cout << "list1.begin() + list1.size() == list1.end(): " << std::boolalpha << (list1.begin() + list1.size() == list1.end()) << endl;
  cout << "list1.begin() & list1.end() - list1.size():  " << list1.begin() << " & " << (list1.end() - list1.size()) << endl << endl;

  it1.begin();
  cout << "Enter a # to change list1's  head:\n>> ";
  cin >> it1;
  cout << "list1 with your changed head input:  " << list1 << endl;
  it1.end();
  cout << "Enter a # to append to list1's tail:\n>> ";
  cin >> it1;
  cout << "list1 with your appended tail input: " << list1 << endl << line_break;

  /******************************************************************************
  * WORKING WITH REVERSE ITERATORS
  ******************************************************************************/

  // Same methods as with regular "Iterator"'s

  // NOTE: reverse_Iterator's increment backwards, decrement forwards, & dereference 1 node prior
  // NOTE: methods w/ "front", "back", "begin", & "end" = opposite in reverse

  cout << "(12) WORKING WITH REVERSE ITERATORS:\n";
  cout << "\t=> As w/ STL, increment backwards, decrement forwards, & dereference 1 node prior\n";
  cout << "\t=> NOTE: methods w/ \"front\", \"back\", \"begin\", & \"end\" = opposite in reverse\n";
  cout << "\t=> NOTE: \".rbegin()\" = \"tail + 1\" & \".rend()\" = \"head\"\n";

  cout << "Outputting list1 elts via a reverse_Iterator:                  ";
  for(auto it = list1.rbegin(); it != list1.rend();)
    cout << *it++ << " ";
  cout << endl;

  r_it1 = list1.rbegin();
  *r_it1 = 333;
  cout << "list1 w/ .rbegin() tail changed to \"333\" via reverse_Iterator: " << list1 << endl;

  r_it1.push_front(444); // push a value as a new node in reverse front of the Iterator
  r_it1.push_back(555);  // push a value as a new node in reverse back of the Iterator
  cout << "Via reverse_Iterator, pushed \"444\" in front & \"555\" in back:   " 
       << list1 << endl;

  r_it1 += 5;            // move Iterator 5 positions reverse up
  r_it1.push_begin(111); // push a value as a new node at the reverse beginning of the list
  r_it1.push_end(222);   // push a value as a new node at the reverse end of the list

  cout << "list1 w/ \"111\" pushed to tail & \"222\" pushed to head:          " << list1 << endl;
  cout << "reverse_Iterator value moved 5 past its position at \"333\":     " << *r_it1 << endl;

  r_it1.begin();
  cout << "Iterator moved to tail via \".begin()\":                         " << *r_it1 << endl;
  r_it1.end();
  --r_it1;
  cout << "Iterator moved to head via \".end()\" then \"- 1\":                " << *r_it1 << endl;

  r_it1.pop_back();
  r_it1.pop_front();
  r_it1.pop_begin();
  r_it1.pop_end();
  cout << "list1 w/ popped head, tail, & values adjacent to the iterator: " << list1 << endl << endl;

  cout << "list1.rbegin() < list1.rend():                  " << std::boolalpha << (list1.rbegin() < list1.rend()) << endl; 
  cout << "list1.rbegin() > list1.rend():                  " << std::boolalpha << (list1.rbegin() > list1.rend()) << endl; 
  cout << "list1.rbegin() + list1.rsize() == list1.rend(): " << std::boolalpha << (list1.rbegin() + list1.size() == list1.rend()) << endl;
  cout << "list1.rbegin() + list1.size() & list1.rend():   " << (list1.rbegin() + list1.size()) << " & " << list1.rend() << endl << endl;

  r_it1.begin();
  cout << "Enter a # to change list1's tail:\n>> ";
  cin >> r_it1;
  cout << "list1 with your changed tail input:  " << list1 << endl;
  r_it1.end();
  cout << "Enter a # to append to list1's head:\n>> ";
  cin >> r_it1;
  cout << "list1 with your appended head input: " << list1 << endl << line_break;

  /******************************************************************************
  * WORKING WITH STREAM ITERATORS
  ******************************************************************************/

  // reverse_end(), reverse_begin(), '>>', '<<', '*', & same methods as regular "Iterator"'s

  // NOTE: whereas dereferencing an iterator at end() or rend() throws an error
  //       (can't deref. nullptr) doing so with a stream_Iterator makes a new node
  // NOTE: reverse_end() & reverse_begin() reverses the entire list & moves the
  //       stream_Iterator to either newly designated end (for rapid input/output)

  cout << "(13) WORKING WITH STREAM ITERATORS:\n";
  Slist<int>::stream_Iterator s_it1 = list2.send();
  // unlike other iterators, dereferencing at "end()" creates a new node and
  // returns a reference to its value output new data to the list
  for(int i = 22; i < 32; i += 2)
    *s_it1++ = i;
  cout << "Outputted evens from 22-30 to list2 via stream_Iterator: " << list2 << endl;


  // inputting data to variables via overloaded '>>'
  int a3;
  cout << "Inputing data from list2 to a variable via stream_Iterator & overloaded '>>' operator:"
       << "\n\tNumbers popped from tail:            ";
  for(int i = 0; i < 5; ++i) {
    s_it1 >> a3;
    cout << a3 << " ";
  }
  cout << "\n\tlist2 after removing data from tail: " << list2 << endl;
  s_it1.begin();
  cout << "\tNumbers input/popped from head:      ";
  for(int i = 0; i < 5; ++i) {
    s_it1 >> a3;
    cout << a3 << " ";
  }
  cout << "\n\tlist2 after removing data from head: " << list2 << endl;


  // outputting data to variables via overloaded '<<'
  cout << "Outputting data to list2 via stream_Iterator & overloaded '<<':\n";
  for(int i = 40; i < 68; i += 4)
    s_it1 << i;
  cout << "\tlist2 prepended multiples of 4 from 40-64: " << list2 << endl << endl;


  // reverse_end() & reverse_begin() enable rapid re-positioning of stream iterators
  // to input/output data from either list end
  cout << "Using \"reverse_end()\" & \"reverse_begin()\" for quick input/output to either end:\n";
  s_it1.reverse_end();
  for(int i = 801; i < 806; ++i)
    *s_it1++ = i;
  cout << "\tlist2 reversed w/ 801-805 output to end(): " << list2 << endl;

  s_it1.reverse_end();
  for(int i = 806; i < 811; ++i)
    *s_it1++ = i;
  cout << "\tlist2 reversed w/ 806-810 output to end(): " << list2 << endl;

  cout << "\treversing list2 & inputting data to a variable from begin(): ";
  s_it1.reverse_begin();
  for(int i = 0; i < 5; ++i) {
    s_it1 >> a3;
    cout << a3 << " ";
  }
  cout << "\n\t\tlist2 after inputting 3 elts from head:              " << list2 << endl;

  cout << "\treversing list2 & inputting data to a variable from begin(): ";
  s_it1.reverse_begin();
  for(int i = 0; i < 5; ++i) {
    s_it1 >> a3;
    cout << a3 << " ";
  }
  cout << "\n\t\tlist2 after inputting 5 more elts from head:         " << list2 << endl << line_break;

  /******************************************************************************
  * USING STL ALGORITHMS WITH SLIST ITERATORS
  ******************************************************************************/

  cout << "(14) USING STL ALGORITHMS WITH SLIST ITERATORS:\n";

  // std::foreach()
  cout << "Outputting list1 via std::for_each():           ";
  std::for_each(list1.begin(),list1.end(), [](auto x){cout << " " << x;});
  cout << endl;
  cout << "Outputting list1 in reverse via std::for_each():";
  std::for_each(list1.rbegin(),list1.rend(), [](auto x){cout << " " << x;});
  cout << endl << endl;

  // std::copy()
  cout << "Copying list1 to the new list4 object via std::copy() & a stream_Iterator:\n"
       << "\tNOTE: Given a stream_Iterator's unique ability to create a new node when\n"
       << "\t      dereferencing at a \"nullptr\", it can be used to create new lists too\n";
  Slist<int> list4;
  std::copy(list1.begin(), list1.end(), list4.sbegin());
  cout << "list1:                    " << list1 <<  "\nlist4 copy of list1:      " << list4 << endl;
  std::copy(list1.rbegin(), list1.rend(), list4.begin());
  cout << "list4 reverse list1 copy: " << list4 << endl << endl;

  // std::transform()
  cout << "Using std::transform() to multiply every elt in list2 by 2:\n";
  cout << "list2 prior transformation: " << list2 << endl;
  std::transform(list2.begin(), list2.end(), list2.begin(), [](auto x){return x * 2;});
  cout << "list2 after transformation: "  << list2 << endl << endl;

  cout << "... and many more! The Slist iterators are designed to be fully compatible with\n"
       << "STL's own concept. For algorithms such as \"std::copy()\" where the lists would normally\n"
       << "have to be the same size, using \"stream_Iterator\"'s allows for nodes to be\n"
       << "dynamically generated at a list's end, eliminating the risk of dereferencing \"nullptr\".\n\n"
       << "Bye!\n";

  return 0;
}
