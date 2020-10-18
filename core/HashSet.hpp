// HashSet.hpp
//
// ICS 46 Winter 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before. 
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;
    int mapSize;
    int mapCapacity;

    struct Node
    {
        struct Node* next;
        ElementType data; 
    };

    Node** map;


    //initialize node array to null;
    void setToNull(Node** &map, int cap);

    //add node to front of list;
    void addToFront(Node* &head, ElementType data);


};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    map = new Node*[DEFAULT_CAPACITY];
    mapSize = 0;
    mapCapacity = DEFAULT_CAPACITY;
    setToNull(map, DEFAULT_CAPACITY);
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    
    for(int i=0; i<mapCapacity; ++i)
    {
        Node* temp = map[i];
        while(map[i]!= nullptr)
        {
            map[i] = map[i]->next;
            delete temp; 
            temp = map[i];
        }
    }

    delete[] map;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}
{ 
    mapSize = s.mapSize;
    mapCapacity = s.mapCapacity;
    map = new Node*[mapCapacity];
    setToNull(map, mapCapacity);
    
    for(int i=0; i<mapSize; ++i)
    {
        while(s.map[i]!= nullptr)
        {
            addToFront(map[i], s.map[i]->data);
            s.map[i] = s.map[i]->next;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{s.hashFunction}
{
    mapSize = s.mapSize;
    mapCapacity = s.mapCapacity;
    map = s.map;

    s.mapSize = 0;
    s.mapCapacity = 0;
    s.map = nullptr;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if(this != &s)
    {
        this -> ~HashSet();
        mapCapacity = s.mapCapacity;
        mapSize = s.mapSize;
        map = new Node*[mapCapacity];
        setToNull(map, mapCapacity);

        for(int i=0; i<mapCapacity; ++i)
        {
            while(s.map[i]!=nullptr)
            {
                addToFront(map[i], s.map[i]->data);
                s.map[i] = s.map[i]->next;
            }
        }
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    this-> ~HashSet();
    mapSize = s.mapSize;
    mapCapacity = s.mapCapacity;
    map = s.map;

    s.mapSize = 0;
    s.mapCapacity = 0;
    s.map = nullptr;

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if(contains(element) == false)
    {
        int hash = hashFunction(element)%mapCapacity;
        addToFront(map[hash], element); 
        mapSize++;    
    }

    if(double(mapSize)/mapCapacity > 0.8)
    {
        int newCap = mapCapacity *2; 
        Node** newMap = new Node*[newCap];
        setToNull(newMap, newCap);
        for(int i=0; i<mapCapacity; ++i)
        {
            Node* current = map[i];
            while(current != nullptr)
            {
                int newHash = hashFunction(current->data)%newCap;
                addToFront(newMap[newHash], current->data);
                current = current->next;
            }
        }

        for(int i=0; i<mapCapacity; ++i)
        {
            Node* current = map[i];
            while(map[i] != nullptr)
            {
                map[i] = map[i]->next;
                delete current;
                current = map[i];
            }
        }

        delete[] map;
        map = newMap;
        mapCapacity = newCap;
    }


}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    
    int hash = (hashFunction(element))%mapCapacity;
    bool exist = false;
    if(map[hash] == nullptr)
    {
        //std::cout << "DEOS IT GET HREE?" << std::endl;
        return exist;
    }
    else
    {
        //std::cout << " in the else loop" << std::endl;
        Node* current = map[hash];
        while(current!=nullptr)
        {
            //std::cout << element << " " << current->data << std::endl;
            if(element == current-> data)
            {
                exist = true;
                return exist;
            }
            current = current ->next;
        }
    }
    return exist;

}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return mapSize;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    int total = 0;
    if(index > mapCapacity)
    {
        return 0;
    }
    else
    {
        Node* current = map[index];
        while(current != nullptr)
        {
            current = current->next;
            total++;
        }
    }
    return total;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    bool exist = false; 
    Node* current = map[index];
    while(current != nullptr)
    {
        if(current->data == element)
        {
            exist = true;
            return exist;
        }
        current = current->next;
    }
    return exist;
}

template <typename ElementType>
void HashSet<ElementType>::setToNull(Node** &temp, int cap)
{
    for(int i=0; i < cap; ++i)
    {
        temp[i] = nullptr; 
    }
}

template <typename ElementType>
void HashSet<ElementType>::addToFront(Node* &head, ElementType data)
{
    Node* temp = new Node;
    temp->next = nullptr;
    temp->data = data;

    if(head == nullptr)
    {
        head = temp;
    }
    else
    {
        temp->next = head;
        head = temp;   
    }   

}

#endif // HASHSET_HPP

