#ifndef SINGLELINK_H
#define SINGLELINK_H

#include<iostream>
using std::ostream; using std::cout; using std::endl;
#include<sstream>
using std::ostringstream;
#include<utility>
using std::swap;
#include<string>
using std::string;
#include <iostream>
using std::cout; using std::endl;

// forward declaration so we can make Element a friend of TriMap
template<typename K, typename V>
class TriMap;

// private Element, friend of TriMap class
template <typename K, typename V>
class Element{

private:
  K key_;
  size_t index_ = 0;
  Element *next_ = nullptr;  

public:
  V value_;
  
  Element() = default;
  Element(K key, V val, size_t i) : key_(key),index_(i), next_(nullptr), value_(val) {};

  // *defined* in the class, makes templating easier.
  friend ostream& operator<<(ostream& out, Element& e){
    out << e.key_ << ":" << e.value_ << ":" << e.index_;
    return out;
  }
  friend class TriMap<K,V>;
};


template<typename K, typename V>
class TriMap{

private:
  Element<K,V> *head_ = nullptr;
  Element<K,V> *tail_ = nullptr;
  size_t size_ = 0;
  
  // a private function, used by operator<<
  void print_list(ostream& out);
  
public:
  TriMap() = default;
  TriMap(K, V);
  TriMap(const TriMap&);
  TriMap& operator=(TriMap);
  ~TriMap();
  long size();
  bool insert(K,V);
  bool remove(K);
  Element<K,V>* find_key(K);
  Element<K,V>* find_value(V);
  Element<K,V>* find_index(size_t);  
  
  // *defined* in the class, makes templating easier.
  // uses private method print_list
  // you can remove print_list and just do it here if you
  // prefer. I won't test print_list
  friend ostream& operator<<(ostream& out, TriMap<K,V>& sl){
    sl.print_list(out);
    return out;
  };
};

template<typename K, typename V>
TriMap<K,V>::TriMap(K key, V value){
	head_ = new Element<K,V>(key, value, 0);
	tail_ = head_;
	size_ = 1;
};

/* 
bit of work. we need to remember a pointer that walks
down the list to copy, as tail_ walks down the new list. 
Make a new node (copy the current node of the list we are copying), 
update the tail_->next_ to point to the new node, update tail_ to new node
 */
template<typename K, typename V>
TriMap<K,V>::TriMap(const TriMap& tm){
	if(tm.head_==nullptr){//if the trimap being copied is empty
		head_=nullptr;
		tail_=nullptr;
	}
	head_=new Element<K,V>(tm.head_->key_, tm.head_->value_, tm.head_->index_);
	tail_=head_;
	Element<K,V>* elem; //element to be added to the new map
	for(auto itr=tm.head_->next_; itr !=nullptr; itr=itr->next_){ //this ans similar for loops were taken from the last lab
		//since head was already created starts from one past head and copies the elements
		elem = new Element<K,V>(itr->key_, itr->value_, itr->index_);
		tail_->next_ = elem;
		tail_ = elem;
	}
	tail_ -> next_ = nullptr;
	size_ = tm.size_;
	
}

template<typename K, typename V>
TriMap<K,V>& TriMap<K,V>::operator=(TriMap tm){
    swap(head_, tm.head_);
    swap(tail_, tm.tail_);
	swap(size_, tm.size_);
    return *this;
}
	

// walk down the list using head_ , keeping a trailer pointer called to_del
// delete what to_del points to
// move head_ and to_del to the next node.
template<typename K, typename V>
TriMap<K,V>::~TriMap(){
	Element<K,V>* to_del = head_;
    while (to_del != nullptr){ //goes through the list assigning the deleted
	// to the head and then assigning head_ to what head_ points to and then deletes the old head
	head_ = head_->next_;
	delete to_del;
	to_del = head_;
    }
    head_ = nullptr;
    tail_ = nullptr;
}



template<typename K, typename V>
long TriMap<K,V>::size(){
  return size_;
}

