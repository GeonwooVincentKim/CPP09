/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:30:50 by geonwkim          #+#    #+#             */
/*   Updated: 2025/04/15 22:43:01 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <exception>
#include <string>

class BitcoinExchange
{
private:
	std::map<std::string, double> _quotes;
	void _readDB();
	bool _validateDate(const std::string &date);
	double _validatePrice(const std::string &price);
	void _multiplyWithQueue(const std::string &date, double price);

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &copy);
	~BitcoinExchange();
	BitcoinExchange &operator=(const BitcoinExchange &copy);

	std::map<std::string, double> const &getQuotes() const;
	void execute(const char *fileName);

	// Exception which printing error message
	class CouldNotOpenFile : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("Error: could not open file.");
		}
	};

	class InvalidDateFormat : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("Error: invalid date format.");
		}
	};

	class InvalidColumnFormat : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("Error: invalid column format.");
		}
	};

	class InvalidPriceFormat : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("Error: invalid price format.");
		}
	};
};

#endif
