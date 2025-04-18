/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:08:27 by geonwkim          #+#    #+#             */
/*   Updated: 2025/04/18 22:31:02 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <string>
# include <stack>
# include <sstream>
# include <exception>

class RPN
{
	private:
		std::stack<double> _stack;
		void _executeStack(double (RPN::*f)(double, double));
		double _plus(double a, double b);
		double _minus(double a, double b);
		double _multiply(double a, double b);
		double _divide(double a, double b);

	public:
		//Constructors
		RPN();
		RPN(const std::string  &inputStr);
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN();

		//Getters
		double getResult() const;


		//Exceptions
		class invalidArgument : public std::runtime_error
		{
			public:
				explicit invalidArgument(const std::string& msg);
		};

		class emptyStack : public std::runtime_error
		{
			public:
				explicit emptyStack(const std::string& msg);
		};

		class divisionByZero : public std::runtime_error
		{
			public:
				explicit divisionByZero(const std::string& msg);
		};	
};

#endif