template<typename K, typename V>
bool TriMap<K,V>::insert(K key, V value){
  // very important, what are all the cases here!!!!
  // elements are inserted in *key order*
  if(head_ == nullptr){//if the map is empty does the same thing as the constructor
	head_ = new Element<K,V>(key, value, 0);
	tail_ = head_;
	size_ = 1;
	return true;
  }
  Element<K,V>* last_check = head_; //used to find what is behind the found 
  Element<K,V>* checking = head_;//the current item being checked
  Element<K,V>* front = nullptr; //what the found variable will point to which is nothing if it goes in the back
  Element<K,V>* back = tail_; //what will point to the added variable
  while (checking != nullptr){
	  if (key == checking -> key_){
		  return false;
	    }
	    if (key<checking -> key_ && key > last_check -> key_){
			//if the element goes between two items finds that point and labels the
			//elements behind and infront of it
				front = checking;
				back = last_check;
	    }
	    last_check = checking;	  
	    checking = checking -> next_;
    }
	if(tail_==head_){//if there's only one element in the map
		if (key < head_ -> key_){//if the element should be added as head
			head_=new Element<K,V>(key, value, size_);
			head_ -> next_ = back;
			size_++;
			if(size_==2){//makes the added element the tail
				tail_ = head_->next_;
			}
		}
		else{//if the element should be added as tail
			tail_ = new Element<K,V>(key, value, size_);
			head_ -> next_ = tail_;
			tail_ -> next_ = nullptr;
			size_++;
		}		
		return true;
	}
	if(head_->key_ > key){//if the element should be added as the head but
	//there is more than one element in the map already
		front = head_;
		head_ = new Element<K,V>(key, value, size_);
		head_ -> next_= front;
		size_++;
		return true;
	}
	back->next_ = new Element<K,V>(key, value, size_);
	back->next_->next_ = front;
	if (front == nullptr && back==tail_){//if the element should be added as the new tail
		tail_ = back->next_;
		tail_ ->next_=nullptr;
	}
	if(head_ == tail_){
		tail_ = head_->next_;
		tail_ ->next_=nullptr;
	}
	size_++;
	return true;
}
    
template<typename K, typename V>
bool TriMap<K,V>::remove(K key){
	// again, what are all the cases!!!
	// also, if you delete a node, the indicies of all
	// nodes with a higher index have that index reduced by one
	Element<K,V>* checking = head_; //element being checked for removal
	Element<K,V>* last_check = nullptr;//the last item checked
	Element<K,V>* to_remove = nullptr;//the item to remove
	Element<K,V>* one_back = nullptr; //element before the one being removed
	while (checking != nullptr){
		if(key == checking -> key_){//finds the element to remove and the item behind it
			to_remove = checking;
			one_back = last_check;
		}
		last_check = checking;
		checking = checking -> next_;
	}
	if (to_remove == nullptr){
		return false;
	}
	for(auto itr=head_; itr !=nullptr; itr=itr->next_){
		if(itr -> index_ > to_remove -> index_){//sets every element with an index
		//higher to the removed one's index to one less
			itr -> index_-= 1;
		}
	}
	if(tail_== to_remove){//changes the tail if it was removed
		tail_=one_back;
	}
	if(head_== to_remove){//changes the head if it was removed
		head_=head_->next_;
		delete to_remove;
		size_--;
		return true;
	}
	one_back -> next_ = to_remove -> next_;
	delete to_remove;
	size_--;
	return true;
}
 
//every find function iterates through the map until it finds the matching items and returns
//the pointer
 
template<typename K, typename V>
Element<K,V>* TriMap<K,V>::find_key(K key){
	// no binary search, just linear search
	Element<K,V>* checking = head_;
	while (checking != nullptr){
		if(key == checking -> key_){
			return checking;
		}
		checking = checking -> next_;
	}
	return nullptr;
}    

template<typename K, typename V>
Element<K,V>* TriMap<K,V>::find_value(V val){
	// linear search
	Element<K,V>* checking = head_;
	while (checking != nullptr){
		if(val == checking -> value_){
			return checking;
		}
		checking = checking -> next_;
	}
	return nullptr;
}  

template<typename K, typename V>
Element<K,V>* TriMap<K,V>::find_index(size_t i){
	// linear search
	Element<K,V>* checking = head_;
	while (checking != nullptr){
		if(i == checking -> index_){
			return checking;
		}
		checking = checking -> next_;
	}
	return nullptr;
}  
template<typename K, typename V>
void TriMap<K,V>::print_list(ostream &out){
	for(auto itr=head_; itr !=nullptr; itr=itr->next_){
		if (head_ == nullptr){//if the map is empty
			out << "nothing in here";
		}
		if (itr == tail_){
			out<< *itr;
		}
		else{
			out << *itr << ", ";
		}
	}
}


#endif
  