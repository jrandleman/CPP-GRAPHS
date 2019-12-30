# **SuperList**
## _Overhauled STL List: Mapping/Valarray Methods & Versatile Iterators!_
-----
## Notes:
* _Compile as C++17 or higher_
* _Avoid C-style strings, use_ `<string>` _class instead_
* _Use_ ['e' as variable](#using-e-as-elt-argument-variable) _to write operations/conditions with_ `.map()`_,_ `.filter()`_, &_ `.unfold()`,</br>
  _as well as_ ['e' & 'e2' as variables](#using-e--e2-as-elt-argument-variables) _('_`e`_' precedes '_`e2`_') with_ `.unfold()`'_s overload &_ `.funnel()`
*  _Basic Iterator Properties (see more [below](#iterator-methods)!):_
   1) `Iterator`: _list traversal, alter existing nodes via `*`, & make new/rmv old nodes via_ `push`_/_`pop` _variants_
   2) `reverse_Iterator`: _inherits_ `Iterator` _methods, but traverses backwards &_ `*` _gives prior node's value_
   3) `stream_Iterator`: _inherits_ `Iterator` _and I/O methods,_ `*` _creates new nodes if at_ `nullptr`
-----
## Using the SuperList Library:
```c++
#include <string>
#include "slist.hpp" // header

int main() {
  using SUPERLIST::Slist; // The 'Slist' class
  // declaring objects
  Slist<int> myList; 
  Slist<std::string> myStringList;
  ...
}
```
-----
# Slist Methods
## _Those Invoked Directly By Linked List Objects_
-----
## Basic Insertion, Removal, & Analysis Methods:
### Insertion & Removal:
`.push_front()` & `.push_back()` => _add elt or_ `Slist` _arg to list's head/tail_</br>
`.pop_front()` & `.pop_back()` => _remove & return head/tail value, or assign it given an elt arg_</br>
`+` & `+=` => _overloaded operators concatentate (via_ `push_back`_) 2 list objects_</br>
`-` & `-=` => _overloaded operators remove instances of the RHS list's elts from the LHS list_

### Analysis:
`.front()` & `.back()` => _return head/tail value, or assign it given an elt arg_</br>
`.has()` => _return boolean of whether list has elt arg_</br>
`.size()` => _get list's current number of elts_</br>
`.empty()` => _return boolean of whether_ `list.size() == 0`</br>
`.show()` => `cout` _list's elts, with optional string arg replacing default spaces btwn elts_</br>
`==` & `!=` => _overloaded operators check whether 2 lists have identical elts_

-----
## List Rearrangement:
### Removing:
`.remove()` => _remove all instances of the elt arg in list_</br>
`.unique()` => _remove all duplicate elts in list, only keeping its 1st instance_</br>
`.clear()` => _remove all elts in list_</br>

### Sorting:
`.sort()` => _sorts by default ascending, pass_ `false` _for descending, or pass a binary predicate for custom_</br>
`.sortAscending()` => _invokes_ `.sort()`</br>
`.sortDescending()` => _invokes_ `.sort(false)`</br>

### Merging:
_=> **Note:** identical to sorting methods, but w/ a list object as the mandatory 1st arg_</br>
`.merge()` => _merges by default ascending, pass_ `false` _for descending, or pass a binary predicate for custom_</br>
`.mergeAscending()` => _invokes_ `.merge()`</br>
`.mergeDescending()` => _invokes_ `.merge(false)`</br>

### Reversal & Swap:
`.reverse()` => _reverses list_</br>
`.swap()` => _swaps list with its list object arg_

-----
## List Splicing, Slicing, & Emplacement Methods:
_=> **Note:**_ `.splice()` _&_ `.slice()`_'s_ [iterator](#iterator-invoked-methods) _removal ranges are \[iter1, iter2)_</br>
_=> **Note:** optional 3rd bool arg for_ `.slice()`_, default false, also removes slice from original list_</br></br>
`.splice()` => _given 2 iterators & a list object or elt, splices arg between the 2 iterators_</br>
`.slice()` => _given 2 iterators, returns a list sliced between them_</br>
`.emplace()` => _given 1 iterator & a list object or elt, inserts arg into list directly prior to the iterator_</br>

-----
## List Mapping, Filtering, Funneling, & Folding/Unfolding Methods:
### Using 'e' as Elt Argument Variable:
`.map(e * 2)` => _map argument's operation across list's elts_</br>
`.filter(e % 2 == 0 && e < 10)` => _only keep list elts satisfying args's condition_</br>
`.unfold(1, e + 1, 15)` => _from 1st arg, recursively operate 2nd arg for 3rd arg # of times (default 10), making list_</br>
### Using 'e' & 'e2' as Elt Argument Variables:
`.funnel(e > e2 ? e : e2)` => _compare every 2 elts, returning the superior to funnel out a value_</br>
`.unfold(0, 1, e + e2, 15)` => _from e = arg1, e2 = arg2, recursively operate arg3 for arg4 # of times, making list_
### Fold Elts Over Each Other via Operator:
`.fold(+)` => _'fold' list elts on top of each other, invoking the operator arg, & returning the total (Beware Overflow!)_

-----
## Arithmetic Operations for Numerical Lists:
`+=`, `-=`, `*=`, & `/=` _followed by any #_ => _iterate operation across all of list's elts_</br>
`%=`, `&=`, `|=`, `^=`, `>>=`, & `<<=` _followed by non-floating point #_ => _iterate operation across all of list's elts_</br>
`.sum()`, `.max()`, & `.min()` => _return sum, max, or min of list's elts_</br>

-----
## Iterator Methods
### General Iterators:
`.begin()` => _return an_ `Iterator` _at list's head_</br>
`.end()` => _return an_ `Iterator` _at 1 position past list's tail (_`nullptr`_)_</br>
`.iterator()` => _return an_ `Iterator` _at 1st instance from_ `list.begin()` _of its elt arg_</br>
`.lastIterator()` => _return an_ `Iterator` _at 1st instance from_ `list.end()` _of its elt arg_
 
### Reverse Iterators (derived from General):
`.rbegin()` => _return a_ `reverse_Iterator` _at 1 position past list's tail (_`nullptr`_)_</br>
`.rend()` => _return a_ `reverse_Iterator` _at list's head_</br>
`.riterator()` => _return a_ `reverse_Iterator` _at 1st instance from_ `list.rbegin()` _of its elt arg_</br>
`.rlastIterator()` => _return a_ `reverse_Iterator` _at 1st instance from_ `list.rend()` _of its elt arg_</br>

### Stream Iterators (derived from General):
`.sbegin()` => _return a_ `stream_Iterator` _at list's head_</br>
`.send()` => _return a_ `stream_Iterator` _at 1 position past list's tail (_`nullptr`_)_</br>
`.siterator()` => _return a_ `stream_Iterator` _at 1st instance from_ `list.sbegin()` _of its elt arg_</br>
`.slastIterator()` => _return a_ `stream_Iterator` _at 1st instance from_ `list.send()` _of its elt arg_</br>

### All:
`.remove()` => _removes the iterator arg from the list_

-----
# Iterator-Invoked Methods:
## _Those Invoked Directly By General, Reverse, & Stream Iterator Objects_

### Design:
* **Fully Compatible With STL Iterator Concept**
* **Actions of the Base "General" Iterator Extend to the Derived "Reverse" & "Stream" Iterators**
* **`begin` = `head` Relative to General/Stream Iterators & `tail + 1` to Reverse Iterators**
* **`end` = `tail + 1` Relative to General/Stream Iterators & `head` to Reverse Iterators**
  * **_WARNING: Dereferencing a General or Reverse Iterator at its Relative_ `end` _Tries to Deref. a_ `nullptr`_!_**

### Insertion & Removal:
`.push_front()`, `.push_back()` => _push new node w/ elt arg's value adjacent to iterator towards relative end/begin_</br>
`.push_begin()`, `.push_end()` => _push new node w/ elt arg's value (w/o moving iterator) to the relative begin/end_</br>
`.pop_front()`, `.pop_back()` => _rmv & return value of node adjacent to iterator towards its relative end/begin_</br>
`.pop_begin()`, `.pop_end()` => _rmv & return value of node (w/o moving iterator) at the relative begin/end_

### Traversal:
`.begin()`, `.end()` => _immediately move iterator to it's relative begin/end_</br>
`++`, `+`, `+=` => _move iterator towards its relative end_</br>
`--`, `-`, `-=` => _move iterator towards its relative begin_

### Comparision & Equality:
`<`, `>`, `<=`, `>=` => _compare 2 iterator's positions w/ `<` denoting "closer to the relative begin"_</br>
`==`, `!=` => _compare 2 iterator's node addresses to see whether they point to the same node or not_

### Dereferencing via `*`:
_**General Iterators**: Returns reference to iterator's node value_</br>
_**Reverse Iterators**: Same as General, **except** returned value is of node directly preceding the iterator_</br>
_**Stream Iterators**: Same as General, but derefing its relative end makes a new node & returns its value's reference_

### I/O:
_**"ostream**_ `<<` _**iterator"**_ => _outputs iterator's node address_</br>
_**"istream**_ `>>` _**iterator"**_ => _assigns value from the "istream" as iterator's node value_</br>
* _inputting an istream value via any iterator's relative end makes a new node and appends it to that end_ 

### Stream Iterator Specializations:
`.reverse_end()`, `.reverse_begin()` => _reverses entire list & immediately moves iterator to list's new_ `tail +1`_/_`head`</br>
_**"iterator**_ `<<` _**value"**_ =>  _output a value to_ `.push_back()` _a new node behind the iterator_</br>
_**"iterator**_ `>>` _**emptyVariable"**_ =>  _input a value to a variable by popping the current iterator's node value_

-----
## Sample Iterator Use (_see_ `slist_SampleExec.cpp` _for more!_):
```c++
#include <iostream>
#include <algorithm>
#include "slist.hpp"

/*
 * Explicitly Declaring Iterators:
 *   General: "Slist<type>::Iterator it;"
 *   Reverse: "Slist<type>::reverse_Iterator it;"
 *   Stream:  "Slist<type>::stream_Iterator it;"
 */

int main() {
  using std::cout;
  using std::endl;
  using SUPERLIST::Slist;

  // dereferencing "stream_Iterator"s at nullptr makes a new node,
  // thus we can use such to initialize a list
  Slist<double> list1;
  Slist<double>::stream_Iterator is = list1.sbegin();
  for(int i = 0; i < 11; ++i)
    *is++ = i * 3;
  cout << "list1 initialized w/ Multiples of 3 from 0-30:    " << list1 << endl;


  // get a reversed copy (list-length slice) of list1 via "reverse_Iterator"s
  Slist<double> list2 = list1.slice(list1.rbegin(), list1.rend());
  cout << "list2 initialized as a reverse copy of list1:     " << list2 << endl;


  // use the std::transfrom() algorithm with general "Iterator"s to 
  // multiply all of list1's values by 5
  std::transform(list1.begin(), list1.end(), list1.begin(), [](auto x){return x * 5;});
  cout << "list1 w/ all elts * by 5 via std::transform():    " << list1 << endl;


  // use std::copy() along with an stream iterator to copy list1 to a new list,
  // Note the dynamic node allocation of stream_Iterators circumvents the need
  // for the containers to be of the same size here w/ std::copy()
  Slist<double> list3;
  std::copy(list1.begin(), list1.end(), list3.sbegin());
  cout << "list3 copy of list1 by std::copy() & 2 stream iterators: " << list3 << endl;

  return 0;
}
```
