/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:44:48 by geonwkim          #+#    #+#             */
/*   Updated: 2025/04/18 22:46:21 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

////////////////////////// shared functions ///////////////////////////////////

// Comparing function for inplace_merge by comparing the second(larger) element of the pairs
bool custom_cmp(std::pair<int, int> a, std::pair<int, int> b)
{
	if (a.second < b.second)
		return (1);
	return (0);
}

void print_before(char **argv)
{
	int i = 1;
	int error = 0;

	std::cout << "Before: ";
	while (argv[i])
	{
		std::string input = argv[i];
		std::cout << input << " ";
		if (input.size() > 10 || std::atol(input.c_str()) > INT_MAX || std::atol(input.c_str()) < 0)
			error = 1;
		for (size_t j = 0; j < input.size(); j++)
		{
			if (!isdigit(input[j]))
				error = 1;
		}
		i++;
	}
	std::cout << "\n";
	if (error == 1)
		throw CustomException("Error: input is invalid");
}

////////////////////////// List functions //////////////////////////////////////

void create_pairs(std::list<std::pair<int, int> > *listy, char **argv, int *additional_value)
{
	int i = 1;

	while (argv[i])
	{
		if (argv[i] && argv[i + 1])
		{
			std::pair<int, int> pair_temp(std::atoi(argv[i]), std::atoi(argv[i + 1]));
			listy->push_back(pair_temp);
			i += 2;
		}
		else
		{
			*additional_value = std::atoi(argv[i]);
			i++;
		}
	}
}

void sort_pairs(std::list<std::pair<int, int> > *listy)
{
	for (std::list<std::pair<int, int> >::iterator it = listy->begin(); it != listy->end(); ++it)
	{
		if (it->first > it->second)
		{
			int temp = it->first;
			it->first = it->second;
			it->second = temp;
		}
	}
}

// Do merge sort with recursive approach splitting the list in half each time and merging them back together
void mergeSort(std::list<std::pair<int, int> >::iterator start, std::list<std::pair<int, int> >::iterator end, size_t size)
{
	if (size == 0 && start != end)
		size = std::distance(start, end);
	if (size <= 1)
		return;

	size_t firstHalf = size / 2;
	size_t secondHalf = size - firstHalf;
	std::list<std::pair<int, int> >::iterator center = start;
	// Move the iterator to the center of the listalways
	std::advance(center, firstHalf);

	mergeSort(start, center, firstHalf);
	mergeSort(center, end, secondHalf);
	std::inplace_merge(start, center, end, &custom_cmp);
}

void init_main_chain(std::list<int> *main_chain, std::list<std::pair<int, int> > listy)
{
	for (std::list<std::pair<int, int> >::iterator it = listy.begin(); it != listy.end(); ++it)
		main_chain->push_back(it->second);

	// the first number of pend can always be inserted as the first number in the main chain.
	// it's always smaller than it's paired number
	if (!listy.empty())
		main_chain->push_front(listy.begin()->first);
}

// Do a binary search to find the correct position within the slice of the main chain and insert the value
void binary_search_insertion(std::list<int> *main_chain, std::list<int>::iterator end, int val)
{
	std::list<int>::iterator place_to_insert = std::lower_bound(main_chain->begin(), end, val);
	main_chain->insert(place_to_insert, val);
	
	/*
	// print main chain
	std::cout << "main chain with current value: " << val << ": "
			  << "end_interator: " << *end << " place_to_insert: " << *(place_to_insert--) << std::endl;
	for (std::list<int>::iterator it = main_chain->begin(); it != main_chain->end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << '\n';
	*/
}


void insert_into_main_chain(std::list<std::pair<int, int> > listy, std::list<int> *main_chain, int additional_value)
{
	size_t Jacobsthal[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 89478485, 178956971, 357913941, 715827883, 1431655765};
	int jacobsthal_idx = 1;

	std::list<int>::iterator slice_delim_it;
	std::list<std::pair<int, int> >::iterator pair_it;
	std::list<std::pair<int, int> >::iterator last_jacob_it = listy.begin();

	while (Jacobsthal[jacobsthal_idx] <= listy.size())
	{
		pair_it = listy.begin();
		std::advance(pair_it, Jacobsthal[jacobsthal_idx] - 1);
		last_jacob_it = pair_it;

		// how many pend elements have been inserted
		int insertion_counter = 0;
		while (Jacobsthal[jacobsthal_idx] - insertion_counter > Jacobsthal[jacobsthal_idx - 1])
		{
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binary_search_insertion(main_chain, slice_delim_it, pair_it->first);
			pair_it--;
			insertion_counter++;
		}
		jacobsthal_idx++;
	}

	// if the Jacobsthal number is larger than the size of the list, insert the remaining elements
	if (Jacobsthal[jacobsthal_idx] != listy.size())
	{
		pair_it = listy.end();
		if (pair_it != listy.begin())
			pair_it--;
		while (pair_it != last_jacob_it)
		{
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binary_search_insertion(main_chain, slice_delim_it, pair_it->first);
			pair_it--;
		}
	}

	if (additional_value != -1)
	{
		binary_search_insertion(main_chain, main_chain->end(), additional_value);
	}
}

