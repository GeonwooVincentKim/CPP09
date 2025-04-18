/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:43:09 by geibwjun          #+#    #+#             */
/*   Updated: 2025/04/18 22:44:46 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"


int	main(int argc, char **argv)
{
	if (argc < 2)
		return (std::cerr << "Not enough arguments provided\n", 1);
	
	// first approach: std::list
	{
		std::cout << "using std::list" << std::endl;
		clock_t start = clock();
		std::list< std::pair<int, int> > listy;
		std::list<int> main_chain;
		// For value if the amount of number is odd
		int additional_value = -1;
		
		try
		{
			print_before(argv);
			create_pairs(&listy, argv, &additional_value);
			sort_pairs(&listy);
			mergeSort(listy.begin(), listy.end(), 0);

			/*
			// print listy
			std::cout << "listy: ";
			for (auto it = listy.begin(); it != listy.end(); it++)
				std::cout << "|" << it->first << ", " << it->second << "| ";
			std::cout << std::endl;
			*/

			init_main_chain(&main_chain, listy);

			/*
			// print main chain
			std::cout << "main chain: ";
			for (auto it = main_chain.begin(); it != main_chain.end(); it++)
				std::cout << *it << " ";
			std::cout << std::endl;
			*/

			insert_into_main_chain(listy, &main_chain, additional_value);
			print_after(main_chain);
			
			clock_t end = clock();
			std::cout << "Time to process a range of " << main_chain.size() << " elements with std::list: " << static_cast<double>(end - start) / 1000 << " millisec" << std::endl << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}


	// second approach: std::deque
	{
		std::cout << "using std::deque" << std::endl;
		clock_t start = clock();
		std::deque< std::pair<int, int> > dequey;
		std::deque<int> main_chain;
		int additional_value = -1;

		try
		{
			print_before(argv);
			create_pairs2(&dequey, argv, &additional_value);
			sort_pairs2(&dequey);
			mergeSort2(dequey.begin(), dequey.end(), 0);
			init_main_chain2(&main_chain, dequey);
			insert_into_main_chain2(dequey, &main_chain, additional_value);
			print_after2(main_chain);
			
			clock_t end = clock();
			std::cout << "Time to process a range of " << main_chain.size() << " elements with std::deque: " << static_cast<double>(end - start) / 1000 << " millisec" << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return (0);
}


/*
Main diffs between chosen containers:

Relevant for FJ:
- Deque: manages its elements with a dynamic array, provides random access
- List: manages its elements as a doubly linked list and does not provide random access

- Deque: provides fast insertions and deletions at both the end and the beginning. Inserting and deleting elements in the middle is relatively slow 
because all elements up to either of both ends may be moved to make room or to fill a gap.
- List: inserting and removing elements is fast at each position, including both ends

Neglible for FJ:
- Deque: Any insertion or deletion of elements other than at the beginning or end invalidates all pointers, references, and iterators that refer to elements of the deque
- List: Inserting and deleting elements does not invalidate pointers, references, and iterators to other elements.

Conclusion
The results you observed align with expectations based on the theoretical underpinnings of each container's design and the
practical implications of those designs on sorting performance. While std::list offers advantages in specific
scenarios (e.g., frequent insertions and deletions at arbitrary positions within a large dataset), std::deque is generally more
suited for operations like sorting, where its structural advantages—such as random access and better cache locality—translate into
significant performance benefits.
*/
