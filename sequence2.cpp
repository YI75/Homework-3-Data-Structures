#include <algorithm>
#include "sequence2.h"
#include <cassert>
using namespace std;
namespace main_savitch_4
{
/* Postcondition: The sequence is initialized, it is empty. Insert and attach functions work efficiently without allocating
   new memory until this capacity is reached.*/
 sequence::sequence(size_type initial_capacity){
    m_sequence = new value_type[initial_capacity];
    m_capacity = initial_capacity;
    m_used = 0;
    m_current_index = m_used;
}
/*Postcondition: This is a copy constructor, so a sequence is initialized as the sequence source.*/
sequence::sequence(const sequence& source){
    m_sequence = new value_type[source.m_capacity];
    m_capacity = source.m_capacity;
    m_used = source.m_used;
    m_current_index = source.m_current_index;
    copy(source.m_sequence, source.m_sequence + m_used, m_sequence);
}
/*Postcondition: Deletes sequence when appropiate*/
sequence::~sequence( ){
    delete [] m_sequence;
}
   // MODIFICATION MEMBER FUNCTIONS
/*Precondition: the sequence is not empty. (Just to be safe)
Postcondition: The first item in the sequence becomes the current item*/
void sequence::start( ){
    assert(m_used >= 0);
    m_current_index = 0;
}
/* Precondition: is_item is true.
Postcondition: If the current item was already the last item in the sequence, then there is no longer any current item.
Otherwise, the new current item is the next item in the sequence.*/
void sequence::advance( ){
     assert(is_item());
     ++m_current_index;
}
/*Postcondition: A new copy of entry has been inserted in the sequence before the current item. If is_item is false,
then the new entry has been inserted at the front of the sequence. Newly inserted item is now the current item. */
void sequence::insert(const value_type& entry){
    //If the array has reached its capacity, increase capacity by one.
    if(m_used >= m_capacity){
        resize(m_used+1);
    }
    //If the sequence is empty, then entry becomes the first and only item.
    if(m_used == 0){
        m_sequence[0] = entry;
        m_current_index = 0;
        m_used++;
        return;
    }
    //If is_item is false, entry will inserted at the front of the sequence.
    if(!is_item()){
        m_current_index = 0;
    }
    //Makes space for entry
    size_type index = m_used;
    while(index > m_current_index){
        m_sequence[index] = m_sequence[index-1];
        --index;
    }
    m_sequence[m_current_index] = entry;
    m_used++;
}


/*Postcondition: A new copy of entry has been inserted in the sequence after the current item. If is_item is flase, then the new
entry has been attached to the end of the sequence. Newly inserted item is now the current item of the sequence. */
void sequence::attach(const value_type& entry){
    //If the array has reached its capacity, increase capacity by one.
    if(m_used >= m_capacity){
        resize(m_used+1);
    }    //If the sequence is empty, then entry becomes the first and only item.
    if(m_used == 0){
        m_sequence[0] = entry;
        m_current_index = 0;
        m_used++;
        return;
    }
    //If is_item is false, or the current index is at the last item in the sequence, place entry at the end of the sequence.
    if(!is_item() || m_current_index == m_used-1){
        m_sequence[m_used] = entry;
        m_current_index = m_used;
        m_used++;
        return;
    }
    //Makes space for entry
    size_type index = m_used;
    while(index > m_current_index + 1){
        m_sequence[index] = m_sequence[index-1];
        --index;
    }
    m_current_index++;
    m_sequence[m_current_index] = entry;
    m_used++;
}
/* Precondition: is_item is true.
Postcondition: The current item has been removed from the sequence, and the item after this is now the new current item.*/
void sequence::remove_current( ){
    assert(is_item());
    //Case where current index is at the end of the sequence.
    if(m_current_index == m_used-1){
        m_used--;
        m_current_index = m_used;
        return;
    }
    //Shifts all items to fill the removed item.
    for(size_type i = m_current_index+1; i < m_used; ++i){
        m_sequence[i-1] = m_sequence[i];
    }
    m_used--;
}
/* Postcondition: The sequence's current capacity is changed to the new capacity. The new capacity can not be less than the
number of items already on the sequence. The insert/attach functions will work efficiently (without allocating new memory) until
this new capacity is reached.*/
void sequence::resize(size_type new_capacity){
   //if the new capacity is the same as the old capacity, end function.
    if(new_capacity == m_capacity){
        return;
    }
    //if the new capacity is less than the number of items in the sequence, set new capacity to m_used
    if(new_capacity < m_used){
        new_capacity = m_used;
    }
    value_type *larger_seq;
    larger_seq = new value_type[new_capacity];
    copy(m_sequence, m_sequence + m_used, larger_seq);
    delete [] m_sequence;
    m_sequence = larger_seq;
    m_capacity = new_capacity;
}
/* Postcondition: This function overloads the = assignment. The source sequence's variables and arrays are copied to the sequence
on the left hand side of the =.*/
void sequence::operator=(const sequence& source){
    //if they are already equal, end function
    if(this == &source){
        return;
    }
    value_type *new_sequence;
    if(m_capacity != source.m_capacity){
        new_sequence = new value_type[source.m_capacity];
        delete [] m_sequence;
        m_sequence = new_sequence;
        m_capacity = source.m_capacity;
    }
    m_used = source.m_used;
    m_current_index = source.m_current_index;
    copy(source.m_sequence, source.m_sequence + m_used, m_sequence);
    
}
/*Precondition: The index must have a value_type in the sequence.
  Postcondition: Return the value_type at the given index.
*/
sequence::value_type sequence::operator[](sequence::size_type index) const{
    assert(index >= 0 && index <= m_used-1);
    return m_sequence[index];
}

//Postcondition: the items in sequence "add" are appended at the end of the given sequence.
void sequence::operator+=(const sequence& add){
    //If the capacity is reached by the sum of the number of items in both sequence, change the capacity of given sequence.
    if(m_used + add.m_used > m_capacity){
        resize(m_used + add.m_used);
    }
    copy(add.m_sequence, add.m_sequence + add.m_used, m_sequence + m_used);
    m_used+=add.m_used;
}

//Postcondition: The return number of items in the sequence.
sequence::size_type sequence::size( ) const{
    return m_used;
}
/*Postcondition: Returns true if there is a valid "current" item that may be retrieved by activating the current
member function. Returns false if there is no valid current item.*/
bool sequence::is_item( ) const{
    return (m_current_index >= 0 && m_current_index < m_used);
}
/*Precondition: is_item is true.
Postcondition: Returns current item in the sequence.*/
sequence::value_type sequence::current() const{
     assert(is_item());
     return m_sequence[m_current_index];
}
/*Postcondition: returns sequence that is sum of sequences s1 and s2, where s2's items come after s1's items.
 */
sequence operator+(const sequence& s1, const sequence& s2){
    sequence sum_seq;
    sum_seq+=s1;
    sum_seq+=s2;
    return sum_seq;
}

}
