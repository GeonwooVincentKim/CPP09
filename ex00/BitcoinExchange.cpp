/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:50:23 by geonwkim          #+#    #+#             */
/*   Updated: 2025/04/15 23:12:37 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

// トリム関数を追加
static std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// Constructors
BitcoinExchange::BitcoinExchange()
{
	_readDB();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy)
{
	if (this != &copy)
		this->_quotes = copy._quotes;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &copy)
{
	if (this != &copy)
		this->_quotes = copy._quotes;
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

// Getters
std::map<std::string, double> const &BitcoinExchange::getQuotes() const
{
	return (this->_quotes);
}

void BitcoinExchange::_readDB()
{
	std::ifstream dbFile("data.csv");
	if (!dbFile.is_open())
		throw CouldNotOpenFile();

	std::string line;
	// Check if first line is date, exchange_rate
	std::getline(dbFile, line);
	if (line != "date,exchange_rate")
	{
		// dbFile.close();
		throw CouldNotOpenFile();
	}

	while (std::getline(dbFile, line))
	{
		std::string date, price;
		std::istringstream ss(line);
		std::getline(ss, date, ',');
		std::getline(ss, price, ',');

		// Parse price using istringstream
		double priceValue;
		std::istringstream priceStream(price);
		if (!(priceStream >> priceValue))
		{
			// dbFile.close();
			throw InvalidPriceFormat();
		}

		_quotes[date] = priceValue;
	}
	dbFile.close();
}

void BitcoinExchange::execute(const char *fileName)
{
	std::ifstream inputFile(fileName);
	if (!inputFile.is_open())
		throw CouldNotOpenFile();

	std::string line;
	// Check if first line is date, value
	std::getline(inputFile, line);
	if (line != "date | value")
	{
		// inputFile.close();
		throw InvalidColumnFormat();
	}

	while (std::getline(inputFile, line))
	{
		std::string date, value;
		double priceValue;
		std::istringstream ss(line);

		std::getline(ss, date, '|');
		std::getline(ss, value, '|');

		// Validate date format
		if (!date.empty())
			date = trim(date);
		// date = date.erase(date.length() - 1);
		if (_validateDate(date) == false)
		{
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Parse value using istringstream
		if (!value.empty())
			value = trim(value);
		// value = value.erase(0, 1);
		priceValue = _validatePrice(value);

		// Multiply with the corresponding quote
		if (priceValue != -1)
			_multiplyWithQueue(date, priceValue);
	}
	inputFile.close();
}

// Validate date format that it contains only numbers and '-'.
bool BitcoinExchange::_validateDate(const std::string &date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (isdigit(date[i]) == 0)
			return (false);
	}

	if (date[5] == '0' && date[6] == '0') 
		return (false);
	if ((date[5] == '1' && date[6] > '2') || date[5] > '1')
		return (false);

	if (date[8] == '0' && date[9] == '0')
		return (false);
	if ((date[8] == '3' && date[9] > '1') || date[8] > '3')
		return (false);

	return (true);
}

double BitcoinExchange::_validatePrice(const std::string &price)
{
	double priceValue;
	std::istringstream priceStream(price);

	if (!(priceStream >> priceValue))
	{
		std::cout << "Error: bad value input => " << priceValue << std::endl;
		return (-1);
	}

	if (priceValue < 0)
	{
		std::cout << "Error: not a positive number => " << priceValue << std::endl;
		return (-1);
	}
	else if (priceValue > 1000)
	{
		std::cout << "Error: too large a number => " << priceValue << std::endl;
		return (-1);
	}
	return (priceValue);
}

// Multiply the value with the corresponding quote
void BitcoinExchange::_multiplyWithQueue(const std::string &date, double price)
{
	std::map<std::string, double>::iterator it = _quotes.find(date);
	if (it != _quotes.end())
		std::cout << date << " => " << price << " = " << price * it->second << std::endl;
	else
	{
		std::map<std::string, double>::iterator it2 = _quotes.lower_bound(date);
		if (it2 == _quotes.begin())
			std::cout << date << " => " << price << " = " << price * it2->second << std::endl;
		else
		{
			it2--;
			std::cout << date << " => " << price << " = " << price * it2->second << std::endl;
		}
	}
}
