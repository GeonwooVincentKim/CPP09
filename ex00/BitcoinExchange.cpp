/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:50:23 by geonwkim          #+#    #+#             */
/*   Updated: 2025/04/18 22:05:37 by geonwkim         ###   ########.fr       */
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

bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int year, int month) {
    const int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2 && isLeap(year)) return 29;
    return days[month - 1];
}

void BitcoinExchange::_readDB() {
    std::ifstream dbFile("data.csv");
    if (!dbFile.is_open())
        throw CouldNotOpenFile();

    std::string line;
    std::getline(dbFile, line); // Skip header

    while (std::getline(dbFile, line)) {
        // Manual count of commas
        size_t commaCount = 0;
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == ',') commaCount++;
        }
        if (commaCount != 1) continue; // invalid line

        std::string date, price;
        std::istringstream ss(line);
        if (!std::getline(ss, date, ',') || !std::getline(ss, price)) continue;

        // trim
        date = trim(date);
        price = trim(price);
        if (!_validateDate(date)) continue;

        double priceValue;
        std::istringstream priceStream(price);
        if (!(priceStream >> priceValue)) continue;

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
bool BitcoinExchange::_validateDate(const std::string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    if (year < 2009 || (year == 2009 && month == 1 && day < 2)) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(year, month)) return false;

    return true;
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
