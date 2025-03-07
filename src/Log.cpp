/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/08 00:05:55 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

Log::Log(void) : start_time_(time(0))
{
}

Log::~Log(void)
{
}

Log::Notice::Notice(void)
{
}

Log::Notice::~Notice(void)
{
	std::cout << buffer_.str() << '\n';
}

Log::Info::Info(void)
{
}

Log::Info::~Info(void)
{
	time_t current_time(std::time(0));
	long elapsed_time(current_time - start_time_);
	std::cout << "[" << elapsed_time << "] " << buffer_.str() << '\n';
}

Log::Err::Err(void)
{
}

Log::Err::~Err(void)
{
	time_t current_time(std::time(0));
	long elapsed_time(current_time - start_time_);
	std::cerr << "[" << elapsed_time << "] " << buffer_.str() << '\n';
}