void print_after(std::list<int> main_chain)
{
	std::cout << "After: ";
	for (std::list<int>::iterator it = main_chain.begin(); it != main_chain.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << '\n';
}

////////////////////////// Deque functions /////////////////////////////////////

void create_pairs2(std::deque<std::pair<int, int> > *dequey, char **argv, int *additional_value)
{
	int i = 1;

	while (argv[i])
	{
		if (argv[i] && argv[i + 1])
		{
			std::pair<int, int> pair_temp(std::atoi(argv[i]), std::atoi(argv[i + 1]));
			dequey->push_back(pair_temp);
			i += 2;
		}
		else
		{
			*additional_value = std::atoi(argv[i]);
			i++;
		}
	}
}

void sort_pairs2(std::deque<std::pair<int, int> > *dequey)
{
	for (std::deque<std::pair<int, int> >::iterator it = dequey->begin(); it != dequey->end(); ++it)
	{
		if (it->first > it->second)
		{
			int temp = it->first;
			it->first = it->second;
			it->second = temp;
		}
	}
}

void mergeSort2(std::deque<std::pair<int, int> >::iterator start, std::deque<std::pair<int, int> >::iterator end, size_t size)
{
	if (size == 0 && start != end)
		size = std::distance(start, end);
	if (size <= 1)
		return;

	size_t firstHalf = size / 2;
	size_t secondHalf = size - firstHalf;
	std::deque<std::pair<int, int> >::iterator center = start + firstHalf;

	mergeSort2(start, center, firstHalf);
	mergeSort2(center, end, secondHalf);
	std::inplace_merge(start, center, end, &custom_cmp);
}

void init_main_chain2(std::deque<int> *main_chain, std::deque<std::pair<int, int> > dequey)
{
	for (std::deque<std::pair<int, int> >::iterator it = dequey.begin(); it != dequey.end(); ++it)
		main_chain->push_back(it->second);
	if (!dequey.empty())
		main_chain->push_front(dequey.begin()->first);
}

void binary_search_insertion2(std::deque<int> *main_chain, std::deque<int>::iterator end, int val)
{
	std::deque<int>::iterator place_to_insert = std::lower_bound(main_chain->begin(), end, val);
	main_chain->insert(place_to_insert, val);
}

void insert_into_main_chain2(std::deque<std::pair<int, int> > dequey, std::deque<int> *main_chain, int additional_value)
{
	size_t Jacobsthal[] = {1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525, 699051, 1398101, 2796203, 5592405, 11184811, 22369621, 44739243, 89478485, 178956971, 357913941, 715827883, 1431655765};
	int jacobsthal_idx = 1;

	std::deque<int>::iterator slice_delim_it;
	std::deque<std::pair<int, int> >::iterator pair_it;
	std::deque<std::pair<int, int> >::iterator last_jacob_it = dequey.begin();

	while (Jacobsthal[jacobsthal_idx] <= dequey.size())
	{
		pair_it = dequey.begin() + (Jacobsthal[jacobsthal_idx] - 1);
		last_jacob_it = pair_it;

		int insertion_counter = 0;
		while (Jacobsthal[jacobsthal_idx] - insertion_counter > Jacobsthal[jacobsthal_idx - 1])
		{
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binary_search_insertion2(main_chain, slice_delim_it, pair_it->first);
			pair_it--;
			insertion_counter++;
		}
		jacobsthal_idx++;
	}

	if (Jacobsthal[jacobsthal_idx] != dequey.size())
	{
		pair_it = dequey.end();
		if (pair_it != dequey.begin())
			pair_it--;
		while (pair_it != last_jacob_it)
		{
			slice_delim_it = std::find(main_chain->begin(), main_chain->end(), pair_it->second);
			binary_search_insertion2(main_chain, slice_delim_it, pair_it->first);
			pair_it--;
		}
	}

	if (additional_value != -1)
	{
		binary_search_insertion2(main_chain, main_chain->end(), additional_value);
	}
}

void print_after2(std::deque<int> main_chain)
{
	std::cout << "After: ";
	for (std::deque<int>::iterator it = main_chain.begin(); it != main_chain.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << '\n';
}


//Expalination:
/*
1. Create pairs from the input arguments and if the amount of arguments is odd, store the last argument as additional_value
	//               23 10 10 97 42 32 40 84 29 26 66 96 57 13 46 30 91 48 11 <- odd
	//               ----- ----- ----- ----- ----- ----- ----- ----- -----
	// becomes ->    10 23 10 97 32 42 40 84 26 29 66 96 13 57 30 46 48 91
	// additional_value = 11

2. Sort the pairs
	//               23 10 10 97 42 32 40 84 29 26 66 96 57 13 46 30 91 48
	//               ----- ----- ----- ----- ----- ----- ----- ----- -----
	// becomes ->    10 23 10 97 32 42 40 84 26 29 66 96 13 57 30 46 48 91

3. // Do merge sort with recursive approach splitting the numbers in half each time and merging them back together
	//               10 23 10 97 32 42 40 84 26 29 66 96 13 57 30 46 48 91
	//               ----- ----- ----- ----- ----- ----- ----- ----- -----
	// becomes ->    10 23 26 29 32 42 30 46 13 57 40 84 48 91 66 96 10 97

4. Create the main chain by adding the second number of each pair to the main chain and the first number of the first pair (as it's always smaller then their paired number)
	// main chain -> 10-23-29-42-46-57-84-91-96-97
	//                     |  |  |  |  |  |  |  |
	// pend       ->       26 32 30 13 40 48 66 10
	// pend index:      0  1  2  3  4  5  6  7  8

5. Use jacobsthal numbers to insert the pend numbers into the main chain
	- Jacobsthal-numbers: 1, 3, 5, 11, 21, 43, 85...
	- lets say we have an index i and k as the current jacobsthal number starting at 1:
    - int i = 0; int k = 1;
	- lets also save i before increasing it to know where to stop; I will call it s
	- int s = i;

	// main chain -> 10-23-29-42-46-57-84-91-96-97
	//                     |  |  |  |  |  |  |  |
	// pend       ->       26 32 30 13 40 48 66 10
	//                  ^  ^
	//                  s  i
	// pend index:      0  1  2  3  4  5  6  7  8


    // we then iterate backwards until we reach the previous i (aka s), while inserting pend[i] inside the main chain
    // the binary insertion has to be done with the low bound as 0 (the start of main chain) and the index - 1 of
    // the value paired with the pend value we are inserting
    // the slice is represented with [] here
    // note that we can always assume that the number to insert should be inside this slice because the main chain
    // is always ordered and the number to insert will always be smaller than it's main chain counter part; thus
    // it is also smaller than every number to the right of it

	// main chain -> [10-23]-29-42-46-57-84-91-96-97
	//                       |  |  |  |  |  |  |  |
	// pend       ->         26 32 30 13 40 48 66 10
	//                   ^   ^
	//                   s   i
	// pend index:       0   1  2  3  4  5  6  7  8

	// so we insert 26 in the slice [10-23] -> [10-23-26] with a binary incertion

	// main chain -> 10-23-26-29-42-46-57-84-91-96-97
	//                           |  |  |  |  |  |  |
	// pend       ->             32 30 13 40 48 66 10
	//                    ^   ^
	//                    s   i
	// pend index:        0   1  2  3  4  5  6  7  8


    // next when we decrement i again, we reach s; we now need a new jacosthal number (3)
    // before we move i, it has to be reset to where it was at the beginning of the iteration (i = 1 in this case)

	// main chain -> 10-23-26-29-42-46-57-84-91-96-97
	//                           |  |  |  |  |  |  |
	// pend       ->             32 30 13 40 48 66 10
	//                        ^     ^
	//                        s     i
	// pend index:        0   1  2  3  4  5  6  7  8


    // we now apply the same logic as before, note the binary insertion slice in each insertion

	// main chain -> [10-23-26-29-42]-46-57-84-91-96-97
	//                            |   |  |  |  |  |  |
	// pend       ->              32  30 13 40 48 66 10
	//                         ^      ^
	//                         s      i
	// pend index:        0    1  2   3  4  5  6  7  8


	// main chain -> 10-23-26-29-30-42-46-57-84-91-96-97
	//                              |     |  |  |  |  |
	// pend       ->                32    13 40 48 66 10
	//                         ^       ^
	//                         s       i
	// pend index:        0    1    2  3  4  5  6  7  8


	// main chain -> [10-23-26-29-30]-42-46-57-84-91-96-97
	//                                |     |  |  |  |  |
	// pend       ->                  32    13 40 48 66 10
	//                           ^    ^
	//                           s    i
	// pend index:         0     1    2  3  4  5  6  7  8


	// main chain -> 10-23-26-29-30-32-42-46-57-84-91-96-97
	//                                       |  |  |  |  |
	// pend       ->                         13 40 48 66 10
	//                                    ^     ^
	//                            	      s     i
	// pend index:          0     1    2  3  4  5  6  7  8

	// and do far and so forth until we reach the end of the pend list.
	// the next jacobsthal number is now: 11
    // since we don't have so much elements left to insert, we can move i forward by the number of elements left in pend

	// main chain -> 10-13-23-26-29-30-32-40-42-46-57-84-91-96-97
	//                                                   |  |  |
	// pend       ->                                     48 66 10
	//                                               ^         ^
	//                                               s         i
	// pend index:          0     1    2    3    4   5   6  7  8

	// now couninuing the binary insertion as before

	// main chain -> 10-13-23-26-29-30-32-40-42-46-48-57-66-84-91-96-97

	// and finally we insert the additional value if it exists

	// main chain -> 10-11-13-23-26-29-30-32-40-42-46-48-57-66-84-91-96-97



	The Role of Jacobsthal Numbers
	Jacobsthal numbers play a crucial role in determining the sizes of the sublists for initial partitioning and 
	the order in which these sublists are merged. Their properties allow for an efficient balance between the number 
	of sublists and the sizes of these sublists, which is crucial for minimizing comparisons during the merge-insertion process. 
	
	
*/